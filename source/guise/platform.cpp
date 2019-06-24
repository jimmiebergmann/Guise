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

#include "guise/platform.hpp"

namespace Guise
{

    std::wstring Platform::getClipboardText()
    {
    #if defined(GUISE_PLATFORM_WINDOWS)

        if (!(::OpenClipboard(nullptr)))
        {
            return L"";
        }

        HANDLE data = ::GetClipboardData(CF_UNICODETEXT);
        if (data == nullptr)
        {
            ::CloseClipboard();
            return L"";
        }

        wchar_t * text = static_cast<wchar_t*>(::GlobalLock(data));
        if (text == nullptr)
        {
            ::CloseClipboard();
            return L"";
        }

        ::GlobalUnlock(data);
        ::CloseClipboard();

        return text;

    #else
        return L"";
    #endif
    }

    bool Platform::setClipboardText(const std::wstring & text)
    {
#if defined(GUISE_PLATFORM_WINDOWS)
        if (!text.size())
        {
            return false;
        }

        const size_t len = (text.size() + 1) * sizeof(std::wstring::size_type);
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
        memcpy(::GlobalLock(hMem), text.c_str(), len);
        ::GlobalUnlock(hMem);
        
        if (!(::OpenClipboard(nullptr)))
        {
            return false;
        }
        ::EmptyClipboard();

        ::SetClipboardData(CF_UNICODETEXT, hMem);
        ::CloseClipboard();

        return true;

    #else
        return false;
    #endif
    }



}