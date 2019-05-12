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

    void Button::update(const Vector2f & availablePosition, const Vector2f & availableSize)
    {
        m_renderPosition = getPosition();
        m_renderPosition.x = m_renderPosition.x > 0.0f ? m_renderPosition.x : 0.0f;
        m_renderPosition.y = m_renderPosition.y > 0.0f ? m_renderPosition.y : 0.0f;

        m_renderSize = getSize();
        m_renderSize.x = m_renderSize.x > 0.0f ? (m_renderSize.x + m_renderPosition.x > availableSize.x ? availableSize.x - m_renderPosition.x : m_renderSize.x) : availableSize.x - m_renderPosition.x;
        m_renderSize.y = m_renderSize.y > 0.0f ? (m_renderSize.y + m_renderPosition.y > availableSize.y ? availableSize.y - m_renderPosition.y : m_renderSize.y) : availableSize.y - m_renderPosition.y;

        m_renderPosition += availablePosition;

        Vector4f bounds = { m_renderPosition, m_renderPosition };
        bounds.z += m_renderSize.x;
        bounds.w += m_renderSize.y;

        if (bounds != m_bounds)
        {
            m_bounds = bounds;
            getCanvas().registerControlBoundsChange(*this, m_bounds);
        }

        auto child = getChilds()[0];
        if (child)
        {

            m_childAvailablePosition = m_renderPosition + getPaddingLow();
            m_childAvailableSize = m_renderSize - getPaddingLow() - getPaddingHigh();
            
            if (m_childAvailableSize.x > 0.0f && m_childAvailableSize.y > 0.0f)
            {
                child->update(m_childAvailablePosition, m_childAvailableSize);
            }
        }
    }

    void Button::render(RendererInterface & renderer)
    {
        if (m_renderSize.x > 0.0f && m_renderSize.y > 0.0f)
        {
            Vector2f backgroundSize = m_renderSize;
            Vector2f backgroundOffset(0.0f, 0.0f);
            float borderWith = getBorderWidth();
            if (borderWith > 0.0f && getBorder() != Style::BorderStyle::None)
            {
                renderer.drawQuad(m_renderPosition, m_renderSize, getBorderColor());
                backgroundSize -= {2.0f * borderWith, 2.0f * borderWith};
                backgroundOffset += {borderWith, borderWith};
            }
            if (backgroundSize.x > 0.0f && backgroundSize.y > 0.0f)
            {
                renderer.drawQuad(m_renderPosition + backgroundOffset, backgroundSize, getBackgroundColor());
            }
            
            auto child = getChilds()[0];
            if (child)
            {
                child->render(renderer);
            }
        }
     
    }

    Vector4f Button::getSelectBounds() const
    {
        return m_bounds;
    }

    Button::Button(Canvas & canvas) :
        ControlContainerSingle(canvas),
        Style(canvas.getStyleSheet()->getStyle(StyleSheet::Entry::Button)),
        m_bounds(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()),
        m_renderPosition(0.0f, 0.0f),
        m_renderSize(0.0f, 0.0f)
    { }

}