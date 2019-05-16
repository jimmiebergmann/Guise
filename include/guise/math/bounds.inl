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


    // Bounds 2D implementations.
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
    inline Bounds<2, T>::Bounds(const Bounds<2, T> & bounds) :
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