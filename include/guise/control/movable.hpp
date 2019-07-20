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

#ifndef GUISE_CONTROL_MOVABLE_HPP
#define GUISE_CONTROL_MOVABLE_HPP
/*
#include "guise/control.hpp"

namespace Guise
{

    template<typename C>
    class Movable : public Control
    {

    public:

        static std::shared_ptr<Movable<C> > create();

        ControlType getType() const;

        bool handleInputEvent(const Input::Event & event);

        void update(const Bounds2f & canvasBound);

        void render(RendererInterface & rendererInterface);

        Bounds2f getSelectBounds() const;
        
        std::shared_ptr<C> getControl() const;

    private:

        Movable();
        Movable(const Movable<C> &) = delete;

        std::shared_ptr<C> m_control;

    };

}

#include "guise/control/movable.inl"*/

#endif