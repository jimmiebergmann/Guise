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

    class GUISE_API Button : public ControlContainerSingle, public Style::ParentPaintRectStyle
    {

    public:

        static std::shared_ptr<Button> create(std::shared_ptr<Canvas> & canvas);

        virtual bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());

        virtual bool handleInputEvent(const Input::Event & event);

        virtual void render(RendererInterface & rendererInterface);

        virtual Bounds2f getRenderBounds() const;

        virtual Bounds2f getSelectBounds() const;

        virtual ControlType getType() const;

        virtual void update();

        Style::ParentPaintRectStyle & getActiveStyle();
        Style::ParentPaintRectStyle & getDisabledStyle();
        Style::ParentPaintRectStyle & getHoverStyle();

        Signal<Vector2f> onPressed;
        Signal<Vector2f> onReleased;
        Signal<Vector2f> onHover;       

    private:

        Button(std::shared_ptr<Canvas> & canvas);
        Button(const Button &) = delete;

        Bounds2f m_childBounds;
        Bounds2f m_renderBounds;

        Style::ParentPaintRectStyle *   m_currentStyle;
        Style::ParentPaintRectStyle     m_activeStyle;
        Style::ParentPaintRectStyle     m_disabledStyle;
        Style::ParentPaintRectStyle     m_hoverStyle;    
        bool                            m_pressed;

    };

}

#endif