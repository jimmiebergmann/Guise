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
#include <functional>

namespace Guise
{

    class GUISE_API Button : public Style::BoxStyle, public ControlContainerSingle
    {

    public:

        static std::shared_ptr<Button> create(std::shared_ptr<Canvas> & canvas);

        virtual ControlType getType() const;

        virtual Control * handleInputEvent(const Input::Event & event);

        virtual void update(const Bounds2f & canvasBound);

        virtual void render(RendererInterface & rendererInterface);

        virtual Bounds2f getRenderBounds() const;

        virtual Bounds2f getSelectBounds() const;

        virtual Control * queryHit(const Vector2f & point) const;

        Style::BoxStyle & getActiveStyle();
        Style::BoxStyle & getDisabledStyle();
        Style::BoxStyle & getHoverStyle();

        std::function<void()> onPressed;

    private:

        Button(std::shared_ptr<Canvas> & canvas);
        Button(const Button &) = delete;

        Bounds2f m_renderBounds;
        Bounds2f m_childBounds;

        Style::BoxStyle m_activeStyle;
        Style::BoxStyle m_disabledStyle;
        Style::BoxStyle m_hoverStyle;
        Style::BoxStyle * m_currentStyle;

    };

}

#endif