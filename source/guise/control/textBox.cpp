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
#include <locale>

namespace Guise
{

    // Text box implementations.
    std::shared_ptr<TextBox> TextBox::create()
    {
        return std::shared_ptr<TextBox>(new TextBox());
    }

    const std::wstring & TextBox::getText() const
    {
        return m_text;
    }

    Style::FontStyle & TextBox::getTextStyle()
    {
        return m_textStyle;
    }
    const Style::FontStyle & TextBox::getTextStyle() const
    {
        return m_textStyle;
    }

    ControlType TextBox::getType() const
    {
        return ControlType::TextBox;
    }

    bool TextBox::handleInputEvent(const Input::Event & e)
    {
        const Input & input = getCanvas()->getInput(); // FIX!
        const bool isShiftPressed = input.getKeyState(Input::Key::ShiftLeft) || input.getKeyState(Input::Key::ShiftRight);
        const bool isControlPressed = input.getKeyState(Input::Key::ControlLeft) || input.getKeyState(Input::Key::ControlRight);
        const bool isSelected = m_cursorIndex != m_cursorSelectIndex;

        switch (e.type)
        {
        case Input::EventType::MouseJustPressed:
        {
            if (e.button != 0)
            {
                break;
            }

            size_t index = 0;
            if (intersectTextInterpolated(e.position.x, index))
            {
                if (!isShiftPressed)
                {
                    m_cursorIndex = index;
                }

                m_cursorSelectIndex = index;
                m_cursorBlinkTimer = std::chrono::system_clock::now();
            }
            else
            {
                m_cursorIndex = 0;
                m_cursorSelectIndex = 0;
            }

            m_mousePressed = true;
        }
        break;
        case Input::EventType::MouseRelease:
        {
            if (e.button != 0)
            {
                break;
            }

            m_mousePressed = false;
        }
        break;
        case Input::EventType::MouseMove:
        {
            if (m_mousePressed)
            {
                size_t index = 0;
                if (intersectTextInterpolated(e.position.x, index))
                {
                    m_cursorSelectIndex = index;
                    m_cursorBlinkTimer = std::chrono::system_clock::now();
                }
                else
                {
                    m_cursorIndex = 0;
                    m_cursorSelectIndex = 0;
                }
            }
        }
        break;
        case Input::EventType::Texting:
        {
            if (e.character > 0x1F && e.character != 0x7F)
            {
                eraseSelected();

                m_text.insert(m_text.begin() + m_cursorIndex, e.character);
                m_cursorIndex++;
                m_cursorSelectIndex = m_cursorIndex;

                m_changedText = true;
                onChange(m_text);
                update();
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
                m_cursorIndex = 0;
                if (!isShiftPressed)
                {
                    m_cursorSelectIndex = m_cursorIndex;
                }

                m_cursorBlinkTimer = std::chrono::system_clock::now();

            }
            break;
            case Input::Key::End:
            {
                m_cursorIndex = m_fontSequence.getCount();
                if (!isShiftPressed)
                {
                    m_cursorSelectIndex = m_cursorIndex;
                }

                m_cursorBlinkTimer = std::chrono::system_clock::now();
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
                    m_changedText = true;
                    onChange(m_text);
                    update();
                    m_cursorBlinkTimer = std::chrono::system_clock::now();
                }
                else if (m_cursorIndex && m_text.size())
                {
                    m_text.erase(m_text.begin() + (m_cursorIndex - 1));
                    m_cursorIndex--;
                    m_cursorSelectIndex = m_cursorIndex;

                    m_changedText = true;
                    onChange(m_text);
                    update();
                    m_cursorBlinkTimer = std::chrono::system_clock::now();
                }
            }
            break;
            case Input::Key::Delete:
            {
                if (eraseSelected())
                {
                    m_changedText = true;
                    onChange(m_text);
                    update();
                    m_cursorBlinkTimer = std::chrono::system_clock::now();
                }
                else if (m_text.size() && m_cursorIndex < m_text.size())
                {
                    m_text.erase(m_text.begin() + m_cursorIndex);

                    m_changedText = true;
                    onChange(m_text);
                    update();
                    m_cursorBlinkTimer = std::chrono::system_clock::now();
                }
            }
            break;
            case Input::Key::X:
            {
                if (isControlPressed)
                {
                    auto cutText = getSelected();
                    if (cutText.size() && Platform::setClipboardText(cutText))
                    {
                        eraseSelected();
                        m_changedText = true;
                        onChange(m_text);
                        update();
                        m_cursorBlinkTimer = std::chrono::system_clock::now();
                    }
                }
            }
            break;
            case Input::Key::C:
            {
                if (isControlPressed)
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
                if (isControlPressed)
                {
                    eraseSelected();

                    auto clipboard = Platform::getClipboardText();
                    if (clipboard.size())
                    {
                        m_text.insert(m_text.begin() + m_cursorIndex, clipboard.begin(), clipboard.end());
                        m_cursorIndex += clipboard.size();
                        m_cursorSelectIndex = m_cursorIndex;
                        m_changedText = true;
                        onChange(m_text);
                        update();
                        m_cursorBlinkTimer = std::chrono::system_clock::now();
                    }
                }

                m_cursorBlinkTimer = std::chrono::system_clock::now();
            }
            break;
            case Input::Key::A:
            {
                if (isControlPressed)
                {
                    m_cursorIndex = 0;
                    m_cursorSelectIndex = m_fontSequence.getCount() ? m_fontSequence.getCount() : 0;
                }
            }
            break;
            case Input::Key::Left:
            {
                if (m_cursorIndex > 0)
                {
                    if (isShiftPressed)
                    {
                        m_cursorIndex--;
                    }
                    else
                    {
                        if (isSelected)
                        {
                            m_cursorIndex = std::min(m_cursorIndex, m_cursorSelectIndex);
                        }
                        else
                        {
                            m_cursorIndex--;
                        }
                        m_cursorSelectIndex = m_cursorIndex;
                    }

                    m_cursorBlinkTimer = std::chrono::system_clock::now();
                }
            }
            break;
            case Input::Key::Right:
            {
                if (m_cursorIndex < m_fontSequence.getCount())
                {
                    if (isShiftPressed)
                    {
                        m_cursorIndex++;
                    }
                    else
                    {
                        if (isSelected)
                        {
                            m_cursorIndex = std::max(m_cursorIndex, m_cursorSelectIndex);
                        }
                        else
                        {
                            m_cursorIndex++;
                        }

                        m_cursorSelectIndex = m_cursorIndex;
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

    void TextBox::setText(const std::wstring & text)
    {
        if (text != m_text)
        {
            m_text = text;
            m_cursorIndex = m_text.size();
            m_cursorSelectIndex = m_cursorIndex;
            m_changedText = true;
            onChange(m_text);
        }
    }

    TextBox::TextBox() :       
        MultiStyleControl<Style::ParentPaintRectStyle>(this),
        m_active(false),
        m_changedText(false),
        m_cursorIndex(0),
        m_cursorSelectIndex(0),
        m_dpi(0),
        m_mousePressed(false),
        m_textBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_textureSize(0, 0)
    {
    }

    void TextBox::onCanvasChange(Canvas * canvas)
    {
        m_dpi = canvas->getDpi();

        // We need to get rid of old connection...
        canvas->onDpiChange.connectAnonymously([this](uint32_t dpi)
        {
            m_dpi = dpi;
            m_changedText = true;
            update();
        });

        updateEmptyProperties(canvas->getStyleSheet()->getSelector("text-box"));
        m_textStyle.updateEmptyProperties(canvas->getStyleSheet()->getSelector("text-box-text"));
        m_font = FontLibrary::get(m_textStyle.getFontFamily());
        m_fontSequence = FontSequence(m_font);
    }

    void TextBox::onDisable()
    {}

    void TextBox::onEnable()
    {}

    void TextBox::onRender(RendererInterface & rendererInterface)
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

        // Render background.
        rendererInterface.drawRect(getBounds(), getCurrentStyle());

        rendererInterface.pushMask(getBounds().cutEdges(scale(getPadding())));

        // Render text.
        if (m_texture)
        {          
            rendererInterface.drawQuad(m_textBounds, m_texture, m_textStyle.getFontColor());
        }

        if (m_active)
        {
            // Render cursor.
            std::chrono::duration<double> duration = std::chrono::system_clock::now() - m_cursorBlinkTimer;            
            if ((static_cast<int>(duration.count() * 1000.0f) % 1000) < 500)
            {
                Bounds2f cursorBounds = getBounds().cutEdges(scale(getPadding()));
                cursorBounds.position.x = getBounds().position.x + getCursorPosition(m_cursorIndex);
                cursorBounds.size.x = 1.0f;

                rendererInterface.drawQuad(cursorBounds, m_textStyle.getFontColor());
            }

            // Render selection.
            if (m_cursorIndex != m_cursorSelectIndex)
            {
                float selectLow = getCursorPosition(m_cursorSelectIndex);
                float selectHigh = getCursorPosition(m_cursorIndex);
                if (selectLow > selectHigh)
                {
                    std::swap(selectLow, selectHigh);
                }

                Bounds2f selectBounds =
                {
                    { selectLow + m_textBounds.position.x, m_textBounds.position.y },
                    { selectHigh - selectLow, m_textBounds.size.y }
                };

                rendererInterface.drawQuad(selectBounds, { 0.0f, 0.45f, 0.85f, 0.7f });
            }
        }

        rendererInterface.popMask();
    }

    void TextBox::onResize()
    {
        setBounds(calcStyledBounds(getCurrentStyle(), getBounds(), getScale()));
        calcTextBounds();
    }

    void TextBox::onUpdate()
    {
        if (m_changedText)
        {
            m_fontSequence.createSequence(m_text, m_textStyle.getFontSize(), m_dpi);

            m_loadData.reset();
            if (m_fontSequence.createBitmapRgba(m_loadData, m_textureSize))
            {
                calcTextBounds();
            }
            else
            {
                m_texture.reset();
                m_changedText = false;
            }
        }
    }

    void TextBox::calcTextBounds()
    {
        m_textBounds = getBounds().cutEdges(scale(getPadding()));
        m_textBounds.position.y += m_fontSequence.calcVerticalPosition(m_textBounds.size.y);
        m_textBounds.size = m_textureSize;
    }

    void TextBox::onActiveChange(bool active)
    {
        m_active = active;
        m_cursorIndex = 0;
        m_cursorSelectIndex = 0;
    
        m_cursorBlinkTimer = std::chrono::system_clock::now();     
    }

    bool TextBox::eraseSelected()
    {
        size_t index1 = m_cursorIndex;
        size_t index2 = m_cursorSelectIndex;
        if (index1 > index2)
        {
            std::swap(index1, index2);
        }

        if (index1 == index2 || index2 > m_text.size())
        {
            return false;
        }

        m_text.erase(m_text.begin() + index1, m_text.begin() + index2);

        m_cursorIndex = index1;
        m_cursorSelectIndex = index1;
        return true;
    }

    std::wstring TextBox::getSelected() const
    {
        size_t index1 = m_cursorIndex;
        size_t index2 = m_cursorSelectIndex;
        if (index1 > index2)
        {
            std::swap(index1, index2);
        }

        if (index1 == index2 || index2 > m_fontSequence.getCount())
        {
            return L"";
        }

        return m_text.substr(index1, index2 - index1);
    }

    float TextBox::getCursorPosition(const size_t index) const
    {
        if (!index)
        {
            return 0.0f;
        }

        const bool atEnd = m_fontSequence.getCount() && index >= m_fontSequence.getCount();
        size_t newIndex = index;
        if (index && atEnd)
        {
            newIndex--;
        }
       
        const auto bounds = m_fontSequence.getHorizontalBounds(newIndex);

        float pos = static_cast<float>(bounds.position);
        if (atEnd)
        {
            pos += bounds.size;
        }

        return pos;
    }

    bool TextBox::intersectText(const float point, size_t & index)
    {
        index = 0;
        if (!m_texture || !m_fontSequence.getCount())
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
            index = m_fontSequence.getCount();
        }
        else
        {
            int32_t left = 0;
            int32_t right = m_fontSequence.getCount();
            int32_t mid = 0;
            int32_t prev = -1;

            while (mid != prev)
            {
                prev = mid;
                mid = (left + right) / 2;
                mid = std::min(mid, static_cast<int32_t>(m_fontSequence.getCount() - 1));

                auto bounds = m_fontSequence.getHorizontalBounds(mid);

                if (static_cast<float>(bounds.position) < textPoint)
                {
                    left = mid;
                }
                else if (static_cast<float>(bounds.position) > textPoint)
                {
                    right = mid;
                }
                else
                {
                    break;
                }
            }

            index = mid;     
        }

        return true;
    }

    bool TextBox::intersectTextInterpolated(const float point, size_t & index)
    {
        if (!intersectText(point, index))
        {
            return false;
        }

        if (index == m_fontSequence.getCount())
        {
            return true;
        }

        size_t prevIndex = index ? index - 1 : 0;
        size_t nextIndex = index + 1;
        size_t nextIndexClamp = nextIndex < m_fontSequence.getCount() ? nextIndex : m_fontSequence.getCount() - 1;
        
        float pos = static_cast<float>(m_fontSequence.getHorizontalBounds(index).position);
        float prev = static_cast<float>(m_fontSequence.getHorizontalBounds(prevIndex).position);
        auto nextBounds = m_fontSequence.getHorizontalBounds(nextIndexClamp);
        float next = static_cast<float>(nextBounds.position);
        next += nextIndex == m_fontSequence.getCount() ? static_cast<float>(nextBounds.size) : 0.0f;
        const float textPoint = point - m_textBounds.position.x;

        if (textPoint < pos - ((pos - prev) / 2.0f))
        {
            index = prevIndex;
        }
        else if (textPoint > pos + ((next - pos) / 2.0f))
        {
            index = nextIndex;
        }

        return true;
    }

}