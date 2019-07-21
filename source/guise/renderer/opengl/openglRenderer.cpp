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

namespace Guise
{

    float OpenGLRenderer::getScale() const
    {
        return m_scale;
    }

    void OpenGLRenderer::setLevel(const size_t level)
    {
        m_level = static_cast<float>(level);
    }

    void OpenGLRenderer::drawRect(const Bounds2f & bounds, const Style::PaintRectStyle & style)
    {
        drawQuad(bounds, style.getBackgroundColor());

        const bool border = style.getBorderStyle() != Style::Property::BorderStyle::None && style.getBorderWidth();
        if (border)
        {
            drawBorder(bounds, style.getBorderWidth(), style.getBorderColor());
        }
    }

    void OpenGLRenderer::drawQuad(const Bounds2f & bounds, const Vector4f & color)
    {
        Bounds2f newBounds = Bounds2f::floor(bounds);

        if (m_maskStack.size())
        {
            newBounds.innerJoin(m_maskStack.top());
        }

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);

        glColor4f(color.x, color.y, color.z, color.w);
        glVertex3f(newBounds.position.x, newBounds.position.y, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x, newBounds.position.y, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x, newBounds.position.y + newBounds.size.y, m_level);
        glVertex3f(newBounds.position.x, newBounds.position.y + newBounds.size.y, m_level);

        glEnd();
    }

    void OpenGLRenderer::drawQuad(const Bounds2f & bounds, const std::shared_ptr<Texture> & texture, const Vector4f & color)
    {
        Bounds2f newBounds = Bounds2f::floor(bounds);

        Vector2f newTexCoords[2] = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };

        if (m_maskStack.size())
        {
            Vector2f boundsVec[2] =
            {
                newBounds.position, newBounds.position + newBounds.size
            };

            newBounds.innerJoin(m_maskStack.top());

            Vector2f maskVec[2] =
            {
                newBounds.position, newBounds.position + newBounds.size
            };            
            
            newTexCoords[0].x =        ((maskVec[0].x - boundsVec[0].x) / (boundsVec[1].x - boundsVec[0].x));
            newTexCoords[0].y = 1.0f - ((maskVec[1].y - boundsVec[0].y) / (boundsVec[1].y - boundsVec[0].y));
            newTexCoords[1].x =        ((maskVec[1].x - boundsVec[0].x) / (boundsVec[1].x - boundsVec[0].x));
            newTexCoords[1].y = 1.0f - ((maskVec[0].y - boundsVec[0].y) / (boundsVec[1].y - boundsVec[0].y));

        }

        glEnable(GL_TEXTURE_2D);
        texture->bind(0);

        glBegin(GL_QUADS);
        glColor4f(color.x, color.y, color.z, color.w);

        glTexCoord2f(newTexCoords[0].x, newTexCoords[1].y);
        glVertex3f(newBounds.position.x, newBounds.position.y, m_level);

        glTexCoord2f(newTexCoords[1].x, newTexCoords[1].y);
        glVertex3f(newBounds.position.x + newBounds.size.x, newBounds.position.y, m_level);

        glTexCoord2f(newTexCoords[1].x, newTexCoords[0].y);
        glVertex3f(newBounds.position.x + newBounds.size.x, newBounds.position.y + newBounds.size.y, m_level);

        glTexCoord2f(newTexCoords[0].x, newTexCoords[0].y);
        glVertex3f(newBounds.position.x, newBounds.position.y + newBounds.size.y, m_level);

        glEnd();

        texture->unbind();
    }

    void OpenGLRenderer::drawBorder(const Bounds2f & bounds, const float width, const Vector4f & color)
    {
        const Bounds2f newBounds = Bounds2f::floor(bounds);

        const float sWidth = std::floor(width * m_scale);

        const float widthX = newBounds.size.x > sWidth ? sWidth : newBounds.size.x;
        const float widthY = newBounds.size.y > sWidth ? sWidth : newBounds.size.y;

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);

        glColor4f(color.x, color.y, color.z, color.w);

        glVertex3f(newBounds.position.x, newBounds.position.y, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x , newBounds.position.y, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x - widthX, newBounds.position.y + widthY, m_level);
        glVertex3f(newBounds.position.x + widthX, newBounds.position.y + widthY, m_level);

        glVertex3f(newBounds.position.x + widthX, newBounds.position.y + newBounds.size.y - widthY, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x - widthX, newBounds.position.y + newBounds.size.y - widthY, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x, newBounds.position.y + newBounds.size.y, m_level);
        glVertex3f(newBounds.position.x, newBounds.position.y + newBounds.size.y, m_level);

        glVertex3f(newBounds.position.x, newBounds.position.y, m_level);
        glVertex3f(newBounds.position.x + widthX, newBounds.position.y + widthY, m_level);
        glVertex3f(newBounds.position.x + widthX, newBounds.position.y + newBounds.size.y - widthY, m_level);
        glVertex3f(newBounds.position.x, newBounds.position.y + newBounds.size.y, m_level);


        glVertex3f(newBounds.position.x + newBounds.size.x, newBounds.position.y, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x, newBounds.position.y + newBounds.size.y, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x - widthX, newBounds.position.y + newBounds.size.y - widthY, m_level);
        glVertex3f(newBounds.position.x + newBounds.size.x - widthX, newBounds.position.y + widthY, m_level);

        glEnd();
    }

    void OpenGLRenderer::drawLine(const Vector2f & point1, const Vector2f & point2, const float width, const Vector4f & color)
    {
        Vector2f p1 = point1 * m_scale;
        Vector2f p2 = point2 * m_scale;

        glDisable(GL_TEXTURE_2D);
        glLineWidth(width * m_scale);
        glBegin(GL_LINES);
        glColor4f(color.x, color.y, color.z, color.w);
        glVertex3f(p1.x, p1.y, m_level);
        glVertex3f(p2.x, p2.y, m_level);
        glEnd();
    }

    void OpenGLRenderer::pushMask(const Bounds2i32 & bounds)
    {
        if (m_maskStack.size() < 255)
        {
            m_maskStack.push(bounds);
        }      
    }

    void OpenGLRenderer::popMask()
    {
        if (m_maskStack.size())
        {
            m_maskStack.pop();
        }
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

    void OpenGLRenderer::setScale(const float scale)
    {
        m_scale = scale;
    }

    void OpenGLRenderer::clearColor()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       
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
        orthoMat.loadOrthographic(0.0f, (float)m_viewPort.size.x, (float)m_viewPort.size.y, 0.0f, -255.0f, 0.0f);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glLoadMatrixf(orthoMat.m);
    }

#if defined(GUISE_PLATFORM_WINDOWS)
    OpenGLRenderer::OpenGLRenderer(HDC deviceContextHandle) :
        m_deviceContextHandle(deviceContextHandle),
        m_scale(1.0f),
        m_level(0.0f)
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
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        
    }

#elif defined(GUISE_PLATFORM_LINUX)
    OpenGLRenderer::OpenGLRenderer(::Display * display, ::Window window, int screen) :
        m_context(NULL),
        m_display(display),
        m_window(window),
        m_scale(1.0f),
        m_level(0.0f)
    {
        if(display == NULL)
        {
            throw std::runtime_error("display is NULL.");
        }
        if(window == 0)
        {
            throw std::runtime_error("window is 0.");
        }

        // Create the visual information
        GLint attribs[] =
        {
            GLX_RGBA, GLX_DOUBLEBUFFER,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_DEPTH_SIZE, 16,
            GLX_STENCIL_SIZE, 8,
            0L, 0L
        };

        //int fbElements = 0;
        //::GLXFBConfig *fbc = glXChooseFBConfig(display, screen, 0, &fbElements);

        ::XVisualInfo * visualInfo = NULL;
        if( (visualInfo = glXChooseVisual(m_display, 0, attribs) ) == NULL )
        {
            throw std::runtime_error("Cannot choose visual information.");
        }

        // Create the color map and set it
        if(!(m_colormap = XCreateColormap(m_display, RootWindow(m_display, screen), visualInfo->visual, AllocNone)))
        {
            XFree(visualInfo);
            throw std::runtime_error("Cannot create the colormap.");
        }

        //m_colormap = DefaultColormap(m_display, screen);

        // Set the new color map
        //XSetWindowColormap(m_display, m_window, m_colormap);

        // Create a temporary context.
        m_context = glXCreateContext(m_display, visualInfo, NULL, GL_TRUE);
        if(!m_context)
        {
            XFree(visualInfo);
            throw std::runtime_error("Cannot create OpenGL context.");
        }

        // Make the temporary context to the current one.
        glXMakeCurrent(m_display, m_window, m_context);

        // Clear the visual info since we are done with it.
        XFree(visualInfo);

        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }

#endif

}

#endif
