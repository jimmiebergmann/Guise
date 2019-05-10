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
    Matrix<4, 4, T> & Matrix<4, 4, T>::orthographic(const T left, const T right, const T bottom,
                                                    const T top, const T zNear, const T zFar)
    {
        /*T tx = -(right + left) / (right - left);
        T ty = -(top + bottom) / (top - bottom);
        T tz = -(zFar + zNear) / (zFar - zNear);

        Identity();
        m[0] = 2 / (p_Right - p_Left);
        m[5] = 2.0f / (p_Top - p_Bottom);
        m[10] = -2 / (p_ZFar - p_ZNear);
        m[12] = TX;
        m[13] = TY;
        m[14] = TZ;*/

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

}