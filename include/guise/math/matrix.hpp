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



#ifndef GUISE_MATH_MATRIX_HPP
#define GUISE_MATH_MATRIX_HPP

#include "guise/build.hpp"
#include "guise/math/vector.hpp"
#include <cmath>

namespace Guise
{

    /**
    * Matrix template.
    *
    * @tparam H Number of horizontal dimensions.
    * @tparam W Number of vertical dimensions.
    * @tparam T Data type of components.
    *
    */
    template <size_t H, size_t W, typename T>
    class Matrix
    {

    public:

        Matrix()
        {

        }
        /*Matrix4x4(const T s00, const T s10, const T s20, const T s30,
            const T s01, const T s11, const T s21, const T s31,
            const T s02, const T s12, const T s22, const T s32,
            const T s03, const T s13, const T s23, const T s33);*/
        /*Matrix4x4(const Matrix4x4<T> & matrix);
        template<typename U>
        Matrix4x4(const Matrix4x4<U> & matrix);

        Vector4<T> row(const size_t index) const;
        Vector4<T> column(const size_t index) const;

        Matrix4x4<T> & identity();
        Matrix4x4<T> & translate(const Vector3<T> & vector);
        Matrix4x4<T> & translate(const T x, const T y, const T z);
        Matrix4x4<T> & scale(const T x, const T y, const T z);
        Matrix4x4<T> & lookAt(const Vector3<T> & position, const Vector3<T> & up, const Vector3<T> & center);

        Matrix4x4<T> operator -() const;
        Matrix4x4<T> operator *(const Matrix4x4<T> & matrix) const;
        Matrix4x4<T> operator *(const Vector4<T> & vector) const;

        Matrix4x4<T> & operator *=(const Matrix4x4<T> & matrix);*/


        union
        {
            T v[H][W];
            T m[H * W];
        };

    };

    /**
    * Matrix4x4 template.
    *
    * @tparam H Number of horizontal dimensions.
    * @tparam W Number of vertical dimensions.
    * @tparam T Data type of components.
    *
    */
    template <typename T>
    class Matrix<4, 4, T>
    {

    public:

        Matrix();

        Matrix(const T c00, const T c10, const T c20, const T c30,
            const T c01, const T c11, const T c21, const T c31,
            const T c02, const T c12, const T c22, const T c32,
            const T c03, const T c13, const T c23, const T c33);

        Matrix<4, 4, T> & loadIdentity();

        Matrix<4, 4, T> & loadOrthographic(const T left, const T right, const T bottom,
                                           const T top, const T zNear, const T zFar);

        Matrix<4, 4, T> & scale(float x, float y, float z);

        Matrix<4, 4, T> & translate(float x, float y, float z);

        Matrix<4, 4, T> operator *(const Matrix<4, 4, T> & matrix) const;


            /*Matrix4x4(const Matrix4x4<T> & matrix);
            template<typename U>
            Matrix4x4(const Matrix4x4<U> & matrix);

            Vector4<T> row(const size_t index) const;
            Vector4<T> column(const size_t index) const;

            Matrix4x4<T> & identity();
            Matrix4x4<T> & translate(const Vector3<T> & vector);
            Matrix4x4<T> & translate(const T x, const T y, const T z);
            Matrix4x4<T> & scale(const T x, const T y, const T z);
            Matrix4x4<T> & lookAt(const Vector3<T> & position, const Vector3<T> & up, const Vector3<T> & center);

            Matrix4x4<T> operator -() const;
            Matrix4x4<T> operator *(const Matrix4x4<T> & matrix) const;
            Matrix4x4<T> operator *(const Vector4<T> & vector) const;

            Matrix4x4<T> & operator *=(const Matrix4x4<T> & matrix);
            */
        union
        {
            T v[4][4];
            T m[16];
        };
    };

}

#include "guise/math/matrix.inl"

#endif