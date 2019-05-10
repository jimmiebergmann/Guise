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

#ifndef GUISE_STYLE_HPP
#define GUISE_STYLE_HPP

#include "guise/build.hpp"


namespace Guise
{

    /**
    * Style class.
    *
    *
    */
    class GUISE_API Style
    {

    public:

        Style();

        const Vector2f & getPosition() const;

        void setPosition(const Vector2f & position);

        const Vector2f & getSize() const;

        void setSize(const Vector2f & size);

        const Vector4f & getPadding() const;
        Vector2f getPaddingLow() const;
        Vector2f getPaddingHigh() const;

        void setPadding(const float padding);
        void setPadding(const Vector2f & padding);
        void setPadding(const Vector4f & padding);
        void setPaddingLow(const Vector2f & low);
        void setPaddingHigh(const Vector2f & high);
      
    private:

        Vector2f   m_position;
        Vector2f   m_size;
        Vector4f   m_padding;

    };

}

#endif