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
            Bounds2f renderBounds = calcRenderBounds(canvasBound, getPosition(), getSize(), getOverflow());
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
        Style::ParentRectStyle(canvas.getStyleSheet()->getSelector("plane")),
        ControlContainerList(canvas),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_childsBounds(0.0f, 0.0f, 0.0f, 0.0f)
    { }

}