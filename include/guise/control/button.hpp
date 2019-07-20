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

#ifndef GUISE_CONTROL_BUTTON_HPP
#define GUISE_CONTROL_BUTTON_HPP

#include "guise/control.hpp"
#include "guise/signal.hpp"
#include <functional>

namespace Guise
{

    class GUISE_API Button : public ControlContainerSingle, public MultiStyleControl<Style::ParentPaintRectStyle>
    {

    public:

        static std::shared_ptr<Button> create();

        virtual bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());       

        Style::ParentPaintRectStyle & getStyleActive();
        const Style::ParentPaintRectStyle & getStyleActive() const;
        Style::ParentPaintRectStyle & getStyleDisabled();
        const Style::ParentPaintRectStyle & getStyleDisabled() const;
        Style::ParentPaintRectStyle & getStyleHover();
        const Style::ParentPaintRectStyle & getStyleHover() const;

        virtual ControlType getType() const;

        virtual bool handleInputEvent(const Input::Event & event);

        Signal<Vector2f> onPress;
        Signal<Vector2f> onRelease;
        Signal<Vector2f> onHover;     

    private:

        Button();
        Button(const Button &) = delete;

        virtual void onAddChild(Control & control, const size_t index);

        virtual void onCanvasChange(Canvas * canvas);

        virtual void onDisable();

        virtual void onEnable();

        virtual void onRender(RendererInterface & rendererInterface);

        virtual void onResize();

        bool                            m_pressed;
        Style::ParentPaintRectStyle     m_styleActive;
        Style::ParentPaintRectStyle     m_styleDisabled;
        Style::ParentPaintRectStyle     m_styleHover;
        

    };

}

#endif