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

#include "guise/control/label.hpp"
#include "guise/canvas.hpp"
#include <map>
#include <iostream>


namespace Guise
{
    // Label style implementations.
    LabelStyle::LabelStyle() :
        m_fontHeight(24),
        m_fontFamily("Arial")
    { }

    LabelStyle::LabelStyle(const std::shared_ptr<Style::Selector> & selector) :
        LabelStyle()
    {
        if (!selector)
        {
            return;
        }

        auto & properties = selector->getProperties();
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            if (it->first == "font-family")
            {
                //m_font = it->second->getVector2f();
            }
            else if (it->first == "font-size")
            {
                /*switch (it->second->getDataType())
                {
                    case Style::Property::DataType::Float:
                    {
                        m_padding.x = m_padding.y = m_padding.w = m_padding.z = it->second->getFloat();
                    }
                    break;
                    case Style::Property::DataType::Integer:
                    {
                        m_padding = { it->second->getVector2f(), 0.0f, 0.0f };
                    }
                    break;
                    case Style::Property::DataType::Vector4f:
                    {
                        m_padding = it->second->getVector4f();
                    }
                    break;
                    default: break;
                }*/
            }
           
        }
    }



    const uint32_t LabelStyle::getFontHeight() const
    {
        return m_fontHeight;
    }
    const std::string & LabelStyle::getFontFamily() const
    {
        return m_fontFamily;
    }
  

    void LabelStyle::setFontHeight(const uint32_t height)
    {
        m_fontHeight = height;
    }
    void LabelStyle::setFontFamily(const std::string & family)
    {
        m_fontFamily = family;
    }
  

    // Button implementations.
    std::shared_ptr<Label> Label::create(std::shared_ptr<Canvas> & canvas, const std::wstring & text)
    {
        return std::shared_ptr<Label>(new Label(canvas, text));
    }

    std::shared_ptr<Label> Label::create(std::shared_ptr<Canvas> & canvas, const std::string & font, const std::wstring & text)
    {
        return std::shared_ptr<Label>(new Label(canvas, font, text));
    }

    ControlType Label::getType() const
    {
        return ControlType::Label;
    }

    Control * Label::handleInputEvent(const Input::Event &)
    {
        
        std::wcout << L"Hit label: " << m_text << std::endl;

        return this;
    }

    void Label::update(const Bounds2f & canvasBound)
    {
        m_renderBounds.position = canvasBound.position;
    }

    void Label::render(RendererInterface & renderer)
    {
        if (m_changed)
        {
            m_changed = false;
            m_renderBounds.size = { 0.0f, 0.0f };

            if (!m_text.size())
            {
                m_texture.reset();
                return;
            }

            if (m_font)
            {
                std::unique_ptr<uint8_t[]> data;
                Vector2<size_t> dimensions = { 0, 0 };
                size_t baseline = 0;

                if (m_font->createBitmap(m_text, m_fontHeight, 96, data, dimensions, baseline))
                {
                    if (!m_texture)
                    {
                        m_texture = renderer.createTexture();
                    }

                    m_texture->load(data.get(), Texture::PixelFormat::RGBA8, dimensions);
                    m_renderBounds.size = m_texture->getDimensions();
                }           
            }

        }

        if (m_texture)
        {
            renderer.drawQuad(m_renderBounds, m_texture, { 0.0f, 0.0, 0.0f, 1.0f });
        }
        
    }


    Bounds2f Label::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f Label::getSelectBounds() const
    {
        return m_renderBounds;
    }

    Control * Label::queryHit(const Vector2f & point) const
    {
        if (m_renderBounds.intersects(point))
        {
            return const_cast<Label *>(this);
        }
      
        return nullptr;
    }

    void Label::setText(const std::wstring & text)
    {
        if (text != m_text)
        {
            m_text = text;
            forceUpdate();
            m_changed = true;
        }
    }

    const std::wstring & Label::getText() const
    {
        return m_text;
    }

    std::shared_ptr<Font> Label::getFont() const
    {
        return m_font;
    }

    Label::Label(std::shared_ptr<Canvas> & canvas, const std::wstring & text) :
        Control(*canvas),
        // LabelStyle(canvas->getStyleSheet()->getSelector("label")),
        m_changed(true),
        m_font(FontLibrary::get(m_fontFamily)),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_text(text),
        m_texture(nullptr)
    { }

    Label::Label(std::shared_ptr<Canvas> & canvas, const std::string & font, const std::wstring & text) :
        Control(*canvas),
        // LabelStyle(canvas->getStyleSheet()->getSelector("label")),
        m_changed(true),
        m_font(FontLibrary::get(font)),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_text(text),
        m_texture(nullptr)
    { }

    //std::unique_ptr<uint8_t[]> Label::stringToBitmap(const std::wstring & /*text*/, const std::string /*family*/, const uint32_t /*height*/,
    //                                                 const Texture::PixelFormat /*pixelFormat*/, Vector2ui32 & /*dimensions*/)
    //{
       /* struct FreeTypeData
        {
            FreeTypeData() :
                error(0),
                face(nullptr),
                library(nullptr)
            { }

            ~FreeTypeData()
            {
                if (library)
                {
                    for (auto it = glyphSet.begin(); it != glyphSet.end(); it++)
                    {
                        FT_Done_Glyph(reinterpret_cast<FT_Glyph>(it->second.bitmapGlyph));
                    }
                    FT_Done_FreeType(library);
                }
            }

            struct Glyph
            {
                Glyph(FT_BitmapGlyph bitmapGlyph, const FT_Vector & advance, const int baseline) :
                    bitmapGlyph(bitmapGlyph),
                    advance(advance),
                    baseline(baseline)
                { }

                FT_Vector       advance;
                FT_BitmapGlyph  bitmapGlyph;
                int             baseline;
                
            };

            FT_Error                    error;
            FT_Face                     face;
            std::map<FT_ULong, Glyph>   glyphSet;
            FT_Library                  library;

        };

        auto freeType = std::make_unique<FreeTypeData>();


        if ((freeType->error = FT_Init_FreeType(&freeType->library)) != 0)
        {
            //  "Failed to initialize the FreeType library, error code: %i\n", FTError);
            return nullptr;
        }

        // Get font file path.
        char *pValue;
        size_t len;
        errno_t err = _dupenv_s(&pValue, &len, "windir");
        if (err)
        {
            return nullptr;
        }
        //std::string filename = pValue;
        //filename += "/fonts/" + family + ".ttf";
        free(pValue);

        std::string filename = "C:\\Users\\sours\\AppData\\Local\\Microsoft\\Windows\\Fonts\\" + family + ".ttf";

        // Create a new font face.
        if ((freeType->error = FT_New_Face(freeType->library, filename.c_str(), 0, &freeType->face)) != 0)
        {
            //  "Failed to load font: %s, FreeType error: %i\n", p_pFileName, FTError);
            return nullptr;
        }*/




        // Setup the font size
        /*if ((freeType->error = FT_Set_Char_Size(freeType->face, 12, 12, 72, 72)) != 0)
        {
            //  "Can not setup the font size, FreeType error: %i\n", FTError);
            return nullptr;
        }*/

        /*if ((freeType->error = FT_Set_Pixel_Sizes(freeType->face, 0, 60)) != 0)
        {
            //  "Can not setup the font size, FreeType error: %i\n", FTError);
            return nullptr;
        }*/




        /*
        if ((freeType->error = FT_Set_Char_Size(freeType->face, 0, 48 * 64, 120, 120)) != 0)
        {
            //  "Can not setup the font size, FreeType error: %i\n", FTError);
            return nullptr;
        }



        // Select the unicode character map
        if ((freeType->error = FT_Select_Charmap(freeType->face, FT_ENCODING_UNICODE)) != 0)
        {
            //  "Can not select the unicode character map, FreeType error: %i\n", FTError);
            return nullptr;
        }

        //auto num_glyphs = freeTypeFace->num_glyphs;

        FT_GlyphSlot slot = freeType->face->glyph;
        Vector2i32 totalDimensions = { 0, 0 };
        int textBaseline = 0;

        for (size_t i = 0; i < text.size(); i++)
        {
            const FT_ULong currChar = static_cast<FT_ULong>(text[i]);

            auto it = freeType->glyphSet.find(currChar);
            if (it == freeType->glyphSet.end())
            {
                if (FT_Get_Char_Index(freeType->face, currChar) == 0)
                {
                    continue;
                }

                // Load the glyph corresponding to the current character
                if ((freeType->error = FT_Load_Char(freeType->face, currChar, FT_LOAD_RENDER)) != 0)
                {
                    //  "Can not load the character '%c'(%i), FreeType error: %i\n", CurChar, (int)CurChar, FTError);
                    return nullptr;
                }
                auto metrics = slot->metrics;

                // Get the FreeType glypth
                FT_Glyph glyph;
                if ((freeType->error = FT_Get_Glyph(slot, &glyph)) != 0)
                {
                    //  "Can not get the glyph, FreeType error: %i\n", FTError);
                    return nullptr;
                }
      

                std::cout << text[i] << std::endl;
                std::cout << "width:" << (metrics.width >> 6) << std::endl;
                std::cout << "height:" << (metrics.height >> 6) << std::endl;
                std::cout << "horiBearingX:" << (metrics.horiBearingX >> 6) << std::endl;
                std::cout << "horiBearingY:" << (metrics.horiBearingY >> 6) << std::endl;
                std::cout << "horiAdvance:" << (metrics.horiAdvance >> 6) << std::endl;
                std::cout << "vertBearingX:" << (metrics.vertBearingX >> 6) << std::endl;
                std::cout << "vertBearingY:" << (metrics.vertBearingY >> 6) << std::endl;
                std::cout << "vertAdvance:" << (metrics.vertAdvance >> 6) << std::endl;
                std::cout << std::endl;


                // Create the FreeType bitmap
                FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
                auto bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
                //FT_Bitmap & bitmap = bitmapGlyph->bitmap;
                const int glyphBaseline = (metrics.height >> 6) - (metrics.horiBearingY >> 6);
                freeType->glyphSet.insert({ currChar, {bitmapGlyph, slot->advance, glyphBaseline } });

                totalDimensions.x += slot->advance.x >> 6;

                if (totalDimensions.y < slot->metrics.height >> 6)
                {
                    totalDimensions.y = slot->metrics.height >> 6;
                }
 
                if (textBaseline < glyphBaseline)
                {
                    textBaseline = glyphBaseline;
                }
            }
            else
            {
                totalDimensions.x += it->second.advance.x >> 6;
            }

        }


        // Create bitmap


        std::unique_ptr<uint8_t[]> pixels = std::make_unique<uint8_t[]>(totalDimensions.x * totalDimensions.y * 4);

        Vector2i32 penPos = { 0, 0 };
        for (size_t i = 0; i < text.size(); i++)
        {
            auto & glyph = freeType->glyphSet.find(static_cast<FT_ULong>(text[i]))->second;
            auto & bitmap = glyph.bitmapGlyph->bitmap;
            auto buffer = bitmap.buffer;
            Vector2i32 bitmapSize = { bitmap.width, bitmap.rows };

            //for (int y = bitmapSize.y - 1; y >= 0; y--)
            for (int y = 0, inverseY = bitmapSize.y - 1; y < bitmapSize.y; y++, inverseY--)
            {
                const int finalY = inverseY - (textBaseline - glyph.baseline);

                for (int x = 0; x < bitmapSize.x; x++)
                {
                    const int pixelIndex = (totalDimensions.x * (inverseY) * 4) + (x * 4) + (penPos.x * 4);
                    const int bufferIndex = (bitmapSize.x * y) + x;
                    //std::cout << (int)buffer[bufferIndex] << " ";

                    pixels[pixelIndex] = 255;
                    pixels[pixelIndex + 1] = 255;
                    pixels[pixelIndex + 2] = 255;
                    pixels[pixelIndex + 3] = buffer[bufferIndex];
                    
                }
                //std::cout << std::endl;

                //buffer += bitmapSize.x;
            }
            //std::cout << std::endl << std::endl;

            penPos.x += glyph.advance.x >> 6;


        }

        dimensions = totalDimensions;
        return pixels;    */

       // return nullptr;
   // }

}