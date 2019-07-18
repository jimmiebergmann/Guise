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
    /*std::shared_ptr<HorizontalGrid> HorizontalGrid::create(std::shared_ptr<Canvas> & canvas)
    {
        return std::shared_ptr<HorizontalGrid>(new HorizontalGrid(canvas));
    }

    ControlType HorizontalGrid::getType() const
    {
        return ControlType::HorizontalGrid;
    }

    bool HorizontalGrid::handleInputEvent(const Input::Event &)
    {
        return false;
    }

    void HorizontalGrid::update(const Bounds2f & canvasBounds)
    {
        bool newChildBounds = false;
        const bool forceUpdate = pollUpdateForced();
        if (forceUpdate)
        {
            Bounds2f renderBounds = calcRenderBounds(canvasBounds, *this);
            if (renderBounds != m_renderBounds || forceUpdate)
            {
                m_renderBounds = renderBounds;
                newChildBounds = true;

                const auto scale = m_canvas.getScale();
                const auto paddingLow = Vector2f::ceil(getPaddingLow() * scale);
                const auto paddingHigh = Vector2f::ceil(getPaddingHigh() * scale);
                const auto slotPadding = m_slotStyle.getPadding() * scale;
                Bounds2f childBoundsLeft(m_renderBounds.position + paddingLow, m_renderBounds.size - paddingLow - paddingHigh);
                childBoundsLeft.position.y += slotPadding.y;
                childBoundsLeft.size.y -= (slotPadding.x + slotPadding.z);

                m_childsBounds.clear();

                auto childs = getChilds();                
                for (auto it = childs.begin(); childBoundsLeft.size.x > 0.0f && it != childs.end(); it++)
                {
                    childBoundsLeft.position.x  += slotPadding.x;
                    childBoundsLeft.size.x      -= (slotPadding.x + slotPadding.z);
                    (*it)->update(childBoundsLeft);
                    m_childsBounds.push_back(childBoundsLeft);

                    auto childRenderBounds = (*it)->getRenderBounds();
                    float childWidth = std::max(childRenderBounds.position.x - childBoundsLeft.position.x, 0.0f) + std::max(childRenderBounds.size.x, 0.0f);
                    
                    childBoundsLeft.position.x += childWidth + slotPadding.z;
                    childBoundsLeft.size.x     -= childWidth;
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

    Bounds2f HorizontalGrid::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f HorizontalGrid::getSelectBounds() const
    {
        return m_renderBounds;
    }

    Style::ParentRectStyle & HorizontalGrid::getSlotStyle()
    {
        return m_slotStyle;
    }

    HorizontalGrid::HorizontalGrid(std::shared_ptr<Canvas> & canvas) :
        ControlContainerList(*canvas),
        Style::ParentRectStyle(canvas->getStyleSheet()->getSelector("horizontal-grid")),       
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_slotStyle(canvas->getStyleSheet()->getSelector("horizontal-grid-slot"))
    {
        forceUpdate();
    }*/

}