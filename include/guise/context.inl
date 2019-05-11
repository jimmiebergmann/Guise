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

#include <iostream>

namespace Guise
{

    template<typename RType>
    Context<RType>::~Context()
    {

    }

    template<typename RType>
    inline std::shared_ptr<Context<RType> > Context<RType>::create()
    {
        static_assert(std::is_base_of<Renderer, RType>(), "Context<RType>::create() requires Renderer to be base of RType.");
        return std::shared_ptr<Context<RType> >(new Context<RType>());
    }

    template<typename RType>
    inline std::shared_ptr<AppWindow> Context<RType>::addAppWindow(const std::wstring & title, const Vector2ui32 & size)
    {
        auto appWindowData = std::make_shared<AppWindowData>();

        Semaphore windowIsCreated;

        appWindowData->m_thread = std::thread([this, &windowIsCreated, appWindowData, &title, &size]()
        {
            appWindowData->appWindow = AppWindow::create(title, size);
            auto appWindow = appWindowData->appWindow;
            auto renderer = RType::create(appWindowData->appWindow);
            appWindowData->appWindow->setRenderer(renderer);
            renderer->setViewportSize({0, 0}, appWindow->getSize());
            windowIsCreated.notifyOne();

            while (1)
            {
                auto timerStart = std::chrono::system_clock::now();

                appWindow->update();
                appWindow->render();
             
                auto timerEnd = std::chrono::system_clock::now();
                std::chrono::duration<double> deltaTime = timerEnd - timerStart;
                std::chrono::duration<double> frameSleepTime = getMaxFrameTime() - deltaTime;

                if (frameSleepTime.count() > 0.0f)
                {
                    std::this_thread::sleep_for(frameSleepTime);
                }
            }
        });

        windowIsCreated.wait();
        return appWindowData->appWindow;
    }

    template<typename RType>
    void Context<RType>::setMaxFrameTime(const std::chrono::duration<double> & frameTime)
    {
        m_maxFrameTime = frameTime;
    }

    template<typename RType>
    std::chrono::duration<double> Context<RType>::getMaxFrameTime() const
    {
        return m_maxFrameTime;
    }

    template<typename RType>
    Context<RType>::Context()
    {

    }

}