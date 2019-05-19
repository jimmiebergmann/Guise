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

#include "guise/control/button.hpp"
#include "guise/canvas.hpp"
#include <iostream>

namespace Guise
{
    // Button style implementations.
    ButtonStyle::ButtonStyle() :
        m_position(0.0f, 0.0f),
        m_size(0.0f, 0.0f),
        m_padding(0.0f, 0.0f, 0.0f, 0.0f),
        m_overflow(Style::Property::Overflow::hidden)
    { }

    ButtonStyle::ButtonStyle(const std::shared_ptr<Style::Selector> & selector) :
        ButtonStyle()
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
            else if (it->first == "background-color")
            {
                m_backgroundColor = it->second->getVector4f();
            }
            else if (it->first == "border-color")
            {
                m_borderColor = it->second->getVector4f();
            }
            else if (it->first == "border-style")
            {
                m_borderStyle = it->second->getBorderStyle();
            }
            else if (it->first == "border-width")
            {
                m_borderWidth = it->second->getFloat();
            }
        }
    }

    const Vector4f & ButtonStyle::getBackgroundColor() const
    {
        return m_backgroundColor;
    }
    const Vector4f & ButtonStyle::getBorderColor() const
    {
        return m_borderColor;
    }
    Style::Property::BorderStyle ButtonStyle::getBorderStyle() const
    {
        return m_borderStyle;
    }
    float ButtonStyle::getBorderWidth() const
    {
        return m_borderWidth;
    }
    const Vector2f & ButtonStyle::getPosition() const
    {
        return m_position;
    }
    const Vector2f & ButtonStyle::getSize() const
    {
        return m_size;
    }
    const Vector4f & ButtonStyle::getPadding() const
    {
        return m_padding;
    }
    const Vector2f ButtonStyle::getPaddingLow() const
    {
        return { m_padding.x, m_padding.y };
    }
    const Vector2f ButtonStyle::getPaddingHigh() const
    {
        return { m_padding.z, m_padding.w };
    }
    Style::Property::Overflow ButtonStyle::getOverflow() const
    {
        return m_overflow;
    }

    void ButtonStyle::setBackgroundColor(const Vector4f & color)
    {
        m_backgroundColor = color;
    }
    void ButtonStyle::setBorderColor(const Vector4f & color)
    {
        m_borderColor = color;
    }
    void ButtonStyle::setBorderStyle(const Style::Property::BorderStyle borderStyle)
    {
        m_borderStyle = borderStyle;
    }
    void ButtonStyle::setBorderWidth(const float width)
    {
        m_borderWidth = width;
    }
    void ButtonStyle::setPosition(const Vector2f & position)
    {
        m_position = position;
    }
    void ButtonStyle::setSize(const Vector2f & size)
    {
        m_size = size;
    }
    void ButtonStyle::setPadding(const Vector4f & padding)
    {
        m_padding = padding;
    }
    void ButtonStyle::setOverflow(const Style::Property::Overflow overflow)
    {
        m_overflow = overflow;
    }


    // Button implementations.
    std::shared_ptr<Button> Button::create(Canvas & canvas)
    {
        return std::shared_ptr<Button>(new Button(canvas));
    }

    ControlType Button::getType() const
    {
        return ControlType::Button;
    }

    bool Button::handleInputEvent(const Input::Event & e)
    {
        std::cout << "Pressed me! " << this << "  " << (int)e.type << "  " << (int)e.button << ": " <<  e.position.x << " " << e.position.y  << std::endl;

        
        /*if (e.type == Input::EventType::MouseJustPressed)
        {
            m_pressedControlPosition = m_renderBounds.position;
            m_pressedMousePosition = e.position;
        }
        else if (e.type == Input::EventType::MouseMove)
        {
            setPosition(m_pressedControlPosition + (e.position - m_pressedMousePosition));
            update(m_canvasBounds);
        }*/
        return true;
    }

    void Button::update(const Bounds2f & canvasBound)
    {
        //m_canvasBounds = canvasBound;
        if (canvasBound != m_renderBounds)
        {
            Bounds2f renderBounds = calcRenderBounds(canvasBound, m_position, m_size, m_overflow);
            if (renderBounds != m_renderBounds)
            {
                m_renderBounds = renderBounds;
                getCanvas().registerControlBoundsChange(*this, m_renderBounds);

                if (auto child = getChilds()[0])
                {
                    const Bounds2f childBounds(m_renderBounds.position + getPaddingLow(), m_renderBounds.size - (getPaddingHigh() * 2.0f));

                    if (childBounds != m_childBounds)
                    {
                        m_childBounds = childBounds;
                        child->update(m_childBounds);
                    }
                }
            }
        }
    }

    void Button::render(RendererInterface & renderer)
    {
        const bool drawBorder = m_borderStyle != Style::Property::BorderStyle::None && m_borderWidth > 0.0f;
        Vector4f firstColor = drawBorder ? getBorderColor() : getBackgroundColor();
        renderer.drawQuad(m_renderBounds, firstColor);

        if (drawBorder)
        {
            Bounds2f innerBounds = m_renderBounds;
            innerBounds.position += {m_borderWidth, m_borderWidth};
            innerBounds.size -= {m_borderWidth * 2.0f, m_borderWidth * 2.0f};

            if (innerBounds.size.x > 0.0f && innerBounds.size.y > 0.0f)
            {
                renderer.drawQuad(innerBounds, getBackgroundColor());
            } 
        }
    }

    Bounds2f Button::getSelectBounds() const
    {
        return m_renderBounds;
    }

    ButtonStyle & Button::getActiveStyle()
    {
        return m_activeStyle;
    }

    ButtonStyle & Button::getDisabledStyle()
    {
        return m_disabledStyle;
    }

    ButtonStyle & Button::getHoverStyle()
    {
        return m_hoverStyle;
    }

    Button::Button(Canvas & canvas) :
        ControlContainerSingle(canvas),
        ButtonStyle(canvas.getStyleSheet()->getSelector("button")),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_childBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_activeStyle(canvas.getStyleSheet()->getSelector("button:active")),
        m_disabledStyle(canvas.getStyleSheet()->getSelector("button:disabled")),
        m_hoverStyle(canvas.getStyleSheet()->getSelector("button:hover"))
    { }

}