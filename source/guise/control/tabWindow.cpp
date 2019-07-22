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

#include "guise/control/tabWindow.hpp"
#include "guise/canvas.hpp"
#include <iostream>

namespace Guise
{

    // Button implementations.
    std::shared_ptr<TabWindow> TabWindow::create()
    {
        return std::shared_ptr<TabWindow>(new TabWindow());
    }

    Style::ParentPaintRectStyle & TabWindow::getStyleTab()
    {
        return m_styleTab;
    }
    const Style::ParentPaintRectStyle & TabWindow::getStyleTab() const
    {
        return m_styleTab;
    }

    ControlType TabWindow::getType() const
    {
        return ControlType::TabWindow;
    }

    bool TabWindow::handleInputEvent(const Input::Event & )
    {
        return true;
    }

    TabWindow::TabWindow() :
        Style::ParentPaintRectStyle(this, nullptr),
        m_styleTab(this, this)
    {
    }

    void TabWindow::onAddChild(Control & control, const size_t)
    {
        control.setBounds(Bounds2f(m_bodyBounds).cutEdges(scale(getPadding())));
    }

    void TabWindow::onCanvasChange(Canvas * canvas)
    {
        updateEmptyProperties(canvas->getStyleSheet()->getSelector("tab-window"));
        m_styleTab.updateEmptyProperties(canvas->getStyleSheet()->getSelector("tab-window-tab"));
    }    

    void TabWindow::onRender(RendererInterface & rendererInterface)
    {
        rendererInterface.drawRect(m_bodyBounds, *this);
        rendererInterface.drawRect(m_tabBounds, m_styleTab);

        forEachChild([&](std::shared_ptr<Control> child, size_t)
        {
            child->draw(rendererInterface);
            return true;
        });
    }

    void TabWindow::onResize()
    {
        setBounds(calcStyledBounds(*this, getBounds(), getScale()));

        m_tabBounds.position = scale(getBounds().position);
        m_tabBounds.size = scale(m_styleTab.getSize());
        
        m_bodyBounds.position = m_tabBounds.position;
        m_bodyBounds.position.y += m_tabBounds.size.y;
        m_bodyBounds.size = scale(getBounds().size);
        
        m_tabBounds.position += scale(m_styleTab.getPosition());

        auto childBounds = Bounds2f(m_bodyBounds).cutEdges(scale(getPadding()));
        forEachChild([&](std::shared_ptr<Control> child, size_t)
        {
            child->setBounds(childBounds);
            return true;
        });
    }

}