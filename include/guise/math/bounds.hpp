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



#ifndef GUISE_MATH_BOUNDS_HPP
#define GUISE_MATH_BOUNDS_HPP

#include "guise/build.hpp"
#include "guise/math/vector.hpp"

namespace Guise
{

    /**
    * Bounds template.
    *
    * @tparam D Number of dimensions.
    * @tparam T Data type of components.
    *
    */
    template <size_t D, typename T>
    class Bounds
    {

    public:

        Bounds();
        Bounds(const Vector<D, T> & position, const Vector<D, T> & size);
        Bounds(const Bounds<D, T> & bounds);

        Bounds<D, T> & operator = (const Bounds<D, T> & bounds);
  
        bool operator == (const Bounds<D, T> & bounds) const;
        bool operator != (const Bounds<D, T> & bounds) const;

        Vector<D, T> position;
        Vector<D, T> size;

    };


    /**
    * Bounds 1D template.
    * @tparam T Data type of components.
    *
    */
    template <typename T>
    class Bounds<1, T>
    {

    public:

        Bounds();
        Bounds(const Vector<1, T> & position, const Vector<1, T> & size);
        Bounds(const T p, const T s);
        Bounds(const Bounds<1, T> & bounds);

        bool intersects(const T point) const;

        Bounds<1, T> & operator = (const Bounds<1, T> & bounds);

        Bounds<1, T> operator * (const T value) const;
        Bounds<1, T> & operator *= (const T value);

        bool operator == (const Bounds<1, T> & bounds) const;
        bool operator != (const Bounds<1, T> & bounds) const;

        T position;
        T size;

    };


    /**
    * Bounds 2D template.
    * @tparam T Data type of components.
    *
    */
    template <typename T>
    class Bounds<2, T>
    {

    public:

        Bounds();
        Bounds(const Vector<2, T> & position, const Vector<2, T> & size);
        Bounds(const T pX, const T pY, const T sX, const T sY);
        Bounds(const Bounds<2, T> & bounds);

        bool intersects(const Vector<2, T> & point) const;

        Bounds<2, T> & operator = (const Bounds<2, T> & bounds);

        Bounds<2, T> operator * (const T value) const;
        Bounds<2, T> & operator *= (const T value);

        bool operator == (const Bounds<2, T> & bounds) const;
        bool operator != (const Bounds<2, T> & bounds) const;

        Vector<2, T> position;
        Vector<2, T> size;

    };

    template <typename T>
    using Bounds2 = Bounds<2, T>;
    using Bounds2f = Bounds<2, float>;
    using Bounds2i32 = Bounds<2, int32_t>;

    template <typename T>
    using Bounds1 = Bounds<1, T>;
    using Bounds1f = Bounds<1, float>;
    using Bounds1i32 = Bounds<1, int32_t>;

}

#include "guise/math/bounds.inl"

#endif