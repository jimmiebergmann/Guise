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

#ifndef GUISE_OPENGL_RENDERER_HPP
#define GUISE_OPENGL_RENDERER_HPP

#include "guise/build.hpp"

#if !defined(GUISE_DISABLE_OPENGL)

#include "guise/renderer/opengl/opengl.hpp"
#include "guise/renderer.hpp"
#include <memory>
#include <stack>

namespace Guise
{

    // Forward declarations
    class AppWindow;

    /**
    * Context class.
    *
    *
    */

    class GUISE_API OpenGLRenderer : public Renderer
    {

    public:

        // Interface functions.
        void setLevel(const size_t level);

        float getScale() const;

        void drawRect(const Bounds2f & bounds, const Style::PaintRectStyle & style);

        void drawQuad(const Bounds2f & bounds, const Vector4f & color);
        void drawQuad(const Bounds2f & bounds, const std::shared_ptr<Texture> & texture, const Vector4f & color);

        void drawBorder(const Bounds2f & bounds, const float width, const Vector4f & color);

        void drawLine(const Vector2f & point1, const Vector2f & point2, const float width, const Vector4f & color);
  
        void pushMask(const Bounds2i32 & bounds);
        void popMask();

        std::shared_ptr<Texture> createTexture();

        // Renderer functions.
        static std::shared_ptr<OpenGLRenderer> create(const std::shared_ptr<AppWindow> & appWindow);
    #if defined(GUISE_PLATFORM_WINDOWS)
        static std::shared_ptr<OpenGLRenderer> create(::HDC deviceContextHandle);
    #elif defined(GUISE_PLATFORM_LINUX)
        static std::shared_ptr<OpenGLRenderer> create(::Display * display, ::Window window, int screen);
    #endif

        const Vector4f & getClearColor();

        void setClearColor(const Vector4f & color);

        void setViewportSize(const Vector2ui32 & position, const Vector2ui32 & size);

        void setScale(const float scale);

        void clearColor();

        void present();

    private:

        
    #if defined(GUISE_PLATFORM_WINDOWS)
        OpenGLRenderer(HDC deviceContextHandle);

        // Windows members.
        ::HDC           m_deviceContextHandle;  ///< Device context handle from the render output.
        ::HGLRC         m_context;              ///< The OpenGL context.
    #elif defined(GUISE_PLATFORM_LINUX)
        OpenGLRenderer(::Display * display, ::Window window, int screen);

        // Linux memebers.
        ::Colormap      m_colormap;
        ::GLXContext    m_context;
        ::Display *     m_display;
        ::Window        m_window;      
    #endif

        void updateProjectionMatrix();

        Vector4f                m_clearColor;
        Bounds2i32              m_viewPort;
        float                   m_scale;
        float                   m_level;
        std::stack<Bounds2i32>  m_maskStack;

    };

}

#endif

#endif