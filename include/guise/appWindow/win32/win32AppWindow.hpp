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

#ifndef GUISE_WINDOW_WIN32_APPLICATION_WINDOW_HPP
#define GUISE_WINDOW_WIN32_APPLICATION_WINDOW_HPP

#include "guise/build.hpp"

#if defined(GUISE_PLATFORM_WINDOWS)

#include "guise/appWindow.hpp"

namespace Guise
{

    /**
    * Win32 window.
    *
    *
    */
    class GUISE_API Win32AppWindow : public AppWindow
    {

    public:

        static std::shared_ptr<Win32AppWindow> create(const std::wstring & title = L"", const Vector2ui32 & size = { 0, 0 });

        ~Win32AppWindow();
       
        std::shared_ptr<Canvas> getCanvas();

        void setRenderer(const std::shared_ptr<Renderer> & renderer);

        void update();

        void render();

        Vector2ui32 getSize();

        HDC getWindowContext() const;
       
    private:

        static std::string createClassName();

        Win32AppWindow(const std::wstring & title, const Vector2ui32 & size);

        void load();

        void destroyWindow();

        static LRESULT windowProcStatic(HWND p_HWND, UINT p_Message, WPARAM p_WParam, LPARAM p_LParam);
        LRESULT windowProc(HWND p_HWND, UINT p_Message, WPARAM p_WParam, LPARAM p_LParam);

        std::shared_ptr<Renderer>   m_renderer;
        std::shared_ptr<Canvas>     m_canvas;
        Input &                     m_input;
        std::wstring                m_title; 
        Vector2ui32                 m_size;
        HWND		                m_windowHandle;
        HDC			                m_deviceContextHandle;
        DWORD                       m_win32Style;           ///< Win32 style of window.
        DWORD                       m_win32ExtendedStyle;   ///< Win32 extended style of window.
        std::string                 m_windowClassName;

    };

}

#endif

#endif