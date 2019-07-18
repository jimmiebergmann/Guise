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
    std::shared_ptr<Checkbox> Checkbox::create(std::shared_ptr<Canvas> & canvas)
    {
        return std::shared_ptr<Checkbox>(new Checkbox(canvas));
    }

    bool Checkbox::handleInputEvent(const Input::Event & e)
    {
        if (!m_enabled)
        {
            return false;
        }

        switch (e.type)
        {       
            case Input::EventType::MouseRelease:
            {
                if (e.button != 0)
                {
                    break;
                }

                if (m_renderBounds.intersects(e.position))
                {
                    m_checked = !m_checked;

                    if (m_checked)
                    {
                        m_currentStyle = &m_checkedStyle;
                    }
                    else
                    {
                        m_currentStyle = this;
                    }

                }

                forceUpdate();
            }
            break;
            default: break;
        }

        return true;
    }

    void Checkbox::render(RendererInterface & renderer)
    {
        renderer.drawQuad(m_renderBounds, m_currentStyle->getBackgroundColor());
    }

    Bounds2f Checkbox::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f Checkbox::getSelectBounds() const
    {
        return m_renderBounds;
    }

    ControlType Checkbox::getType() const
    {
        return ControlType::Checkbox;
    }

    void Checkbox::update()
    {
        Bounds2f renderBounds = calcRenderBounds(*m_currentStyle);
        if (renderBounds != m_renderBounds || isUpdateForced())
        {
            m_renderBounds = { renderBounds.position , renderBounds.size };

            m_canvas.updateControlRendering(this);
        }
    }

    Style::PaintRectStyle & Checkbox::getChekedStyle()
    {
        return m_checkedStyle;
    }

    Style::PaintRectStyle & Checkbox::getChekedHoverStyle()
    {
        return m_checkedHoverStyle;
    }

    Style::PaintRectStyle & Checkbox::getChekedDisabledStyle()
    {
        return m_checkedDisabledStyle;
    }

    Style::PaintRectStyle & Checkbox::getDisabledStyle()
    {
        return m_disabledStyle;
    }

    Style::PaintRectStyle & Checkbox::getHoverStyle()
    {
        return m_hoverStyle;
    }

    Checkbox::Checkbox(std::shared_ptr<Canvas> & canvas) :
        Control(*canvas),
        Style::PaintRectStyle(canvas->getStyleSheet()->getSelector("checkbox")),
        m_checked(false),
        m_checkedStyle(*this),
        m_checkedHoverStyle(*this),
        m_checkedDisabledStyle(*this),
        m_disabledStyle(*this),
        m_hoverStyle(*this),
        m_currentStyle(this),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f)
    {
        if (auto checkedStyle = canvas->getStyleSheet()->getSelector("checkbox:checked"))
        {
            m_checkedStyle = { checkedStyle, this };
        }

        if (auto checkedHoverStyle = canvas->getStyleSheet()->getSelector("checkbox:checked:hover"))
        {
            m_checkedHoverStyle = { checkedHoverStyle, this };
        }

        if (auto checkedDisabledStyle = canvas->getStyleSheet()->getSelector("checkbox:checked:disabled"))
        {
            m_checkedDisabledStyle = { checkedDisabledStyle, this };
        }

        if (auto disabledStyle = canvas->getStyleSheet()->getSelector("checkbox:disabled"))
        {
            m_disabledStyle = { disabledStyle, this };
        }

        if (auto hoverStyle = canvas->getStyleSheet()->getSelector("checkbox:hover"))
        {
            m_hoverStyle = { hoverStyle, this };
        }
    }

}