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

#ifndef GUISE_RENDERER_TEXTURE_HPP
#define GUISE_RENDERER_TEXTURE_HPP

#include "guise/build.hpp"

namespace Guise
{
    
    /**
    * Texture class.
    *
    *
    */
    class GUISE_API Texture
    {

    public:

        enum class PixelFormat : uint32_t
        {
            RGB8,
            RGBA8
        };

        virtual ~Texture();

        virtual void load(const uint8_t * data, const PixelFormat pixelFormat, const Vector2ui32 & dimensions) = 0;
        virtual void unload() = 0;
        virtual void bind(const size_t index) const = 0;
        virtual void unbind() const = 0;

        virtual PixelFormat getPixelFormat() const = 0;
        virtual Vector2ui32 getDimensions() const = 0;
     
    };

}

#endif
