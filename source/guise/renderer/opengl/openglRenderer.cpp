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

#include "guise/renderer/opengl/openglRenderer.hpp"

#if !defined(GUISE_DISABLE_OPENGL)

#include "guise/renderer/opengl/openglTexture.hpp"
#include "guise/appWindow.hpp"
#include <iostream>

namespace Guise
{

    int32_t OpenGLRenderer::getDpi()
    {
        return m_dpi;
    }

    void OpenGLRenderer::setCulling(const Vector2f &, const Vector2f &)
    {

    }

    void OpenGLRenderer::drawQuad(const Bounds2f & bounds, const Vector4f & color)
    {
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);

        glColor4f(color.x, color.y, color.z, color.w);
        glVertex2f(bounds.position.x, bounds.position.y);
        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y);
        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
        glVertex2f(bounds.position.x, bounds.position.y + bounds.size.y);

        glEnd();
    }

    void OpenGLRenderer::drawQuad(const Bounds2f & bounds, const std::shared_ptr<Texture> & texture, const Vector4f & color)
    {
        glEnable(GL_TEXTURE_2D);
        texture->bind(0);

        glBegin(GL_QUADS);
        glColor4f(color.x, color.y, color.z, color.w);
        
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(bounds.position.x, bounds.position.y);
        
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y);
        
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
        
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(bounds.position.x, bounds.position.y + bounds.size.y);
        
        glEnd();
    }

    void OpenGLRenderer::drawBorder(const Bounds2f & bounds, const float width, const Vector4f & color)
    {
        glDisable(GL_TEXTURE_2D);

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);

        const float widthX = bounds.size.x > width ? width : bounds.size.x;
        const float widthY = bounds.size.y > width ? width : bounds.size.y;

        glColor4f(color.x, color.y, color.z, color.w);

        glVertex2f(bounds.position.x, bounds.position.y);
        glVertex2f(bounds.position.x + bounds.size.x , bounds.position.y);
        glVertex2f(bounds.position.x + bounds.size.x - widthX, bounds.position.y + widthY);
        glVertex2f(bounds.position.x + widthX, bounds.position.y + widthY);
       
        glVertex2f(bounds.position.x + widthX, bounds.position.y + bounds.size.y - widthY);
        glVertex2f(bounds.position.x + bounds.size.x - widthX, bounds.position.y + bounds.size.y - widthY);
        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
        glVertex2f(bounds.position.x, bounds.position.y + bounds.size.y);
        
        glVertex2f(bounds.position.x, bounds.position.y);
        glVertex2f(bounds.position.x + widthX, bounds.position.y + widthY);
        glVertex2f(bounds.position.x + widthX, bounds.position.y + bounds.size.y - widthY);
        glVertex2f(bounds.position.x, bounds.position.y + bounds.size.y);
        

        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y);
        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
        glVertex2f(bounds.position.x + bounds.size.x - widthX, bounds.position.y + bounds.size.y - widthY);
        glVertex2f(bounds.position.x + bounds.size.x - widthX, bounds.position.y + widthY);

        glEnd();
    }

    void OpenGLRenderer::drawLine(const Vector2f & point1, const Vector2f & point2, const float width, const Vector4f & color)
    {
        glDisable(GL_TEXTURE_2D);
        glLineWidth(width);
        glBegin(GL_LINES);
        glColor4f(color.x, color.y, color.z, color.w);
        glVertex2f(point1.x, point1.y);
        glVertex2f(point2.x, point2.y);
        glEnd();
    }

    std::shared_ptr<Texture> OpenGLRenderer::createTexture()
    {
        return std::make_shared<OpenGLTexture>();
    }

    std::shared_ptr<OpenGLRenderer> OpenGLRenderer::create(const std::shared_ptr<AppWindow> & appWindow)
    {
    #if defined(GUISE_PLATFORM_WINDOWS)
        auto windowContext = appWindow->getWin32HDC();
        return std::shared_ptr<OpenGLRenderer>(new OpenGLRenderer(windowContext));
    #elif defined(GUISE_PLATFORM_LINUX)
        auto display = appWindow->getLinuxDisplay();
        auto window = appWindow->getLinuxWindow();
        auto screen = appWindow->getLinuxScreen();
        return std::shared_ptr<OpenGLRenderer>(new OpenGLRenderer(display, window, screen));
    #endif
    }

#if defined(GUISE_PLATFORM_WINDOWS)
    std::shared_ptr<OpenGLRenderer> OpenGLRenderer::create(::HDC deviceContextHandle)
    {
        return std::shared_ptr<OpenGLRenderer>(new OpenGLRenderer(deviceContextHandle));
    }
#elif defined(GUISE_PLATFORM_WINDOWS)
    std::shared_ptr<OpenGLRenderer> OpenGLRenderer::create(::Display * display, ::Window window, int screen)
    {
        return std::shared_ptr<OpenGLRenderer>(new OpenGLRenderer(display, window, screen));
    }
#endif

    const Vector4f & OpenGLRenderer::getClearColor()
    {
        return m_clearColor;
    }

    void OpenGLRenderer::setClearColor(const Vector4f & color)
    {
        m_clearColor = color;
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void OpenGLRenderer::setViewportSize(const Vector2ui32 & position, const Vector2ui32 & size)
    {
        m_viewPort = { position, size };
        glViewport(position.x, position.y, size.x, size.y);
        updateProjectionMatrix();
    }

    void OpenGLRenderer::setDpi(const int32_t dpi)
    {
        m_dpi = dpi;
        updateProjectionMatrix();
    }

    void OpenGLRenderer::clearColor()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRenderer::present()
    {
    #if defined(GUISE_PLATFORM_WINDOWS)
        ::SwapBuffers(m_deviceContextHandle);
    #elif defined(GUISE_PLATFORM_LINUX)
        ::glXSwapBuffers(m_display, m_window);
    #endif
    }

    void OpenGLRenderer::updateProjectionMatrix()
    {
        Matrix4x4f orthoMat;
        orthoMat.loadOrthographic(0.0f, (float)m_viewPort.size.x, (float)m_viewPort.size.y, 0.0f, 0.0f, 1.0f);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glLoadMatrixf(orthoMat.m);
    }

#if defined(GUISE_PLATFORM_WINDOWS)
    OpenGLRenderer::OpenGLRenderer(HDC deviceContextHandle) :
        m_deviceContextHandle(deviceContextHandle),
        m_dpi(GUISE_DEFAULT_DPI)
    {
        // Filling the pixel fromat structure.
        static PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW |
            PFD_SUPPORT_OPENGL |
            PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            24,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            16, //DepthBits,
            8, //StencilBits,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };


        GLuint pixelFormat;
        if ((pixelFormat = ChoosePixelFormat(deviceContextHandle, &pfd)) == 0)
        {
            throw std::runtime_error("Cannot choose pixel format.");
        }
        if (::SetPixelFormat(deviceContextHandle, pixelFormat, &pfd) == false)
        {
            throw std::runtime_error("Cannot set pixel format.");
        }

        m_context = ::wglCreateContext(deviceContextHandle);
        if (m_context == NULL)
        {
            throw std::runtime_error("Failed to create OpenGL context..");
        }

        wglMakeCurrent(NULL, NULL);
        wglMakeCurrent(deviceContextHandle, m_context);

        if (!OpenGL::loadExtensions())
        {
            throw std::runtime_error("Missing OpenGL extensions.");
        }

        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

#elif defined(GUISE_PLATFORM_LINUX) 
    OpenGLRenderer::OpenGLRenderer(::Display * display, ::Window window, int screen) :
        m_display(display),
        m_window(window),
        m_dpi(GUISE_DEFAULT_DPI)
    {

    }

#endif

}

#endif