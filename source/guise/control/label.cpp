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
        getCanvas().queueControlRendering(this);
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

                if (m_font->createBitmap(m_text, m_fontHeight, m_dpi, data, dimensions, baseline))
                {
                    if (!m_texture)
                    {
                        m_texture = renderer.createTexture();
                    }

                    m_texture->load(data.get(), Texture::PixelFormat::RGBA8, dimensions);
                    m_renderBounds.size = m_texture->getDimensions() * GUISE_DEFAULT_DPI / m_dpi;
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

    Label::~Label()
    {
        getCanvas().unregisterDpiSensitive(this);
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
        m_dpi(canvas->getDpi()),
        m_font(FontLibrary::get(m_fontFamily)),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_text(text),
        m_texture(nullptr)
    {
        getCanvas().registerDpiSensitive(this);
    }

    Label::Label(std::shared_ptr<Canvas> & canvas, const std::string & font, const std::wstring & text) :
        Control(*canvas),
        m_dpi(canvas->getDpi()),
        // LabelStyle(canvas->getStyleSheet()->getSelector("label")),
        m_changed(true),
        m_font(FontLibrary::get(font)),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_text(text),
        m_texture(nullptr)
    {
        getCanvas().registerDpiSensitive(this);
    }

    void Label::onNewDpi(const int32_t dpi)
    {
        m_dpi = dpi;
        m_changed = true;
    }

}