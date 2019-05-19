/*
* MIT License
*
* Copyright (c) 2019 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#include "guise/control.hpp"
#include "guise/canvas.hpp"

template <typename T, typename U>
static bool ptrEquals(const std::weak_ptr<T>& t, const std::shared_ptr<U>& u)
{
    return !t.owner_before(u) && !u.owner_before(t);
}


namespace Guise
{

    // Control implementations.
    Control::Control(Canvas & canvas) :
        m_canvas(canvas),
        m_level(0)
    { }

    Control::~Control()
    {
        m_canvas.unregisterControl(*this);
    }

    Canvas & Control::getCanvas()
    {
        return m_canvas;
    }

    const Canvas & Control::getCanvas() const
    {
        return m_canvas;
    }

    bool Control::handleInputEvent(const Input::Event &)
    {
        return false;
    }

    void Control::update(const Bounds2f &)
    {
    }

    void Control::render(RendererInterface &)
    {
    }

    Bounds2f Control::getSelectBounds() const
    {
        return { {0.0f, 0.0f}, {0.0f, 0.0f} };
    }

    size_t Control::getLevel() const
    {
        return m_level;
    }

    void Control::setLevel(const size_t level)
    {
        if (level != m_level)
        {
            m_level = level;
            //m_canvas.registerControlLevelChange(*this, m_level);
        }
    }

    std::weak_ptr<Control> Control::getParent()
    {
        return m_parent;
    }
    std::weak_ptr<const Control> Control::getParent() const
    {
        return m_parent;
    }

    std::vector<std::shared_ptr<Control> > Control::getChilds()
    {
        return { };
    }
    std::vector<std::shared_ptr<const Control> > Control::getChilds() const
    {
        return {};
    }

    bool Control::add(const std::shared_ptr<Control> &, const size_t)
    {
        return false;
    }

    bool Control::remove(Control &)
    {
        return false;
    }

    bool Control::remove(const std::shared_ptr<Control> &)
    {
        return false;
    }
    bool Control::remove(const size_t)
    {
        return false;
    }

    size_t Control::removeAll()
    {
        return 0;
    }

    void Control::release()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto parent = m_parent.lock();
        if (!parent)
        {
            return;
        }

        m_parent.reset();
        parent->remove(*this);
    }

    Bounds2f Control::calcRenderBounds(const Bounds2f & canvasBound, const Vector2f & position, const Vector2f & size, const Style::Property::Overflow overflow) const
    {
        const bool clamp = overflow == Style::Property::Overflow::hidden;     
        Bounds2f bounds = { canvasBound.position + position, size };

        if (clamp)
        {
            bounds.position = Vector2f::clamp(bounds.position, canvasBound.position, Vector2f::max(canvasBound.position, canvasBound.position + canvasBound.size));
        }

        Vector2f lower = bounds.position;
        Vector2f higherCanvas = canvasBound.position + canvasBound.size;
        Vector2f higherThis = bounds.position + Vector2f::max({ 0.0f, 0.0f }, size);

        if ((clamp && higherThis.x > higherCanvas.x) || size.x <= 0.0f)
        {
            bounds.size.x = higherCanvas.x - lower.x;
        }
        if ((clamp && higherThis.y > higherCanvas.y) || size.y <= 0.0f)
        {
            bounds.size.y = higherCanvas.y - lower.y;
        }

        bounds.size.x = std::max(0.0f, bounds.size.x);
        bounds.size.y = std::max(0.0f, bounds.size.y);

        return bounds;
    }
 

    // ControlContainer implementations.
    ControlContainer::ControlContainer(Canvas & canvas) :
        Control(canvas)
    { }

    ControlContainer::~ControlContainer()
    { }

    void ControlContainer::adoptControl(Control & control)
    {
        control.release();

        std::lock_guard<std::mutex> lock(control.m_mutex);
        control.m_parent = Control::shared_from_this();
    }

    void ControlContainer::releaseControl(Control & control)
    {
        std::lock_guard<std::mutex> lock(control.m_mutex);
        control.m_parent.reset();
    }


    // ControlContainerSingle implementations.
    ControlContainerSingle::ControlContainerSingle(Canvas & canvas) :
        ControlContainer(canvas)
    { }

    ControlContainerSingle::~ControlContainerSingle()
    {
        removeAll();
    }

    void ControlContainerSingle::setLevel(const size_t level)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (level == getLevel())
        {
            return;
        }

        Control::setLevel(level);

        if (m_child)
        {
            m_child->setLevel(level + 1);
        }
    }

    std::vector<std::shared_ptr<Control> > ControlContainerSingle::getChilds()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return { m_child };
    }
    std::vector<std::shared_ptr<const Control> > ControlContainerSingle::getChilds() const
    {
        return { m_child };
    }

    bool ControlContainerSingle::add(const std::shared_ptr<Control> & control, const size_t)
    {
        if (!control)
        {
            return false;
        }

        removeAll();

        std::lock_guard<std::mutex> lock(m_mutex);

        adoptControl(*control.get());
        m_child = control;

        size_t level = getLevel();
        if (level != 0)
        {
            m_child->setLevel(level + 1);
        }    

        return true;
    }

    bool ControlContainerSingle::remove(Control & control)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_child || m_child.get() != &control)
        {
            return false;
        }
 
        releaseControl(control);
        m_child->setLevel(0);
        m_child.reset();

        return true;
    }

    bool ControlContainerSingle::remove(const std::shared_ptr<Control> & control)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_child || !control || m_child != control)
        {
            return false;
        }

        releaseControl(*control.get());
        m_child.reset();

        return true;
    }

    bool ControlContainerSingle::remove(const size_t)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_child)
        {
            return false;
        }

        releaseControl(*m_child.get());
        m_child.reset();

        return true;
    }

    size_t ControlContainerSingle::removeAll()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_child)
        {
            return 0;
        }

        releaseControl(*m_child.get());
        m_child.reset();

        return 1;
    }

    
    // ControlContainerList implementations.
    ControlContainerList::ControlContainerList(Canvas & canvas) :
        ControlContainer(canvas)
    { }

    ControlContainerList::~ControlContainerList()
    {
        removeAll();
    }

    void ControlContainerList::setLevel(const size_t level)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (level == getLevel())
        {
            return;
        }

        Control::setLevel(level);

        const size_t nextLevel = level + 1;
        for (auto & child : m_childs)
        {
            child->setLevel(nextLevel);
        }
    }

    std::vector<std::shared_ptr<Control> > ControlContainerList::getChilds()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_childs;
    }
    std::vector<std::shared_ptr<const Control> > ControlContainerList::getChilds() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        std::vector<std::shared_ptr<const Control> > childs;
        for (auto & child : m_childs)
        {
            childs.push_back(child);
        }
        return childs;
    }

    bool ControlContainerList::add(const std::shared_ptr<Control> & control, const size_t index)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!control)
        {
            return false;
        }        

        adoptControl(*control.get());
        if(index >= m_childs.size())
        {
            m_childs.push_back(control);
        }
        else
        {
            m_childs.insert(m_childs.begin() + index, control);
        }

        size_t level = getLevel();
        if (level != 0)
        {
            control->setLevel(level + 1);
        }
        
        return true;
    }

    bool ControlContainerList::remove(Control & control)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (auto it = m_childs.begin(); it != m_childs.end(); it++)
        {
            if (it->get() == &control)
            {
                releaseControl(*it->get());
                (*it)->setLevel(0);
                m_childs.erase(it);
                return true;
            }
        }

        return false;
    }
    bool ControlContainerList::remove(const std::shared_ptr<Control> & control)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (auto it = m_childs.begin(); it != m_childs.end(); it++)
        {
            if (*it == control)
            {
                releaseControl(*it->get());
                m_childs.erase(it);
                return true;
            }
        }

        return false;
    }

    bool ControlContainerList::remove(const size_t index)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (index >= m_childs.size())
        {
            return false;
        }

        auto child = m_childs.at(index);

        releaseControl(*child.get());
        m_childs.erase(m_childs.begin() + index);

        return true;
    }

    size_t ControlContainerList::removeAll()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (auto & child : m_childs)
        {
            releaseControl(*child.get());
        }

        size_t count = m_childs.size();
        m_childs.clear();
        return count;
    }


}