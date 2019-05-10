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
/*
#ifndef GUISE_CONTROL_DOCKABLE_HPP
#define GUISE_CONTROL_DOCKABLE_HPP

#include "guise/control.hpp"
#include <type_traits>

namespace Guise
{

    template<typename T, typename E = std::enable_if<std::is_base_of<Control, T>::value>>
    class Dockable
    {

    public:

        static std::shared_ptr<Dockable<T> > create()
        {
            return std::shared_ptr<Dockable<T> >(new Dockable<T>());
        }

        ControlType getType() const;

        void render(RendererInterface & rendererInterface)
        {}

        void foo()
        {}

    private:

        Dockable()
        {}

        Dockable(const Dockable &) = delete;

    };

    // Include inline implementations.
    #include "guise/control/dockable.inl"

}

#endif*/