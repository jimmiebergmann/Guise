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

#include "guise/control/textBox.hpp"
#include "guise/canvas.hpp"
#include "guise/platform.hpp"
#include <iostream>

namespace Guise
{
    TextInputHandler::TextInputHandler() :
        m_shiftKey(false),
        m_controlKey(false),
        m_cKey(false),
        m_vKey(false)
    { }

    void TextInputHandler::addEvent(const Input::Event & e)
    {
        if (e.type == Input::EventType::KeyboardPress)
        {
            switch (e.key)
            {
                case Input::Key::ControlLeft:
                case Input::Key::ControlRight:
                {
                    if (!m_controlKey)
                    {
                        if (m_vKey)
                        {
                            onPaste();
                        }
                        else if (m_cKey)
                        {
                            onCopy();
                        }
                        m_controlKey = true;
                    }                   
                }
                break;
                case Input::Key::V:
                {
                    if (!m_vKey)
                    {
                        if (m_controlKey)
                        {
                            onPaste();
                        }
                        m_vKey = true;
                    }
                }
                break;
                case Input::Key::C:
                {
                    if (!m_cKey)
                    {
                        if (m_controlKey)
                        {
                            onCopy();
                        }
                        m_cKey = true;
                    }                   
                }
                break;
            }
        }
        else if (e.type == Input::EventType::KeyboardRelease)
        {
            switch (e.key)
            {
                case Input::Key::ShiftLeft:
                case Input::Key::ShiftRight:
                    m_shiftKey = false;
                    break;
                case Input::Key::V:
                    m_vKey = false;
                    break;
                case Input::Key::C:
                    m_cKey = false;
                    break;
            }
        }
    }


    // Text box implementations.
    std::shared_ptr<TextBox> TextBox::create(std::shared_ptr<Canvas> & canvas)
    {
        return std::shared_ptr<TextBox>(new TextBox(canvas));
    }

    ControlType TextBox::getType() const
    {
        return ControlType::TextBox;
    }

    bool TextBox::handleInputEvent(const Input::Event & e)
    {
        m_textInputHandle.addEvent(e);

        switch (e.type)
        {
            case Input::EventType::Texting:
            {
                if (e.character == '\b')
                {
                    if (m_text.size())
                    {
                        m_text.pop_back();
                        onChange(m_text);
                    }
                }
                else
                {
                    m_text += e.character;
                    onChange(m_text);
                }

                m_cursorBlinkTimer = std::chrono::system_clock::now();
                m_changed = true;
            }
            break;
            case Input::EventType::KeyboardPress:
            {
                const Input & input = m_canvas.getInput();
                switch (e.key)
                {
                    case Input::Key::V:
                    {
                        if (input.getKeyState(Input::Key::ControlLeft) || input.getKeyState(Input::Key::ControlRight))
                        {
                            m_text += Platform::getClipboardText();
                        }
                    }
                    break;
                }

                /*if (e.key == Input::Key::Left || e.key == Input::Key::Right)
                {
                    std::wcout << Platform::getClipboardText() << std::endl;
                }*/
            }
            break;
            /*case Input::EventType::MouseJustPressed:
            {
                if (m_renderBounds.intersects(e.position))
                {
                    m_active = true;
                }
            }
            break;
            case Input::EventType::MouseRelease:
            {
                if (!m_renderBounds.intersects(e.position))
                {
                    m_active = false;
                }
            }
            break;*/
            default: break;
        }

        return true;
    }

    void TextBox::update(const Bounds2f & canvasBound)
    {
        m_renderBounds = calcRenderBounds(canvasBound, getPosition(), getSize(), getOverflow());
        getCanvas().queueControlRendering(this);
    }

    void TextBox::render(RendererInterface & renderer)
    {
        const float scale = m_canvas.getScale();

        if (m_changed)
        {
            m_changed = false;

            if (!m_text.size())
            {
                m_textTexture.reset();
            }
            else
            {
                if (m_font)
                {
                    std::unique_ptr<uint8_t[]> data;
                    Vector2<size_t> dimensions = { 0, 0 };

                    size_t baseline = 0;
                    if (m_font->createBitmap(m_text, getFontSize(), m_dpi, data, dimensions, baseline))
                    {
                        m_baseline = static_cast<float>(baseline);
                        m_baseHeight = static_cast<float>(getFontSize()) * scale;

                        if (!m_textTexture)
                        {
                            m_textTexture = renderer.createTexture();
                        }

                        m_textTexture->load(data.get(), Texture::PixelFormat::RGBA8, dimensions);
                    }
                }
            }
        }

        renderer.drawQuad(m_renderBounds, getBackgroundColor());

        const bool drawBorder = getBorderStyle() != Style::Property::BorderStyle::None && getBorderWidth() > 0.0f;
        if (drawBorder)
        {
            const float borderWidth = std::floor(getBorderWidth() * scale);
            renderer.drawBorder(m_renderBounds, borderWidth, getBorderColor());
        }

        float cursorPos = m_renderBounds.position.x + getPaddingLow().x + 1.0f;
        if (m_textTexture)
        {
            Bounds2f textBounds = { m_renderBounds.position, m_textTexture->getDimensions() };
            textBounds.position.y += m_renderBounds.size.y - ((m_renderBounds.size.y - m_baseHeight) / 2.0f) + m_baseline - textBounds.size.y;
            textBounds.position.x += getPaddingLow().x;

            cursorPos += textBounds.size.x;

            renderer.drawQuad(textBounds, m_textTexture, getFontColor());
        }

        if (m_active)
        {
            auto endTime = std::chrono::system_clock::now();
            std::chrono::duration<double> duration = endTime - m_cursorBlinkTimer;

            bool showCursor = (static_cast<int>(duration.count() * 1000.0f) % 1000) < 500;          

            if (showCursor)
            {
                Bounds2f cursorBounds = { { cursorPos, m_renderBounds.position.y },{ 1.0f, m_renderBounds.size.y } };
                renderer.drawQuad(cursorBounds, getFontColor());
            }         
        }      
    }

    Bounds2f TextBox::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f TextBox::getSelectBounds() const
    {
        return m_renderBounds;
    }

    TextBox::~TextBox()
    {
        getCanvas().unregisterDpiSensitive(this);
    }

    TextBox::TextBox(std::shared_ptr<Canvas> & canvas) :
        Style::TextBoxStyle(canvas->getStyleSheet()->getSelector("text-box")),
        Control(*canvas),
        m_active(false),
        m_baseline(0.0f),
        m_baseHeight(0.0f),
        m_changed(true),
        m_cursorPosition(0),
        m_dpi(canvas->getDpi()),
        m_font(FontLibrary::get(getFontFamily())),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_childBounds(0.0f, 0.0f, 0.0f, 0.0f)
    {
        m_textInputHandle.onCopy = [this]()
        {
            std::cout << "Copy!" << std::endl;
        };

        m_textInputHandle.onPaste = [this]()
        {
            std::cout << "Paste!" << std::endl;
        };

        getCanvas().registerDpiSensitive(this);

        /*if (auto activeStyle = canvas->getStyleSheet()->getSelector("button:active"))
        {
            m_activeStyle = { activeStyle, this };
        }

        if (auto disabledStyle = canvas->getStyleSheet()->getSelector("button:disabled"))
        {
            m_disabledStyle = { disabledStyle, this };
        }

        if (auto hoverStyle = canvas->getStyleSheet()->getSelector("button:hover"))
        {
            m_hoverStyle = { hoverStyle, this };
        }*/
    }

    void TextBox::onNewDpi(const int32_t dpi)
    {
        m_dpi = dpi;
        m_changed = true;
    }

    void TextBox::onActiveChange(bool active)
    {
        m_active = active;
        m_cursorBlinkTimer = std::chrono::system_clock::now();
    }

}