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

#include "guise/control/checkbox.hpp"
#include "guise/canvas.hpp"
#include <iostream>

namespace Guise
{

    // Button implementations.
    std::shared_ptr<Checkbox> Checkbox::create()
    {
        return std::shared_ptr<Checkbox>(new Checkbox());
    }

    Style::PaintRectStyle & Checkbox::getCheckedStyle()
    {
        return m_styleChecked;
    }
    const Style::PaintRectStyle & Checkbox::getCheckedStyle() const
    {
        return m_styleChecked;
    }

    Style::PaintRectStyle & Checkbox::getCheckedHoverStyle()
    {
        return m_styleCheckedHover;
    }
    const Style::PaintRectStyle & Checkbox::getCheckedHoverStyle() const
    {
        return m_styleCheckedHover;
    }

    Style::PaintRectStyle & Checkbox::getCheckedDisabledStyle()
    {
        return m_styleCheckedDisabled;
    }
    const Style::PaintRectStyle & Checkbox::getCheckedDisabledStyle() const
    {
        return m_styleCheckedDisabled;
    }

    Style::PaintRectStyle & Checkbox::getDisabledStyle()
    {
        return m_styleDisabled;
    }
    const Style::PaintRectStyle & Checkbox::getDisabledStyle() const
    {
        return m_styleDisabled;
    }

    Style::PaintRectStyle & Checkbox::getHoverStyle()
    {
        return m_styleHover;
    }
    const Style::PaintRectStyle & Checkbox::getHoverStyle() const
    {
        return m_styleHover;
    }

    ControlType Checkbox::getType() const
    {
        return ControlType::Checkbox;
    }

    bool Checkbox::handleInputEvent(const Input::Event & e)
    {
        switch (e.type)
        {
            case Input::EventType::MouseRelease:
            {
                if (e.button != 0)
                {
                    break;
                }

                if (getBounds().intersects(e.position))
                {
                    m_checked = !m_checked;

                    if (m_checked)
                    {
                        setCurrentStyle(m_styleChecked);
                    }
                    else
                    {
                        setCurrentStyle(this);
                    }

                }

                update();
            }
            break;
            default: break;
        }

        return true;
    }

    Checkbox::Checkbox() :
        MultiStyleControl<Style::PaintRectStyle>(this),
        m_checked(false),
        m_styleChecked(this, this),
        m_styleCheckedHover(this, this),
        m_styleCheckedDisabled(this, this),
        m_styleDisabled(this, this),
        m_styleHover(this, this)
    {
    }

    void Checkbox::onCanvasChange(Canvas * canvas)
    {
        updateEmptyProperties(canvas->getStyleSheet()->getSelector("checkbox"));
        m_styleChecked.updateEmptyProperties(canvas->getStyleSheet()->getSelector("checkbox:checked"));
        m_styleCheckedHover.updateEmptyProperties(canvas->getStyleSheet()->getSelector("checkbox:checked:hover"));
        m_styleCheckedDisabled.updateEmptyProperties(canvas->getStyleSheet()->getSelector("checkbox:checked:disabled"));
        m_styleDisabled.updateEmptyProperties(canvas->getStyleSheet()->getSelector("checkbox:disabled"));
        m_styleHover.updateEmptyProperties(canvas->getStyleSheet()->getSelector("checkbox:hover"));
    }

    void Checkbox::onDisable()
    {
        setCurrentStyle(m_styleDisabled);
    }

    void Checkbox::onEnable()
    {
        setCurrentStyle(this);
    }

    void Checkbox::onRender(RendererInterface & rendererInterface)
    {
        rendererInterface.drawRect(getBounds(), getCurrentStyle());
    }

    void Checkbox::onResize()
    {
        setBounds(calcStyledBounds(getCurrentStyle(), getBounds(), getScale()));
    }

}