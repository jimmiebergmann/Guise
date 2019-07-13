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

#ifndef GUISE_FONT_HPP
#define GUISE_FONT_HPP

#include "guise/build.hpp"
#include "guise/math/bounds.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Guise
{

    class FontSequence;

    class GUISE_API Font
    {

    public:

        /**
        *
        * @param font Path to font file, or name of font family.
        */
        static std::shared_ptr<Font> create(const std::string & font);

        ~Font();

        bool isValid() const;

        float getVerticalMax() const;
        float getVerticalMin() const;

    private:

        Font(const Font &) = delete;
        Font(const std::string & font);

        bool readFile(const std::string & path, std::unique_ptr<uint8_t[]> & data, size_t & dataSize) const;

        std::string getFontPath(const std::string & font) const;

        friend class FontSequence;

        bool                    m_isValid;
        float                   m_verticalMax;
        float                   m_verticalMin;
        struct Impl;
        std::shared_ptr<Impl>   m_impl;

    };

    class GUISE_API FontSequence
    {

    public:

        FontSequence();
        FontSequence(std::shared_ptr<Font> & font);

        float calcVerticalPosition(const float height) const;

        bool createSequence(const std::wstring & text, const uint32_t height, const uint32_t dpi);

        bool createBitmapaAlpha(std::unique_ptr<uint8_t[]> & buffer, Vector2<size_t> & dimensions,
                                const size_t from = 0, const size_t to = std::numeric_limits<size_t>::max());
        bool createBitmapRgba(std::unique_ptr<uint8_t[]> & buffer, Vector2<size_t> & dimensions,
                              const size_t from = 0, const size_t to = std::numeric_limits<size_t>::max());

        bool findIndex(const int32_t width, const size_t from, size_t & to) const;

        size_t getBaseline() const;

        Bounds1i32 getHorizontalBounds(const size_t index) const;

        size_t getCount() const;

        size_t intersect(const Vector2f & point) const;


    private:

       

        struct Impl;
        std::shared_ptr<Impl>   m_impl;

    };

    class GUISE_API FontLibrary
    {

    public:

        static std::shared_ptr<Font> get(const std::string & font);

    };

}


#endif