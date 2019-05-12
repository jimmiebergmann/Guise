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
        return  point.x >= bounds.x && point.x <= bounds.z &&
                point.y >= bounds.y && point.y <= bounds.w;
    }


    // ControlGrid implementations.
    ControlGrid::ControlGrid(const Vector2f & fullSize, const float gridSize) :
        m_gridSize(gridSize),
        m_dimensions(0, 0)
    {
        build(fullSize);
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

    void ControlGrid::set(Control & control, const Vector4f & bounds)
    {
        Vector2i32 gridCoordLow = Vector2i32(Vector2f(bounds.x, bounds.y) / m_gridSize);
        Vector2i32 gridCoordHigh = Vector2i32(Vector2f(bounds.z, bounds.w) / m_gridSize);

        if (gridCoordLow.x < 0 || gridCoordLow.x >= static_cast<int32_t>(m_dimensions.x) ||
            gridCoordLow.y < 0 || gridCoordLow.y >= static_cast<int32_t>(m_dimensions.y) ||
            gridCoordLow.x > gridCoordHigh.x || gridCoordLow.y > gridCoordHigh.y)
        {
            return;
        }

        // Not yet added to grid.
        auto it = m_controlMap.find(&control);
        if (it == m_controlMap.end())
        {
            auto controlNode = new ControlNode;

            controlNode->bounds = bounds;
            controlNode->control = &control;
            controlNode->gridHigh = gridCoordHigh;
            controlNode->gridLow = gridCoordLow;
            controlNode->level = control.getLevel();

            for (int y = gridCoordLow.y; y <= gridCoordHigh.y; y++)
            {
                for (int x = gridCoordLow.x; x <= gridCoordHigh.x; x++)
                {
                    m_controls[y][x]->controls.insert({ controlNode->level, controlNode });
                }
            }

            m_controlMap.insert({ &control, controlNode });
            return;
        }

        // Already added to grid.
        auto controlNode = it->second;

        if (controlNode->gridLow == gridCoordLow && controlNode->gridHigh == gridCoordHigh)
        {
            return;
        } 

        removeFromGrid(controlNode);
        controlNode->bounds = bounds;
        controlNode->gridHigh = gridCoordHigh;
        controlNode->gridLow = gridCoordLow;

        for (int y = gridCoordLow.y; y <= gridCoordHigh.y; y++)
        {
            for (int x = gridCoordLow.x; x <= gridCoordHigh.x; x++)
            {
                m_controls[y][x]->controls.insert({controlNode->level, controlNode });
            }
        }

    }

    void ControlGrid::resize(const float /*size*/, const size_t /*levels*/)
    {

    }

    void ControlGrid::unset(Control & control)
    {
        // Erase control from map.
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

    void ControlGrid::updateLevel(Control & /*control*/, const size_t /*level*/)
    {

    }

    const ControlGrid::Node * ControlGrid::query(const Vector2f & point)
    {
        Vector2i32 gridCoord = Vector2i32(point / m_gridSize);

        if (gridCoord.x < 0 || gridCoord.x >= static_cast<int32_t>(m_dimensions.x) ||
            gridCoord.y < 0 || gridCoord.y >= static_cast<int32_t>(m_dimensions.y))
        {
            return nullptr;
        }

        auto controlNode = m_controls[gridCoord.y][gridCoord.x];
        return controlNode;
    }

    void ControlGrid::build(const Vector2f & fullSize)
    {
        m_dimensions = Vector2<size_t>(fullSize / m_gridSize) + Vector2<size_t>(1, 1);
        m_controls = std::vector<std::vector<Node*> >(m_dimensions.y);
        
        for (auto it = m_controls.begin(); it != m_controls.end(); it++)
        {
            *it = std::vector<Node*>(m_dimensions.x);

            for (size_t i = 0; i < m_dimensions.x; i++)
            {
                (*it).at(i) = new Node;
            }
        }
    }

    void ControlGrid::removeFromGrid(const ControlNode * controlNode)
    {
        for (size_t y = controlNode->gridLow.y; y <= controlNode->gridHigh.y; y++)
        {
            for (size_t x = controlNode->gridLow.x; x <= controlNode->gridHigh.x; x++)
            {
                auto & controlList = m_controls[y][x]->controls;
                for (auto it = controlList.begin(); it != controlList.end(); it++)
                {
                    if (it->first == controlNode->level && it->second == controlNode)
                    {
                        controlList.erase(it);
                        break;
                    }
                }
            }
        }
    }

}