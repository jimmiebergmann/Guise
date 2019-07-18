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
       
        void close(); // IMPLEMENT

        std::shared_ptr<Canvas> getCanvas();

        int32_t getDpi() const;

        Vector2i32 getPosition() const;

        Vector2ui32 getSize() const;

        ::HDC getWin32HDC() const;

        void maximize(); // IMPLEMENT

        void minimize(); // IMPLEMENT

        void render();

        void setDpi(const int32_t dpi);

        void setRenderer(const std::shared_ptr<Renderer> & renderer);

        void show(const bool focus = true); // IMPLEMENT

        void update();
       
    private:

        static std::string createClassName();

        static LRESULT windowProcStatic(HWND HWND, UINT message, WPARAM wParam, LPARAM lParam);

        Win32AppWindow(const std::wstring & title, const Vector2ui32 & size);

        void destroyWindow();

        void load();
        
        LRESULT windowProc(HWND HWND, UINT message, WPARAM wParam, LPARAM lParam);

        bool                        m_loaded;
        std::shared_ptr<Renderer>   m_renderer;
        std::shared_ptr<Canvas>     m_canvas;
        int32_t                     m_dpi;
        Input &                     m_input;
        std::wstring                m_title; 
        Vector2ui32                 m_position;
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