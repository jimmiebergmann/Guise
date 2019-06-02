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

#include "guise/renderer/opengl/openglTexture.hpp"

#if !defined(GUISE_DISABLE_OPENGL)

namespace Guise
{
    // Global helper functions
    static const GLenum g_OpenGLInternalFormat[2] =
    {
        GL_RGB,
        GL_RGBA,
    };

    // OpenGLTexture implementations.
    OpenGLTexture::OpenGLTexture() :
        m_dimensions(0, 0),
        m_id(0),
        m_pixelFormat(PixelFormat::RGBA8)
    { }

    OpenGLTexture::OpenGLTexture(const uint8_t * data, const PixelFormat pixelFormat, const Vector2ui32 & dimensions) :
        OpenGLTexture()
    {
        load(data, pixelFormat, dimensions);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        if (m_id)
        {
            glDeleteTextures(1, &m_id);
        }
    }

    void OpenGLTexture::load(const uint8_t * data, const PixelFormat pixelFormat, const Vector2ui32 & dimensions)
    {
        if (m_id)
        {
            glDeleteTextures(1, &m_id);
        }

        m_dimensions = dimensions;
        m_pixelFormat = pixelFormat;

        // Generate an OpenGL texture id.
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        const GLint internalFormat = g_OpenGLInternalFormat[static_cast<size_t>(pixelFormat)];

        // Set the texure data;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_dimensions.x, m_dimensions.y, 0,
            internalFormat, GL_UNSIGNED_BYTE, static_cast<const GLvoid *>(data));

        // Set default filers
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture::unload()
    {
        if (m_id)
        {
            glDeleteTextures(1, &m_id);
            m_id = 0;
            m_dimensions = {0, 0};
        }
    }

    void OpenGLTexture::bind(const size_t index) const
    {
        OpenGL::glActiveTexture(GL_TEXTURE0 + index);
        ::glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void OpenGLTexture::unbind() const
    {
        ::glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::PixelFormat OpenGLTexture::getPixelFormat() const
    {
        return m_pixelFormat;
    }

    Vector2ui32 OpenGLTexture::getDimensions() const
    {
        return m_dimensions;
    }

}

#endif