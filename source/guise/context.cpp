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

#include "guise/context.hpp"

namespace Guise
{

    Context::~Context()
    { }

    std::shared_ptr<Context> Context::create()
    {
        return std::shared_ptr<Context>(new Context());
    }

    std::shared_ptr<AppWindow> Context::addAppWindow(const std::wstring & title, const Vector2ui32 & size)
    {
        auto appWindowData = std::make_shared<AppWindowData>();

        Semaphore windowIsCreated;

        appWindowData->m_thread = std::thread([this, &windowIsCreated, appWindowData, &title, &size]()
        {
            appWindowData->appWindow = AppWindow::create(title, size);
            auto appWindow = appWindowData->appWindow;
            auto renderer = Renderer::createDefault(appWindowData->appWindow);
            appWindowData->appWindow->setRenderer(renderer);
            renderer->setViewportSize({0, 0}, appWindow->getSize());
            renderer->setDpi(appWindow->getDpi());

            /*auto windowSize = appWindow->getSize();
            auto windowDpi = appWindow->getDpi();
            Matrix4x4f orthoMat;
            orthoMat.loadOrthographic(0.0f, (float)windowSize.x, (float)windowSize.y, 0.0f, 0.0f, 1.0f);
            float scale = static_cast<float>(windowDpi) / static_cast<float>(GUISE_DEFAULT_DPI);
            orthoMat.scale(scale, scale, scale);
            renderer->setProjectionMatrix(orthoMat);*/

            windowIsCreated.notifyOne();

            while (1)
            {
                auto timerStart = std::chrono::system_clock::now();

                appWindow->update();

                /*auto currentDpi = appWindowData->appWindow->getDpi();
                auto currentSize = appWindow->getSize();
                if (currentSize != windowSize || currentDpi != windowDpi)
                {
                    windowSize = currentSize;
                    windowDpi = currentDpi;

                    scale = static_cast<float>(windowDpi) / static_cast<float>(GUISE_DEFAULT_DPI);
                    orthoMat.loadOrthographic(0.0f, (float)windowSize.x, (float)windowSize.y, 0.0f, 0.0f, 1.0f);
                    orthoMat.scale(scale, scale, scale);
                    renderer->setProjectionMatrix(orthoMat);
                }*/

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

    void Context::setMaxFrameTime(const std::chrono::duration<double> & frameTime)
    {
        m_maxFrameTime = frameTime;
    }

    std::chrono::duration<double> Context::getMaxFrameTime() const
    {
        return m_maxFrameTime;
    }

    bool Context::setDpiAware()
    {
    #if defined(GUISE_PLATFORM_WINDOWS)
        #if GUISE_PLATFORM_WINDOWS >= GUISE_PLATFORM_WINDOWS_10_1703
            #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
                return SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            #else
                return false;
            #endif
        #else
            return false;
        #endif
    #else
        return false;
    #endif
    }

    Context::Context()
    {

    }

}
