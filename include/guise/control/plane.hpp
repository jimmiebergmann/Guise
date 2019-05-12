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

#ifndef GUISE_CONTROL_PLANE_HPP
#define GUISE_CONTROL_PLANE_HPP

#include "guise/control.hpp"

namespace Guise
{

    class GUISE_API Plane : public Style, public ControlContainerList
    {

    public:

        static std::shared_ptr<Plane> create(Canvas & canvas);

        ControlType getType() const;

        bool handleInputEvent(const Input::Event & event);

        void update(const Vector2f & availablePosition, const Vector2f & availableSize);

        void render(RendererInterface & rendererInterface);

        Vector4f getSelectBounds() const;

    private:

        Plane(Canvas & canvas);
        Plane(const Plane &) = delete;

        Vector4f m_bounds;

    };

}

#endif