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

#include "guise/appWindow/win32/win32AppWindow.hpp"

#if defined(GUISE_PLATFORM_WINDOWS)

#include <algorithm>
#include <iostream>
namespace Guise
{

    Win32AppWindow::~Win32AppWindow()
    {
        destroyWindow();
    }

    std::shared_ptr<Win32AppWindow> Win32AppWindow::create(const std::wstring & title, const Vector2ui32 & size)
    {
        return std::shared_ptr<Win32AppWindow>(new Win32AppWindow(title, size));
    }

    std::shared_ptr<Canvas> Win32AppWindow::getCanvas()
    {
        return m_canvas;
    }

    void Win32AppWindow::setRenderer(const std::shared_ptr<Renderer> & renderer)
    {
        m_renderer = renderer;
    }

    void Win32AppWindow::update()
    {
        MSG message;
        while (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            // A modal function is being called when you press the alt key,
            // fix this by ignoring the alt(menu) key event.
            if (message.message == WM_SYSCOMMAND &&
                message.wParam == SC_KEYMENU)
            {
                break;
            }

            // Translate the dispatch the message
            // This will call the WindowProcStatic function
            ::TranslateMessage(&message);
            ::DispatchMessage(&message);
        }

        m_canvas->update();
    }

    void Win32AppWindow::render()
    {
        auto backgroundColor = m_canvas->getBackgroundColor();
        m_renderer->setClearColor(backgroundColor);
        m_renderer->clearColor();
        m_canvas->render(*m_renderer.get());
        m_renderer->present();
    }

    Vector2ui32 Win32AppWindow::getSize()
    {
        return m_size;
    }

    HDC Win32AppWindow::getWindowContext() const
    {
        return m_deviceContextHandle;
    }

    std::string Win32AppWindow::createClassName()
    {
        static int classCount = 0;
        classCount++;
        return "guise_win32_class_" + std::to_string(classCount);

        // Generate class name.
        /*GUID guid = { 0 };
        const size_t guidArraySize = 64;
        wchar_t guidArray[guidArraySize] = { 0 };
        CoCreateGuid(&guid);
        StringFromGUID2(guid, guidArray, guidArraySize);
        std::wstring className = L"guier_" + std::wstring(guidArray);
        */
    }

    Win32AppWindow::Win32AppWindow(const std::wstring & title, const Vector2ui32 & size) :
        m_canvas(Canvas::create(size)),
        m_input(m_canvas->getInput()),
        m_title(title),
        m_size(size),
        m_windowHandle(NULL),
        m_deviceContextHandle(NULL),
        m_win32Style(0),
        m_win32ExtendedStyle(0),
        m_windowClassName(createClassName())
    {
        load();
    }

    void Win32AppWindow::load()
    {
        m_win32Style |= WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
        m_win32ExtendedStyle |= WS_EX_APPWINDOW;

        // Create a window class(WNDCLASS - win32)
        WNDCLASS winClass;
        HINSTANCE winInstance = GetModuleHandle(NULL); // Grab any old handle
        winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
        winClass.lpfnWndProc = (WNDPROC)Win32AppWindow::windowProcStatic;
        winClass.cbClsExtra = 0;
        winClass.cbWndExtra = 0;
        winClass.hInstance = winInstance;
        winClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        auto canvasBGColor = m_canvas->getBackgroundColor();
        Vector3i32 bgColorInt(canvasBGColor.x * 255.0f, canvasBGColor.y * 255.0f, canvasBGColor.z * 255.0f);
        Vector3<BYTE> bgColor(  std::min(std::max(bgColorInt.x, 0), 255),
                                std::min(std::max(bgColorInt.y, 0), 255),
                                std::min(std::max(bgColorInt.z, 0), 255));

        winClass.hbrBackground = CreateSolidBrush(RGB(bgColor.x, bgColor.y, bgColor.z));
        #ifdef UNICODE
            std::wstring tempClassName(m_windowClassName.length(), L' ');
            std::copy(m_windowClassName.begin(), m_windowClassName.end(), tempClassName.begin());
            winClass.lpszClassName = tempClassName.c_str();
        #else
            winClass.lpszClassName = m_windowClassName.c_str();
        #endif
        winClass.lpszMenuName = NULL;

        // Register the window class
        if (!RegisterClass(&winClass))
        {
            throw std::runtime_error("Failed to register Window class.");
        }

        // Apply the style
        RECT windowRect;
        windowRect.left = static_cast<LONG>(0);
        windowRect.right = static_cast<LONG>(m_size.x);
        windowRect.top = static_cast<LONG>(0);
        windowRect.bottom = static_cast<LONG>(m_size.y);

        AdjustWindowRectEx(&windowRect, m_win32Style, FALSE, m_win32ExtendedStyle);

        // Convert title if needed.
        #ifdef UNICODE
            std::wstring title = m_title;
        #else   
            std::string title = "";
            if (!m_title.empty())
            {
                int size_needed = WideCharToMultiByte(CP_UTF8, 0, &m_title[0], (int)m_title.size(), NULL, 0, NULL, NULL);
                title = std::string(size_needed, 0);
                WideCharToMultiByte(CP_UTF8, 0, &m_title[0], (int)m_title.size(), &title[0], size_needed, NULL, NULL);
            }
        #endif

        // Let's create the window.
        m_windowHandle = CreateWindowEx(
            m_win32ExtendedStyle,
            winClass.lpszClassName,
            title.c_str(),

            WS_CLIPSIBLINGS |
            WS_CLIPCHILDREN |
            m_win32Style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,

            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            NULL,
            NULL,
            winInstance,
            this);

        // Did the window creation succeed?
        if (m_windowHandle == NULL)
        {
            throw std::runtime_error("Failed to create window.");
        }

        // Get the device context
        m_deviceContextHandle = GetDC(m_windowHandle);
        //m_DPI = GetDpiForWindow(m_WindowHandle);

        ShowWindow(m_windowHandle, SW_RESTORE);
        SetForegroundWindow(m_windowHandle);
        SetFocus(m_windowHandle);
    }

    void Win32AppWindow::destroyWindow()
    {
        HINSTANCE Hinstance = GetModuleHandle(NULL);

        // Destroy the window
        if (m_windowHandle)
        {
            // Release the device context
            if (m_deviceContextHandle)
            {
                ReleaseDC(m_windowHandle, m_deviceContextHandle);
            }

            DestroyWindow(m_windowHandle);
        }

        // Unregister the window class
        if (m_windowClassName.size())
        {
            // Convert class name if needed.
            #ifdef UNICODE
                std::wstring tempClassName(m_windowClassName.length(), L' ');
                std::copy(m_windowClassName.begin(), m_windowClassName.end(), tempClassName.begin());
                UnregisterClass(tempClassName.c_str(), Hinstance);
            #else
                UnregisterClass(m_windowClassName.c_str(), Hinstance);
            #endif
        }
    }

    LRESULT Win32AppWindow::windowProcStatic(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (message == WM_NCCREATE)
        {
            SetWindowLongPtr(hWND, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
        }
        else
        {
            Win32AppWindow * window = (Win32AppWindow*)GetWindowLongPtr(hWND, GWLP_USERDATA);

            if (window != NULL)
            {
                return window->windowProc(hWND, message, wParam, lParam);
            }
        }

        return DefWindowProc(hWND, message, wParam, lParam);
    }

    LRESULT Win32AppWindow::windowProc(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {

        // Windows events.
        case WM_CREATE:
        {
            //SetProcessDpiAwareness(PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE);
        }
        case WM_ERASEBKGND:
            return 1;
            break;
        case WM_PAINT:
        {
            
        }
        break;
        case WM_SIZE:
        {
            if (!m_renderer)
            {
                break;
            }
            Vector2ui32 size(static_cast<uint32_t>(LOWORD(lParam)), static_cast<uint32_t>(HIWORD(lParam)));
            m_renderer->setViewportSize({0, 0}, size);
            m_canvas->resize(size);
            render();
        }
        break;

        // Keyboard events
        case WM_KEYDOWN:
            m_input.pushEvent({ Input::EventType::KeyboardPress, Input::transalteWin32Key(LOWORD(wParam)) });
            break;
        case WM_KEYUP:
            m_input.pushEvent({ Input::EventType::KeyboardRelease, Input::transalteWin32Key(LOWORD(wParam)) });
            break;
        case WM_CHAR:
            m_input.pushEvent({Input::EventType::Texting, static_cast<wchar_t>(wParam)});
            break;
        
        // Mouse events.
        case WM_MOUSEMOVE:
            m_input.pushEvent({ Input::EventType::MouseMove, { static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_LBUTTONDOWN:
            m_input.pushEvent({ Input::EventType::MousePress, uint8_t(0), { static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_MBUTTONDOWN:
            m_input.pushEvent({ Input::EventType::MousePress, uint8_t(1),{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_RBUTTONDOWN:
            m_input.pushEvent({ Input::EventType::MousePress, uint8_t(2),{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_LBUTTONUP:
            m_input.pushEvent({ Input::EventType::MouseRelease, uint8_t(0),{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_MBUTTONUP:
            m_input.pushEvent({ Input::EventType::MouseRelease, uint8_t(1),{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_RBUTTONUP:
            m_input.pushEvent({ Input::EventType::MouseRelease, uint8_t(2),{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_LBUTTONDBLCLK:
            m_input.pushEvent({ Input::EventType::MouseDoubleClick, uint8_t(0),{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_MBUTTONDBLCLK:
            m_input.pushEvent({ Input::EventType::MouseDoubleClick, uint8_t(1),{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_RBUTTONDBLCLK:
            m_input.pushEvent({ Input::EventType::MouseDoubleClick, uint8_t(2),{ static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)) } });
            break;
        case WM_MOUSEWHEEL:
        {
            auto zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (zDelta == 0)
            {
                break;
            }

            float delta = static_cast<float>(zDelta) / static_cast<float>(WHEEL_DELTA);
            m_input.pushEvent({ Input::EventType::MouseScroll, delta });
        } 
        break;
        default:
            break;

        }

       

        return DefWindowProc(hWND, message, wParam, lParam);
    }

}

#endif