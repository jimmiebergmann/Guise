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
    // Vector implementations
    template <size_t D, typename T>
    inline Vector<D, T>::Vector()
    { }

    template <size_t D, typename T>
    template <typename... Targs>
    inline Vector<D, T>::Vector(Targs ... components) :
        c{ components... }
    { }

    template <size_t D, typename T>
    inline bool Vector<D, T>::operator == (const Vector<D, T> & vector) const
    { 
        return ::memcmp(c, vector.c, D * sizeof(T)) == 0;
    }

    template <size_t D, typename T>
    inline bool Vector<D, T>::operator != (const Vector<D, T> & vector) const
    {
        return ::memcmp(c, vector.c, D * sizeof(T)) != 0;
    }

    template <size_t D, typename T>
    inline T Vector<D, T>::operator[] (const size_t index)
    {
        return c[index];
    }


    // Vector 2 implementations.
    template <typename T>
    inline Vector<2, T>::Vector()
    {}

    template <typename T>
    inline Vector<2, T>::Vector(const T x, const T y) :
        x(x),
        y(y)
    {}

    template <typename T>
    template <typename U>
    inline Vector<2, T>::Vector(const U x, const U y) :
        x(static_cast<T>(x)),
        y(static_cast<T>(y))
    {}

    template <typename T>
    template <typename U>
    inline Vector<2, T>::Vector(const Vector<2, U> & vector) :
        x(static_cast<T>(vector.x)),
        y(static_cast<T>(vector.y))
    {}
    
    template <typename T>
    template <typename U>
    inline Vector<2, T> & Vector<2, T>::operator = (const Vector<2, U> & vector)
    {
        x = static_cast<T>(vector.x);
        y = static_cast<T>(vector.y);
        return *this;
    }

    template <typename T>
    inline bool Vector<2, T>::operator == (const Vector<2, T> & vector) const
    {
        return (x == vector.x) && (y == vector.y);
    }

    template <typename T>
    inline bool Vector<2, T>::operator != (const Vector<2, T> & vector) const
    {
        return (x != vector.x) || (y != vector.y);
    }

    template <typename T>
    inline Vector<2, T> & Vector<2, T>::operator += (const Vector<2, T> & vector)
    {
        x += vector.x;
        y += vector.y;
        return *this;
    }

    template <typename T>
    inline Vector<2, T> & Vector<2, T>::operator -= (const Vector<2, T> & vector)
    {
        x -= vector.x;
        y -= vector.y;
        return *this;
    }

    template <typename T>
    inline Vector<2, T> & Vector<2, T>::operator *= (const Vector<2, T> & vector)
    {
        x *= vector.x;
        y *= vector.y;
        return *this;
    }

    template <typename T>
    inline Vector<2, T> & Vector<2, T>::operator *= (const T value)
    {
        x *= value;
        y *= value;
        return *this;
    }

    template <typename T>
    inline Vector<2, T> & Vector<2, T>::operator /= (const Vector<2, T> & vector)
    {
        x /= vector.x;
        y /= vector.y;
        return *this;
    }

    template <typename T>
    inline Vector<2, T> & Vector<2, T>::operator /= (const T value)
    {
        x /= value;
        y /= value;
        return *this;
    }

    template <typename T>
    inline Vector<2, T> Vector<2, T>::operator + (const Vector<2, T> & vector) const
    {
        return Vector2<T>(x + vector.x, y + vector.y);
    }

    template <typename T>
    inline Vector<2, T> Vector<2, T>::operator - () const
    {
        return Vector2<T>(-x, -y);
    }

    template <typename T>
    inline Vector<2, T> Vector<2, T>::operator - (const Vector<2, T> & vector) const
    {
        return Vector2<T>(x - vector.x, y - vector.y);
    }

    template <typename T>
    inline Vector<2, T> Vector<2, T>::operator * (const Vector<2, T> & vector) const
    {
        return Vector2<T>(x * vector.x, y * vector.y);
    }

    template <typename T>
    inline Vector<2, T> Vector<2, T>::operator * (const T value) const
    {
        return Vector2<T>(x * value, y * value);
    }

    template <typename T>
    inline Vector<2, T> Vector<2, T>::operator / (const Vector<2, T> & vector) const
    {
        return Vector2<T>(x / vector.x, y / vector.y);
    }

    template <typename T>
    inline Vector<2, T> Vector<2, T>::operator / (const T value) const
    {
        return Vector2<T>(x / value, y / value);
    }

    template <typename T>
    inline Vector<2, T> Vector<2, T>::absolute() const
    {
        return Vector2<T>(std::abs(x), std::abs(y));
    }

    template <typename T>
    inline T Vector<2, T>::operator[] (const size_t index)
    {
        return c[index];
    }


    // Vector 3 implementations.
    template <typename T>
    inline Vector<3, T>::Vector()
    {}

    template <typename T>
    inline Vector<3, T>::Vector(const T x, const T y, const T z) :
        x(x),
        y(y),
        z(z)
    {}

    template <typename T>
    template <typename U>
    inline Vector<3, T>::Vector(const U x, const U y, const U z) :
        x(static_cast<T>(x)),
        y(static_cast<T>(y)),
        z(static_cast<T>(z))
    {}

    template <typename T>
    inline Vector<3, T>::Vector(const Vector<3, T> & vector) :
        x(vector.x),
        y(vector.y),
        z(vector.z)
    {}
    template <typename T>
    inline Vector<3, T>::Vector(const Vector<2, T> & vector, const T z) :
        x(vector.x),
        y(vector.y),
        z(z)
    {}
    template <typename T>
    inline Vector<3, T>::Vector(const T x, const Vector<2, T> & vector) :
        x(x),
        y(vector.x),
        z(vector.y)
    {}

    template <typename T>
    template <typename U>
    inline Vector<3, T>::Vector(const Vector<3, U> & vector) :
        x(static_cast<T>(vector.x)),
        y(static_cast<T>(vector.y)),
        z(static_cast<T>(vector.z))
    {}

    template <typename T>
    template <typename U>
    inline Vector<3, T> & Vector<3, T>::operator = (const Vector<3, U> & vector)
    {
        x = static_cast<T>(vector.x);
        y = static_cast<T>(vector.y);
        z = static_cast<T>(vector.z);
        return *this;
    }

    template <typename T>
    inline bool Vector<3, T>::operator == (const Vector<3, T> & vector) const
    {
        return (x == vector.x) && (y == vector.y) && (z == vector.z);
    }

    template <typename T>
    inline bool Vector<3, T>::operator != (const Vector<3, T> & vector) const
    {
        return (x != vector.x) || (y != vector.y) || (z != vector.z);
    }

    template <typename T>
    inline Vector<3, T> & Vector<3, T>::operator += (const Vector<3, T> & vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        return *this;
    }

    template <typename T>
    inline Vector<3, T> & Vector<3, T>::operator -= (const Vector<3, T> & vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        return *this;
    }

    template <typename T>
    inline Vector<3, T> & Vector<3, T>::operator *= (const Vector<3, T> & vector)
    {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
        return *this;
    }

    template <typename T>
    inline Vector<3, T> & Vector<3, T>::operator *= (const T value)
    {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    template <typename T>
    inline Vector<3, T> & Vector<3, T>::operator /= (const Vector<3, T> & vector)
    {
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
        return *this;
    }

    template <typename T>
    inline Vector<3, T> & Vector<3, T>::operator /= (const T value)
    {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    template <typename T>
    inline Vector<3, T> Vector<3, T>::operator + (const Vector<3, T> & vector) const
    {
        return Vector2<T>(x + vector.x, y + vector.y, z + vector.z);
    }

    template <typename T>
    inline Vector<3, T> Vector<3, T>::operator - () const
    {
        return Vector2<T>(-x, -y, -z);
    }

    template <typename T>
    inline Vector<3, T> Vector<3, T>::operator - (const Vector<3, T> & vector) const
    {
        return Vector2<T>(x - vector.x, y - vector.y, z - vector.z);
    }

    template <typename T>
    inline Vector<3, T> Vector<3, T>::operator * (const Vector<3, T> & vector) const
    {
        return Vector2<T>(x * vector.x, y * vector.y, z * vector.z);
    }

    template <typename T>
    inline Vector<3, T> Vector<3, T>::operator * (const T value) const
    {
        return Vector2<T>(x * value, y * value, z * value);
    }

    template <typename T>
    inline Vector<3, T> Vector<3, T>::operator / (const Vector<3, T> & vector) const
    {
        return Vector2<T>(x / vector.x, y / vector.y, z / vector.z);
    }

    template <typename T>
    inline Vector<3, T> Vector<3, T>::operator / (const T value) const
    {
        return Vector2<T>(x / value, y / value, z / value);
    }

    template <typename T>
    inline Vector<3, T> Vector<3, T>::absolute() const
    {
        return Vector2<T>(std::abs(x), std::abs(y), std::abs(z));
    }

    template <typename T>
    inline T Vector<3, T>::operator[] (const size_t index)
    {
        return c[index];
    }


    // Vector 4 implementations.
    template <typename T>
    inline Vector<4, T>::Vector()
    {}

    template <typename T>
    inline Vector<4, T>::Vector(const T x, const T y, const T z, const T w) :
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    template <typename T>
    template <typename U>
    inline Vector<4, T>::Vector(const U x, const U y, const U z, const U w) :
        x(static_cast<T>(x)),
        y(static_cast<T>(y)),
        z(static_cast<T>(z)),
        w(static_cast<T>(w))
    {}

    template <typename T>
    inline Vector<4, T>::Vector(const Vector<4, T> & vector) :
        x(vector.x),
        y(vector.y),
        z(vector.z),
        w(vector.w)
    {}
    template <typename T>
    inline Vector<4, T>::Vector(const Vector<3, T> & vector, const T w) :
        x(vector.x),
        y(vector.y),
        z(vector.z),
        w(w)
    {}
    template <typename T>
    inline Vector<4, T>::Vector(const Vector<2, T> & vector, const T z, const T w) :
        x(vector.x),
        y(vector.y),
        z(z),
        w(w)
    {}
    template <typename T>
    inline Vector<4, T>::Vector(const Vector<2, T> & vector1, const Vector<2, T> & vector2) :
        x(vector1.x),
        y(vector1.y),
        z(vector2.y),
        w(vector2.y)
    {}
    template <typename T>
    inline Vector<4, T>::Vector(const T x, const Vector<3, T> & vector) :
        x(x),
        y(vector.x),
        z(vector.y),
        w(vector.z)
    {}
    template <typename T>
    inline Vector<4, T>::Vector(const T x, const T y, const Vector<2, T> & vector) :
        x(x),
        y(y),
        z(vector.x),
        w(vector.y)
    {}

    template <typename T>
    template <typename U>
    inline Vector<4, T>::Vector(const Vector<4, U> & vector) :
        x(static_cast<T>(vector.x)),
        y(static_cast<T>(vector.y)),
        z(static_cast<T>(vector.z)),
        w(static_cast<T>(vector.w))
    {}

    template <typename T>
    template <typename U>
    inline Vector<4, T> & Vector<4, T>::operator = (const Vector<4, U> & vector)
    {
        x = static_cast<T>(vector.x);
        y = static_cast<T>(vector.y);
        z = static_cast<T>(vector.z);
        w = static_cast<T>(vector.w);
        return *this;
    }

    template <typename T>
    inline bool Vector<4, T>::operator == (const Vector<4, T> & vector) const
    {
        return (x == vector.x) && (y == vector.y) && (z == vector.z) && (w == vector.w);
    }

    template <typename T>
    inline bool Vector<4, T>::operator != (const Vector<4, T> & vector) const
    {
        return (x != vector.x) || (y != vector.y) || (z != vector.z) || (w != vector.w);
    }

    template <typename T>
    inline Vector<4, T> & Vector<4, T>::operator += (const Vector<4, T> & vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        w += vector.w;
        return *this;
    }

    template <typename T>
    inline Vector<4, T> & Vector<4, T>::operator -= (const Vector<4, T> & vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        w -= vector.w;
        return *this;
    }

    template <typename T>
    inline Vector<4, T> & Vector<4, T>::operator *= (const Vector<4, T> & vector)
    {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
        w *= vector.w;
        return *this;
    }

    template <typename T>
    inline Vector<4, T> & Vector<4, T>::operator *= (const T value)
    {
        x *= value;
        y *= value;
        z *= value;
        w *= value;
        return *this;
    }

    template <typename T>
    inline Vector<4, T> & Vector<4, T>::operator /= (const Vector<4, T> & vector)
    {
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
        w /= vector.w;
        return *this;
    }

    template <typename T>
    inline Vector<4, T> & Vector<4, T>::operator /= (const T value)
    {
        x /= value;
        y /= value;
        z /= value;
        w /= value;
        return *this;
    }

    template <typename T>
    inline Vector<4, T> Vector<4, T>::operator + (const Vector<4, T> & vector) const
    {
        return Vector2<T>(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
    }

    template <typename T>
    inline Vector<4, T> Vector<4, T>::operator - () const
    {
        return Vector2<T>(-x, -y, -z, -w);
    }

    template <typename T>
    inline Vector<4, T> Vector<4, T>::operator - (const Vector<4, T> & vector) const
    {
        return Vector2<T>(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
    }

    template <typename T>
    inline Vector<4, T> Vector<4, T>::operator * (const Vector<4, T> & vector) const
    {
        return Vector2<T>(x * vector.x, y * vector.y, z * vector.z, w * vector.w);
    }

    template <typename T>
    inline Vector<4, T> Vector<4, T>::operator * (const T value) const
    {
        return Vector2<T>(x * value, y * value, z * value, w * value);
    }

    template <typename T>
    inline Vector<4, T> Vector<4, T>::operator / (const Vector<4, T> & vector) const
    {
        return Vector2<T>(x / vector.x, y / vector.y, z / vector.z, w / vector.w);
    }

    template <typename T>
    inline Vector<4, T> Vector<4, T>::operator / (const T value) const
    {
        return Vector2<T>(x / value, y / value, z / value, w / value);
    }

    template <typename T>
    inline Vector<4, T> Vector<4, T>::absolute() const
    {
        return Vector2<T>(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
    }

    template <typename T>
    inline T Vector<4, T>::operator[] (const size_t index)
    {
        return c[index];
    }
    
}