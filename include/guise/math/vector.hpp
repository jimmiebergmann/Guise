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

#ifndef GUISE_MATH_VECTOR2_HPP
#define GUISE_MATH_VECTOR2_HPP

#include "guise/build.hpp"
#include <algorithm>
#include <cmath>

namespace Guise
{

    // Forward declarations.
    template <size_t D, typename T>
    class Vector;


    /**
    * Vector class.
    *
    * @tparam D Number of dimensions.
    * @tparam T Data type of components.
    *
    */
    template <size_t D, typename T>
    class Vector
    {

    public:

        Vector();
        
        template <typename... Targs>
        Vector(Targs ... components);

        bool operator == (const Vector<D, T> & vector) const;

        bool operator != (const Vector<D, T> & vector) const;
        
        T operator[] (const size_t index);

        size_t dimensions;
        T c[D];
        
    };

    /**
    * Vector2 class.
    *
    * @tparam T Data type of components.
    *
    */
    template <typename T>
    class Vector<2, T>
    {

    public:

        Vector();

        Vector(const T x, const T y);

        template <typename U>
        Vector(const U x, const U y);

        template <typename U>
        Vector(const Vector<2, U> & vector);

        template <typename U>
        Vector<2, T> & operator = (const Vector<2, U> & vector);

        bool operator == (const Vector<2, T> & vector) const;

        bool operator != (const Vector<2, T> & vector) const;

        Vector<2, T> & operator += (const Vector<2, T> & vector);

        Vector<2, T> & operator -= (const Vector<2, T> & vector);

        Vector<2, T> & operator *= (const Vector<2, T> & vector);
        Vector<2, T> & operator *= (const T value);

        Vector<2, T> & operator /= (const Vector<2, T> & vector);
        Vector<2, T> & operator /= (const T value);

        Vector<2, T> operator + (const Vector<2, T> & vector) const;

        Vector<2, T> operator - () const;
        Vector<2, T> operator - (const Vector<2, T> & vector) const;

        Vector<2, T> operator * (const Vector<2, T> & vector) const;
        Vector<2, T> operator * (const T value) const;

        Vector<2, T> operator / (const Vector<2, T> & vector) const;
        Vector<2, T> operator / (const T value) const;

        Vector<2, T> absolute() const;

        T operator[] (const size_t index);

        static Vector<2, T> clamp(const Vector<2, T> & vector, const Vector<2, T> & low, const Vector<2, T> & high);
        static Vector<2, T> min(const Vector<2, T> & low, const Vector<2, T> & high);
        static Vector<2, T> max(const Vector<2, T> & low, const Vector<2, T> & high);
        static Vector<2, T> ceil(const Vector<2, T> & vec);
        static Vector<2, T> floor(const Vector<2, T> & vec);

        T x;
        T y;

    };

    /**
    * Vector3 class.
    *
    * @tparam T Data type of components.
    *
    */
    template <typename T>
    class Vector<3, T>
    {

    public:

        Vector();

        Vector(const T x, const T y, const T z);

        template <typename U>
        Vector(const U x, const U y, const U z);

        Vector(const Vector<3, T> & vector);
        Vector(const Vector<2, T> & vector, const T z);
        Vector(const T x, const Vector<2, T> & vector);

        template <typename U>
        Vector(const Vector<3, U> & vector);

        template <typename U>
        Vector<3, T> & operator = (const Vector<3, U> & vector);

        bool operator == (const Vector<3, T> & vector) const;

        bool operator != (const Vector<3, T> & vector) const;

        Vector<3, T> & operator += (const Vector<3, T> & vector);

        Vector<3, T> & operator -= (const Vector<3, T> & vector);

        Vector<3, T> & operator *= (const Vector<3, T> & vector);
        Vector<3, T> & operator *= (const T value);

        Vector<3, T> & operator /= (const Vector<3, T> & vector);
        Vector<3, T> & operator /= (const T value);

        Vector<3, T> operator + (const Vector<3, T> & vector) const;

        Vector<3, T> operator - () const;
        Vector<3, T> operator - (const Vector<3, T> & vector) const;

        Vector<3, T> operator * (const Vector<3, T> & vector) const;
        Vector<3, T> operator * (const T value) const;

        Vector<3, T> operator / (const Vector<3, T> & vector) const;
        Vector<3, T> operator / (const T value) const;

        Vector<3, T> absolute() const;

        T operator[] (const size_t index);

        T x;
        T y;
        T z;

    };

    /**
    * Vector4 class.
    *
    * @tparam T Data type of components.
    *
    */
    template <typename T>
    class Vector<4, T>
    {

    public:

        Vector();

        Vector(const T x, const T y, const T z, const T w);

        template <typename U>
        Vector(const U x, const U y, const U z, const U w);

        Vector(const Vector<4, T> & vector);
        Vector(const Vector<3, T> & vector, const T w);
        Vector(const Vector<2, T> & vector, const T z, const T w);
        Vector(const Vector<2, T> & vector1, const Vector<2, T> & vector2);
        Vector(const T x, const Vector<3, T> & vector);
        Vector(const T x, const T y, const Vector<2, T> & vector);

        template <typename U>
        Vector(const Vector<4, U> & vector);

        template <typename U>
        Vector<4, T> & operator = (const Vector<4, U> & vector);

        bool operator == (const Vector<4, T> & vector) const;

        bool operator != (const Vector<4, T> & vector) const;

        Vector<4, T> & operator += (const Vector<4, T> & vector);

        Vector<4, T> & operator -= (const Vector<4, T> & vector);

        Vector<4, T> & operator *= (const Vector<4, T> & vector);
        Vector<4, T> & operator *= (const T value);

        Vector<4, T> & operator /= (const Vector<4, T> & vector);
        Vector<4, T> & operator /= (const T value);

        Vector<4, T> operator + (const Vector<4, T> & vector) const;

        Vector<4, T> operator - () const;
        Vector<4, T> operator - (const Vector<4, T> & vector) const;

        Vector<4, T> operator * (const Vector<4, T> & vector) const;
        Vector<4, T> operator * (const T value) const;

        Vector<4, T> operator / (const Vector<4, T> & vector) const;
        Vector<4, T> operator / (const T value) const;

        Vector<4, T> absolute() const;

        T operator[] (const size_t index);

        T x;
        T y;
        T z;
        T w;       

    };

}

// Include inline implementations.
#include "guise/math/vector.inl"

#endif
