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

    class GUISE_API PlaneStyle
    {

    public:

        PlaneStyle();
        PlaneStyle(const std::shared_ptr<Style::Selector> & selector);
      
        const Vector2f & getPosition() const;
        const Vector2f & getSize() const;
        const Vector4f & getPadding() const;
        const Vector2f getPaddingLow() const;
        const Vector2f getPaddingHigh() const;
        Style::Property::Overflow getOverflow() const;
     
        void setPosition(const Vector2f & position);
        void setSize(const Vector2f & size);
        void setPadding(const Vector4f & padding);
        void setOverflow(const Style::Property::Overflow overflow);

    protected:
    
        Vector2f m_position;
        Vector2f m_size;
        Vector4f m_padding;
        Style::Property::Overflow m_overflow;

    };

    class GUISE_API Plane : public PlaneStyle, public ControlContainerList
    {

    public:

        static std::shared_ptr<Plane> create(Canvas & canvas);

        virtual ControlType getType() const;

        virtual bool handleInputEvent(const Input::Event & event);

        virtual void update(const Bounds2f & canvasBound);

        virtual void render(RendererInterface & renderer);

        virtual Bounds2f getRenderBounds() const;

        virtual Bounds2f getSelectBounds() const;

    private:

        Plane(Canvas & canvas);
        Plane(const Plane &) = delete;

        Bounds2f m_renderBounds;
        Bounds2f m_childsBounds;

    };

}

#endif