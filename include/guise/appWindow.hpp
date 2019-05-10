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

#ifndef GUISE_APPLICATION_WINDOW_HPP
#define GUISE_APPLICATION_WINDOW_HPP

#include "guise/build.hpp"
#if defined(GUISE_PLATFORM_WINDOWS)
    #include "guise/platform/win32Headers.hpp"
#endif
#include "guise/canvas.hpp"
#include <memory>
#include <string>
#include <thread>

namespace Guise
{

    // Forward declarations.
    //class Context;

    /**
    * Application window base class.
    *
    *
    */
    class GUISE_API AppWindow
    {

    public:

        static std::shared_ptr<AppWindow> create(const std::wstring & title = L"", const Vector2ui32 & size = {0, 0});

        virtual ~AppWindow();

        virtual std::shared_ptr<Canvas> getCanvas() = 0;

        virtual void setRenderer(const std::shared_ptr<Renderer> & renderer) = 0;

        virtual void update() = 0;

        virtual void render() = 0;

        virtual Vector2ui32 getSize() = 0;

    #if defined(GUISE_PLATFORM_WINDOWS)
        virtual HDC getWindowContext() const = 0;
    #endif
        //virtual Context & getContext() = 0;

        /**
        * Modal function for processing event.  
        */
        //static void processEvents();

        /**
        * Terminates processEvents().
        */
       // static void terminateEventProcessing(const std::thread::native_handle_type & threadHandle);

    protected:

        //friend class Context;

       // virtual void load() = 0;

    };

}

#endif