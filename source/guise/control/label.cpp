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

    bool Label::handleInputEvent(const Input::Event &)
    {
        return false;
    }

    void Label::update(const Bounds2f & canvasBound)
    {
        m_renderBounds.position = canvasBound.position;

        if (m_changedText)
        {
            m_changedText = false;
            m_fontSequence.createSequence(m_text, getFontSize(), m_dpi);
            m_changed = true;
        }

        if (m_changed)
        {
            m_loadData.reset();
            if (m_fontSequence.createBitmapRgba(m_loadData, m_loadDimensions))
            {
                m_renderBounds.size = m_loadDimensions;
            }
            else
            {
                m_changed = false;
            }
        }

        getCanvas().queueControlRendering(this);    
    }

    void Label::render(RendererInterface & renderer)
    {
        if (m_changed)
        {
            m_changed = false;

            if (m_loadData)
            {
                if (!m_texture)
                {
                    m_texture = renderer.createTexture();
                }

                m_texture->load(m_loadData.get(), Texture::PixelFormat::RGBA8, m_loadDimensions);
                m_loadData.reset();
            }
        }

        if (m_texture)
        {
            renderer.drawQuad(m_renderBounds, m_texture, getFontColor());
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
            m_changedText = true;          
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
        Style::FontStyle(canvas->getStyleSheet()->getSelector("label")),       
        m_changed(true),
        m_changedText(true),
        m_dpi(canvas->getDpi()),
        m_font(FontLibrary::get(getFontFamily())),
        m_fontSequence(m_font),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_text(text),
        m_texture(nullptr)
    {
        getCanvas().registerDpiSensitive(this);
    }

    Label::Label(std::shared_ptr<Canvas> & canvas, const std::string & font, const std::wstring & text) :
        Control(*canvas),
        Style::FontStyle(canvas->getStyleSheet()->getSelector("label")),
        m_changed(true),
        m_changedText(true),
        m_dpi(canvas->getDpi()),
        m_font(FontLibrary::get(font)),
        m_fontSequence(m_font),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_text(text),
        m_texture(nullptr)
    {
        getCanvas().registerDpiSensitive(this);
    }

    void Label::onNewDpi(const int32_t dpi)
    {
        m_dpi = dpi;
        m_changedText = true;
        forceUpdate();
    }

}