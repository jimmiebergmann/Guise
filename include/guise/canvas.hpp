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

#ifndef GUISE_CANVAS_HPP
#define GUISE_CANVAS_HPP

#include "guise/build.hpp"
#include "guise/control.hpp"
#include "guise/renderer.hpp"
#include "guise/style.hpp"
#include "guise/input.hpp"
#include "guise/control/plane.hpp"
#include "guise/utility/controlGrid.hpp"
#include <memory>
#include <mutex>
#include <vector>
#include <set>


namespace Guise
{


    class GUISE_API CanvasStyle
    {

    public:

        CanvasStyle();
        CanvasStyle(const Style::Selector & selector);

        const Vector4f & getBackgroundColor() const;

        void setBackgroundColor(const Vector4f & color);

    private:

        Vector4f m_backgroundColor;

    };

    /**
    * Canvas class.
    *
    *
    */
    class GUISE_API Canvas : public CanvasStyle
    {

    public:

        static std::shared_ptr<Canvas> create(const Vector2ui32 & size, std::shared_ptr<Style::Sheet> * styleSheet = nullptr);
        
        ~Canvas();

        bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());

        void update();

        void render(RendererInterface & renderInterface);

        const Input & getInput() const;
        Input & getInput();

        std::shared_ptr<Style::Sheet> getStyleSheet() const;

        const Vector2ui32 & getSize() const;

        void resize(const Vector2ui32 & size);

        //void registerControl(Control & control);

        //void unregisterControl(Control & control);

        void registerControlBoundsChange(Control & control, const Bounds2f & bounds);

        //void registerControlLevelChange(Control & control, const size_t level);

        void unregisterControl(Control & control);

    private:

        Canvas(const Vector2ui32 & size, std::shared_ptr<Style::Sheet> * styleSheet);

        ControlGrid                     m_controlGrid;
        Input                           m_input;
        std::shared_ptr<Plane>          m_plane;
        Control *                       m_selectedControl;
        Vector2ui32                     m_size;
        std::shared_ptr<Style::Sheet>   m_styleSheet;

    };

}

#endif