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

#ifndef GUISE_WINDOW_LINUX_APPLICATION_WINDOW_HPP
#define GUISE_WINDOW_LINUX_APPLICATION_WINDOW_HPP

#include "guise/build.hpp"

#if defined(GUISE_PLATFORM_LINUX)

#include "guise/appWindow.hpp"

namespace Guise
{

    /**
    * Linux application window.
    *
    *
    */
    class GUISE_API LinuxAppWindow : public AppWindow
    {

    public:

        
        static std::shared_ptr<LinuxAppWindow> create(const std::wstring & title = L"", const Vector2ui32 & size = { 0, 0 });

        ~LinuxAppWindow();
       
        std::shared_ptr<Canvas> getCanvas();

        void setRenderer(const std::shared_ptr<Renderer> & renderer);

        void update();

        void render();

        void setDpi(const int32_t dpi);

        Vector2ui32 getSize() const;

        Vector2i32 getPosition() const;    

        int32_t getDpi() const;

        ::Display * getLinuxDisplay() const;
        ::Window getLinuxWindow() const;
        int getLinuxScreen() const;
       
    private:

        /*static std::string createClassName();
*/
        LinuxAppWindow(const std::wstring & title, const Vector2ui32 & size);

        void load();

        void destroyWindow();
/*
        //int getDpi() const;

        static LRESULT windowProcStatic(HWND p_HWND, UINT p_Message, WPARAM p_WParam, LPARAM p_LParam);
        LRESULT windowProc(HWND p_HWND, UINT p_Message, WPARAM p_WParam, LPARAM p_LParam);
*/
        std::shared_ptr<Renderer>   m_renderer;
        std::shared_ptr<Canvas>     m_canvas;
        int32_t                     m_dpi;
        Input &                     m_input;
        std::wstring                m_title; 
        Vector2ui32                 m_size;
   
        ::Display *                 m_display;        
        ::Window                    m_window;
        int                         m_screen;

    };

}

#endif

#endif