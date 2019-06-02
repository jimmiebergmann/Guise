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

#ifndef GUISE_CONTEXT_HPP
#define GUISE_CONTEXT_HPP

#include "guise/build.hpp"
#include "guise/renderer.hpp"
#include "guise/appWindow.hpp"
#include "guise/utility/semaphore.hpp"
#include <memory>
#include <thread>
#include <atomic>
#include <set>
#include <chrono>

namespace Guise
{

    /**
    * Context class.
    *
    *
    */
    class GUISE_API Context
    {

    public:
        
        ~Context();
              
        static std::shared_ptr<Context> create();
        
        std::shared_ptr<AppWindow> addAppWindow(const std::wstring & title = L"", const Vector2ui32 & size = { 0, 0 });

        void setMaxFrameTime(const std::chrono::duration<double> & frameTime);

        std::chrono::duration<double> getMaxFrameTime() const;

    private:

        Context();

        Context(const Context &) = delete;

        struct AppWindowData
        {
            std::thread m_thread;
            std::shared_ptr<AppWindow> appWindow;
        };

        std::set<std::shared_ptr<AppWindowData> >   m_appWindows;
        std::shared_ptr<Renderer>                   m_renderer;
        std::atomic<std::chrono::duration<double>>  m_maxFrameTime;
    };

}

#endif