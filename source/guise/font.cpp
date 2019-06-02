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

#include "guise/font.hpp"
#include <map>
#include <unordered_map>
#include <fstream>
#include <limits>
#include <algorithm>
#include "freetype/FreeTypeAmalgam.h"


namespace Guise
{

    static inline uint64_t getGlyphIndex(const wchar_t character, const uint32_t height)
    {
        return static_cast<uint64_t>(character) | (static_cast<uint64_t>(height) << 32);
    }

    static inline wchar_t getCharacterFromGlypthIndex(const uint64_t index)
    {
        return static_cast<wchar_t>(index & uint64_t(0xFFFFFFFFUL));
    }

    static inline uint32_t getHeightFromGlypthIndex(const uint64_t index)
    {
        return static_cast<uint32_t>((index >> 32) & uint64_t(0xFFFFFFFFUL));
    }

    // Font implementations.
    struct Font::Impl
    {
        Impl() :
            dataSize(0),
            library(nullptr),
            face(nullptr),
            currentFontSize(0)
        { }

        ~Impl()
        {
            if (library)
            {
                glyphs.clear();
                FT_Done_FreeType(library);
            }
        }


        struct Glyph
        {

            Glyph(const FT_UInt index, FT_BitmapGlyph bitmapGlyph, const FT_Pos baseline, const FT_Pos horiAdvance, const FT_Pos horiBearingX, const FT_Pos horiBearingY) :
                index(index),
                bitmapGlyph(bitmapGlyph),               
                baseline(baseline),
                horiAdvance(horiAdvance),
                horiBearingX(horiBearingX),
                horiBearingY(horiBearingY)
            { }

            ~Glyph()
            {
                FT_Done_Glyph(reinterpret_cast<FT_Glyph>(bitmapGlyph));
            }

            FT_UInt         index;
            FT_BitmapGlyph  bitmapGlyph;
            FT_Pos          baseline;
            FT_Pos          horiAdvance;
            FT_Pos          horiBearingX;
            FT_Pos          horiBearingY;

        };


        Glyph * getGlypth(const wchar_t character, const uint32_t height)
        {
            auto mapIndex = getGlyphIndex(character, height);

            auto it = glyphs.find(mapIndex);
            if (it != glyphs.end())
            {
                return it->second.get();
            }

            FT_Error error = 0;

            FT_UInt index = 0;
            if ((index = FT_Get_Char_Index(face, static_cast<FT_ULong>(character))) == 0)
            {
                //  "Can not get the character index"
                return nullptr;
            }

            if ((error = FT_Load_Glyph(face, index, FT_LOAD_RENDER)) != 0)
            {
                //  "Can not load the character"
                return nullptr;
            }

            auto & metrics = face->glyph->metrics;

            FT_GlyphSlot slot = face->glyph;
            FT_Glyph glyph;
            if ((error = FT_Get_Glyph(slot, &glyph)) != 0)
            {
                //  "Can not get the glyph"
                return nullptr;
            }
            
            FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
            auto bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
            const FT_Pos baseline = (metrics.height >> 6) - (metrics.horiBearingY >> 6);

            auto newIt = glyphs.insert({ mapIndex, std::make_unique<Glyph>(index, bitmapGlyph, baseline, metrics.horiAdvance >> 6, metrics.horiBearingX >> 6, metrics.horiBearingY >> 6) });
            return newIt.first->second.get();
        }

        std::unique_ptr<uint8_t[]>                  data;
        size_t                                      dataSize;
        FT_Library                                  library;
        FT_Face                                     face;
        std::map<uint64_t, std::unique_ptr<Glyph> > glyphs;
        uint32_t                                    currentFontSize;
    };



    std::shared_ptr<Font> Font::create(const std::string & font)
    {
        return std::shared_ptr<Font>(new Font(font));
    }

    Font::~Font()
    { }

    bool Font::isValid() const
    {
        return m_isValid;
    }

    bool Font::createBitmap(const std::wstring & text, const uint32_t height, const uint32_t dpi, std::unique_ptr<uint8_t[]> & buffer, Vector2<size_t> & dimensions, size_t & /*baseline*/)
    {
        if (!m_isValid || !text.size())
        {
            return false;
        }

        FT_Error error = 0;

        const uint32_t fontSize = height * dpi / 96;
        if (fontSize != m_impl->currentFontSize)
        {
            if ((error = FT_Set_Char_Size(m_impl->face, 0, height * 64, dpi, dpi)) != 0)
            {
                //  "Can not setup the font size, FreeType error: %i\n", FTError);
                return false;
            }

            m_impl->currentFontSize = fontSize;
        }

        std::vector<Impl::Glyph*> glyphs;

        Vector2<FT_Pos> size(0, 0);

        //FT_Pos startPos;
        const bool hasKerning = FT_HAS_KERNING(m_impl->face);
        Vector2<FT_Pos> lowDim = { std::numeric_limits<FT_Pos>::max(), std::numeric_limits<FT_Pos>::max() };
        Vector2<FT_Pos> highDim = { std::numeric_limits<FT_Pos>::min(), std::numeric_limits<FT_Pos>::min() };
        FT_Pos penPos = 0;
        FT_UInt prevIndex = 0;


        // Calcualte text bounding box and pen start position.
        for (size_t i = 0; i < text.size(); i++)
        {
            auto glyph = m_impl->getGlypth(text[i], fontSize);
            if (glyph)
            {
                glyphs.push_back(glyph);
                auto & bitmap = glyph->bitmapGlyph->bitmap;

                // Move pen if font has kerning.
                if (hasKerning && prevIndex)
                {
                    FT_Vector  delta;
                    FT_Get_Kerning(m_impl->face, prevIndex, glyph->index, FT_KERNING_DEFAULT, &delta);

                    penPos += delta.x >> 6;
                }
                prevIndex = glyph->index;


                // Calc X dimensions.
                FT_Pos lowX = penPos + glyph->horiBearingX;
                if (lowX < lowDim.x)
                {
                    lowDim.x = lowX;
                }

                FT_Pos highX = penPos + glyph->horiBearingX + bitmap.width;
                if (highX > highDim.x)
                {
                    highDim.x = highX;
                }

                // Calc Y dimensions.
                FT_Pos lowY = glyph->horiBearingY - bitmap.rows;
                if (lowY < lowDim.y)
                {
                    lowDim.y = lowY;
                }

                FT_Pos highY = glyph->horiBearingY;
                if (highY > highDim.y)
                {
                    highDim.y = highY;
                }

                penPos += glyph->horiAdvance;
            }    
        }
        
        penPos = -lowDim.x;
        prevIndex = 0;

        size.x = highDim.x - lowDim.x;
        size.y = highDim.y - lowDim.y;

        const size_t bufferSize = size.x * size.y * 4;
        dimensions = size;

        buffer = std::make_unique<uint8_t[]>(bufferSize);
        uint8_t * glyphBuffer = buffer.get();
        memset(glyphBuffer, 0, bufferSize);
       
        // Render glyphs.
        for (auto it = glyphs.begin(); it != glyphs.end(); it++)
        {
            auto glyph = (*it);
            auto & bitmap = glyph->bitmapGlyph->bitmap;
            auto bitmapBuffer = bitmap.buffer;

            // Move pen if font has kerning.
            if (hasKerning && prevIndex)
            {
                FT_Vector  delta;
                FT_Get_Kerning(m_impl->face, prevIndex, glyph->index, FT_KERNING_DEFAULT, &delta);

                penPos += delta.x >> 6;
            }
            prevIndex = glyph->index;

            for (int y = 0; y < bitmap.rows; y++)
            {
                int intY = bitmap.rows - 1 - y;

                for (int x = 0; x < bitmap.width; x++)
                {
                    const int glyphIndex = ((y - (glyph->baseline + lowDim.y)) * dimensions.x * 4) + ((x + penPos + glyph->horiBearingX) * 4);
                    const int bitmapIndex = (intY * bitmap.width) + x;

                    glyphBuffer[glyphIndex] = 255;
                    glyphBuffer[glyphIndex + 1] = 255;
                    glyphBuffer[glyphIndex + 2] = 255;
                    glyphBuffer[glyphIndex + 3] = std::max(bitmapBuffer[bitmapIndex], glyphBuffer[glyphIndex + 3]);
                }
            }

            penPos += glyph->horiAdvance;
        }

        return true;
    }
    
    Font::Font(const std::string & font) :
        m_isValid(false),
        m_impl(std::make_shared<Impl>())
    {
        

        // Try to open font as path. 
        if (!readFile(font, m_impl->data, m_impl->dataSize))
        {
            // Try to open file as font family.
            std::string systemFontPath = getFontPath(font);
            if (!systemFontPath.size() || !readFile(systemFontPath, m_impl->data, m_impl->dataSize))
            {
                return;
            }
        }

        FT_Error error = 0;

        if ((error = FT_Init_FreeType(&(m_impl->library))) != 0)
        {
            //  "Failed to initialize the FreeType library, error code: %i\n", FTError);
            return;
        }

        // Load face from memory.
        if ((error = FT_New_Memory_Face(m_impl->library, reinterpret_cast<FT_Byte *>(m_impl->data.get()), m_impl->dataSize, 0, &(m_impl->face))) != 0)
        {
            //  "Failed to load font: %s, FreeType error: %i\n", p_pFileName, FTError);
            return;
        }
       
        // Select the unicode character map
        if ((error = FT_Select_Charmap(m_impl->face, FT_ENCODING_UNICODE)) != 0)
        {
            //  "Can not select the unicode character map, FreeType error: %i\n", FTError);
            return;
        }

        m_isValid = true;
    }

    bool Font::readFile(const std::string & path, std::unique_ptr<uint8_t[]> & data, size_t & dataSize) const
    {
        std::ifstream file(path, std::ifstream::binary);
        if (!file.is_open())
        {
            return false;
        }

        file.seekg(0, std::fstream::end);
        dataSize = static_cast<size_t>(file.tellg());
        if (!dataSize)
        {
            return false;
        }


        file.seekg(0, std::fstream::beg);
        data = std::make_unique<uint8_t[]>(dataSize);
        file.read(reinterpret_cast<char*>(data.get()), dataSize);

        return true;
    }

    std::string Font::getFontPath(const std::string & font) const
    {
    #if defined(GUISE_PLATFORM_WINDOWS)
        char * charPath = nullptr;
        size_t len = 0;
        errno_t err = _dupenv_s(&charPath, &len, "windir");
        if (err || !charPath)
        {
            return "";
        }

        std::string path = std::string(charPath) + "\\Fonts\\" + font + ".ttf";
        free(charPath);
        
        return path;
    #endif
    }


    // Font library implementations.
    static std::unordered_map<std::string, std::shared_ptr<Font>> g_fontLibrary;
    std::shared_ptr<Font> FontLibrary::get(const std::string & font)
    {
        auto it = g_fontLibrary.find(font);
        if (it == g_fontLibrary.end())
        {
            auto newFont = Font::create(font);
            if (!newFont->isValid())
            {
                return nullptr;
            }

            g_fontLibrary.insert({font, newFont});
            return newFont;
        }

        return it->second;
    }
    
  

}