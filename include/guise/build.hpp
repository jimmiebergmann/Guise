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

#ifndef GUISE_BUILD_HPP
#define GUISE_BUILD_HPP

// Platforms

// Win32
#if defined( _WIN32 ) || defined( __WIN32__ ) || defined( _WIN64 ) || defined( __WIN64__ )
    #define GUISE_PLATFORM_WINDOWS

    #if defined (_MSC_VER)
        #pragma comment(lib, "ws2_32.lib")
    #endif
// Linux
#elif defined( linux ) || defined( __linux )
    #define GUISE_PLATFORM_LINUX
#else
    #error Unkown platform.
#endif

// Build type
#if defined( NDEBUG ) || !defined( _DEBUG )
    #define GUISE_BUILD_RELEASE
#else
    #define GUISE_BUILD_DEBUG
#endif


#ifdef _MSC_VER
    #pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
#endif

#if !defined(GUISE_STATIC)
    // Turn off microsoft STL vsc warning
    #ifdef _MSC_VER
        #pragma warning(disable : 4251) // identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    #endif

    // Define as export or import, if FLARE_EXPORTS is defined.
    #if defined(GUISE_PLATFORM_WINDOWS)
        #if defined(GUISE_EXPORT)
            #define GUISE_API __declspec(dllexport)
        #else
            #define GUISE_API __declspec(dllimport)
        #endif
    #else
        #define GUISE_API
    #endif
#else
    #define GUISE_API
#endif

#include <stdint.h>

// Math types
#include "guise/math/matrix.hpp"
#include "guise/math/vector.hpp"
namespace Guise
{
    ///< Typedefs of common vector types.
    template<typename T>
    using Vector2       = Vector<2, T>;
    using Vector2i32    = Vector<2, int32_t>;
    using Vector2ui32   = Vector<2, uint32_t>;
    using Vector2f      = Vector<2, float>;
    using Vector2d      = Vector<2, double>;

    template<typename T>
    using Vector3       = Vector<3, T>;
    using Vector3i32    = Vector<3, int32_t>;
    using Vector3ui32   = Vector<3, uint32_t>;
    using Vector3f      = Vector<3, float>;
    using Vector3d      = Vector<3, double>;

    template<typename T>
    using Vector4       = Vector<4, T>;
    using Vector4i32    = Vector<4, int32_t>;
    using Vector4ui32   = Vector<4, uint32_t>;
    using Vector4f      = Vector<4, float>;
    using Vector4d      = Vector<4, double>;

    // Typedefs of common matrix types
    template<typename T>
    using Matrix4x4     = Matrix<4, 4, T>;
    using Matrix4x4f    = Matrix<4, 4, float>;
    using Matrix4x4d    = Matrix<4, 4, double>;
}

// Renderer settings.
#define GUISE_USE_DEFAULT_RENDERER

#define GUISE_ENABLE_OPENGL_RENDERER


#if defined(GUISE_USE_DEFAULT_RENDERER)
    #if defined(GUISE_ENABLE_OPENGL_RENDERER)
        #include "guise/renderer/opengl/openglRenderer.hpp"
        #define GUISE_DEFAULT_RENDERER OpenGLRenderer
    #else
        #undef GUISE_USE_DEFAULT_RENDERER
    #endif  
    
#endif


#endif
