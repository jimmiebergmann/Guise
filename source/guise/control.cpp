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

template <typename T, typename U>
static bool ptrEquals(const std::weak_ptr<T>& t, const std::shared_ptr<U>& u)
{
    return !t.owner_before(u) && !u.owner_before(t);
}


namespace Guise
{

    // Control implementations.
    Control::~Control()
    {
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


    // ControlContainer implementations.
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
    ControlContainerSingle::~ControlContainerSingle()
    {
        removeAll();
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
    ControlContainerList::~ControlContainerList()
    {
        removeAll();
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