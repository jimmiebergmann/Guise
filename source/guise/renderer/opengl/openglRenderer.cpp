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

#if defined(GUISE_ENABLE_OPENGL_RENDERER)

#include "guise/appWindow.hpp"

namespace Guise
{

    void OpenGLRenderer::setCulling(const Vector2f &, const Vector2f &)
    {

    }

    void OpenGLRenderer::drawQuad(const Bounds2f & bounds, const Vector4f & color)
    {
        glBegin(GL_QUADS);
        glColor4f(color.x, color.y, color.z, color.w);
        glVertex2f(bounds.position.x, bounds.position.y);
        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y);
        glVertex2f(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
        glVertex2f(bounds.position.x, bounds.position.y + bounds.size.y);
        glEnd();
    }

    void OpenGLRenderer::drawLine(const Vector2f & point1, const Vector2f & point2, const float width, const Vector4f & color)
    {
        glLineWidth(width);
        glBegin(GL_LINES);
        glColor4f(color.x, color.y, color.z, color.w);
        glVertex2f(point1.x, point1.y);
        glVertex2f(point2.x, point2.y);
        glEnd();
    }

    std::shared_ptr<Renderer> OpenGLRenderer::create(const std::shared_ptr<AppWindow> & appWindow)
    {
        auto windowContext = appWindow->getWindowContext();

        return std::shared_ptr<Renderer>(new OpenGLRenderer(windowContext));
    }
    std::shared_ptr<Renderer> OpenGLRenderer::create(HDC deviceContextHandle)
    {
        return std::shared_ptr<Renderer>(new OpenGLRenderer(deviceContextHandle));
    }

    /*std::shared_ptr<Renderer> OpenGLRenderer::create(const std::shared_ptr<Renderer> & sibling)
    {
        return std::shared_ptr<Renderer>(new OpenGLRenderer(sibling));
    }*/

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
        Matrix4x4f orthoMat;
        orthoMat.orthographic(  (float)position.x, (float)position.x + (float)size.x,
                                (float)position.y + (float)size.y, (float)position.y,
                                0.0f, 1.0f);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glLoadMatrixf(orthoMat.m);

        glViewport(position.x, position.y, size.x, size.y);
    }

    void OpenGLRenderer::clearColor()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRenderer::present()
    {
        ::SwapBuffers(m_deviceContextHandle);
    }

    OpenGLRenderer::OpenGLRenderer(HDC deviceContextHandle) :
        m_deviceContextHandle(deviceContextHandle)
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

        glEnable(GL_COLOR_MATERIAL);
    }

}

#endif