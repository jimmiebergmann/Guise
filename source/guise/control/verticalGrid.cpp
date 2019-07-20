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

#include "guise/control/verticalGrid.hpp"
#include "guise/canvas.hpp"

namespace Guise
{
  
    // Vertical grid implementations.
    std::shared_ptr<VerticalGrid> VerticalGrid::create()
    {
        return std::shared_ptr<VerticalGrid>(new VerticalGrid());
    }    

    Style::ParentRectStyle & VerticalGrid::getSlotStyle()
    {
        return m_slotStyle;
    }
    const Style::ParentRectStyle & VerticalGrid::getSlotStyle() const
    {
        return m_slotStyle;
    }

    ControlType VerticalGrid::getType() const
    {
        return ControlType::VerticalGrid;
    }

    VerticalGrid::VerticalGrid() :             
        Style::ParentRectStyle(this, nullptr),
        m_slotStyle(this, nullptr),
        m_childRenderCount(0)
    {
        setChildBoundsAware(true);
    }

    void VerticalGrid::onAddChild(Control &, const size_t index)
    {
        if (index + 1 <= m_childRenderCount )
        {
            return;
        }
        resizeChilds();
    }

    void VerticalGrid::onCanvasChange(Canvas * canvas)
    {
        updateEmptyProperties(canvas->getStyleSheet()->getSelector("vertical-grid"));
        m_slotStyle.updateEmptyProperties(canvas->getStyleSheet()->getSelector("vertical-grid-slot"));
    }

    void VerticalGrid::onRemoveChild(Control &, const size_t index)
    {
        if (index + 1 >= m_childRenderCount)
        {
            return;
        }
        resizeChilds();
    }

    void VerticalGrid::onRender(RendererInterface & rendererInterface)
    {
        auto childs = getChilds();
        for (auto it = childs.begin(); it != childs.begin() + m_childRenderCount; it++)
        {
            (*it)->draw(rendererInterface);
        }
    }

    void VerticalGrid::onResize()
    {
        setBounds(calcStyledBounds(*this, getBounds(), getScale()));
        resizeChilds();
    }

    void VerticalGrid::resizeChilds()
    {
        auto boundsLeft = getBounds().cutEdges(scale(getPadding()));
        auto slotPadding = scale(m_slotStyle.getPadding());

        m_childRenderCount = 0;
        auto childs = getChilds();
        for (auto it = childs.begin(); it != childs.end(); it++)
        {
            auto child = *it;
            auto cutBoundsLeft = Bounds2f(boundsLeft).cutEdges(slotPadding);
            child->setBounds(cutBoundsLeft);

            auto childBounds = child->getBounds();
            auto childHeight = childBounds.size.y ? childBounds.size.y : 0.0f;

            boundsLeft.cutTop(childHeight + slotPadding.y + slotPadding.w);

            m_childRenderCount++;
            if (!boundsLeft.size.y)
            {
                break;
            }
        }
    }

}