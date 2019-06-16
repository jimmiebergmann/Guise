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

    // Matrix implementations.

    // Matrix4x4 implementations.
    template <typename T>
    Matrix<4, 4, T>::Matrix()
    {}

    template <typename T>
    Matrix<4, 4, T>::Matrix(const T c00, const T c10, const T c20, const T c30,
                            const T c01, const T c11, const T c21, const T c31,
                            const T c02, const T c12, const T c22, const T c32,
                            const T c03, const T c13, const T c23, const T c33) :
        m{  c00, c01, c02, c03,
            c10, c11, c12, c13,
            c20, c21, c22, c23,
            c30, c31, c32, c33 }
    {}

    template <typename T>
    Matrix<4, 4, T> & Matrix<4, 4, T>::loadOrthographic(const T left, const T right, const T bottom,
                                                    const T top, const T zNear, const T zFar)
    {
        m[0]  = static_cast<T>(2) / (right - left);
        m[1]  = static_cast<T>(0);
        m[2]  = static_cast<T>(0);
        m[3]  = static_cast<T>(0);
        m[4]  = static_cast<T>(0);
        m[5]  = static_cast<T>(2) / (top - bottom);
        m[6]  = static_cast<T>(0);
        m[7]  = static_cast<T>(0);
        m[8]  = static_cast<T>(0);
        m[9]  = static_cast<T>(0);
        m[10] = static_cast<T>(-2) / (zFar - zNear);
        m[11] = static_cast<T>(0);
        m[12] = -(right + left) / (right - left);
        m[13] = -(top + bottom) / (top - bottom);
        m[14] = -(zFar + zNear) / (zFar - zNear);
        m[15] = static_cast<T>(1);

        return *this;
    }

    template <typename T>
    Matrix<4, 4, T> & Matrix<4, 4, T>::loadScale(float , float , float )
    {
        return *this;
    }

    template <typename T>
    Matrix<4, 4, T> & Matrix<4, 4, T>::scale(float x, float y, float z)
    {
        Matrix<4, 4, T> scale(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
        return *this = *this * scale;
    }

    template <typename T>
    Matrix<4, 4, T> Matrix<4, 4, T>::operator *(const Matrix<4, 4, T> & mat) const
    {
        auto & Mat1 = *this;
        Matrix<4, 4, T> Dest;

        Dest.m[0] = Mat1.m[0] * mat.m[0] + Mat1.m[4] * mat.m[1] + Mat1.m[8] * mat.m[2] + Mat1.m[12] * mat.m[3];
        Dest.m[4] = Mat1.m[0] * mat.m[4] + Mat1.m[4] * mat.m[5] + Mat1.m[8] * mat.m[6] + Mat1.m[12] * mat.m[7];
        Dest.m[8] = Mat1.m[0] * mat.m[8] + Mat1.m[4] * mat.m[9] + Mat1.m[8] * mat.m[10] + Mat1.m[12] * mat.m[11];
        Dest.m[12] = Mat1.m[0] * mat.m[12] + Mat1.m[4] * mat.m[13] + Mat1.m[8] * mat.m[14] + Mat1.m[12] * mat.m[15];

        Dest.m[1] = Mat1.m[1] * mat.m[0] + Mat1.m[5] * mat.m[1] + Mat1.m[9] * mat.m[2] + Mat1.m[13] * mat.m[3];
        Dest.m[5] = Mat1.m[1] * mat.m[4] + Mat1.m[5] * mat.m[5] + Mat1.m[9] * mat.m[6] + Mat1.m[13] * mat.m[7];
        Dest.m[9] = Mat1.m[1] * mat.m[8] + Mat1.m[5] * mat.m[9] + Mat1.m[9] * mat.m[10] + Mat1.m[13] * mat.m[11];
        Dest.m[13] = Mat1.m[1] * mat.m[12] + Mat1.m[5] * mat.m[13] + Mat1.m[9] * mat.m[14] + Mat1.m[13] * mat.m[15];

        Dest.m[2] = Mat1.m[2] * mat.m[0] + Mat1.m[6] * mat.m[1] + Mat1.m[10] * mat.m[2] + Mat1.m[14] * mat.m[3];
        Dest.m[6] = Mat1.m[2] * mat.m[4] + Mat1.m[6] * mat.m[5] + Mat1.m[10] * mat.m[6] + Mat1.m[14] * mat.m[7];
        Dest.m[10] = Mat1.m[2] * mat.m[8] + Mat1.m[6] * mat.m[9] + Mat1.m[10] * mat.m[10] + Mat1.m[14] * mat.m[11];
        Dest.m[14] = Mat1.m[2] * mat.m[12] + Mat1.m[6] * mat.m[13] + Mat1.m[10] * mat.m[14] + Mat1.m[14] * mat.m[15];

        Dest.m[3] = Mat1.m[3] * mat.m[0] + Mat1.m[7] * mat.m[1] + Mat1.m[11] * mat.m[2] + Mat1.m[15] * mat.m[3];
        Dest.m[7] = Mat1.m[3] * mat.m[4] + Mat1.m[7] * mat.m[5] + Mat1.m[11] * mat.m[6] + Mat1.m[15] * mat.m[7];
        Dest.m[11] = Mat1.m[3] * mat.m[8] + Mat1.m[7] * mat.m[9] + Mat1.m[11] * mat.m[10] + Mat1.m[15] * mat.m[11];
        Dest.m[15] = Mat1.m[3] * mat.m[12] + Mat1.m[7] * mat.m[13] + Mat1.m[11] * mat.m[14] + Mat1.m[15] * mat.m[15];

        return Dest;
    }

    

}