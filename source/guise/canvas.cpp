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
#include <iostream>

namespace Guise
{
    // Canvas style implementations.
    CanvasStyle::CanvasStyle() :
        m_backgroundColor(1.0f, 1.0f, 1.0f, 1.0f)
    { }

    CanvasStyle::CanvasStyle(const Style::Selector & selector) :
        CanvasStyle()
    {
        auto & properties = selector.getProperties();
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            if (it->first == "background-color")
            { 
                m_backgroundColor = it->second->getVector4f();
            }
        }
    }

    const Vector4f & CanvasStyle::getBackgroundColor() const
    {
        return m_backgroundColor;
    }

    void CanvasStyle::setBackgroundColor(const Vector4f & color)
    {
        m_backgroundColor = color;
    }


    // Canvas implementations.
    std::shared_ptr<Canvas> Canvas::create(const Vector2ui32 & size, std::shared_ptr<Style::Sheet> * styleSheet)
    {
        return std::shared_ptr<Canvas>(new Canvas(size, styleSheet));
    }

    Canvas::~Canvas()
    { }

    bool Canvas::add(const std::shared_ptr<Control> & control, const size_t)
    {
        m_childs.push_back(control);
        control->setCanvas(this);
        control->setLevel(1);
        control->setBounds({ { 0.0f, 0.0f }, m_size });
        return true;
    }

    void Canvas::update()
    {
        m_input.update();
        
        struct MouseIntersector
        {
            MouseIntersector() :
                control(nullptr),
                isQueried(false)
            { }

            Control * control;
            bool isQueried;
            Vector2f position;
        } mouseHit;

        auto mouseEventFunc = [this, &mouseHit](Input::Event & e)
        {         
            if (!mouseHit.isQueried || mouseHit.position != e.position)
            {
                mouseHit.control = queryControlHit(e.position);
                mouseHit.position = e.position;
                mouseHit.isQueried = true;
            }

            if (m_hoveredControl && mouseHit.control != m_hoveredControl)
            {
                if (e.type == Input::EventType::MouseMove)
                {
                    m_hoveredControl->handleInputEvent(e);
                }
            }

            m_hoveredControl = mouseHit.control;
        };

        Input::Event e;
        while (m_input.pollEvent(e))
        {
            switch (e.type)
            {
                // Keyboard events.
                case Input::EventType::KeyboardJustPressed:
                case Input::EventType::KeyboardPress:
                case Input::EventType::KeyboardHolding:
                case Input::EventType::KeyboardRelease:
                case Input::EventType::Texting:
                {
                    if (m_activeControl)
                    {
                        m_activeControl->handleInputEvent(e);
                    }
                }
                break;

                // Mouse events.
                case Input::EventType::MouseJustPressed:
                case Input::EventType::MouseRelease:                
                case Input::EventType::MousePress:
                case Input::EventType::MouseDoubleClick:
                case Input::EventType::MouseMove:
                {
                    mouseEventFunc(e);

                    switch (e.type)
                    {
                        case Input::EventType::MouseJustPressed:
                        {
                            if (e.button == 0)
                            {
                                setActiveControl(mouseHit.control);
                            }
                        }
                        case Input::EventType::MouseRelease:
                        case Input::EventType::MousePress:
                        case Input::EventType::MouseDoubleClick:
                        case Input::EventType::MouseMove:
                        {
                            if (m_activeControl)
                            {
                                m_activeControl->handleInputEvent(e);
                            }
                            if (m_hoveredControl && m_hoveredControl != m_activeControl)
                            {
                                if (e.type == Input::EventType::MouseMove)
                                {
                                    m_hoveredControl->handleInputEvent(e);
                                }
                                
                            }
                        }
                        break;
                        default: break;
                    }
                }
                break;
                default: break;
            }
        }

        for (auto * control : m_updateControls)
        {
            control->onUpdate();
        }
        m_updateControls.clear();
    }

    void Canvas::render(RendererInterface & renderInterface)
    {
        for (auto & child : m_childs)
        {
            child->draw(renderInterface);
        }
    }

    const Input & Canvas::getInput() const
    {
        return m_input;
    }
    Input & Canvas::getInput()
    {
        return m_input;
    }

    std::shared_ptr<Style::Sheet> Canvas::getStyleSheet() const
    {
        return m_styleSheet;
    }

    const Vector2ui32 & Canvas::getSize() const
    {
        return m_size;
    }

    void Canvas::resize(const Vector2ui32 & size)
    {
        m_size = size;

        for (auto & child : m_childs)
        {
            child->setBounds({ {0.0f, 0.0f}, size });
        }
    }

    void Canvas::setDpi(const uint32_t dpi)
    {
        if (dpi != m_dpi)
        {
            m_dpi = dpi;
            m_scale = static_cast<float>(m_dpi) / GUISE_DEFAULT_DPI;            
            onDpiChange(m_dpi);
        }   
    }

    void Canvas::setScale(const float scale)
    {
        m_scale = std::max(scale, 0.0f);             
        uint32_t dpi = static_cast<uint32_t>(m_scale * GUISE_DEFAULT_DPI);
        if (dpi != m_dpi)
        {
            m_dpi = dpi;
            onDpiChange(m_dpi);
        }
    }

    void Canvas::setActiveControl(Control * control)
    {
        if (control == m_activeControl)
        {
            return;
        }

        if (m_activeControl)
        {
            m_activeControl->onActiveChange(false);
        }

        m_activeControl = control;

        if (m_activeControl)
        {                
            m_activeControl->onActiveChange(true);
        }
    }

    uint32_t Canvas::getDpi() const
    {
        return m_dpi;
    }

    float Canvas::getScale() const
    {
        return m_scale;
    }

    Control * Canvas::getActiveControl()
    {
        return m_activeControl;
    }

    void Canvas::reportControlChange(Control * control)
    {   
        if (control == nullptr)
        {
            return;
        }

        auto itControl = m_selectControls.find(control);

        // Add new.
        if (itControl == m_selectControls.end())
        {
            if (!control->getBounds().intersects(Bounds2f{ { 0.0f, 0.0f }, m_size }))
            {
                return;
            }

            const size_t level = control->getLevel();
            auto itLevel = m_selectControlLevels.find(level);
            if (itLevel == m_selectControlLevels.end())
            {
                itLevel = m_selectControlLevels.insert({ level, {} }).first;
            }

            itLevel->second.push_back(control);
            m_selectControls.insert({ control, level });
        }
        // Update existing.
        else
        {
            // Not inside canvas anymore, remove it.
            if (!control->getBounds().intersects(Bounds2f{ { 0.0f, 0.0f }, m_size }))
            {
                auto itLevel = m_selectControlLevels.find(itControl->second);
                auto itControl2 = std::find(itLevel->second.begin(), itLevel->second.end(), control);

                itLevel->second.erase(itControl2);
                m_selectControls.erase(control);

                if (control == m_selectedControl)
                {
                    m_selectedControl = nullptr;
                }
                if (control == m_activeControl)
                {
                    m_activeControl = nullptr;
                }
                if (control == m_hoveredControl)
                {
                    m_hoveredControl = nullptr;
                }
            }
            else
            {
                const size_t level = itControl->second;
                const size_t controlLevel = control->getLevel();

                // Level has been changed.
                if (level != controlLevel)
                {
                    // IMPLEMENT THIS!
                }
            }
        }
    }

    void Canvas::reportControlRemove(Control * control)
    {
        auto itLevel = m_selectControlLevels.find(control->getLevel());
        if (itLevel == m_selectControlLevels.end())
        {
            return;
        }

        auto itControl = std::find(itLevel->second.begin(), itLevel->second.end(), control);
        if (itControl == itLevel->second.end())
        {
            return;
        }

        itLevel->second.erase(itControl);
        m_selectControls.erase(control);
        m_updateControls.erase(control);

        if (control == m_selectedControl)
        {
            m_selectedControl = nullptr;
        }
        if (control == m_activeControl)
        {
            m_activeControl = nullptr;
        }
        if (control == m_hoveredControl)
        {
            m_hoveredControl = nullptr;
        }
    }

    void Canvas::updateControl(Control * control)
    {
        m_updateControls.insert(control);
    }

    void Canvas::resizeControl(Control * control)
    {
        Control * rootControl = control;
        Control * parent = nullptr;

        do
        {
            parent = rootControl->getParent().lock().get();

            auto oldBounds = rootControl->getBounds();
            rootControl->setBounds(rootControl->getAvailableBounds());
            if (oldBounds == rootControl->getBounds())
            {
                return;
            }

            rootControl = parent;            
        }
        while (parent && parent->isChildBoundsAware());
    }

    Canvas::Canvas(const Vector2ui32 & size, std::shared_ptr<Style::Sheet> * styleSheet) :
        m_dpi(GUISE_DEFAULT_DPI),
        m_scale(1.0f),
        m_selectedControl(nullptr),
        m_size(size),
        m_activeControl(nullptr),
        m_hoveredControl(nullptr)
    {
        if (styleSheet != nullptr)
        {
            m_styleSheet = *styleSheet;
        }
        if(!m_styleSheet)
        {
            m_styleSheet = Style::Sheet::createDefault();
        }

        CanvasStyle::operator=(*m_styleSheet->getSelector("canvas"));
    }

    Control * Canvas::queryControlHit(const Vector2f & point) const
    {
        for (auto levelIt = m_selectControlLevels.rbegin(); levelIt != m_selectControlLevels.rend(); levelIt++)
        {
            for (auto controlIt = levelIt->second.rbegin(); controlIt != levelIt->second.rend(); controlIt++)
            {
                if ((*controlIt)->intersects(point))
                {
                    return *controlIt;
                }
            }
        }
        return nullptr;
    }

}