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
#include "guise/control/dpiSensitive.hpp"
#include "guise/font.hpp"
#include <functional>
#include <string>
#include <chrono>

namespace Guise
{

    class GUISE_API TextBox : public Control, public Style::PaintRectStyle, public Style::ParentStyle, public DpiSensitive
    {

    public:

        static std::shared_ptr<TextBox> create(std::shared_ptr<Canvas> & canvas);

        virtual ControlType getType() const;

        virtual bool handleInputEvent(const Input::Event & event);

        virtual void update(const Bounds2f & canvasBound);

        virtual void render(RendererInterface & rendererInterface);

        virtual Bounds2f getRenderBounds() const;

        virtual Bounds2f getSelectBounds() const;

        Signal<const std::wstring &> onChange;

        Style::FontStyle & getTextStyle();

        ~TextBox();

    private:

        TextBox(std::shared_ptr<Canvas> & canvas);
        TextBox(const TextBox &) = delete;

        void onNewDpi(const int32_t dpi);

        void onActiveChange(bool active);

        bool eraseSelected();

        std::wstring getSelected();

        bool intersectText(const float point, size_t & index);

        bool                                    m_active;
        float                                   m_baseline;
        float                                   m_baseHeight;
        bool                                    m_changed; 
        std::chrono::system_clock::time_point   m_cursorBlinkTimer;
        size_t                                  m_cursorIndex;
        size_t                                  m_cursorSelectFromIndex;
        std::vector<int32_t>                    m_charPositions;
        bool                                    m_mousePressed;
        int32_t                                 m_dpi;
        std::shared_ptr<Font>                   m_font;
        Bounds2f                                m_renderBounds;
        Bounds2f                                m_textBounds;
        std::wstring                            m_text;
        std::shared_ptr<Texture>                m_textTexture; 

        Style::FontStyle                        m_textStyle;

    };

}

#endif