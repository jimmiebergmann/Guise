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
                        /*if (!shiftPressed)
                        {
                            m_cursorIndex = index;
                        }*/
                
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
                if(e.character > 0x1F && e.character != 0x7F)
                {
                    eraseSelected();

                    m_text.insert(m_text.begin() + m_cursorIndex, e.character);
                    m_cursorIndex++;
                    m_cursorSelectIndex = m_cursorIndex;

                    m_changedText = true;
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
                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                        else if (m_cursorIndex && m_text.size())
                        {
                            m_text.erase(m_text.begin() + (m_cursorIndex - 1));
                            m_cursorIndex--;
                            m_cursorSelectIndex = m_cursorIndex;

                            m_changedText = true;
                            onChange(m_text);
                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                    }
                    break;
                    case Input::Key::Delete:
                    {
                        if (eraseSelected())
                        {
                            m_changedText = true;
                            m_cursorBlinkTimer = std::chrono::system_clock::now();
                        }
                        else if (m_text.size() && m_cursorIndex < m_text.size())
                        {
                            m_text.erase(m_text.begin() + m_cursorIndex);

                            m_changedText = true;
                            onChange(m_text);
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

    void TextBox::update(const Bounds2f & canvasBound)
    {
        auto renderBounds = calcRenderBounds(canvasBound, getPosition(), getSize(), getOverflow());
        if (renderBounds != m_renderBounds)
        {
            m_renderBounds = renderBounds;
            m_changed = true;
        }
        getCanvas().queueControlRendering(this);
    }

    void TextBox::render(RendererInterface & renderer)
    {
        const float scale = m_canvas.getScale();

        if (m_changedText)
        {
            m_changedText = false;
            m_fontSequence.createSequence(m_text, m_textStyle.getFontSize(), m_dpi);
            m_changed = true;
        }

        if (m_changed)
        {
            m_changed = false;

            std::unique_ptr<uint8_t[]> data;
            Vector2<size_t> dimensions = { 0, 0 };

            if (m_fontSequence.createBitmapRgba(data, dimensions, 0, m_text.size()))
            {
                if (!m_textTexture)
                {
                    m_textTexture = renderer.createTexture();
                }

                m_textTexture->load(data.get(), Texture::PixelFormat::RGBA8, dimensions);
            }
            else
            {
                m_textTexture.reset();
            }
        }

        renderer.drawQuad(m_renderBounds, getBackgroundColor());

        const bool drawBorder = getBorderStyle() != Style::Property::BorderStyle::None && getBorderWidth() > 0.0f;
        if (drawBorder)
        {
            const float borderWidth = std::floor(getBorderWidth() * scale);
            renderer.drawBorder(m_renderBounds, borderWidth, getBorderColor());
        }

        const auto paddingLow = Vector2f::ceil(getPaddingLow() * scale);
        const auto paddingHigh = Vector2f::ceil(getPaddingHigh() * scale);

        m_textBounds = { { m_renderBounds.position.x + paddingLow.x, m_renderBounds.position.y }, { 0.0f, 0.0f } };
        if (m_textTexture)
        {     
            Bounds2i32 textMask =
            {
                Vector2i32::max({ 0, 0 }, { m_textBounds.position.x, m_textBounds.position.y + paddingLow.y }),
                Vector2i32::max({ 0, 0 }, m_renderBounds.size - paddingLow - paddingLow)
            };
            
            renderer.pushMask(textMask);
      
            // Render text     
            m_textBounds.size = m_textTexture->getDimensions();         
            m_textBounds.position.y += m_fontSequence.calcVerticalPosition(m_renderBounds.size.y);
            renderer.drawQuad(m_textBounds, m_textTexture, m_textStyle.getFontColor());

            // Render masks
            //renderer.drawQuad(m_textBounds, { 0.0f, 1.0f, 0.0f, 0.3f });
            //renderer.drawQuad(textMask, { 1.0f, 0.0f, 0.0f, 0.3f });

            const bool isSelected = m_cursorIndex != m_cursorSelectIndex;
            if (isSelected)
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

                renderer.drawQuad(selectBounds, { 0.0f, 0.45f, 0.85f, 0.7f });
            }

            renderer.popMask();
        }

        if (m_active)
        {
            auto endTime = std::chrono::system_clock::now();
            std::chrono::duration<double> duration = endTime - m_cursorBlinkTimer;

            const bool renderCursor = (static_cast<int>(duration.count() * 1000.0f) % 1000) < 500;
            if (renderCursor)
            {
                float cursorPosition = getCursorPosition(m_cursorIndex);
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
    
    TextBox::~TextBox()
    {
        getCanvas().unregisterDpiSensitive(this);
    }

    Style::FontStyle & TextBox::getTextStyle()
    {
        return m_textStyle;
    }

    const std::wstring & TextBox::getText() const
    {
        return m_text;
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

    TextBox::TextBox(std::shared_ptr<Canvas> & canvas) :       
        Control(*canvas),
        Style::PaintRectStyle(canvas->getStyleSheet()->getSelector("text-box")),
        Style::ParentStyle(canvas->getStyleSheet()->getSelector("text-box")),
        m_active(false),
        m_changed(true),
        m_changedText(false),
        m_cursorIndex(0),
        m_cursorSelectIndex(0),
        m_dpi(canvas->getDpi()),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_textBounds(0.0f, 0.0f, 0.0f, 0.0f)
    {
        getCanvas().registerDpiSensitive(this);

        if (auto textStyle = canvas->getStyleSheet()->getSelector("text-box-text"))
        {
            m_textStyle = { textStyle };
            m_font = FontLibrary::get(m_textStyle.getFontFamily());
            m_fontSequence = FontSequence(m_font);
        }
    }

    void TextBox::onNewDpi(const int32_t dpi)
    {
        m_dpi = dpi;
        m_changedText = true;
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
        if (!m_textTexture || !m_fontSequence.getCount())
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