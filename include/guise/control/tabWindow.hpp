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

#ifndef GUISE_CONTROL_TAB_WINDOW_HPP
#define GUISE_CONTROL_TAB_WINDOW_HPP

#include "guise/plane.hpp"
#include "guise/signal.hpp"
#include <functional>

namespace Guise
{

    class GUISE_API TabWindow : public Plane, public Style::ParentPaintRectStyle
    {

    public:

        static std::shared_ptr<TabWindow> create();

        Style::ParentPaintRectStyle & getStyleTab();
        const Style::ParentPaintRectStyle & getStyleTab() const;

        virtual ControlType getType() const;

        virtual bool handleInputEvent(const Input::Event & event);

    protected:

        TabWindow();

    private:

        TabWindow(const TabWindow &) = delete;

        virtual void onAddChild(Control & control, const size_t index);

        virtual void onCanvasChange(Canvas * canvas);

        virtual void onRender(RendererInterface & rendererInterface);

        virtual void onResize();

        Style::ParentPaintRectStyle m_styleTab;
        Bounds2f                    m_bodyBounds;
        Bounds2f                    m_tabBounds;

    };

}

#endif