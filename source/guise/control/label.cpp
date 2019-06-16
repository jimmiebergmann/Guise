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

                if (m_font->createBitmap(m_text, m_fontSize, m_dpi, data, dimensions, baseline))
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
        Style::LabelStyle(canvas->getStyleSheet()->getSelector("label")),
        Control(*canvas),
        m_changed(true),
        m_dpi(canvas->getDpi()),
        m_font(FontLibrary::get(m_fontFamily)),
        m_newTexture(false),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_text(text),
        m_texture(nullptr)
    {
        getCanvas().registerDpiSensitive(this);
    }

    Label::Label(std::shared_ptr<Canvas> & canvas, const std::string & font, const std::wstring & text) :       
        Style::LabelStyle(canvas->getStyleSheet()->getSelector("label")),
        Control(*canvas),        
        m_changed(true),
        m_dpi(canvas->getDpi()),
        m_font(FontLibrary::get(font)),
        m_newTexture(false),
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