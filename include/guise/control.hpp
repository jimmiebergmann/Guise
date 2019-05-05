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
#include <memory>
#include <vector>
#include <list>

namespace Guise
{

    // Forward declarations
    class Control;
    class ControlContainer;
    class ControlContainerList;

    /**
    * Control base class.
    *
    *
    */
    class GUISE_API Control : public std::enable_shared_from_this<Control>
    {

    public:

        virtual ~Control();

        virtual std::weak_ptr<Control> getParent();
        virtual std::weak_ptr<const Control> getParent() const;
        virtual std::vector<std::shared_ptr<Control> > getChilds();
        virtual std::vector<std::shared_ptr<const Control> > getChilds() const;

        virtual bool add(std::shared_ptr<Control> control, const size_t index);
        virtual bool remove(Control & control);
        virtual bool remove(std::shared_ptr<Control> control);
        virtual bool remove(const size_t index);
        virtual size_t removeAll();

        virtual void release();

    private:

        friend class ControlContainer;

        std::weak_ptr<Control> m_parent;

    };

    class GUISE_API ControlContainer : public Control
    {

    public:

        virtual ~ControlContainer();

        virtual std::vector<std::shared_ptr<Control> > getChilds() = 0;
        virtual std::vector<std::shared_ptr<const Control> > getChilds() const = 0;

        virtual bool add(std::shared_ptr<Control> control, const size_t index) = 0;
        virtual bool remove(Control & control) = 0;
        virtual bool remove(std::shared_ptr<Control> control) = 0;
        virtual bool remove(const size_t index) = 0;
        virtual size_t removeAll() = 0;

    protected:

        void adoptControl(Control & control);
        void releaseControl(Control & control);

    };

    class GUISE_API ControlContainerList : public ControlContainer
    {

    public:

        virtual ~ControlContainerList();

        std::vector<std::shared_ptr<Control> > getChilds();
        std::vector<std::shared_ptr<const Control> > getChilds() const;

        bool add(std::shared_ptr<Control> control, const size_t index = 0);
        bool remove(Control & control);
        bool remove(std::shared_ptr<Control> control);
        bool remove(const size_t index);
        size_t removeAll();

    private:

        std::vector<std::shared_ptr<Control> > m_childs;

    };

}

#endif