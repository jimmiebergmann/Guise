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

    bool Button::add(const std::shared_ptr<Control> & control, const size_t)
    {
        if (ControlContainerSingle::add(control))
        {
            control->disableInput();
            return true;
        }

        return false;
    }


    bool Button::handleInputEvent(const Input::Event & e)
    {
        if (!m_enabled)
        {
            return false;
        }               

        switch (e.type)
        {   
            case Input::EventType::MouseMove:          
            {    
                if (!m_pressed)
                {
                    if (m_renderBounds.intersects(e.position))
                    {
                        m_currentStyle = &m_hoverStyle;
                        onHover(e.position);
                    }
                    else
                    {
                        m_currentStyle = this;
                    }

                    forceUpdate();
                }     
            }
            break;
            case Input::EventType::MouseJustPressed:
            {
                if (e.button != 0)
                {
                    break;
                }

                if (m_renderBounds.intersects(e.position))
                {
                    m_pressed = true;
                    m_currentStyle = &m_activeStyle;

                    onPressed(e.position); 
                    forceUpdate();
                }        
            }
            break;
            case Input::EventType::MouseRelease:
            {
                if (e.button != 0)
                {
                    break;
                }

                m_pressed = false;

                if (m_renderBounds.intersects(e.position))
                {
                    m_currentStyle = &m_hoverStyle;
                    onReleased(e.position);   
                }
                else
                {
                    m_currentStyle = this;
                }

                forceUpdate();
            }
            break;
            default: break;
        }

        return true;
    }

    void Button::render(RendererInterface & renderer)
    {
        renderer.drawQuad(m_renderBounds, m_currentStyle->getBackgroundColor());

        const bool drawBorder = m_currentStyle->getBorderStyle() != Style::Property::BorderStyle::None && m_currentStyle->getBorderWidth() > 0.0f;
        if (drawBorder)
        {
            const float scale = m_canvas.getScale();
            const float borderWidth = std::floor(m_currentStyle->getBorderWidth() * scale);
            renderer.drawBorder(m_renderBounds, borderWidth, m_currentStyle->getBorderColor());
        } 
    }

    Bounds2f Button::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f Button::getSelectBounds() const
    {
        return m_renderBounds;
    }

    ControlType Button::getType() const
    {
        return ControlType::Button;
    }

    void Button::update()
    {
        if (!m_enabled && m_currentStyle != &m_disabledStyle)
        {
            m_currentStyle = &m_disabledStyle;
        }

        Bounds2f renderBounds = calcRenderBounds(*m_currentStyle);
        if (renderBounds != m_renderBounds || isUpdateForced())
        {
            m_renderBounds = { renderBounds.position, renderBounds.size };
            m_childBounds = calcChildRenderBounds(*m_currentStyle);

            if (auto child = getChilds()[0])
            {
                child->update(m_childBounds);
            }
        }

        m_canvas.updateControlRendering(this);
    }
   
    Style::ParentPaintRectStyle & Button::getActiveStyle()
    {
        return m_activeStyle;
    }

    Style::ParentPaintRectStyle & Button::getDisabledStyle()
    {
        return m_disabledStyle;
    }

    Style::ParentPaintRectStyle & Button::getHoverStyle()
    {
        return m_hoverStyle;
    }

    Button::Button(std::shared_ptr<Canvas> & canvas) :
        ControlContainerSingle(*canvas),
        Style::ParentPaintRectStyle(canvas->getStyleSheet()->getSelector("button")),
        m_childBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),     
        m_currentStyle(this),
        m_activeStyle(this),
        m_disabledStyle(this),
        m_hoverStyle(this),      
        m_pressed(false)
    {
        if (auto activeStyle = canvas->getStyleSheet()->getSelector("button:active"))
        {
            m_activeStyle = { activeStyle, this };
        }
 
        if (auto disabledStyle = canvas->getStyleSheet()->getSelector("button:disabled"))
        {
            m_disabledStyle = { disabledStyle, this };
        }

        if (auto hoverStyle = canvas->getStyleSheet()->getSelector("button:hover"))
        {
            m_hoverStyle = { hoverStyle, this };
        }
    }
}