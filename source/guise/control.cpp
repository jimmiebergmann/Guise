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

    bool Control::add(std::shared_ptr<Control>, const size_t)
    {
        return false;
    }

    bool Control::remove(Control &)
    {
        return false;
    }

    bool Control::remove(std::shared_ptr<Control>)
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


    // ControlContainer implementations.
    ControlContainer::~ControlContainer()
    { }

    void ControlContainer::adoptControl(Control & control)
    {
        control.release();     
        control.m_parent = Control::shared_from_this();
    }

    void ControlContainer::releaseControl(Control & control)
    {
        control.m_parent.reset();
    }

    
    // ControlContainerList implementations.
    ControlContainerList::~ControlContainerList()
    {
        removeAll();
    }

    std::vector<std::shared_ptr<Control> > ControlContainerList::getChilds()
    {
        return m_childs;
    }
    std::vector<std::shared_ptr<const Control> > ControlContainerList::getChilds() const
    {
        std::vector<std::shared_ptr<const Control> > childs;
        for (auto & child : m_childs)
        {
            childs.push_back(child);
        }
        return childs;
    }

    bool ControlContainerList::add(std::shared_ptr<Control> control, const size_t)
    {
        if (!control)
        {
            return false;
        }

        adoptControl(*control.get());
        m_childs.push_back(control);
        return true;
    }

    bool ControlContainerList::remove(Control & control)
    {
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
    bool ControlContainerList::remove(std::shared_ptr<Control> control)
    {
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
        for (auto & child : m_childs)
        {
            releaseControl(*child.get());
            
        }

        size_t count = m_childs.size();
        m_childs.clear();
        return count;
    }


}