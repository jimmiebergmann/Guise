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
#include <iostream>

#define GUISE_CONTROL_CHECK_FLAG(x) static_cast<bool>(m_flags & x)
#define GUISE_CONTROL_SET_FLAG(x) m_flags |= x
#define GUISE_CONTROL_UNSET_FLAG(x) m_flags &= ~x

#define GUISE_CONTROL_FLAG_RESIZING         0x01    // 1    0000 0001
#define GUISE_CONTROL_FLAG_ENABLED          0x02    // 2    0000 0010
#define GUISE_CONTROL_FLAG_INPUTENABLED     0x04    // 4    0000 0100
#define GUISE_CONTROL_FLAG_VISIBLE          0x08    // 8    0000 1000
#define GUISE_CONTROL_FLAG_CHILDBOUNDSAWARE 0x10    // 16   0001 0000

namespace Guise
{

    // Control implementations.
    Control::Control() :
        m_availableBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_bounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_canvas(nullptr),
        m_flags(GUISE_CONTROL_FLAG_ENABLED | GUISE_CONTROL_FLAG_INPUTENABLED | GUISE_CONTROL_FLAG_VISIBLE),
        m_level(0)
    { }

    Control::~Control()
    {
        if (m_canvas)
        {
            m_canvas->reportControlRemove(this);
        }
    }

    Canvas * Control::getCanvas()
    {
        return m_canvas;
    }

    const Canvas * Control::getCanvas() const
    {
        return m_canvas;
    }

    void Control::draw(RendererInterface & rendererInterface)
    {
        rendererInterface.setLevel(m_level);
        onRender(rendererInterface);
    }

    bool Control::handleInputEvent(const Input::Event &)
    {
        return false;
    }

    Bounds2f Control::getSelectBounds() const
    {
        return m_bounds;
    }

    void Control::enable()
    {
        if (!GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_ENABLED))
        {
            GUISE_CONTROL_SET_FLAG(GUISE_CONTROL_FLAG_ENABLED);
            onEnable();
        }    
    }

    void Control::disable()
    {
        if (GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_ENABLED))
        {
            GUISE_CONTROL_UNSET_FLAG(GUISE_CONTROL_FLAG_ENABLED);
            onDisable();
        }     
    }

    bool Control::isEnabled()
    {
        return GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_ENABLED);
    }

    void Control::enableInput()
    {
        GUISE_CONTROL_SET_FLAG(GUISE_CONTROL_FLAG_INPUTENABLED);
    }

    void Control::disableInput()
    {
        GUISE_CONTROL_UNSET_FLAG(GUISE_CONTROL_FLAG_INPUTENABLED);
    }

    bool Control::isInputEnabled() const
    {
        return GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_INPUTENABLED);
    }

    void Control::show()
    {
        GUISE_CONTROL_SET_FLAG(GUISE_CONTROL_FLAG_VISIBLE);
    }

    void Control::hide(const bool)
    {
        GUISE_CONTROL_UNSET_FLAG(GUISE_CONTROL_FLAG_VISIBLE);
    }

    bool Control::isVisible() const
    {
        return GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_VISIBLE);
    }

    bool Control::intersects(const Vector2f & point) const
    {
        return GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_ENABLED) &&
               GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_INPUTENABLED) &&
               getSelectBounds().intersects(point);
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
            if (m_canvas)
            {
                m_canvas->reportControlChange(this);
            }
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

    std::shared_ptr<Control> Control::getChild()
    {
        return nullptr;
    }
    std::shared_ptr<const Control> Control::getChild() const
    {
        return nullptr;
    }

    std::vector<std::shared_ptr<Control> > Control::getChilds()
    {
        return { };
    }
    std::vector<std::shared_ptr<const Control> > Control::getChilds() const
    {
        return {};
    }
    void Control::forEachChild(std::function<bool(std::shared_ptr<Control>, size_t)> )
    {
    }
    void Control::forEachChild(std::function<bool(std::shared_ptr<const Control>, size_t)> ) const
    {
    }
    void Control::forEachChild(size_t , size_t , std::function<bool(std::shared_ptr<Control>, size_t)> )
    {
    }
    void Control::forEachChild(size_t , size_t , std::function<bool(std::shared_ptr<const Control>, size_t)> ) const
    {
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
        auto parent = m_parent.lock();
        if (!parent)
        {
            return;
        }

        m_parent.reset();
        parent->remove(*this);
    }

    void Control::onActiveChange(bool)
    {
    }

    Bounds2f Control::getAvailableBounds() const
    {
        return m_availableBounds;
    }

    Bounds2f Control::getBounds() const
    {
        return m_bounds;
    }

    const Bounds2f & Control::setBounds(const Bounds2f & bounds)
    {
        if (bounds != m_bounds)
        {
            m_bounds = bounds;
        
            if (m_canvas)
            {
                m_canvas->reportControlChange(this);
            }
        }

        if (!GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_RESIZING))
        {
            GUISE_CONTROL_SET_FLAG(GUISE_CONTROL_FLAG_RESIZING);
            m_availableBounds = m_bounds;
            onResize();            
            GUISE_CONTROL_UNSET_FLAG(GUISE_CONTROL_FLAG_RESIZING);
        }

        return m_bounds;
    }

    float Control::getScale() const
    {
        return m_canvas ? m_canvas->getScale() : 1.0f;
    }

    void Control::resize()
    {
        if (m_canvas)
        {
            m_canvas->resizeControl(this);
        }
        /*m_bounds = m_givenBounds;

        if (m_canvas)
        {
            m_canvas->reportControlChange(this);
        }

        if (!GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_RESIZING))
        {
            GUISE_CONTROL_SET_FLAG(GUISE_CONTROL_FLAG_RESIZING);
            onResize();
            GUISE_CONTROL_UNSET_FLAG(GUISE_CONTROL_FLAG_RESIZING);
        }*/
    }

    bool Control::isChildBoundsAware() const
    {
        return GUISE_CONTROL_CHECK_FLAG(GUISE_CONTROL_FLAG_CHILDBOUNDSAWARE);
    }

    void Control::setChildBoundsAware(const bool aware)
    {
        if (aware)
        {
            GUISE_CONTROL_SET_FLAG(GUISE_CONTROL_FLAG_CHILDBOUNDSAWARE);
        }
        else
        {
            GUISE_CONTROL_UNSET_FLAG(GUISE_CONTROL_FLAG_CHILDBOUNDSAWARE);
        }
    }

    void Control::onUpdate()
    {
    }
    void Control::onAddChild(Control &, const size_t)
    {
    }
    void Control::onCanvasChange(Canvas *)
    {
    }
    void Control::onDisable()
    {
    }
    void Control::onEnable()
    {
    }
    void Control::onRemoveChild(Control &, const size_t)
    {
    }
    void Control::onRender(RendererInterface &)
    {
    }
    void Control::onResize()
    {
    }

    Bounds2f Control::scale(const Bounds2f & bounds) const
    {
        return Bounds2f::ceil(bounds * getScale());
    }

    Vector4f Control::scale(const Vector4f & vector) const
    {
        return Vector4f::ceil(vector * getScale());
    }
    Vector2f Control::scale(const Vector2f & vector) const
    {
        return Vector2f::ceil(vector * getScale());
    }

    void Control::update()
    {
        if (m_canvas)
        {
            m_canvas->updateControl(this);
        }
    }

    void Control::setCanvas(Canvas * canvas)
    {
        if (m_canvas != canvas)
        {
            if (m_canvas)
            {
                m_canvas->reportControlRemove(this);
            }

            m_canvas = canvas;
            onCanvasChange(m_canvas);
        }
    }
    
    /*
    Bounds2f Control::calcRenderBounds(const Style::RectStyle & style) const
    {
        const float scale = m_canvas.getScale();
        const Vector2f newPos = style.getPosition() * scale;
        const Vector2f newSize = style.getSize() * scale;

        const bool clamp = style.getOverflow() == Style::Property::Overflow::Hidden;
        Bounds2f bounds = { m_canvasBounds.position + newPos, newSize };

        if (clamp)
        {
            bounds.position = Vector2f::clamp(bounds.position, m_canvasBounds.position,
                                              Vector2f::max(m_canvasBounds.position, m_canvasBounds.position + m_canvasBounds.size));
        }

        Vector2f lower = bounds.position;
        Vector2f higherCanvas = m_canvasBounds.position + m_canvasBounds.size;
        Vector2f higherThis = bounds.position + Vector2f::max({ 0.0f, 0.0f }, newSize);

        if ((clamp && higherThis.x > higherCanvas.x) || newSize.x <= 0.0f)
        {
            bounds.size.x = higherCanvas.x - lower.x;
        }
        if ((clamp && higherThis.y > higherCanvas.y) || newSize.y <= 0.0f)
        {
            bounds.size.y = higherCanvas.y - lower.y;
        }

        bounds.size.x = std::max(0.0f, bounds.size.x);
        bounds.size.y = std::max(0.0f, bounds.size.y);

        bounds.position.x = std::floor(bounds.position.x);
        bounds.position.y = std::floor(bounds.position.y);
        bounds.size.x = std::floor(bounds.size.x);
        bounds.size.y = std::floor(bounds.size.y);

        return bounds;
    }

    Bounds2f Control::calcChildRenderBounds(const Style::ParentStyle & style) const
    {
        const auto paddingLow = style.getPaddingLow() * getCanvasScale();
        const auto paddingHigh = style.getPaddingHigh() * getCanvasScale();
        return Bounds2f(m_canvasBounds.position + paddingLow, m_canvasBounds.size - paddingLow - paddingHigh);
    }

    Bounds2f Control::getCanvasBounds() const
    {
        return m_canvasBounds;
    }

    float Control::getCanvasScale() const
    {
        return m_canvas.getScale();
    }*/


    // ControlContainer implementations.
    ControlContainer::~ControlContainer()
    { }

    void ControlContainer::adoptControl(Control & control)
    {
        control.setCanvas(m_canvas);
        control.release();
        control.m_parent = Control::shared_from_this();
    }

    void ControlContainer::releaseControl(Control & control)
    {
        if (m_canvas)
        {
            m_canvas->reportControlRemove(&control);
        }
        control.m_parent.reset();
    }

    void ControlContainer::setCanvas(Canvas * canvas)
    {
        Control::setCanvas(canvas);
    }

    void ControlContainer::setControlCanvas(Control * control)
    {
        control->setCanvas(m_canvas);
    }


    // ControlContainerSingle implementations.
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

    std::shared_ptr<Control> ControlContainerSingle::getChild()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_child;
    }
    std::shared_ptr<const Control> ControlContainerSingle::getChild() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_child;
    }

    std::vector<std::shared_ptr<Control> > ControlContainerSingle::getChilds()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return { m_child };
    }
    std::vector<std::shared_ptr<const Control> > ControlContainerSingle::getChilds() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return { m_child };
    }

    void ControlContainerSingle::forEachChild(std::function<bool(std::shared_ptr<Control>, size_t)> callback)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_child)
        {
            return;
        }

        callback(m_child, 0);
    }
    void ControlContainerSingle::forEachChild(std::function<bool(std::shared_ptr<const Control>, size_t)> callback) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_child)
        {
            return;
        }

        callback(m_child, 0);
    }

    void ControlContainerSingle::forEachChild(size_t position, size_t count, std::function<bool(std::shared_ptr<Control>, size_t)> callback)
    {
        if (position != 0 || count != 1)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_child)
        {
            return;
        }

        callback(m_child, 0);
    }
    void ControlContainerSingle::forEachChild(size_t position, size_t count, std::function<bool(std::shared_ptr<const Control>, size_t)> callback) const
    {
        if (position != 0 || count != 1)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_child)
        {
            return;
        }

        callback(m_child, 0);
    }

    void ControlContainerSingle::enable()
    {
        if (!isEnabled())
        {
            Control::enable();

            std::lock_guard<std::mutex> lock(m_mutex);

            if (m_child)
            {
                m_child->enable();
            }
        }
    }

    void ControlContainerSingle::disable()
    {
        if (isEnabled())
        {
            Control::disable();

            std::lock_guard<std::mutex> lock(m_mutex);

            if (m_child)
            {
                m_child->disable();
            }
        }
    }

    bool ControlContainerSingle::add(const std::shared_ptr<Control> & control, const size_t)
    {
        if (!control)
        {
            return false;
        }

        removeAll();

        std::shared_ptr<Control> newChild;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            adoptControl(*control.get());
            m_child = control;

            size_t level = getLevel();
            if (level != 0)
            {
                m_child->setLevel(level + 1);
            }
            newChild = m_child;
        }

        onAddChild(*newChild, 0);

        return true;
    }

    bool ControlContainerSingle::remove(Control & control)
    {
        std::shared_ptr<Control> oldChild;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (!m_child || m_child.get() != &control)
            {
                return false;
            }

            oldChild = m_child;
            releaseControl(*oldChild.get());
            m_child->setLevel(0);
            m_child.reset();
        }
        
        onRemoveChild(*oldChild, 0);

        return true;
    }

    bool ControlContainerSingle::remove(const std::shared_ptr<Control> & control)
    {
        std::shared_ptr<Control> oldChild;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (!m_child || !control || m_child != control)
            {
                return false;
            }

            oldChild = m_child;
            releaseControl(*oldChild.get());
            m_child.reset();
        }
        
        onRemoveChild(*oldChild, 0);

        return true;
    }

    bool ControlContainerSingle::remove(const size_t)
    {
        std::shared_ptr<Control> oldChild;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (!m_child)
            {
                return false;
            }

            oldChild = m_child;
            releaseControl(*oldChild.get());
            m_child.reset();
        }
        
        onRemoveChild(*oldChild, 0);

        return true;
    }

    size_t ControlContainerSingle::removeAll()
    {
        std::shared_ptr<Control> oldChild;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (!m_child)
            {
                return 0;
            }

            oldChild = m_child;
            releaseControl(*oldChild.get());
            m_child.reset();
        }
        
        onRemoveChild(*oldChild, 0);

        return 1;
    }

    void ControlContainerSingle::setCanvas(Canvas * canvas)
    {
        ControlContainer::setCanvas(canvas);

        {
            std::lock_guard<std::mutex> lock(m_mutex);
         
            if (m_child)
            {
                setControlCanvas(m_child.get());
            }
        }
    }


    // ControlContainerList implementations.
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

    std::shared_ptr<Control> ControlContainerList::getChild()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_childs.size() ? m_childs[0] : nullptr;
    }
    std::shared_ptr<const Control> ControlContainerList::getChild() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_childs.size() ? m_childs[0] : nullptr;
    }

    std::vector<std::shared_ptr<Control> > ControlContainerList::getChilds()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_childs;
    }
    std::vector<std::shared_ptr<const Control> > ControlContainerList::getChilds() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return { m_childs.begin(), m_childs.end() };
    }

    void ControlContainerList::forEachChild(std::function<bool(std::shared_ptr<Control>, size_t)> callback)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        size_t index = 0;
        for (auto & child : m_childs)
        {
            if (!callback(child, index))
            {
                break;
            }
            index++;
        }
    }
    void ControlContainerList::forEachChild(std::function<bool(std::shared_ptr<const Control>, size_t)> callback) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        size_t index = 0;
        for (auto & child : m_childs)
        {
            if (!callback(child, index))
            {
                break;
            }
            index++;
        }
    }

    void ControlContainerList::forEachChild(size_t position, size_t count, std::function<bool(std::shared_ptr<Control>, size_t)> callback)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (count == 0 || position >= m_childs.size())
        {
            return;
        }

        auto start = m_childs.begin() + position;
        auto end = m_childs.begin() + std::min(position + (count), m_childs.size());

        size_t index = 0;
        for (auto it = start; it != end; it++)
        {
            if (!callback(*it, index))
            {
                break;
            }
            index++;
        }
    }

    void ControlContainerList::forEachChild(size_t position, size_t count, std::function<bool(std::shared_ptr<const Control>, size_t)> callback) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (count == 0 || position >= m_childs.size())
        {
            return;
        }

        auto start = m_childs.begin() + position;
        auto end = m_childs.begin() + std::min(position + (count), m_childs.size());

        size_t index = 0;
        for (auto it = start; it != end; it++)
        {
            if (!callback(*it, index))
            {
                break;
            }
            index++;
        }
    }

    

    void ControlContainerList::enable()
    {
        if (!isEnabled())
        {
            Control::enable();

            std::lock_guard<std::mutex> lock(m_mutex);

            for (auto & child : m_childs)
            {
                child->enable();
            }
        }
    }

    void ControlContainerList::disable()
    {
        if (isEnabled())
        {
            Control::disable();

            std::lock_guard<std::mutex> lock(m_mutex);

            for (auto & child : m_childs)
            {
                child->disable();
            }
        }        
    }

    bool ControlContainerList::add(const std::shared_ptr<Control> & control, const size_t index)
    {
        size_t newIndex = index;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (!control)
            {
                return false;
            }

            adoptControl(*control.get());
            if (index >= m_childs.size())
            {
                newIndex = m_childs.size();
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
        }

        onAddChild(*control, newIndex);

        return true;
    }

    bool ControlContainerList::remove(Control & control)
    {
        std::shared_ptr<Control> foundControl;
        size_t index = 0;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            for (auto it = m_childs.begin(); it != m_childs.end(); it++)
            {
                if (it->get() == &control)
                {
                    foundControl = *it;
                    releaseControl(*it->get());
                    m_childs.erase(it);
                    break;
                }
                index++;
            }
        }

        if (foundControl)
        {
            onRemoveChild(*foundControl, index);
            return true;
        }

        return false;
    }
    bool ControlContainerList::remove(const std::shared_ptr<Control> & control)
    {
        std::shared_ptr<Control> foundControl;
        size_t index = 0;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            for (auto it = m_childs.begin(); it != m_childs.end(); it++)
            {
                if (*it == control)
                {
                    foundControl = *it;
                    releaseControl(*control);
                    m_childs.erase(it);
                    break;
                }
                index++;
            }
        }

        if (foundControl)
        {
            onRemoveChild(*foundControl, index);
            return true;
        }

        return false;
    }

    bool ControlContainerList::remove(const size_t index)
    {
        std::shared_ptr<Control> foundControl;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (index >= m_childs.size())
            {
                return false;
            }

            foundControl = m_childs.at(index);
            releaseControl(*foundControl);
            m_childs.erase(m_childs.begin() + index);
        }


        onRemoveChild(*foundControl, index);

        return true;
    }

    size_t ControlContainerList::removeAll()
    {
        std::vector<std::shared_ptr<Control> > oldChilds;

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            oldChilds = m_childs;

            for (auto & child : m_childs)
            {
                releaseControl(*child);
            }
            m_childs.clear();
        }

        size_t index = 0;
        for (auto & child : oldChilds)
        {
            onRemoveChild(*child, index);
            index++;
        }
        
        return oldChilds.size();
    }

    void ControlContainerList::setCanvas(Canvas * canvas)
    {
        ControlContainer::setCanvas(canvas);

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            for (auto & child : m_childs)
            {
                setControlCanvas(child.get());
            }
        }
    }

}