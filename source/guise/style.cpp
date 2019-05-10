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

#include "guise/style.hpp"

namespace Guise
{

    Style::Style() :
        m_position(0.0f, 0.0f),
        m_size(0.0f, 0.0f),
        m_padding(0.0f, 0.0f, 0.0f, 0.0f)
    { }

    const Vector2f & Style::getPosition() const
    {
        return m_position;
    }

    void Style::setPosition(const Vector2f & position)
    {
        m_position = position;
    }

    const Vector2f & Style::getSize() const
    {
        return m_size;
    }

    void Style::setSize(const Vector2f & size)
    {
        m_size = size;
    }

    const Vector4f & Style::getPadding() const
    {
        return m_padding;
    }

    Vector2f Style::getPaddingLow() const
    {
        return { m_padding.x, m_padding.y };
    }

    Vector2f Style::getPaddingHigh() const
    {
        return { m_padding.z, m_padding.w };
    }

    void Style::setPadding(const float padding)
    {
        m_padding = { padding, padding, padding, padding };
    }

    void Style::setPadding(const Vector2f & padding)
    {
        m_padding = { padding.x, padding.y, padding.x, padding.y };
    }

    void Style::setPadding(const Vector4f & padding)
    {
        m_padding = padding;
    }

    void Style::setPaddingLow(const Vector2f & low)
    {
        m_padding.x = low.x;
        m_padding.y = low.y;
    }

    void Style::setPaddingHigh(const Vector2f & high)
    {
        m_padding.z = high.x;
        m_padding.w = high.y;
    }
  

}