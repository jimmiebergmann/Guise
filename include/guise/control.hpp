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
#include "guise/input.hpp"
#include "guise/math/bounds.hpp"
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
        Checkbox,
        HorizontalGrid,
        Label,
        Plane,
        TextBox,
        VerticalGrid,
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

        Control();

        virtual ~Control();

        Canvas * getCanvas();
        const Canvas * getCanvas() const;

        void draw(RendererInterface & rendererInterface);

        virtual bool handleInputEvent(const Input::Event & event);

        virtual Bounds2f getSelectBounds() const;        

        virtual ControlType getType() const = 0;

        virtual void enable();
        virtual void disable();
        virtual bool isEnabled();

        virtual void enableInput();
        virtual void disableInput();
        virtual bool isInputEnabled() const;

        virtual void show();
        virtual void hide(const bool occupySpace = false);
        virtual bool isVisible() const;

        virtual bool intersects(const Vector2f & point) const;

        virtual size_t getLevel() const;
        virtual void setLevel(const size_t level);

        virtual std::weak_ptr<Control> getParent();
        virtual std::weak_ptr<const Control> getParent() const;
        virtual std::shared_ptr<Control> getChild();
        virtual std::shared_ptr<const Control> getChild() const;
        virtual std::vector<std::shared_ptr<Control> > getChilds();
        virtual std::vector<std::shared_ptr<const Control> > getChilds() const;

        virtual bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());
        virtual bool remove(Control & control);
        virtual bool remove(const std::shared_ptr<Control> & control);
        virtual bool remove(const size_t index);
        virtual size_t removeAll();
        virtual void release();

        virtual void onActiveChange(bool active);

        // TEST.
        Bounds2f getAvailableBounds() const;

        Bounds2f getBounds() const;

        const Bounds2f & setBounds(const Bounds2f & bounds);

        float getScale() const;

        void resize();

        bool isChildBoundsAware() const;
        void setChildBoundsAware(const bool aware);

        

    protected:

        virtual void onAddChild(Control & control, const size_t index);

        virtual void onCanvasChange(Canvas * canvas);

        virtual void onDisable();

        virtual void onEnable();

        virtual void onRemoveChild(Control & control, const size_t index);

        virtual void onRender(RendererInterface & rendererInterface);

        virtual void onResize();

        virtual void onUpdate();

        Bounds2f scale(const Bounds2f & bounds) const;
        Vector4f scale(const Vector4f & vector) const;
        Vector2f scale(const Vector2f & vector) const;

        void update();

    private:

        virtual void setCanvas(Canvas * canvas);

        Bounds2f                m_availableBounds;
        Bounds2f                m_bounds;
        Canvas *                m_canvas;
        uint8_t                 m_flags;
        size_t                  m_level;
        std::weak_ptr<Control>  m_parent;

        friend class Canvas;
        friend class ControlContainer;

    };

    class GUISE_API ControlContainer : public Control
    {

    public:
        
        virtual ~ControlContainer();

    protected:

        void adoptControl(Control & control);

        void releaseControl(Control & control);

        virtual void setCanvas(Canvas * canvas);

        void setControlCanvas(Control * control);
    };

    class GUISE_API ControlContainerSingle : public ControlContainer
    {

    public:

        virtual ~ControlContainerSingle();

        void setLevel(const size_t level);

        std::shared_ptr<Control> getChild();
        std::shared_ptr<const Control> getChild() const;
        std::vector<std::shared_ptr<Control> > getChilds();
        std::vector<std::shared_ptr<const Control> > getChilds() const;

        virtual void enable();
        virtual void disable();

        bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());
        bool remove(Control & control);
        bool remove(const std::shared_ptr<Control> & control);
        bool remove(const size_t index);
        size_t removeAll();

    private:

        virtual void setCanvas(Canvas * canvas);

        std::shared_ptr<Control>    m_child;
        mutable std::mutex          m_mutex;

    };

    class GUISE_API ControlContainerList : public ControlContainer
    {

    public:

        virtual ~ControlContainerList();

        void setLevel(const size_t level);

        std::shared_ptr<Control> getChild();
        std::shared_ptr<const Control> getChild() const;
        std::vector<std::shared_ptr<Control> > getChilds();
        std::vector<std::shared_ptr<const Control> > getChilds() const;

        virtual void enable();
        virtual void disable();

        bool add(const std::shared_ptr<Control> & control, const size_t index = std::numeric_limits<size_t>::max());
        bool remove(Control & control);
        bool remove(const std::shared_ptr<Control> & control);
        bool remove(const size_t index);
        size_t removeAll();

    private:

        virtual void setCanvas(Canvas * canvas);

        std::vector<std::shared_ptr<Control> >  m_childs;
        mutable std::mutex                      m_mutex;

    };

}

#endif