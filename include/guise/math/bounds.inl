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

namespace Guise
{

    // Bounds implementations.
    template <size_t D, typename T>
    inline Bounds<D, T>::Bounds()
    { }

    template <size_t D, typename T>
    inline Bounds<D, T>::Bounds(const Vector<D, T> & position, const Vector<D, T> & size) :
        position(position),
        size(size)
    { }

    template <size_t D, typename T>
    inline Bounds<D, T>::Bounds(const Bounds<D, T> & bounds) :
        position(bounds.position),
        size(bounds.size)
    { }

    template <size_t D, typename T>
    inline Bounds<D, T> & Bounds<D, T>::operator = (const Bounds<D, T> & bounds)
    {
        position = bounds.position;
        size = bounds.size;
        return *this;
    }

    template <size_t D, typename T>
    inline bool Bounds<D, T>::operator == (const Bounds<D, T> & bounds) const
    {
        return position == bounds.position && size == bounds.size;
    }

    template <size_t D, typename T>
    inline bool Bounds<D, T>::operator != (const Bounds<D, T> & bounds) const
    {
        return position != bounds.position || size != bounds.size;
    }


    // Bounds 1D implementations.
    template <typename T>
    inline Bounds<1, T>::Bounds()
    { }

    template <typename T>
    inline Bounds<1, T>::Bounds(const Vector<1, T> & position, const Vector<1, T> & size) :
        position(position),
        size(size)
    { }

    template <typename T>
    inline Bounds<1, T>::Bounds(const T p, const T s) :
        position(p),
        size(s)
    { }

    template <typename T>
    inline bool Bounds<1, T>::intersects(const T point) const
    {
        return point >= position && point <= position + size ;
    }

    template <typename T>
    inline Bounds<1, T>::Bounds(const Bounds<1, T> & bounds) :
        position(bounds.position),
        size(bounds.size)
    { }

    template <typename T>
    inline Bounds<1, T> & Bounds<1, T>::operator = (const Bounds<1, T> & bounds)
    {
        position = bounds.position;
        size = bounds.size;
        return *this;
    }

    template <typename T>
    inline Bounds<1, T>  Bounds<1, T>::operator * (const T value) const
    {
        return { position * value, size * value };
    }

    template <typename T>
    inline Bounds<1, T> &  Bounds<1, T>::operator *= (const T value)
    {
        position *= value;
        size *= value;
        return *this;
    }

    template <typename T>
    inline bool Bounds<1, T>::operator == (const Bounds<1, T> & bounds) const
    {
        return position == bounds.position && size == bounds.size;
    }

    template <typename T>
    inline bool Bounds<1, T>::operator != (const Bounds<1, T> & bounds) const
    {
        return position != bounds.position || size != bounds.size;
    }


    // Bounds 2D implementations.
    template <typename T>
    Bounds<2, T> Bounds<2, T>::ceil(const Bounds<2, T> & in)
    {

        return { Vector2f::ceil(in.position), Vector2f::ceil(in.size) };
    }

    template <typename T>
    Bounds<2, T> Bounds<2, T>::floor(const Bounds<2, T> & in)
    {
        return { Vector2f::floor(in.position), Vector2f::floor(in.size) };
    }

    template <typename T>
    inline Bounds<2, T>::Bounds()
    { }

    template <typename T>
    inline Bounds<2, T>::Bounds(const Vector<2, T> & position, const Vector<2, T> & size) :
        position(position),
        size(size)
    { }

    template <typename T>
    inline Bounds<2, T>::Bounds(const T pX, const T pY, const T sX, const T sY) :
        position(pX, pY),
        size(sX, sY)
    { }

    template <typename T>
    inline bool Bounds<2, T>::intersects(const Vector<2, T> & point) const
    {
        return point.x >= position.x && point.y >= position.y &&
               point.x <= position.x + size.x && point.y <= position.y + size.y;
    }

    template <typename T>
    inline bool Bounds<2, T>::intersects(const Bounds<2, T> & bounds) const
    {
        return position.x <= bounds.position.x + bounds.size.x &&
               position.x + size.x >= bounds.position.x &&
               position.y <= bounds.position.y + bounds.size.y &&
               position.y + size.y >= bounds.position.y;
    }

    template <typename T>
    inline Bounds<2, T> & Bounds<2, T>::cutEdges(const T value)
    {
        position += {value, value};
        size     -= {value * T(2), value * T(2)};
        return *this;
    }

    template <typename T>
    inline Bounds<2, T> & Bounds<2, T>::cutEdges(const Vector2<T> & value)
    {
        position += value;
        size     -= value * T(2);
        return *this;
    }

    template <typename T>
    inline Bounds<2, T> & Bounds<2, T>::cutEdges(const Vector4<T> & value)
    {
        position += {value.x, value.y};
        size     -= {value.x + value.z, value.y + value.w};
        return *this;
    }

    template <typename T>
    inline Bounds<2, T> & Bounds<2, T>::cutLeft(const T value)
    {
        position.x += value;
        size.x     -= value;
        return *this;
    }

    template <typename T>
    inline Bounds<2, T> & Bounds<2, T>::cutTop(const T value)
    {
        position.y += value;
        size.y -= value;
        return *this;
    }

    template <typename T>
    inline Bounds<2, T> & Bounds<2, T>::innerJoin(const Bounds<2, T> & right)
    {
        
        size = Vector<2, T>::min(position + size, right.position + right.size);
        position = Vector<2, T>::max(position, right.position);
        size = Vector<2, T>::max({ 0, 0 }, size - position);  
        
        return *this;
    }

    template <typename T>
    inline Bounds<2, T>::Bounds(const Bounds<2, T> & bounds) :
        position(bounds.position),
        size(bounds.size)
    { }

    template <typename T>
    template <typename U>
    inline Bounds<2, T>::Bounds(const Vector<2, U> position, Vector<2, U> size) :
        position(position),
        size(size)
    { }

    template <typename T>
    template <typename U>
    inline Bounds<2, T>::Bounds(const Bounds<2, U> & bounds) :
        position(bounds.position),
        size(bounds.size)
    { }

    template <typename T>
    inline Bounds<2, T> & Bounds<2, T>::operator = (const Bounds<2, T> & bounds)
    {
        position = bounds.position;
        size = bounds.size;
        return *this;
    }

    template <typename T>
    inline Bounds<2, T>  Bounds<2, T>::operator * (const T value) const
    {
        return { position * value, size * value};
    }

    template <typename T>
    inline Bounds<2, T> &  Bounds<2, T>::operator *= (const T value)
    {
        position *= value;
        size *= value;
        return *this;
    }

    template <typename T>
    inline bool Bounds<2, T>::operator == (const Bounds<2, T> & bounds) const
    {
        return position == bounds.position && size == bounds.size;
    }

    template <typename T>
    inline bool Bounds<2, T>::operator != (const Bounds<2, T> & bounds) const
    {
        return position != bounds.position || size != bounds.size;
    }

}