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
   /* std::shared_ptr<Plane> Plane::create(std::shared_ptr<Canvas> & canvas)
    {
        return std::shared_ptr<Plane>(new Plane(canvas));
    }

    Bounds2f Plane::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f Plane::getSelectBounds() const
    {
        return m_renderBounds;
    }

    ControlType Plane::getType() const
    {
        return ControlType::Plane;
    }

    void Plane::update()
    {
        Bounds2f renderBounds = calcRenderBounds(*this);
        if (renderBounds != m_renderBounds || isUpdateForced())
        {
            m_renderBounds = renderBounds;

            m_canvas.reportControlChange(this);

            Bounds2f childsBounds = calcChildRenderBounds(*this);
            if (childsBounds != m_childsBounds || isUpdateForced())
            {
                m_childsBounds = childsBounds;

                for (auto & child : getChilds())
                {
                    child->update(m_childsBounds);
                }
            }
        }
    }

    Plane::Plane(std::shared_ptr<Canvas> & canvas) :
        ControlContainerList(*canvas),
        Style::ParentRectStyle(canvas->getStyleSheet()->getSelector("plane")),    
        m_childsBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f)      
    { }*/

}