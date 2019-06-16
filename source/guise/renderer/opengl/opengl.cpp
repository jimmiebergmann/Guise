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

#include "guise/renderer/opengl/opengl.hpp"

#if !defined(GUISE_DISABLE_OPENGL)

#if defined(GUISE_PLATFORM_WINDOWS)
#include "guise/platform/win32Headers.hpp"
#endif

// Function for grabbing the opengl functions
#if defined(GUISE_PLATFORM_WINDOWS)
    #define glGetProcAddress(extension) wglGetProcAddress(extension)
#elif defined(GUISE_PLATFORM_LINUX)
    #define glGetProcAddress(extension) glXGetProcAddress((const GLubyte *)extension )
    //#define GLX_CONTEXT_MAJOR_VERSION_ARB		0x2091
    //#define GLX_CONTEXT_MINOR_VERSION_ARB		0x2092
    //typedef ::GLXContext(*GLXCREATECONTEXTATTRIBSARBPROC)(::Display*, ::GLXFBConfig, ::GLXContext, int, const int*);
#endif


namespace Guise
{

    namespace OpenGL
    {

        static bool g_loaded = false;
        static bool g_loadStatus = false;

        PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;

        bool loadExtensions()
        {
            if (g_loaded)
            {
                return g_loadStatus;
            }

            g_loadStatus = true;
            g_loadStatus &= (glActiveTexture = (PFNGLACTIVETEXTUREPROC)glGetProcAddress("glActiveTexture")) != NULL;

            g_loaded = true;
            return g_loadStatus;
        }

    }
    
}

#endif