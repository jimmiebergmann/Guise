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

#ifndef GUISE_CONTROL_TEXT_BOX_HPP
#define GUISE_CONTROL_TEXT_BOX_HPP

#include "guise/control.hpp"
#include "guise/signal.hpp"
#include "guise/font.hpp"
#include <functional>
#include <string>
#include <chrono>

namespace Guise
{

    class GUISE_API TextBox : public Control, public MultiStyleControl<Style::ParentPaintRectStyle>
    {

    public:

        static std::shared_ptr<TextBox> create();
        
        const std::wstring & getText() const;

        Style::FontStyle & getTextStyle();
        const Style::FontStyle & getTextStyle() const;

        virtual ControlType getType() const;

        virtual bool handleInputEvent(const Input::Event & event);

        Signal<const std::wstring &> onChange;

        void setText(const std::wstring & text);

    protected:

        TextBox();

    private:

        TextBox(const TextBox &) = delete;

        virtual void onCanvasChange(Canvas * canvas);

        virtual void onDisable();

        virtual void onEnable();

        virtual void onRender(RendererInterface & rendererInterface);

        virtual void onResize();

        virtual void onUpdate();

        void calcTextBounds();

        void onActiveChange(bool active);

        bool eraseSelected();

        std::wstring getSelected() const;

        float getCursorPosition(const size_t index) const;

        bool intersectText(const float point, size_t & index);
        bool intersectTextInterpolated(const float point, size_t & index);

        bool                                    m_active;
        bool                                    m_changedText;
        std::chrono::system_clock::time_point   m_cursorBlinkTimer;
        size_t                                  m_cursorIndex;
        size_t                                  m_cursorSelectIndex;
        uint32_t                                m_dpi;
        std::shared_ptr<Font>                   m_font;
        FontSequence                            m_fontSequence;
        std::unique_ptr<uint8_t[]>              m_loadData;
        bool                                    m_mousePressed;
        Bounds2f                                m_textBounds;
        std::wstring                            m_text;
        std::shared_ptr<Texture>                m_texture; 
        Vector2<size_t>                         m_textureSize;

        Style::FontStyle                        m_textStyle;

    };

}

#endif