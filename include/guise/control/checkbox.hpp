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

    class GUISE_API Checkbox : public Control, public Style::PaintRectStyle
    {

    public:

        static std::shared_ptr<Checkbox> create(std::shared_ptr<Canvas> & canvas);

        virtual ControlType getType() const;

        virtual bool handleInputEvent(const Input::Event & event);

        virtual void update(const Bounds2f & canvasBound);

        virtual void render(RendererInterface & rendererInterface);

        virtual Bounds2f getRenderBounds() const;

        virtual Bounds2f getSelectBounds() const;

        Style::PaintRectStyle & getChekedStyle();
        Style::PaintRectStyle & getChekedHoverStyle();
        Style::PaintRectStyle & getChekedDisabledStyle();
        Style::PaintRectStyle & getDisabledStyle();
        Style::PaintRectStyle & getHoverStyle();

        Signal<bool>        onChanged;
        Signal<Vector2f>    onPressed;
        Signal<Vector2f>    onReleased;
        Signal<Vector2f>    onHover;

    private:

        Checkbox(std::shared_ptr<Canvas> & canvas);
        Checkbox(const Checkbox &) = delete;

        Bounds2f m_renderBounds;

        Style::PaintRectStyle   m_checkedStyle;
        Style::PaintRectStyle   m_checkedHoverStyle;
        Style::PaintRectStyle   m_checkedDisabledStyle;
        Style::PaintRectStyle   m_disabledStyle;
        Style::PaintRectStyle   m_hoverStyle;
        Style::PaintRectStyle * m_currentStyle;
        bool                    m_checked;

    };

}

#endif