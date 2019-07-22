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

#ifndef GUISE_CONTROL_LABEL_HPP
#define GUISE_CONTROL_LABEL_HPP

#include "guise/control.hpp"
#include "guise/font.hpp"

namespace Guise
{

    class Texture;

    class GUISE_API Label : public Control, public Style::FontStyle
    {

    public:

        static std::shared_ptr<Label> create(const std::wstring & text = L"");
        static std::shared_ptr<Label> create(const std::string & font, const std::wstring & text = L"");

        std::shared_ptr<Font> getFont() const;

        const std::wstring & getText() const;

        virtual ControlType getType() const;

        void setText(const std::wstring & text); 

        Signal<const std::wstring &> onChange;

    protected:

        Label(const std::wstring & text = L"");
        Label(const std::string & font, const std::wstring & text = L"");

    private:

        Label(const Label &) = delete;

        virtual void onCanvasChange(Canvas * canvas);

        virtual void onRender(RendererInterface & rendererInterface);

        virtual void onResize();

        virtual void onUpdate();

        bool                        m_changedText;
        int32_t                     m_dpi;
        std::shared_ptr<Font>       m_font;
        FontSequence                m_fontSequence;
        std::unique_ptr<uint8_t[]>  m_loadData;      
        std::wstring                m_text;
        std::shared_ptr<Texture>    m_texture;
        Vector2<size_t>             m_textureSize;

    };

}

#endif