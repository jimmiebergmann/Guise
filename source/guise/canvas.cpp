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

#include "guise/canvas.hpp"

namespace Guise
{

    std::shared_ptr<Canvas> Canvas::create(const Vector2ui32 & size, std::shared_ptr<StyleSheet> * styleSheet)
    {
        return std::shared_ptr<Canvas>(new Canvas(size, styleSheet));
    }

    Canvas::~Canvas()
    { }

    bool Canvas::add(const std::shared_ptr<Control> & control, const size_t)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_controls.push_back(control);

        return true;
    }

    void Canvas::render(RendererInterface & renderInterface)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (auto it = m_controls.begin(); it != m_controls.end(); it++)
        {
            Vector2f test = m_size;
            (*(*it)).render(renderInterface, *m_styleSheet,{ 0.0f, 0.0f }, m_size);
        }
    }

    std::shared_ptr<StyleSheet> Canvas::getStyleSheet() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_styleSheet;
    }

    const Vector2ui32 & Canvas::getSize() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_size;
    }

    void Canvas::setSize(const Vector2ui32 & size)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_size = size;
    }
    
    Canvas::Canvas(const Vector2ui32 & size, std::shared_ptr<StyleSheet> * styleSheet) :
        m_size(size)
    {
        if (styleSheet != nullptr)
        {
            m_styleSheet = *styleSheet;
        }
        if(!m_styleSheet)
        {
            m_styleSheet = StyleSheet::createDefault();
        }

        setStyle(m_styleSheet->getStyle(StyleSheet::Entry::Canvas));
    }

}