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
#include <iostream>

#if defined(GUISE_PLATFORM_WINDOWS)

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
    }

    void Win32AppWindow::render()
    {
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


   /* std::weak_ptr<Context> Win32AppWindow::getContext()
    {
        return m_context;
    }*/

    /*void Win32AppWindow::processEvents()
    {
        MSG message;
        BOOL result = 0;
        while ((result = ::GetMessage(&message, NULL, 0, 0)) != 0)
        {
            if (result == -1)
            {
                throw std::runtime_error("GetMessage returned -1.");
            }
            else
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
        }
        return;
        /*MSG message;
        while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }*/
    /*}

    void Win32AppWindow::terminateEventProcessing(const std::thread::native_handle_type & threadHandle)
    {
        ::PostThreadMessage(::GetThreadId(threadHandle), WM_QUIT, 0, 0);
    }*/

    std::string Win32AppWindow::createClassName()
    {
        static int classCount = 0;
        classCount++;
        return "guise_win32_class_" + std::to_string(classCount);
    }

    Win32AppWindow::Win32AppWindow(const std::wstring & title, const Vector2ui32 & size) :
        m_canvas(Canvas::create(size)),
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

        // Generate class name.
        /*GUID guid = { 0 };
        const size_t guidArraySize = 64;
        wchar_t guidArray[guidArraySize] = { 0 };
        CoCreateGuid(&guid);
        StringFromGUID2(guid, guidArray, guidArraySize);
        std::wstring className = L"guier_" + std::wstring(guidArray);
        */
        // Create a window class(WNDCLASS - win32)
        WNDCLASS winClass;
        HINSTANCE winInstance = GetModuleHandle(NULL); // Grab any old handle
        winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        winClass.lpfnWndProc = (WNDPROC)Win32AppWindow::windowProcStatic;
        winClass.cbClsExtra = 0;
        winClass.cbWndExtra = 0;
        winClass.hInstance = winInstance;
        winClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        winClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
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
        //m_pRenderer = new GdipRenderer(m_WindowHandle, m_pSkin);
        //m_pRenderer->Dpi(m_DPI);

        // Create plane control
        //m_pPlane = new Plane(m_pWindow);
        //m_pSkin->load();

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

        case WM_CREATE:
        {
            //SetProcessDpiAwareness(PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE);
        }
        case WM_ERASEBKGND:
            return 1;
            break;
        case WM_PAINT:
        {
            /*m_pRenderer->beginRendering();

            m_pRenderer->renderRectangle(Vector2i(0, 0), m_Size, Color::White);


            m_pRenderer->renderControl(m_pPlane, Vector2i(0, 0), m_Size);
            m_pRenderer->endRendering();*/
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
        }
        break;
        default:
            break;

        }

        return DefWindowProc(hWND, message, wParam, lParam);
    }

}

#endif