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
    // Vertical grid style implementations.
    VerticalGridStyle::VerticalGridStyle() :
        m_position(0.0f, 0.0f),
        m_size(0.0f, 0.0f),
        m_padding(0.0f, 0.0f, 0.0f, 0.0f),
        m_overflow(Style::Property::Overflow::hidden)
    { }

    VerticalGridStyle::VerticalGridStyle(const std::shared_ptr<Style::Selector> & selector) :
        VerticalGridStyle()
    {
        if (!selector)
        {
            return;
        }
        
        auto & properties = selector->getProperties();
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            if (it->first == "position")
            {
                m_position = it->second->getVector2f();
            }
            else if (it->first == "size")
            {
                m_size = it->second->getVector2f();
            }
            else if (it->first == "padding")
            {
                switch (it->second->getDataType())
                {
                case Style::Property::DataType::Float:
                {
                    m_padding.x = m_padding.y = m_padding.w = m_padding.z = it->second->getFloat();
                }
                break;
                case Style::Property::DataType::Vector2f:
                {
                    m_padding = { it->second->getVector2f(), 0.0f, 0.0f };
                }
                break;
                case Style::Property::DataType::Vector4f:
                {
                    m_padding = it->second->getVector4f();
                }
                break;
                default: break;
                }
            }
            else if (it->first == "overflow")
            {
                m_overflow = it->second->getOverflow();
            }
        }
    }


    const Vector2f & VerticalGridStyle::getPosition() const
    {
        return m_position;
    }
    const Vector2f & VerticalGridStyle::getSize() const
    {
        return m_size;
    }
    const Vector4f & VerticalGridStyle::getPadding() const
    {
        return m_padding;
    }
    const Vector2f VerticalGridStyle::getPaddingLow() const
    {
        return { m_padding.x, m_padding.y };
    }
    const Vector2f VerticalGridStyle::getPaddingHigh() const
    {
        return { m_padding.z, m_padding.w };
    }
    Style::Property::Overflow VerticalGridStyle::getOverflow() const
    {
        return m_overflow;
    }

    void VerticalGridStyle::setPosition(const Vector2f & position)
    {
        m_position = position;
    }
    void VerticalGridStyle::setSize(const Vector2f & size)
    {
        m_size = size;
    }
    void VerticalGridStyle::setPadding(const Vector4f & padding)
    {
        m_padding = padding;
    }
    void VerticalGridStyle::setOverflow(const Style::Property::Overflow overflow)
    {
        m_overflow = overflow;
    }


    // Vertical grid implementations.
    std::shared_ptr<VerticalGrid> VerticalGrid::create(std::shared_ptr<Canvas> & canvas)
    {
        return std::shared_ptr<VerticalGrid>(new VerticalGrid(canvas));
    }

    ControlType VerticalGrid::getType() const
    {
        return ControlType::VerticalGrid;
    }

    Control * VerticalGrid::handleInputEvent(const Input::Event &/* event*/)
    {
        return nullptr;
    }

    void VerticalGrid::update(const Bounds2f & canvasBound)
    {
        bool newChildBounds = false;
        const bool childsUpdate = pollUpdateForced();
        if (canvasBound != m_renderBounds || childsUpdate)
        {
            Bounds2f renderBounds = calcRenderBounds(canvasBound, m_position, m_size, m_overflow);
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

        getCanvas().queueControlRendering(this);

        if (!newChildBounds)
        {
            auto childs = getChilds();
            for (size_t i = 0; i < m_childsBounds.size(); i++)
            {
                childs[i]->update(m_childsBounds[i]);
            }
        }
        
    }

    void VerticalGrid::render(RendererInterface & /*renderer*/)
    {
        /*auto childs = getChilds();
        
        for (auto it = childs.begin(); it != childs.end(); it++)
        {
            (*it)->render(renderer);
        }*/
    }


    Bounds2f VerticalGrid::getRenderBounds() const
    {
        return m_renderBounds;
    }

    Bounds2f VerticalGrid::getSelectBounds() const
    {
        return m_renderBounds;
    }

    VerticalGridStyle & VerticalGrid::getSlotStyle()
    {
        return m_slotStyle;
    }

    VerticalGrid::VerticalGrid(std::shared_ptr<Canvas> & canvas) :      
        VerticalGridStyle(canvas->getStyleSheet()->getSelector("vertical-grid")),
        ControlContainerList(*canvas),
        m_renderBounds(0.0f, 0.0f, 0.0f, 0.0f),
        m_slotStyle(canvas->getStyleSheet()->getSelector("vertical-grid-slot"))
    { }

}