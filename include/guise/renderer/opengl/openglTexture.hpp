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

#ifndef GUISE_OPENGL_TEXTURE_HPP
#define GUISE_OPENGL_TEXTURE_HPP

#include "guise/build.hpp"

#if !defined(GUISE_DISABLE_OPENGL)

#include "guise/renderer/texture.hpp"
#include "guise/renderer/opengl/opengl.hpp"

namespace Guise
{

    /**
    * OpenGL Texture class.
    *
    *
    */
    class GUISE_API OpenGLTexture : public Texture
    {

    public:

        OpenGLTexture();
        OpenGLTexture(const uint8_t * data, const PixelFormat pixelFormat, const Vector2ui32 & dimensions);
        ~OpenGLTexture();

        void load(const uint8_t * data, const PixelFormat pixelFormat, const Vector2ui32 & dimensions);
        void unload();
        void bind(const size_t index) const;
        void unbind() const;

        PixelFormat getPixelFormat() const;
        Vector2ui32 getDimensions() const;

    private:

        Vector2ui32 m_dimensions;
        GLuint      m_id;
        PixelFormat m_pixelFormat;

    };

}

#endif

#endif