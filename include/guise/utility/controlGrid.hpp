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

#ifndef GUISE_CONTROL_GRID_HPP
#define GUISE_CONTROL_GRID_HPP

#include "guise/build.hpp"
#include <vector>
#include <map>
#include <list>

namespace Guise
{

    // Forward declarations.
    class Control;


    /**
    * Semaphore class.
    *
    */
    class ControlGrid
    {

    public:


        struct ControlNode
        {
            Vector4f        bounds;
            Control *       control;
            Vector2<size_t> gridHigh;
            Vector2<size_t> gridLow;
            size_t          level;

            bool intersects(const Vector2f & point) const;
        };

        struct Node
        {
            std::multimap<size_t, ControlNode * > controls; // Key = level.
        };

        ControlGrid(const Vector2f & fullSize, const float gridSize);
        ~ControlGrid();

        void resize(const float size, const size_t levels);

        void set(Control & control, const Vector4f & bounds);

        void unset(Control & control);  

        void updateLevel(Control & control, const size_t level);

        const Node * query(const Vector2f & point);

    private:

        ControlGrid(const ControlGrid &) = delete;


        float                                   m_gridSize;
        Vector2<size_t>                         m_dimensions;
        std::map<Control *, ControlNode *>      m_controlMap;
        std::vector<std::vector<Node*> >        m_controls;

        void build(const Vector2f & fullSize);

        void removeFromGrid(const ControlNode * controlNode);
        
    };

}


#endif