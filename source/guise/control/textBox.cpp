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
        const Input & input = m_canvas.getInput();
        const bool shiftPressed = input.getKeyState(Input::Key::ShiftLeft) || input.getKeyState(Input::Key::ShiftRight);

        switch (e.type)
        {
            case Input::EventType::MouseJustPressed:
            {
                size_t index = 0;
                if (intersectText(e.position.x, index))
                {                  
                    if (!shiftPressed)
                    {
                        m_cursorIndex = index;
                    }
                     
                    m_cursorSelectFromIndex = index;                    
                    m_cursorBlinkTimer = std::chrono::system_clock::now();
                }
                else
                {
                    m_cursorIndex = 0;
                    m_cursorSelectFromIndex = 0;
                }

                m_mousePressed = true;
            }
            break;
            case Input::EventType::MouseRelease:
            {
                m_mousePressed = false;
            }
            break;
            case Input::EventType::MouseMove:
            {
               // std::cout << "Move: " << e.position.x << " " << e.position.y << std::endl;
                if (m_mousePressed)
                {
                    size_t index = 0;
                    if (intersectText(e.position.x, index))
                    {
                        /*if (!shiftPressed)
                        {
                            m_cursorIndex = index;
                        }*/

                        m_cursorSelectFromIndex = index;
                        m_cursorBlinkTimer = std::chrono::system_clock::now();
                    }
                    else
                    {
                        m_cursorIndex = 0;
                        m_cursorSelectFromIndex = 0;
                    }
                }
            }
            break;
            case Input::EventType::Texting:
            {
                if(e.character > 0x1F && e.character != 0x7F)
                {
                    eraseSelected();

                    m_text.insert(m_text.begin() + m_cursorIndex, e.character);
                    m_cursorIndex++;
                    m_cursorSelectFromIndex = m_cursorIndex;

                    m_changed = true;
                    onChange(m_text);
                    m_cursorBlinkTimer = std::chrono::system_clock::now();
                }
            }
            break;
            case Input::EventType::KeyboardJustPressed:
            {
                switch (e.key)
                {
                    case Input::Key::Home:
                    {
                        if (m_charPositions.size())
                        {
                            m_cursorIndex = 0;
                            if (!shiftPressed)
                            {
                                m_cursorSelectFromIndex = m_cursorIndex;
                            }

                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                    }
                    break;
                    case Input::Key::End:
                    {
                        if (m_charPositions.size())
                        {
                            m_cursorIndex = m_charPositions.size() - 1;
                            if (!shiftPressed)
                            {
                                m_cursorSelectFromIndex = m_cursorIndex;
                            }

                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                    }
                    break;
                    default: break;
                }
            }
            break;
            case Input::EventType::KeyboardPress:
            {
                switch (e.key)
                {
                    case Input::Key::Backspace:
                    {
                        if (eraseSelected())
                        {
                            m_changed = true;
                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                        else if (m_cursorIndex && m_text.size())
                        {
                            m_text.erase(m_text.begin() + (m_cursorIndex - 1));
                            m_cursorIndex--;
                            m_cursorSelectFromIndex = m_cursorIndex;

                            m_changed = true;
                            onChange(m_text);
                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                    }
                    break;
                    case Input::Key::Delete:
                    {
                        if (eraseSelected())
                        {
                            m_changed = true;
                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                        else if (m_text.size() && m_cursorIndex < m_text.size())
                        {
                            m_text.erase(m_text.begin() + m_cursorIndex);

                            m_changed = true;
                            onChange(m_text);
                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                    }
                    break;
                    case Input::Key::X:
                    {
                        if (input.getKeyState(Input::Key::ControlLeft) || input.getKeyState(Input::Key::ControlRight))
                        {
                            auto cutText = getSelected();
                            if (cutText.size() && Platform::setClipboardText(cutText))
                            {
                                eraseSelected();
                                m_changed = true;
                                onChange(m_text);
                                m_cursorBlinkTimer = std::chrono::system_clock::now();
                            }
                        }                       
                    }
                    break;
                    case Input::Key::C:
                    {
                        if (input.getKeyState(Input::Key::ControlLeft) || input.getKeyState(Input::Key::ControlRight))
                        {
                            auto copyText = getSelected();
                            if (copyText.size())
                            {
                                Platform::setClipboardText(copyText);
                            }
                        }        
                    }
                    break;
                    case Input::Key::V:
                    {
                        if (input.getKeyState(Input::Key::ControlLeft) || input.getKeyState(Input::Key::ControlRight))
                        {
                            eraseSelected();

                            auto clipboard = Platform::getClipboardText();
                            if (clipboard.size())
                            {
                                m_text.insert(m_text.begin() + m_cursorIndex, clipboard.begin(), clipboard.end());
                                m_cursorIndex += clipboard.size();
                                m_cursorSelectFromIndex = m_cursorIndex;
                                m_changed = true;
                                onChange(m_text);
                                m_cursorBlinkTimer = std::chrono::system_clock::now();
                            }
                        }

                        m_cursorBlinkTimer = std::chrono::system_clock::now();
                    }
                    break;
                    case Input::Key::A:
                    {
                        if (input.getKeyState(Input::Key::ControlLeft) || input.getKeyState(Input::Key::ControlRight))
                        {
                            m_cursorIndex = 0;
                            m_cursorSelectFromIndex = m_charPositions.size() ? m_charPositions.size() - 1 : 0;
                        }
                    }
                    break;
                    case Input::Key::Left:
                    {
                        if (m_cursorIndex > 0)
                        {
                            if (shiftPressed)
                            {
                                m_cursorIndex--;
                            }
                            else
                            {
                                if (m_cursorSelectFromIndex != m_cursorIndex)
                                {
                                    m_cursorIndex = std::min(m_cursorIndex, m_cursorSelectFromIndex);
                                    m_cursorSelectFromIndex = m_cursorIndex;
                                }
                                else
                                {
                                    m_cursorIndex--;
                                    m_cursorSelectFromIndex = m_cursorIndex;
                                }
                            }

                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                    }
                    break;
                    case Input::Key::Right:
                    {
                        if (m_charPositions.size())
                        {
                            if (shiftPressed)
                            {
                                m_cursorIndex = std::min(m_cursorIndex + 1, m_charPositions.size() - 1);
                            }
                            else
                            {
                                if (m_cursorSelectFromIndex != m_cursorIndex)
                                {
                                    m_cursorIndex = std::max(m_cursorIndex, m_cursorSelectFromIndex);
                                    m_cursorSelectFromIndex = m_cursorIndex;
                                }
                                else
                                {
                                    m_cursorIndex = std::min(m_cursorIndex + 1, m_charPositions.size() - 1);
                                    m_cursorSelectFromIndex = m_cursorIndex;
                                }    
                            }

                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                    }
                    break;
                    default: break;
                }
            }
            break;
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
                m_cursorIndex = 0;
                m_cursorSelectFromIndex = 0;
                m_textTexture.reset();
            }
            else
            {
                if (m_font)
                {
                    std::unique_ptr<uint8_t[]> data;
                    Vector2<size_t> dimensions = { 0, 0 };

                    size_t baseline = 0;
                    m_charPositions.clear();
                    if (m_font->createBitmap(m_text, m_textStyle.getFontSize(), m_dpi, data, dimensions, baseline, &m_charPositions))
                    {
                        m_baseline = static_cast<float>(baseline);
                        m_baseHeight = static_cast<float>(m_textStyle.getFontSize()) * scale;

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

        const bool isSelected = m_cursorIndex != m_cursorSelectFromIndex;
        m_textBounds = { { m_renderBounds.position.x + getPaddingLow().x, m_renderBounds.position.y }, { 0.0f, 0.0f } };
        if (m_textTexture)
        {          
            // Render text     
            m_textBounds.size = m_textTexture->getDimensions();
            m_textBounds.position.y += m_renderBounds.size.y - ((m_renderBounds.size.y - m_baseHeight) / 2.0f) + m_baseline - m_textBounds.size.y;
            renderer.drawQuad(m_textBounds, m_textTexture, m_textStyle.getFontColor());

            // Render select background.
            if (isSelected && m_cursorIndex < m_charPositions.size() && m_cursorSelectFromIndex < m_charPositions.size())
            {
                size_t index1 = m_cursorIndex;
                size_t index2 = m_cursorSelectFromIndex;
                if (index1 > index2)
                {
                    std::swap(index1, index2);
                }

                Bounds2f selectBounds =
                {
                    { static_cast<float>(m_charPositions[index1]), m_textBounds.position.y },
                    { static_cast<float>(m_charPositions[index2] - m_charPositions[index1]), m_textBounds.size.y }
                };
                selectBounds.position.x += m_textBounds.position.x;

                renderer.drawQuad(selectBounds, { 0.0f, 0.45f, 0.85f, 0.7f });
            }
        }

        if (m_active)
        {
            auto endTime = std::chrono::system_clock::now();
            std::chrono::duration<double> duration = endTime - m_cursorBlinkTimer;

            bool showCursor = (static_cast<int>(duration.count() * 1000.0f) % 1000) < 500;          

            if (!isSelected && showCursor)
            {
                float cursorPosition = static_cast<float>(m_charPositions.size()) ? static_cast<float>(m_charPositions[m_cursorIndex]) : 0.0f;
                cursorPosition += m_textBounds.position.x;

                Bounds2f cursorBounds = { { cursorPosition, m_renderBounds.position.y },{ 1.0f, m_renderBounds.size.y } };
                renderer.drawQuad(cursorBounds, m_textStyle.getFontColor());
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

    Style::FontStyle & TextBox::getTextStyle()
    {
        return m_textStyle;
    }

    TextBox::~TextBox()
    {
        getCanvas().unregisterDpiSensitive(this);
    }

    TextBox::TextBox(std::shared_ptr<Canvas> & canvas) :       
        Control(*canvas),
        Style::PaintRectStyle(canvas->getStyleSheet()->getSelector("text-box")),
        Style::ParentStyle(canvas->getStyleSheet()->getSelector("text-box")),
        m_active(false),
        m_baseline(0.0f),
        m_baseHeight(0.0f),
        m_changed(true),
        m_cursorIndex(0),
        m_cursorSelectFromIndex(0),
        m_dpi(canvas->getDpi()),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_textBounds(0.0f, 0.0f, 0.0f, 0.0f)
    {
        getCanvas().registerDpiSensitive(this);

        if (auto textStyle = canvas->getStyleSheet()->getSelector("text-box-text"))
        {
            m_textStyle = { textStyle };
            m_font = FontLibrary::get(m_textStyle.getFontFamily());
        }
    }

    void TextBox::onNewDpi(const int32_t dpi)
    {
        m_dpi = dpi;
        m_changed = true;
    }

    void TextBox::onActiveChange(bool active)
    {
        m_active = active;
        if (m_active)
        {
            if (m_charPositions.size())
            {
                m_cursorIndex = m_charPositions.size() - 1;
                m_cursorSelectFromIndex = m_cursorIndex;
            }
            else
            {
                m_cursorIndex =  0;
                m_cursorSelectFromIndex = 0;
            }    
        }
        else
        {
            m_cursorIndex = 0;
            m_cursorSelectFromIndex = 0;
        }

        m_cursorBlinkTimer = std::chrono::system_clock::now();     
    }

    bool TextBox::eraseSelected()
    {
        size_t index1 = m_cursorIndex;
        size_t index2 = m_cursorSelectFromIndex;
        if (index1 > index2)
        {
            std::swap(index1, index2);
        }

        if (m_cursorIndex == m_cursorSelectFromIndex || index2 > m_text.size())
        {
            return false;
        }

        m_text.erase(m_text.begin() + index1, m_text.begin() + index2);

        m_cursorIndex = index1;
        m_cursorSelectFromIndex = index1;
        return true;
    }

    std::wstring TextBox::getSelected()
    {
        size_t index1 = m_cursorIndex;
        size_t index2 = m_cursorSelectFromIndex;
        if (index1 > index2)
        {
            std::swap(index1, index2);
        }

        if (m_cursorIndex == m_cursorSelectFromIndex || index2 >= m_charPositions.size())
        {
            return L"";
        }

        return m_text.substr(index1, index2 - index1);
    }

    bool TextBox::intersectText(const float point, size_t & index)
    {
        if (!m_textTexture)
        {
            return false;
        }

        const float textPoint = point - m_textBounds.position.x;
        if (textPoint < 0.0f)
        {
            index = 0;
        }
        else if (textPoint > m_textBounds.size.x)
        {
            index = m_charPositions.size() ? m_charPositions.size() - 1 : 0;
        }
        else
        {
            index = 0;

            for (size_t i = 0; i < m_charPositions.size(); i++)
            {
                float pos = static_cast<float>(m_charPositions[i]);
                if (textPoint <= pos)
                {
                    
                    size_t prevIndex = i ? i - 1 : 0;
                    size_t nextIndex = i + 1 < m_charPositions.size() ? i + 1 : m_charPositions.size() - 1;
                    float prev = static_cast<float>(m_charPositions[prevIndex]);
                    float next = static_cast<float>(m_charPositions[nextIndex]);

                    if (textPoint < pos - ((pos - prev) / 2.0f) )
                    {
                        index = prevIndex;
                    }
                    else if (textPoint > pos + ((next - pos) / 2.0f))
                    {
                        index = nextIndex;
                    }
                    else
                    {
                        index = i;
                    }

                    break;
                }
                
                index = i;
            }            
        }

        return true;
    }

}