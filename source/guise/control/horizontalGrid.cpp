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

#include "guise/control/horizontalGrid.hpp"
#include "guise/canvas.hpp"

namespace Guise
{

    // Vertical grid implementations.
    std::shared_ptr<HorizontalGrid> HorizontalGrid::create()
    {
        return std::shared_ptr<HorizontalGrid>(new HorizontalGrid());
    }

    Style::ParentRectStyle & HorizontalGrid::getSlotStyle()
    {
        return m_slotStyle;
    }
    const Style::ParentRectStyle & HorizontalGrid::getSlotStyle() const
    {
        return m_slotStyle;
    }

    ControlType HorizontalGrid::getType() const
    {
        return ControlType::HorizontalGrid;
    }

    HorizontalGrid::HorizontalGrid() :
        Style::ParentRectStyle(this, nullptr),
        m_slotStyle(this, nullptr),
        m_childRenderCount(0)
    {
        setChildBoundsAware(true);
    }

    void HorizontalGrid::onAddChild(Control &, const size_t index)
    {
        if (index + 1 <= m_childRenderCount)
        {
            return;
        }
        setBounds(getAvailableBounds());
    }

    void HorizontalGrid::onCanvasChange(Canvas * canvas)
    {
        updateEmptyProperties(canvas->getStyleSheet()->getSelector("horizontal-grid"));
        m_slotStyle.updateEmptyProperties(canvas->getStyleSheet()->getSelector("horizontal-grid-slot"));
    }

    void HorizontalGrid::onRemoveChild(Control &, const size_t index)
    {
        if (index + 1 >= m_childRenderCount)
        {
            return;
        }
        setBounds(getAvailableBounds());
    }

    void HorizontalGrid::onRender(RendererInterface & rendererInterface)
    {
        //rendererInterface.drawQuad(getBounds(), Vector4f(0.0f, 0.0f, 1.0f, 0.4f));

        forEachChild(0, m_childRenderCount, [&](std::shared_ptr<Control> child, size_t)
        {
            child->draw(rendererInterface);
            return true;
        });
    }

    void HorizontalGrid::onResize()
    {
        setBounds(calcStyledBounds(*this, getBounds(), getScale()));
        resizeChilds();
    }

    void HorizontalGrid::resizeChilds()
    {
        auto padding = scale(getPadding());
        auto boundsLeft = getBounds().cutEdges(padding);
        auto slotPadding = scale(m_slotStyle.getPadding());
        float maxHeight = 0.0f;

        m_childRenderCount = 0;
        
        forEachChild([&](std::shared_ptr<Control> child, size_t)
        {
            auto cutBoundsLeft = Bounds2f(boundsLeft).cutEdges(slotPadding);
            child->setBounds(cutBoundsLeft);

            auto childBounds = child->getBounds();

            float extraWidth = std::max(childBounds.position.x - cutBoundsLeft.position.x, 0.0f);
            auto childWidth = std::max(childBounds.size.x + extraWidth, 0.0f);
            boundsLeft.cutLeft(childWidth + slotPadding.x + slotPadding.z);

            maxHeight = std::max(maxHeight, childBounds.size.y);

            m_childRenderCount++;
            if (!boundsLeft.size.x)
            {
                return false;
            }

            return true;
        });

        maxHeight += padding.y + padding.w + slotPadding.y + slotPadding.w;
        maxHeight = std::min(maxHeight, getBounds().size.y);

        auto newBounds = getBounds();
        newBounds.size.y = maxHeight;
        newBounds.size.x -= boundsLeft.size.x;
        setBounds(newBounds);
    }

}