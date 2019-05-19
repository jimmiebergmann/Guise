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

#include "guise/utility/controlGrid.hpp"
#include "guise/control.hpp"
#include <stack>
#include <iostream>

namespace Guise
{

    // Quad indices:
    // 0 = Top left.
    // 1 = Top right.
    // 2 = Bottom left.
    // 3 = Bottom right.


    // ControlGrid ControlNode implementations.
    bool ControlGrid::ControlNode::intersects(const Vector2f & point) const
    {
        return  point.x >= bounds.position.x && point.x <= bounds.position.x + bounds.size.x &&
                point.y >= bounds.position.y && point.y <= bounds.position.y + bounds.size.y;
    }


    // ControlGrid implementations.
    ControlGrid::ControlGrid(const Vector2f & gridSize, const float pieceSize) :
        m_dimensions(0, 0),
        m_gridSize(0, 0),
        m_pieceSize(pieceSize),
        m_renderStage(0)
    {
        resize(gridSize);
    }


    ControlGrid::~ControlGrid()
    {
        for (auto it1 = m_controls.begin(); it1 != m_controls.end(); it1++)
        {
            for (auto it2 = it1->begin(); it2 != it1->end(); it2++)
            {
                delete (*it2);
            }
        }

        for (auto it = m_controlMap.begin(); it != m_controlMap.end(); it++)
        {
            delete it->second;
        }
    }

    bool ControlGrid::isControlSet(Control & control)
    {
        return m_controlMap.find(&control) != m_controlMap.end();
    }

    void ControlGrid::setControlBounds(Control & control, const Bounds2f & bounds)
    {
        Bounds2<size_t> presence;
        bool inside = calcGridPresence(bounds, presence);

        auto it = m_controlMap.find(&control);

        // Create control node
        if (it == m_controlMap.end())
        {
            if(!inside)
            {
                return;
            }

            auto controlNode = new ControlNode;
            controlNode->bounds = bounds;
            controlNode->control = &control;
            controlNode->gridPresence = presence;
            controlNode->level = control.getLevel();
            controlNode->renderStage = m_renderStage;
            m_controlMap.insert({ &control, controlNode });

            for (size_t y = presence.position.y; y <= presence.position.y + presence.size.y; y++)
            {
                for (size_t x = presence.position.x; x <= presence.position.x + presence.size.x; x++)
                {
                    m_controls[y][x]->controls.insert({ controlNode->level, controlNode });
                }
            }

            return;
        }

        auto controlNode = it->second;

        // Ignore control node if bounds are the same.
        if (controlNode->bounds == bounds)
        {
            return;
        }
     
        // Update bounds of found control node.
        removeFromGrid(controlNode);

        controlNode->bounds = bounds;
        controlNode->gridPresence = presence;
        controlNode->renderStage = m_renderStage;

        for(size_t y = presence.position.y; y <= presence.position.y + presence.size.y; y++)
        {
            for (size_t x = presence.position.x; x <= presence.position.x + presence.size.x; x++)
            {
                m_controls[y][x]->controls.insert({ controlNode->level, controlNode });
            }
        }
    }

    //void ControlGrid::setControlLevel(Control &/* control*/, const size_t/* level*/)
    //{
        
    //}

    void ControlGrid::resize(const Vector2f & gridSize)
    {
        m_gridSize = gridSize;

        auto newDimensions = Vector2<size_t>(gridSize / m_pieceSize) + Vector2<size_t>(1, 1);
        if (newDimensions == m_dimensions)
        {
            return;
        }

        // Resize y.
        for (size_t y = newDimensions.y; y < m_dimensions.y; y++)
        {
            for (size_t x = 0; x < m_dimensions.x; x++)
            {
                auto node = m_controls[y][x];
                /*for (auto it = node->controls.begin(); it != node->controls.end(); it++)
                {
                    it->second->gridPresence.
                }*/

                delete node;
            }
        }

        m_controls.resize(newDimensions.y);

        for (size_t y = m_dimensions.y; y < newDimensions.y; y++)
        {
            m_controls[y] = std::vector<Node*>(m_dimensions.x);

            for (size_t x = 0; x < m_dimensions.x; x++)
            {
                m_controls[y][x] = new Node;
            }
        }

        // Resize x.
        if (newDimensions.x != m_dimensions.x)
        {
            for (size_t y = 0; y < newDimensions.y; y++)
            {
                m_controls[y].resize(newDimensions.x);
                for (size_t x = m_dimensions.x; x < newDimensions.x; x++)
                {
                    m_controls[y][x] = new Node;
                }
            }
        }

        m_dimensions = newDimensions;
    }

    void ControlGrid::removeControl(Control & control)
    {
        auto it = m_controlMap.find(&control);
        if (it == m_controlMap.end())
        {
            return;
        }

        auto controlNode = it->second;
        removeFromGrid(controlNode);
        delete controlNode;
        m_controlMap.erase(it);
    }

    const ControlGrid::Node * ControlGrid::query(const Vector2f & point)
    {
        Vector2i32 gridCoord = Vector2i32(point / m_pieceSize);

        if (gridCoord.x < 0 || gridCoord.x >= static_cast<int32_t>(m_dimensions.x) ||
            gridCoord.y < 0 || gridCoord.y >= static_cast<int32_t>(m_dimensions.y))
        {
            return nullptr;
        }

        auto controlNode = m_controls[gridCoord.y][gridCoord.x];
        return controlNode;
    }

    void ControlGrid::render(RendererInterface & renderer, const Vector2f & size)
    {
        const Vector2i32 grids =
        {
            std::min<int32_t>(std::max(static_cast<int>(size.x / m_pieceSize), 0), m_dimensions.x - 1),
            std::min<int32_t>(std::max(static_cast<int>(size.y / m_pieceSize), 0), m_dimensions.y - 1)
        };

        for (int y = 0; y <= grids.y; y++)
        {
            for (int x = 0; x <= grids.x; x++)
            {
                auto & controls = m_controls[y][x]->controls;
                
                for (auto it = controls.begin(); it != controls.end(); it++)
                {
                    auto controlNode = it->second;
                    if (controlNode->renderStage == m_renderStage)
                    {
                        controlNode->control->render(renderer);
                        controlNode->renderStage++;
                    }
                    
                }

            }
        }

        m_renderStage++;
    }

    void ControlGrid::renderGrid(RendererInterface & renderer, const Vector2f & size)
    {
        for (float x = m_pieceSize; x < std::min(size.x, m_gridSize.x); x += m_pieceSize)
        {
            renderer.drawLine({ x, 0.0f }, { x, m_gridSize.y }, 1.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
        }

        for (float y = m_pieceSize; y < std::min(size.y, m_gridSize.y); y += m_pieceSize)
        {
            renderer.drawLine({ 0.0f, y }, { m_gridSize.x, y }, 1.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
        }
    }
 
    void ControlGrid::removeFromGrid(const ControlNode * controlNode)
    {
        auto & presence = controlNode->gridPresence;
        for (size_t y = presence.position.y; y <= presence.position.y + presence.size.y; y++)
        {
            if (y >= m_dimensions.y) // TEMP!
            {
                break;
            }
            for (size_t x = presence.position.x; x <= presence.position.x + presence.size.x; x++)
            {
                if (x >= m_dimensions.x) // TEMP!
                {
                    break;
                }

                auto & controlList = m_controls[y][x]->controls;
                for (auto it = controlList.begin(); it != controlList.end(); it++)
                {
                    if (it->second == controlNode)
                    {
                        controlList.erase(it);
                        break;
                    }
                }
            }
        }
    }

    bool ControlGrid::calcGridPresence(const Bounds2f & bounds, Bounds2<size_t> & presence) const
    {
        if (bounds.size.x <= 0.0f || bounds.size.y <= 0.0f)
        {
            return false;
        }

        Vector2i32 lower = Vector2i32(Vector2f(bounds.position) / m_pieceSize);
        Vector2i32 higher = Vector2i32(Vector2f(bounds.position + bounds.size) / m_pieceSize);

        if (lower.x > static_cast<int32_t>(m_dimensions.x) || higher.x < 0 ||
            lower.y > static_cast<int32_t>(m_dimensions.y) || higher.y < 0)
        {
            return false;
        }

        presence.position.x = std::min<size_t>(std::max(lower.x, 0), static_cast<int32_t>(m_dimensions.x) - 1);
        presence.position.y = std::min<size_t>(std::max(lower.y, 0), static_cast<int32_t>(m_dimensions.y) - 1);

        presence.size.x = std::min<size_t>(std::max(higher.x - lower.x, 0), static_cast<int32_t>(m_dimensions.x) - 1);
        presence.size.y = std::min<size_t>(std::max(higher.y - lower.y, 0), static_cast<int32_t>(m_dimensions.y) - 1);

        //std::cout


        return true;
    }

}