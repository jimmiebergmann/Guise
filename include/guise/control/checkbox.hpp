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

#ifndef GUISE_CONTROL_CHECKBOX_HPP
#define GUISE_CONTROL_CHECKBOX_HPP

#include "guise/control.hpp"
#include "guise/signal.hpp"
#include <functional>

namespace Guise
{

    class GUISE_API Checkbox : public Control, public MultiStyleControl<Style::PaintRectStyle>
    {

    public:

        static std::shared_ptr<Checkbox> create();

        Style::PaintRectStyle & getCheckedStyle();
        const Style::PaintRectStyle & getCheckedStyle() const;
        Style::PaintRectStyle & getCheckedHoverStyle();
        const Style::PaintRectStyle & getCheckedHoverStyle() const;
        Style::PaintRectStyle & getCheckedDisabledStyle();
        const Style::PaintRectStyle & getCheckedDisabledStyle() const;
        Style::PaintRectStyle & getDisabledStyle();
        const Style::PaintRectStyle & getDisabledStyle() const;
        Style::PaintRectStyle & getHoverStyle();
        const Style::PaintRectStyle & getHoverStyle() const;

        virtual ControlType getType() const;

        virtual bool handleInputEvent(const Input::Event & event);

        Signal<bool>        onChanged;
        Signal<Vector2f>    onPressed;
        Signal<Vector2f>    onReleased;
        Signal<Vector2f>    onHover;

    private:

        Checkbox();
        Checkbox(const Checkbox &) = delete;

        virtual void onCanvasChange(Canvas * canvas);

        virtual void onDisable();

        virtual void onEnable();

        virtual void onRender(RendererInterface & rendererInterface);

        virtual void onResize();

        bool                    m_checked;
        Style::PaintRectStyle   m_styleChecked;
        Style::PaintRectStyle   m_styleCheckedHover;
        Style::PaintRectStyle   m_styleCheckedDisabled;
        Style::PaintRectStyle   m_styleDisabled;
        Style::PaintRectStyle   m_styleHover;

    };

}

#endif