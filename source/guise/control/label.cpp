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
    std::shared_ptr<Label> Label::create(const std::wstring & text)
    {
        return std::shared_ptr<Label>(new Label(text));
    }

    std::shared_ptr<Label> Label::create(const std::string & font, const std::wstring & text)
    {
        return std::shared_ptr<Label>(new Label(font, text));
    }

    std::shared_ptr<Font> Label::getFont() const
    {
        return m_font;
    }

    const std::wstring & Label::getText() const
    {
        return m_text;
    }

    ControlType Label::getType() const
    {
        return ControlType::Label;
    }

    void Label::setText(const std::wstring & text)
    {
        if (text != m_text)
        {
            m_text = text;
            onChange(m_text);
            update();
        }
    }

    Label::Label(const std::wstring & text) :        
        Style::FontStyle(this, nullptr),
        m_changedText(true),
        m_dpi(0),
        m_fontSequence(m_font),
        m_text(text),
        m_texture(nullptr),
        m_textureSize(0, 0)
    {
    }

    Label::Label(const std::string & font, const std::wstring & text) :
        Style::FontStyle(this, nullptr),
        m_changedText(true),
        m_dpi(0),
        m_font(FontLibrary::get(font)),
        m_fontSequence(m_font),
        m_text(text),
        m_texture(nullptr),
        m_textureSize(0, 0)
    {
    }

    void Label::onCanvasChange(Canvas * canvas)
    {
        m_dpi = canvas->getDpi();

        // We need to get rid of old connection...
        canvas->onDpiChange.connectAnonymously([this](uint32_t dpi)
        {
            m_dpi = dpi;
            m_changedText = true;
            update();
        });

        updateEmptyProperties(canvas->getStyleSheet()->getSelector("label"));
        m_font = FontLibrary::get(getFontFamily());

        onUpdate();
    }

    void Label::onRender(RendererInterface & rendererInterface)
    {
        if (m_changedText)
        {
            m_changedText = false;

            if (m_loadData)
            {
                if (!m_texture)
                {
                    m_texture = rendererInterface.createTexture();
                }

                m_texture->load(m_loadData.get(), Texture::PixelFormat::RGBA8, m_textureSize);
                m_loadData.reset();
            }
        }

        if (m_texture)
        {
            rendererInterface.drawQuad(getBounds(), m_texture, getFontColor());
        }
    }

    void Label::onResize()
    {
        setBounds({ getBounds().position, m_textureSize });
    }

    void Label::onUpdate()
    {
        if (m_changedText)
        {
            m_fontSequence.createSequence(m_text, getFontSize(), m_dpi);

            m_loadData.reset();
            if (m_fontSequence.createBitmapRgba(m_loadData, m_textureSize))
            {
                resize();
            }
            else
            {
                m_changedText = false;
            }
        }
    }

}