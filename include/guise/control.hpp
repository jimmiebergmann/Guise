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

#ifndef GUISE_CONTROL_CONTROL_HPP
#define GUISE_CONTROL_CONTROL_HPP

#include "guise/build.hpp"
#include "guise/style.hpp"
#include "guise/renderer.hpp"
#include <memory>
#include <vector>
#include <list>
#include <limits>
#include <mutex>

namespace Guise
{

    // Forward declarations
    class Canvas;
    class Control;
    class ControlContainer;
    class ControlContainerList;

    /**
    * Enumerator describing the type of control.
    *
    *
    */
    enum class ControlType
    {
        Custom,
        Button,
        Window
    };

    /**
    * Control base class.
    *
    *
    */
    class GUISE_API Control : public std::enable_shared_from_this<Control>
    {

    public:

        Control(Canvas & canvas);

        Canvas & getCanvas();
        const Canvas & getCanvas() const;

        virtual ~Control();

        virtual ControlType getType() const = 0;

        virtual void render(RendererInterface & rendererInterface, const StyleSheet & styleSheet, const Vector2f & canvasPosition, const Vector2f & canvasSize) = 0;

        virtual std::weak_ptr<Control> getParent();
        virtual std::weak_ptr<const Control> getParent() const;
        virtual std::vector<std::shared_ptr<Control> > getChilds();
        virtual std::vector<std::shared_ptr<const Control> > getChilds() const;

        virtual bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());
        virtual bool remove(Control & control);
        virtual bool remove(const std::shared_ptr<Control> & control);
        virtual bool remove(const size_t index);
        virtual size_t removeAll();
        virtual void release();

    private:

        friend class ControlContainer;

        Canvas & m_canvas;
        std::weak_ptr<Control> m_parent;
        mutable std::mutex     m_mutex;

    };

    class GUISE_API ControlContainer : public Control
    {

    public:

        ControlContainer(Canvas & canvas);
        
        virtual ~ControlContainer();

    protected:

        void adoptControl(Control & control);
        void releaseControl(Control & control);

    };

    class GUISE_API ControlContainerSingle : public ControlContainer
    {

    public:

        ControlContainerSingle(Canvas & canvas);

        virtual ~ControlContainerSingle();

        std::vector<std::shared_ptr<Control> > getChilds();
        std::vector<std::shared_ptr<const Control> > getChilds() const;

        bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());
        bool remove(Control & control);
        bool remove(const std::shared_ptr<Control> & control);
        bool remove(const size_t index);
        size_t removeAll();

    private:

        std::shared_ptr<Control>  m_child;
        mutable std::mutex        m_mutex;

    };

    class GUISE_API ControlContainerList : public ControlContainer
    {

    public:

        ControlContainerList(Canvas & canvas);

        virtual ~ControlContainerList();

        std::vector<std::shared_ptr<Control> > getChilds();
        std::vector<std::shared_ptr<const Control> > getChilds() const;

        bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());
        bool remove(Control & control);
        bool remove(const std::shared_ptr<Control> & control);
        bool remove(const size_t index);
        size_t removeAll();

    private:

        std::vector<std::shared_ptr<Control> > m_childs;
        mutable std::mutex                     m_mutex;

    };

}

#endif