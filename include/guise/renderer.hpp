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

#ifndef GUISE_RENDERER_HPP
#define GUISE_RENDERER_HPP

#include "guise/build.hpp"
#include "guise/math/bounds.hpp"
#include "guise/renderer/texture.hpp"
#include "guise/style.hpp"
#include <memory>

namespace Guise
{

    // Forward declarations
    class AppWindow;

    /**
    * Renderer interface class.
    *
    *
    */
    class GUISE_API RendererInterface
    {

    public:

        virtual float getScale() const = 0;

        virtual void setLevel(const size_t level) = 0;

        virtual void drawRect(const Bounds2f & bounds, const Style::PaintRectStyle & style) = 0;

        virtual void drawQuad(const Bounds2f & bounds, const Vector4f & color) = 0;      
        virtual void drawQuad(const Bounds2f & bounds, const std::shared_ptr<Texture> & texture, const Vector4f & color) = 0;

        virtual void drawBorder(const Bounds2f & bounds, const float width, const Vector4f & color) = 0;

        virtual void drawLine(const Vector2f & point1, const Vector2f & point2, const float width, const Vector4f & color) = 0;

        virtual void pushMask(const Bounds2i32 & bounds) = 0;
        virtual void popMask() = 0;

        virtual std::shared_ptr<Texture> createTexture() = 0;

        //virtual void drawQuadRounded(const Vector2f & position, const Vector2f & size) = 0;
    };

    /**
    * Renderer base class.
    *
    *
    */
    class GUISE_API Renderer : public RendererInterface
    {

    public:

        static std::shared_ptr<Renderer> createDefault(const std::shared_ptr<AppWindow> & appWindow);
    #if defined(GUISE_PLATFORM_WINDOWS)
        static std::shared_ptr<Renderer> createDefault(HDC deviceContextHandle);
    #endif

        virtual ~Renderer();

        virtual const Vector4f & getClearColor() = 0;

        virtual void setClearColor(const Vector4f & color) = 0;

        virtual void setViewportSize(const Vector2ui32 & position, const Vector2ui32 & size) = 0;

        virtual void setScale(const float scale) = 0;

        virtual void clearColor() = 0;

        virtual void clearDepth() = 0;

        virtual void present() = 0;

    };

}

#endif