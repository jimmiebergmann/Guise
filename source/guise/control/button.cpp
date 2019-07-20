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
    std::shared_ptr<Button> Button::create()
    {
        return std::shared_ptr<Button>(new Button());
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
        switch (e.type)
        {   
            case Input::EventType::MouseMove:          
            {    
                if (!m_pressed)
                {
                    if (getBounds().intersects(e.position))
                    {
                        setCurrentStyle(m_styleHover);
                        onHover(e.position);
                    }
                    else
                    {
                        setCurrentStyle(this);
                    }
                }     
            }
            break;
            case Input::EventType::MouseJustPressed:
            {
                if (e.button != 0)
                {
                    break;
                }

                if (getBounds().intersects(e.position))
                {
                    m_pressed = true;
                    setCurrentStyle(m_styleActive);

                    onPress(e.position); 
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

                if (getBounds().intersects(e.position))
                {
                    setCurrentStyle(m_styleHover);
                    onRelease(e.position);   
                }
                else
                {
                    setCurrentStyle(this);
                }
            }
            break;
            default: break;
        }

        return true;
    }

    ControlType Button::getType() const
    {
        return ControlType::Button;
    }

    Style::ParentPaintRectStyle & Button::getStyleActive()
    {
        return m_styleActive;
    }
    const Style::ParentPaintRectStyle & Button::getStyleActive() const
    {
        return m_styleActive;
    }

    Style::ParentPaintRectStyle & Button::getStyleDisabled()
    {
        return m_styleHover;
    }
    const Style::ParentPaintRectStyle & Button::getStyleDisabled() const
    {
        return m_styleHover;
    }

    Style::ParentPaintRectStyle & Button::getStyleHover()
    {
        return m_styleHover;
    }
    const Style::ParentPaintRectStyle & Button::getStyleHover() const
    {
        return m_styleHover;
    }

    Button::Button() :
        Style::MultiStyle<Style::ParentPaintRectStyle>(this),
        m_pressed(false),
        m_styleActive(this, this),
        m_styleDisabled(this, this),
        m_styleHover(this, this)
    {
    }

    void Button::onAddChild(Control & control, const size_t)
    {
        control.setBounds(getBounds().cutEdges(scale(getCurrentStyle().getPadding())));
    }

    void Button::onCanvasChange(Canvas * canvas)
    {
        updateEmptyProperties(canvas->getStyleSheet()->getSelector("button"));
        m_styleActive.updateEmptyProperties(canvas->getStyleSheet()->getSelector("button:active"));
        m_styleDisabled.updateEmptyProperties(canvas->getStyleSheet()->getSelector("button:disabled"));
        m_styleHover.updateEmptyProperties(canvas->getStyleSheet()->getSelector("button:hover"));
    }

    void Button::onDisable()
    {
        setCurrentStyle(m_styleDisabled);
    }

    void Button::onEnable()
    {
        setCurrentStyle(this);
    }

    void Button::onRender(RendererInterface & rendererInterface)
    {
        rendererInterface.drawRect(getBounds(), getCurrentStyle());

        if (auto child = getChild())
        {
            child->draw(rendererInterface);
        }
    }

    void Button::onResize()
    {
        setBounds(calcStyledBounds(getCurrentStyle(), getBounds(), getScale()));

        if (auto child = getChild())
        {
            child->setBounds(getBounds().cutEdges(scale(getCurrentStyle().getPadding())));
        }
    }

}