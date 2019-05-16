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
#include "guise/math/bounds.hpp"
#include "guise/renderer.hpp"
#include <vector>
#include <map>
#include <list>

namespace Guise
{

    // Forward declarations.
    class Control;


    /**
    * Control grid class.
    *
    */
    class ControlGrid
    {

    public:

        struct ControlNode
        {
            Bounds2f        bounds;
            Control *       control;
            Bounds2<size_t> gridPresence;
            size_t          level;
            uint8_t         renderStage;

            bool intersects(const Vector2f & point) const;
        };

        struct Node
        {            
            std::multimap<size_t, ControlNode * > controls; // Key = level.
        };

        ControlGrid(const Vector2f & gridSize, const float pieceSize);
        ~ControlGrid();

        //void addControl(Control & control);

        void setControlBounds(Control & control, const Bounds2f & bounds);
        void setControlLevel(Control & control, const size_t level);

        void removeControl(Control & control);

        void resize(const Vector2f & gridSize);

        const Node * query(const Vector2f & point);

        void render(RendererInterface & renderer, const Vector2f & size);

        void renderGrid(RendererInterface & renderer, const Vector2f & size);

    private:

        ControlGrid(const ControlGrid &) = delete;

        std::map<Control *, ControlNode *>      m_controlMap;
        std::vector<std::vector<Node*> >        m_controls;
        Vector2<size_t>                         m_dimensions;
        Vector2f                                m_gridSize;
        std::vector<Node *>                     m_hiddenNodes;
        const float                             m_pieceSize;
        uint8_t                                 m_renderStage;
        

        //void build(const Vector2f & fullSize);

        void removeFromGrid(const ControlNode * controlNode);

        /**
        * @return True if any of the bounds are inside the grid.
        */
        bool calcGridPresence(const Bounds2f & bounds, Bounds2<size_t> & presence) const;
        
    };

}


#endif