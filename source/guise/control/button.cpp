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
        std::cout << "Pressed me! " << (int)e.button << ": " <<  e.position.x << " " << e.position.y  << std::endl;
        return true;
    }

    void Button::update(const Bounds2f & canvasBound)
    {
        if (canvasBound != m_renderBounds)
        {
            Bounds2f renderBounds = calcRenderBounds(canvasBound);
            if (renderBounds != m_renderBounds)
            {
                m_renderBounds = renderBounds;
                getCanvas().registerControlBoundsChange(*this, m_renderBounds);
            }
        }

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

    void Button::render(RendererInterface & renderer)
    {
        const float borderWidth = getBorderWidth();
        const bool drawBorder = getBorder() != Style::BorderStyle::None && borderWidth > 0.0f;
        Vector4f firstColor = drawBorder ? getBorderColor() : getBackgroundColor();
        renderer.drawQuad(m_renderBounds, firstColor);

        if (drawBorder)
        {
            Bounds2f innerBounds = m_renderBounds;
            innerBounds.position += {borderWidth, borderWidth};
            innerBounds.size -= {borderWidth * 2.0f, borderWidth * 2.0f};

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

    Button::Button(Canvas & canvas) :
        ControlContainerSingle(canvas),
        Style(canvas.getStyleSheet()->getStyle(StyleSheet::Entry::Button)),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_childBounds(0.0f, 0.0f, 0.0f, 0.0f)
    { }

}