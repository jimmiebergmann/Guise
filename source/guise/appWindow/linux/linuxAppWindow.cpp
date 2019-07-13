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

#include "guise/appWindow/linux/linuxAppWindow.hpp"

#if defined(GUISE_PLATFORM_LINUX)

#include <algorithm>

namespace Guise
{
    /*static bool enableDpiAware(HWND windowHandle)
    {
    #if GUISE_PLATFORM_WINDOWS >= GUISE_PLATFORM_WINDOWS_10
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        linuxAppWindow.hpp
        if (!EnableNonClientDpiScaling(windowHandle))
        {
            return false;
        }
    #elif GUISE_PLATFORM_WINDOWS >= GUISE_PLATFORM_WINDOWS_VISTA
        if (!SetProcessDPIAware())
        {
            return false;
        }
    #endif

        return true;
    }*/

    /*#if GUISE_PLATFORM_WINDOWS >= GUISE_PLATFORM_WINDOWS_10
    static int getSystemDpi(HWND windowHandle, HDC )
    {
        return static_cast<int>(GetDpiForWindow(windowHandle));
    }
    #else
    static int getSystemDpi(HWND, HDC deviceContextHandle)
    {
        return GetDeviceCaps(deviceContextHandle, LOGPIXELSX);
    }

    #endif*/


    LinuxAppWindow::~LinuxAppWindow()
    {
        destroyWindow();
    }

    std::shared_ptr<LinuxAppWindow> LinuxAppWindow::create(const std::wstring & title, const Vector2ui32 & size)
    {
        return std::shared_ptr<LinuxAppWindow>(new LinuxAppWindow(title, size));
    }

    std::shared_ptr<Canvas> LinuxAppWindow::getCanvas()
    {
        return m_canvas;
    }

    void LinuxAppWindow::setRenderer(const std::shared_ptr<Renderer> & renderer)
    {
        m_renderer = renderer;
    }

    void LinuxAppWindow::update()
    {
        ::XEvent e;
        while(XPending(m_display) > 0)
        {
            XNextEvent(m_display, &e );

            switch(e.type)
            {
                case ConfigureNotify:
                {
                    Vector2ui32 size = { static_cast<uint32_t>(e.xconfigure.width), static_cast<uint32_t>(e.xconfigure.height) } ;  
                    if(size != m_size)
                    {
                        m_size = size;
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
                case MotionNotify:
                    m_input.pushEvent({ Input::EventType::MouseMove, { static_cast<float>(e.xmotion.x), static_cast<float>(e.xmotion.y) } });
                    break;
                default: break;
            }
        }

        /*MSG message;
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
        }*/

        m_canvas->update();
    }

    void LinuxAppWindow::render()
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

    void LinuxAppWindow::setDpi(const int32_t dpi)
    {
        m_dpi = dpi;
    }

    Vector2ui32 LinuxAppWindow::getSize() const
    {
        return m_size;
    }

    Vector2i32 LinuxAppWindow::getPosition() const
    {
        return {0, 0};
    }   

    int32_t LinuxAppWindow::getDpi() const
    {
        return m_dpi;
    }

    ::Display * LinuxAppWindow::getLinuxDisplay() const
    {
        return m_display;
    }

    ::Window LinuxAppWindow::getLinuxWindow() const
    {
        return m_window;
    }

    int LinuxAppWindow::getLinuxScreen() const
    {
        return m_screen;
    }

    LinuxAppWindow::LinuxAppWindow(const std::wstring & title, const Vector2ui32 & size) :
        m_canvas(Canvas::create(size)),
        m_dpi(GUISE_DEFAULT_DPI),
        m_input(m_canvas->getInput()),
        m_title(title),
        m_size(size),
        m_display(NULL),
        m_window(0),
        m_screen(0)
    {
        load();
    }

    void LinuxAppWindow::load()
    {
        // open a connection with X server
        if((m_display = XOpenDisplay(NULL)) == NULL)
        {
            throw std::runtime_error("Failed to connect to X server.");
        }

        // Initialize the X thread
        // Should we?!?!
        XInitThreads();

        // Get the screen
        m_screen = DefaultScreen(m_display);

        // Creat the window attributes
        XSetWindowAttributes windowAttributes;
        windowAttributes.colormap = DefaultColormap(m_display, m_screen);
        windowAttributes.event_mask =   KeyPressMask | KeyReleaseMask |
                                        ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | PointerMotionMask |
                                        EnterWindowMask | LeaveWindowMask | VisibilityChangeMask |
                                        FocusChangeMask | ExposureMask | StructureNotifyMask;

        // Create the window
        m_window = XCreateWindow(m_display,
                                 DefaultRootWindow(m_display),
                                 0, 0, m_size.x, m_size.y,
                                 0,
                                 DefaultDepth(m_display, m_screen),
                                 InputOutput,
                                 DefaultVisual(m_display, m_screen),
                                 CWBorderPixel | CWEventMask | CWColormap,
                                 &windowAttributes );

        // It's very important to set the delete message. Else we wont be able to close the window.
        ::Atom wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
        XSetWMProtocols(m_display, m_window, &wmDeleteMessage, 1);

        XStoreName(m_display, m_window, "Guise window" );

        // Let's set up the window decoration and the functionality
        //::Atom propertyAtom = XInternAtom(m_display, "_MOTIF_WM_HINTS", false);

        // Let's set up the window decoration and the functionality
        //::Atom PropertyAtom = XInternAtom( m_pDisplay, "_MOTIF_WM_HINTS", false );

        // Display the window.
        XMapWindow(m_display, m_window);
        XFlush(m_display);




        /*m_win32Style |= WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
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
        //winClass.hbrBackground = CreateSolidBrush(RGB(bgColor.x, bgColor.y, bgColor.z));
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

        int32_t systemDPi = getSystemDpi(m_windowHandle, m_deviceContextHandle);
        if (systemDPi != m_dpi)
        {
            RECT rect;
            if (GetWindowRect(m_windowHandle, &rect))
            {
                m_dpi = systemDPi;
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

        ShowWindow(m_windowHandle, SW_RESTORE);
        SetForegroundWindow(m_windowHandle);
        SetFocus(m_windowHandle);*/
    }

    void LinuxAppWindow::destroyWindow()
    {
        if(m_display)
        {
            XDestroyWindow(m_display, m_window );
            XCloseDisplay(m_display);
            m_display = NULL;
            m_window = 0;
            m_screen = 0;
        }
    }

/*
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

    LRESULT Win32AppWindow::windowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {

        // Windows events.
        case WM_CREATE:
        {

        }
        break;
        case WM_ERASEBKGND:
            return 1;
        case WM_PAINT:
        {

        }
        break;
        case WM_SIZE:
        {
            m_size = { static_cast<uint32_t>(LOWORD(lParam)), static_cast<uint32_t>(HIWORD(lParam)) };
            if (m_renderer)
            {
                m_renderer->setViewportSize({ 0, 0 }, m_size);
            }
            m_canvas->resize(m_size);
            update();
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
    #if GUISE_PLATFORM_WINDOWS >= GUISE_PLATFORM_WINDOWS_7
        case WM_DPICHANGED:
        {
            m_dpi = LOWORD(wParam);

            if (m_renderer)
            {
                m_renderer->setDpi(m_dpi);
            }
            m_canvas->setDpi(m_dpi);

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
*/
}

#endif
