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

namespace Guise
{
    std::shared_ptr<Plane> Plane::create(Canvas & canvas)
    {
        return std::shared_ptr<Plane>(new Plane(canvas));
    }

    ControlType Plane::getType() const
    {
        return ControlType::Plane;
    }

    bool Plane::handleInputEvent(const Input::Event &/* event*/)
    {
        return true;
    }

    void Plane::update(const Vector2f & availablePosition, const Vector2f & availableSize)
    {
        Vector4f bounds = { availablePosition, availablePosition };
        bounds.z += availableSize.x;
        bounds.w += availableSize.y;

        if (bounds != m_bounds)
        {
            m_bounds = bounds;
            getCanvas().registerControlBoundsChange(*this, m_bounds);
        }

        auto childs = getChilds();
        for (auto it = childs.begin(); it != childs.end(); it++)
        {
            (*it)->update(availablePosition, availableSize);
        }
    }

    void Plane::render(RendererInterface & renderer)
    {
        auto childs = getChilds();
        for (auto it = childs.begin(); it != childs.end(); it++)
        {
            (*it)->render(renderer);
        }
    }

    Vector4f Plane::getSelectBounds() const
    {
        return m_bounds;
    }

    Plane::Plane(Canvas & canvas) :
        ControlContainerList(canvas),
        Style(canvas.getStyleSheet()->getStyle(StyleSheet::Entry::Window)),
        m_bounds(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min())
    { }

}