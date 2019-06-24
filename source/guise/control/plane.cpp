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

#include "guise/control/plane.hpp"
#include "guise/canvas.hpp"
#include <iostream>

namespace Guise
{
    // Plane style implementations.
    PlaneStyle::PlaneStyle() :
        m_position(0.0f, 0.0f),
        m_size(0.0f, 0.0f),
        m_padding(0.0f, 0.0f, 0.0f, 0.0f),
        m_overflow(Style::Property::Overflow::hidden)
    { }

    PlaneStyle::PlaneStyle(const std::shared_ptr<Style::Selector> & selector) :
        PlaneStyle()
    {
        if (!selector)
        {
            return;
        }

        auto & properties = selector->getProperties();
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            if (it->first == "position")
            {
                m_position = it->second->getVector2f();
            }
            else if (it->first == "size")
            {
                m_size = it->second->getVector2f();
            }
            else if (it->first == "padding")
            {
                switch (it->second->getDataType())
                {
                    case Style::Property::DataType::Float:
                    {
                        m_padding.x = m_padding.y = m_padding.w = m_padding.z = it->second->getFloat();
                    }
                    break;
                    case Style::Property::DataType::Vector2f:
                    {
                        m_padding = { it->second->getVector2f(), 0.0f, 0.0f };
                    }
                    break;
                    case Style::Property::DataType::Vector4f:
                    {
                        m_padding = it->second->getVector4f();
                    }
                    break;
                    default: break;
                }
            }
            else if (it->first == "overflow")
            {
                m_overflow = it->second->getOverflow();
            }          
        }
    }

    
    const Vector2f & PlaneStyle::getPosition() const
    {
        return m_position;
    }
    const Vector2f & PlaneStyle::getSize() const
    {
        return m_size;
    }
    const Vector4f & PlaneStyle::getPadding() const
    {
        return m_padding;
    }
    const Vector2f PlaneStyle::getPaddingLow() const
    {
        return { m_padding.x, m_padding.y };
    }
    const Vector2f PlaneStyle::getPaddingHigh() const
    {
        return { m_padding.z, m_padding.w };
    }
    Style::Property::Overflow PlaneStyle::getOverflow() const
    {
        return m_overflow;
    }

    void PlaneStyle::setPosition(const Vector2f & position)
    {
        m_position = position;
    }
    void PlaneStyle::setSize(const Vector2f & size)
    {
        m_size = size;
    }
    void PlaneStyle::setPadding(const Vector4f & padding)
    {
        m_padding = padding;
    }
    void PlaneStyle::setOverflow(const Style::Property::Overflow overflow)
    {
        m_overflow = overflow;
    }


    // Plane implementations.
    std::shared_ptr<Plane> Plane::create(Canvas & canvas)
    {
        return std::shared_ptr<Plane>(new Plane(canvas));
    }

    ControlType Plane::getType() const
    {
        return ControlType::Plane;
    }

    bool Plane::handleInputEvent(const Input::Event & /*e*/)
    {
        return false;
    }

    void Plane::update(const Bounds2f & canvasBound)
    {
        const bool childsUpdate = pollUpdateForced();
        if (canvasBound != m_renderBounds || childsUpdate)
        {
            Bounds2f renderBounds = calcRenderBounds(canvasBound, m_position, m_size, m_overflow);
            if (renderBounds != m_renderBounds || childsUpdate)
            {
                m_renderBounds = renderBounds;
                m_childsBounds = Bounds2f(m_renderBounds.position + getPaddingLow(), m_renderBounds.size - getPaddingLow() - getPaddingHigh());
            }
        }

        auto childs = getChilds();
        for (auto it = childs.begin(); it != childs.end(); it++)
        {
            (*it)->update(m_childsBounds);
        }
    }

    Bounds2f Plane::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f Plane::getSelectBounds() const
    {
        return m_renderBounds;
    }

    Plane::Plane(Canvas & canvas) :       
        PlaneStyle(canvas.getStyleSheet()->getSelector("plane")),
        ControlContainerList(canvas),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_childsBounds(0.0f, 0.0f, 0.0f, 0.0f)
    { }

}