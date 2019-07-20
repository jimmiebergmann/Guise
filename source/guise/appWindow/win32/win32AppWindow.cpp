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

#include <shellscalingapi.h>
#include <algorithm>
#include <iostream>
namespace Guise
{

#if GUISE_PLATFORM_WINDOWS >= GUISE_PLATFORM_WINDOWS_10
    static uint32_t getSystemDpi(HWND windowHandle, HDC )
    {
        return static_cast<uint32_t>(::GetDpiForWindow(windowHandle));
    }
#else
    static int getSystemDpi(HWND, HDC deviceContextHandle)
    {
        return ::GetDeviceCaps(deviceContextHandle, LOGPIXELSX);
    }
#endif
    

    // Win32 Applicaiton window implementations.
    std::shared_ptr<Win32AppWindow> Win32AppWindow::create(const std::wstring & title, const Vector2ui32 & size)
    {
        return std::shared_ptr<Win32AppWindow>(new Win32AppWindow(title, size));
    }

    Win32AppWindow::~Win32AppWindow()
    {
        close();
    }

    void Win32AppWindow::close()
    {
        if (m_windowHandle)
        {
            if (m_deviceContextHandle)
            {
                ::ReleaseDC(m_windowHandle, m_deviceContextHandle);
                m_deviceContextHandle = NULL;
            }

            ::DestroyWindow(m_windowHandle);
            m_windowHandle = NULL;
        }

        if (m_windowClassName.size())
        {
            HINSTANCE Hinstance = ::GetModuleHandle(NULL);

        // Convert class name if needed.
        #ifdef UNICODE
            std::wstring tempClassName(m_windowClassName.length(), L' ');
            std::copy(m_windowClassName.begin(), m_windowClassName.end(), tempClassName.begin());
            ::UnregisterClass(tempClassName.c_str(), Hinstance);
        #else
            ::UnregisterClass(m_windowClassName.c_str(), Hinstance);
        #endif

            m_windowClassName.clear();
        }

        m_focused = m_maximized = m_minimized = m_showing = false;
    }

    std::shared_ptr<Canvas> Win32AppWindow::getCanvas()
    {
        return m_canvas;
    }

    uint32_t Win32AppWindow::getDpi() const
    {
        return m_dpi;
    }

    Vector2i32 Win32AppWindow::getPosition() const
    {
        return m_position;
    }

    float Win32AppWindow::getScale() const
    {
        return m_scale;
    }

    Vector2ui32 Win32AppWindow::getSize() const
    {
        return m_size;
    }

    HDC Win32AppWindow::getWin32HDC() const
    {
        return m_deviceContextHandle;
    }

    bool Win32AppWindow::isFocused() const
    {
        return m_focused;
    }

    bool Win32AppWindow::isMaximized() const
    {
        return m_maximized;
    }

    bool Win32AppWindow::isMinimized() const
    {
        return m_minimized;
    }

    bool Win32AppWindow::isShowing() const
    {
        return m_showing;
    }

    void Win32AppWindow::maximize()
    {
        ShowWindow(m_windowHandle, SW_SHOWMAXIMIZED);
    }

    void Win32AppWindow::minimize()
    {
        ShowWindow(m_windowHandle, SW_SHOWMINIMIZED);
    }

    void Win32AppWindow::render()
    {
        if (!m_renderer)
        {
            return;
        }

        auto backgroundColor = m_canvas->getBackgroundColor();
        m_renderer->setClearColor(backgroundColor);
        m_renderer->clearColor();
        m_canvas->render(*m_renderer.get());
        m_renderer->present();
    }

    void Win32AppWindow::setDpi(const uint32_t dpi)
    {
        //if (dpi != m_dpi)
        //{
            m_dpi = dpi;
            m_scale = static_cast<float>(m_dpi) / GUISE_DEFAULT_DPI;
        //}
    }

    void Win32AppWindow::setRenderer(const std::shared_ptr<Renderer> & renderer)
    {
        m_renderer = renderer;
        if (m_renderer)
        {
            m_renderer->setScale(m_scale);
        }
    }

    void Win32AppWindow::setScale(const float scale)
    {
        m_scale = std::max(scale, 0.0f);
        m_dpi = static_cast<uint32_t>(m_scale * GUISE_DEFAULT_DPI);
        //uint32_t dpi = static_cast<uint32_t>(m_scale * GUISE_DEFAULT_DPI);
        //if (dpi != m_dpi)
        //{
        //    m_dpi = dpi;
            //onDpiChange(m_dpi);
        //}
    }

    void Win32AppWindow::show(const bool focus)
    {
        ::ShowWindow(m_windowHandle, SW_RESTORE);

        if (focus)
        {
            ::SetForegroundWindow(m_windowHandle);
            ::SetFocus(m_windowHandle);
        }
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

    Win32AppWindow::Win32AppWindow(const std::wstring & title, const Vector2ui32 & size) :
        m_loaded(false),
        m_canvas(Canvas::create(size)),
        m_dpi(GUISE_DEFAULT_DPI),
        m_scale(1.0f),
        m_input(m_canvas->getInput()),
        m_title(title),
        m_position(0, 0),
        m_size(size),
        m_windowHandle(NULL),
        m_deviceContextHandle(NULL),
        m_win32Style(0),
        m_win32ExtendedStyle(0),
        m_windowClassName(""),
        m_focused(false),
        m_maximized(false),
        m_minimized(false),
        m_showing(false)
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

        winClass.hbrBackground = NULL;
        
        m_windowClassName = createClassName();

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
            m_windowClassName.clear();
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

        uint32_t systemDPi = getSystemDpi(m_windowHandle, m_deviceContextHandle);
        
        RECT rect;
        if (GetWindowRect(m_windowHandle, &rect))
        {
            m_position.x = static_cast<int32_t>(rect.left);
            m_position.y = static_cast<int32_t>(rect.top);

            //RECT rect2;
            //::GetClientRect(m_windowHandle, &rect2);

            if (systemDPi != m_dpi)
            {
                m_dpi = systemDPi;
                m_scale = static_cast<float>(m_dpi) / GUISE_DEFAULT_DPI;
                m_size = m_size * m_dpi / GUISE_DEFAULT_DPI;

                RECT newWindowRect;
                newWindowRect.left = static_cast<LONG>(rect.left);
                newWindowRect.right = static_cast<LONG>(m_size.x + rect.left);
                newWindowRect.top = static_cast<LONG>(rect.top);
                newWindowRect.bottom = static_cast<LONG>(m_size.y + rect.top);

                AdjustWindowRectEx(&newWindowRect, m_win32Style, FALSE, m_win32ExtendedStyle);

                SetWindowPos(m_windowHandle,
                             NULL,
                             newWindowRect.left,
                             newWindowRect.top,
                             newWindowRect.right - newWindowRect.left,
                             newWindowRect.bottom - newWindowRect.top,
                             SWP_NOZORDER | SWP_NOACTIVATE);
            }   
        }

        m_canvas->setDpi(m_dpi);
        if (m_renderer)
        {
            m_renderer->setScale(m_scale);
        }


        onClose = [this]()
        {
            close();
        };

        onMaximize = [this]()
        {
            maximize();
        };

        onMinimize = [this]()
        {
            minimize();
        };

        m_loaded = true;
    }

    LRESULT Win32AppWindow::windowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            /*case WM_CREATE:
            {          
            }
            break;*/
            case WM_CLOSE:
            {
                onClose();
                return 0;
            }
            break;
            case WM_SYSCOMMAND:
            {
                switch (wParam)
                {
                    case SC_MAXIMIZE: onMaximize(); return 0; break;
                    case SC_MINIMIZE: onMinimize(); return 0; break;
                    default: break;
                }
            }
            break;
            case WM_SIZE:
            {
                m_size = { static_cast<uint32_t>(LOWORD(lParam)), static_cast<uint32_t>(HIWORD(lParam)) };
                
                switch (wParam)
                {
                    case SIZE_MAXIMIZED: m_showing = m_maximized = true; m_minimized = false; break;
                    case SIZE_MINIMIZED: m_minimized = true; m_showing = m_maximized = false; break;
                    case SIZE_RESTORED:  m_showing = true; m_maximized = m_minimized = false; break;
                    default: break;
                }

                onResize(m_size);

                if (m_loaded && m_showing)
                {
                    if (m_renderer)
                    {
                        m_renderer->setViewportSize({ 0, 0 }, m_size);
                    }
                    m_canvas->resize(m_size);
                    update();
                    render();
                }
            }
            break;
            case WM_MOVE:
            {
                m_position = { static_cast<int32_t>(LOWORD(lParam)), static_cast<int32_t>(HIWORD(lParam)) };
                onMove(m_position);
            }
            break;

            // Keyboard events
            case WM_KEYDOWN:
                m_input.pushEvent({ Input::EventType::KeyboardPress, Input::translateFromWin32Key(LOWORD(wParam)) });
                break;
            case WM_KEYUP:
                m_input.pushEvent({ Input::EventType::KeyboardRelease, Input::translateFromWin32Key(LOWORD(wParam)) });
                break;
            case WM_CHAR:
                m_input.pushEvent({Input::EventType::Texting, static_cast<wchar_t>(wParam)});
                break;        
            /*case WM_ACTIVATE:
                
                break;*/
            case WM_SETFOCUS:
            {   m_focused = true;
                onFocusChange(m_focused);
            }
            break;               
            case WM_KILLFOCUS:
            {   
                m_focused = false;
                onFocusChange(m_focused);
            }
            break;
        
            // Mouse events.
            case WM_MOUSEMOVE:
            { 
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MouseMove, { static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
            break;
            case WM_LBUTTONDOWN:
            {
                ::SetCapture(m_windowHandle);
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MousePress, uint8_t(0),{ static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
            break;
            case WM_MBUTTONDOWN:
            {
                ::SetCapture(m_windowHandle);
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MousePress, uint8_t(1),{ static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
            break;
            case WM_RBUTTONDOWN:
            {
                ::SetCapture(m_windowHandle);
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MousePress, uint8_t(2),{ static_cast<float>(point.x), static_cast<float>(point.y) } });

                /*UINT_PTR ID_CLOSE = MF_STRING;
                UINT_PTR ID_EXIT = MF_STRING;
                HMENU hPopupMenu = CreatePopupMenu();
                InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_CLOSE, "Exit");
                InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_EXIT, "Play");

                //SetForegroundWindow();
                TrackPopupMenu(hPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, point.x, point.y, 0, m_windowHandle, NULL);
                */
            }
            break;
            case WM_LBUTTONUP:
            {
                ::SetCapture(NULL);
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MouseRelease, uint8_t(0),{ static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
            break;
            case WM_MBUTTONUP:
            {
                ::SetCapture(NULL);
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MouseRelease, uint8_t(1),{ static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
            break;
            case WM_RBUTTONUP:
            {
                ::SetCapture(NULL);
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MouseRelease, uint8_t(2),{ static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
            break;
            case WM_LBUTTONDBLCLK:
            {
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MouseDoubleClick, uint8_t(0),{ static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
            break;
            case WM_MBUTTONDBLCLK:
            {
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MouseDoubleClick, uint8_t(1),{ static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
            break;
            case WM_RBUTTONDBLCLK:
            {
                auto point = MAKEPOINTS(lParam);
                m_input.pushEvent({ Input::EventType::MouseDoubleClick, uint8_t(2),{ static_cast<float>(point.x), static_cast<float>(point.y) } });
            }
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
        #if GUISE_PLATFORM_WINDOWS >= GUISE_PLATFORM_WINDOWS_7
            case WM_DPICHANGED:
            {
                m_dpi = LOWORD(wParam);
                m_scale = static_cast<float>(m_dpi) / GUISE_DEFAULT_DPI;

                if (m_renderer)
                {
                    m_renderer->setScale(m_scale);
                }
                m_canvas->setScale(m_scale);

                RECT * const newSize = reinterpret_cast<RECT*>(lParam);
                SetWindowPos(windowHandle,
                    NULL,
                    newSize->left,
                    newSize->top,
                    newSize->right - newSize->left,
                    newSize->bottom - newSize->top,
                    SWP_NOZORDER | SWP_NOACTIVATE);
            }
            break;
        #endif
            default:
                break;

        }

        return DefWindowProc(windowHandle, message, wParam, lParam);
    }

}

#endif