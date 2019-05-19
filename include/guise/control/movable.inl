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

namespace Guise
{

    // Movable implementations.
    template<typename C>
    inline std::shared_ptr<Movable<C> > Movable<C>::create(Canvas & canvas)
    {
        return std::shared_ptr<Movable<C> >(new Movable<C>(canvas));
    }

    template<typename C>
    inline ControlType Movable<C>::getType() const
    {
        return m_control->getType();
    }

    /* bool handleInputEvent(const Input::Event & event);

    void update(const Bounds2f & canvasBound);

    void render(RendererInterface & rendererInterface);

    Bounds2f getSelectBounds() const;
    */

    template<typename C>
    inline Movable<C>::Movable(Canvas & canvas) :
        Control(canvas)
    { }

    template<typename C>
    inline std::shared_ptr<C> Movable<C>::getControl() const
    {
        return m_control;
    }


}