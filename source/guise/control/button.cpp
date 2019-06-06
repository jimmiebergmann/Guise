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
  
    // Button implementations.
    std::shared_ptr<Button> Button::create(std::shared_ptr<Canvas> & canvas)
    {
        return std::shared_ptr<Button>(new Button(canvas));
    }

    ControlType Button::getType() const
    {
        return ControlType::Button;
    }

    Control * Button::handleInputEvent(const Input::Event & e)
    {
        switch (e.type)
        {
            case Input::EventType::MouseMove:          
            {
                if (m_renderBounds.intersects(e.position))
                {
                    m_currentStyle = &m_hoverStyle;
                }
                else
                {
                    m_currentStyle = this;
                }
            }
            break;
            case Input::EventType::MouseJustPressed:    onPressed(e.position); break;
            case Input::EventType::MouseDoubleClick:    onPressed(e.position); break;
            default: break;
        }

        return this;
    }

    void Button::update(const Bounds2f & canvasBound)
    {
        const bool childsUpdate = pollUpdateForced();
        if (canvasBound != m_renderBounds || childsUpdate)
        {
            Bounds2f renderBounds = calcRenderBounds(canvasBound, m_position, m_size, m_overflow);
            if (renderBounds != m_renderBounds || childsUpdate)
            {
                m_renderBounds = renderBounds;

                m_childBounds = Bounds2f(m_renderBounds.position + getPaddingLow(), m_renderBounds.size - (getPaddingHigh() * 2.0f));

                /*if (auto child = getChilds()[0])
                {
                    const Bounds2f childBounds(m_renderBounds.position + getPaddingLow(), m_renderBounds.size - (getPaddingHigh() * 2.0f));

                    if (childBounds != m_childBounds)
                    {
                        m_childBounds = childBounds;
                        child->update(m_childBounds);
                    }
                }*/
            }
        }

        getCanvas().queueControlRendering(this);

        if (auto child = getChilds()[0])
        {
            child->update(m_childBounds);
        }
    }

    void Button::render(RendererInterface & renderer)
    {
        const bool drawBorder = m_currentStyle->getBorderStyle() != Style::Property::BorderStyle::None && m_currentStyle->getBorderWidth() > 0.0f;
        Vector4f firstColor = drawBorder ? m_currentStyle->getBorderColor() : m_currentStyle->getBackgroundColor();
        renderer.drawQuad(m_renderBounds, firstColor);

        if (drawBorder)
        {
            const auto borderWidth = m_currentStyle->getBorderWidth();
            Bounds2f innerBounds = m_renderBounds;
            innerBounds.position += {borderWidth, borderWidth};
            innerBounds.size -= {borderWidth * 2.0f, borderWidth * 2.0f};

            if (innerBounds.size.x > 0.0f && innerBounds.size.y > 0.0f)
            {
                renderer.drawQuad(innerBounds, m_currentStyle->getBackgroundColor());
            } 
        }

        /*auto childs = getChilds();
        if (childs.size() && childs[0])
        {
            childs[0]->render(renderer);
        }*/
    }

    Bounds2f Button::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f Button::getSelectBounds() const
    {
        return m_renderBounds;
    }

    bool Button::add(const std::shared_ptr<Control> & control, const size_t)
    {
        if (ControlContainerSingle::add(control))
        {
            control->disableInput();
            return true;
        }

        return false;
    }

    Style::BoxStyle & Button::getActiveStyle()
    {
        return m_activeStyle;
    }

    Style::BoxStyle & Button::getDisabledStyle()
    {
        return m_disabledStyle;
    }

    Style::BoxStyle & Button::getHoverStyle()
    {
        return m_hoverStyle;
    }

    Button::Button(std::shared_ptr<Canvas> & canvas) :
        ControlContainerSingle(*canvas),
        Style::BoxStyle(canvas->getStyleSheet()->getSelector("button")),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_childBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_activeStyle(*this),
        m_disabledStyle(*this),
        m_hoverStyle(*this),
        m_currentStyle(this)
    {
        if (auto activeStyle = canvas->getStyleSheet()->getSelector("button:active"))
        {
            m_activeStyle = activeStyle;
        }
 
        if (auto disabledStyle = canvas->getStyleSheet()->getSelector("button:disabled"))
        {
            m_disabledStyle = disabledStyle;
        }

        if (auto hoverStyle = canvas->getStyleSheet()->getSelector("button:hover"))
        {
            m_hoverStyle = hoverStyle;
        }
    }

}