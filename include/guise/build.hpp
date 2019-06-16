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
    #include "SDKDDKVer.h"
    #pragma comment(lib, "Shcore.lib")

    #define GUISE_PLATFORM_WINDOWS_10_1803  0x0A000005
    #define GUISE_PLATFORM_WINDOWS_10_1709  0x0A000004
    #define GUISE_PLATFORM_WINDOWS_10_1703  0x0A000003
    #define GUISE_PLATFORM_WINDOWS_10_1607  0x0A000002
    #define GUISE_PLATFORM_WINDOWS_10_1511  0x0A000001
    #define GUISE_PLATFORM_WINDOWS_10       0x0A000000
    #define GUISE_PLATFORM_WINDOWS_8_1      0x06030000
    #define GUISE_PLATFORM_WINDOWS_8        0x06020000
    #define GUISE_PLATFORM_WINDOWS_7        0x06010000
    #define GUISE_PLATFORM_WINDOWS_VISTA    0x06000000
    #define GUISE_PLATFORM_WINDOWS_XP       0x05010000

    #if(WINVER >= 0x0A00)
        #if(NTDDI_VERSION >= 0x0A000005)
            #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_10_1803
        #elif(NTDDI_VERSION >= 0x0A000004)
            #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_10_1709
        #elif(NTDDI_VERSION >= 0x0A000003)
            #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_10_1703
        #elif(NTDDI_VERSION >= 0x0A000002)
            #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_10_1607
        #elif(NTDDI_VERSION >= 0x0A000001)
            #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_10_1511
        #else
            #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_10
        #endif
    #elif(WINVER >= 0x0603)
        #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_8_1
        #pragma comment(lib, "Shcore.lib")
    #elif(WINVER >= 0x0602)
        #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_8
    #elif(WINVER >= 0x0601)
        #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_7
    #elif(WINVER >= 0x0600)
        #define GUISE_PLATFORM_WINDOWSGUISE_PLATFORM_WINDOWS_VISTA
    #elif(WINVER >= 0x0501)
        #define GUISE_PLATFORM_WINDOWS GUISE_PLATFORM_WINDOWS_XP
    #else
        #define GUISE_PLATFORM_WINDOWS 0x0000
    #endif

    #include "guise/platform/win32Headers.hpp"
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

#define GUISE_DEFAULT_DPI 96

#include <stdint.h>
#include <stddef.h>
#include <cstring>

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
#define GUISE_OPENGL_RENDERER 0

#if !defined(GUISE_DISABLE_OPENGL)
    #define GUISE_DEFAULT_RENDERER GUISE_OPENGL_RENDERER
#endif

#endif
