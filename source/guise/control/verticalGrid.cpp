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
        setBounds(getAvailableBounds());
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
        setBounds(getAvailableBounds());
    }

    void VerticalGrid::onRender(RendererInterface & rendererInterface)
    {
        //rendererInterface.drawQuad(getBounds(), Vector4f(0.0f, 1.0f, 0.0f, 0.4f));

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
        auto padding = scale(getPadding());
        auto boundsLeft = getBounds().cutEdges(padding);
        auto slotPadding = scale(m_slotStyle.getPadding());
        float maxWidth = 0.0f;

        m_childRenderCount = 0;
        auto childs = getChilds();
        for (auto it = childs.begin(); it != childs.end(); it++)
        {
            auto child = *it;
            auto cutBoundsLeft = Bounds2f(boundsLeft).cutEdges(slotPadding);
            child->setBounds(cutBoundsLeft);

            auto childBounds = child->getBounds();

            float extraHeight = std::max(childBounds.position.y - cutBoundsLeft.position.y, 0.0f);
            auto childHeight = std::max(childBounds.size.y + extraHeight, 0.0f);
            boundsLeft.cutTop(childHeight + slotPadding.y + slotPadding.w);

            maxWidth = std::max(maxWidth, childBounds.size.x);

            m_childRenderCount++;
            if (!boundsLeft.size.x)
            {
                break;
            }
        }

        maxWidth += padding.x + padding.z + slotPadding.x + slotPadding.z;
        maxWidth = std::min(maxWidth, getBounds().size.x);

        auto newBounds = getBounds();
        newBounds.size.x = maxWidth;
        newBounds.size.y -= boundsLeft.size.y;
        setBounds(newBounds);
    }

}