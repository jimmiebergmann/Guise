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
    std::shared_ptr<VerticalGrid> VerticalGrid::create(std::shared_ptr<Canvas> & canvas)
    {
        return std::shared_ptr<VerticalGrid>(new VerticalGrid(canvas));
    }

    ControlType VerticalGrid::getType() const
    {
        return ControlType::VerticalGrid;
    }

    bool VerticalGrid::handleInputEvent(const Input::Event &/* event*/)
    {
        return false;
    }

    void VerticalGrid::update(const Bounds2f & canvasBound)
    {
        bool newChildBounds = false;
        const bool childsUpdate = pollUpdateForced();
        if (canvasBound != m_renderBounds || childsUpdate)
        {
            Bounds2f renderBounds = calcRenderBounds(canvasBound, getPosition(), getSize(), getOverflow());
            if (renderBounds != m_renderBounds || childsUpdate)
            {
                m_renderBounds = renderBounds;
                newChildBounds = true;

                auto scale = m_canvas.getScale();
                auto slotPadding = m_slotStyle.getPadding() * scale;
                Bounds2f childBoundsLeft(m_renderBounds.position + getPaddingLow(), m_renderBounds.size - getPaddingLow() - getPaddingHigh());
                childBoundsLeft.position.x  += slotPadding.x;
                childBoundsLeft.size.x      -= (slotPadding.x + slotPadding.z);

                m_childsBounds.clear();

                auto childs = getChilds();                
                for (auto it = childs.begin(); childBoundsLeft.size.y > 0.0f && it != childs.end(); it++)
                {
                    childBoundsLeft.position.y  += slotPadding.y;
                    childBoundsLeft.size.y      -= (slotPadding.y + slotPadding.w);
                    (*it)->update(childBoundsLeft);
                    m_childsBounds.push_back(childBoundsLeft);

                    auto childRenderBounds = (*it)->getRenderBounds();
                    float childHeight = std::max(childRenderBounds.position.y - childBoundsLeft.position.y, 0.0f) + std::max(childRenderBounds.size.y, 0.0f);
                    
                    childBoundsLeft.position.y += childHeight + slotPadding.w;
                    childBoundsLeft.size.y     -= childHeight;
                }
            }
        }

        if (!newChildBounds)
        {
            auto childs = getChilds();
            for (size_t i = 0; i < m_childsBounds.size(); i++)
            {
                childs[i]->update(m_childsBounds[i]);
            }
        }
        
    }

    Bounds2f VerticalGrid::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f VerticalGrid::getSelectBounds() const
    {
        return m_renderBounds;
    }

    Style::ParentRectStyle & VerticalGrid::getSlotStyle()
    {
        return m_slotStyle;
    }

    VerticalGrid::VerticalGrid(std::shared_ptr<Canvas> & canvas) :             
        ControlContainerList(*canvas),
        Style::ParentRectStyle(canvas->getStyleSheet()->getSelector("vertical-grid")),       
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_slotStyle(canvas->getStyleSheet()->getSelector("vertical-grid-slot"))
    { }

}