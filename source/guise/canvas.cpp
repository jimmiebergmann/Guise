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

    bool Canvas::add(const std::shared_ptr<Control> & control, const size_t index)
    {
        m_plane->add(control, index);

        return true;
    }

    void Canvas::update()
    {
        m_plane->update({ { 0.0f, 0.0f }, m_size });
         
        m_input.update();
        
        struct HitHelper
        {
            HitHelper() :
                control(nullptr),
                isQueried(false),
                mouseMoved(false)
            { }

            Control * control;
            bool isQueried;
            Vector2f position;
            bool mouseMoved;
        } hit;

        auto mouseMovedFunc = [this, &hit](Input::Event & e)
        {         
            hit.mouseMoved = true;

            if (!hit.isQueried || hit.position != e.position)
            {
                hit.control = queryControlHit(e.position);
                hit.position = e.position;
                hit.isQueried = true;
            }

            if (hit.control)
            {
                hit.control->handleInputEvent(e);
            }

            if (hit.control != m_hoveredControl)
            {
                if (m_hoveredControl)
                {
                    m_hoveredControl->handleInputEvent(e);
                }
                m_hoveredControl = hit.control;
            }
        };

        Input::Event e;
        while (m_input.pollEvent(e))
        {
            switch (e.type)
            {
                case Input::EventType::MouseJustPressed:
                {
                    if (!hit.isQueried || hit.position != e.position)
                    {
                        hit.control = queryControlHit(e.position);
                        hit.position = e.position;
                        hit.isQueried = true;
                    }

                    setActiveControl(hit.control);

                    if (m_activeControl)
                    {
                        m_activeControl->handleInputEvent(e);
                    }
                }
                break;
                //case Input::EventType::MousePress:
                //case Input::EventType::MouseDoubleClick:
                case Input::EventType::KeyboardJustPressed:
                case Input::EventType::KeyboardPress:
                case Input::EventType::KeyboardHolding:
                case Input::EventType::KeyboardRelease:
                case Input::EventType::Texting:
                case Input::EventType::MouseRelease:
                {
                    if (m_activeControl)
                    {
                        m_activeControl->handleInputEvent(e);
                    }

                }
                break;
                case Input::EventType::MouseMove:
                {
                    mouseMovedFunc(e);
                }
                break;
                default: break;
            }
        }

        if (!hit.mouseMoved)
        {
            Input::Event moveEvent(Input::EventType::MouseMove, m_input.getLastMousePosition());
            mouseMovedFunc(moveEvent);
        }

        /*
        Input::Event e;
        while (m_input.pollEvent(e))
        {
            switch (e.type)
            {
            case Input::EventType::MouseJustPressed:    std::cout << "MouseJustPressed:"    << (int)e.button << std::endl; break;
            case Input::EventType::MouseDoubleClick:    std::cout << "MouseDoubleClick:"    << (int)e.button << std::endl; break;
            case Input::EventType::MouseMove:           std::cout << "MouseMove:"           << e.position.x << ", " << e.position.y << std::endl; break;
            case Input::EventType::MousePress:          std::cout << "MousePress:"          << (int)e.button << std::endl; break;
            case Input::EventType::MouseRelease:        std::cout << "MouseRelease:"        << (int)e.button << std::endl; break;
            case Input::EventType::MouseScroll:         std::cout << "MouseScroll:"         << e.distance << std::endl; break;
            case Input::EventType::KeyboardJustPressed: std::cout << "KeyboardJustPressed:" << (int)e.key << std::endl; break;
            case Input::EventType::KeyboardPress:       std::cout << "KeyboardPress:"       << (int)e.key << std::endl; break;
            case Input::EventType::KeyboardRelease:     std::cout << "KeyboardRelease:"     << (int)e.key << std::endl; break;
            //case Input::EventType::Texting:             std::cout << "Texting:"             << (int)e.character << std::endl; break;
            default: break;//                                    std::cout << "None event" << std::endl; break;
            }
        }*/

    }

    void Canvas::render(RendererInterface & renderInterface)
    {
        for (auto levelIt = m_renderControls.begin(); levelIt != m_renderControls.end(); levelIt++)
        {
            for (auto controlIt = levelIt->second.begin(); controlIt != levelIt->second.end(); controlIt++)
            {
                (*controlIt)->render(renderInterface);
            }
        }

        m_renderControls.clear();
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
    }

    void Canvas::setDpi(const uint32_t dpi)
    {
        if (dpi != m_dpi)
        {
            m_dpi = dpi;
            m_scale = static_cast<float>(m_dpi) / GUISE_DEFAULT_DPI;
            for (auto it = m_dpiSensitiveObjects.begin(); it != m_dpiSensitiveObjects.end(); it++)
            {
                (*it)->onNewDpi(m_dpi);
            }
        }   
    }

    void Canvas::setScale(const float scale)
    {
        if (scale < 0.0f)
        {
            m_scale = 0.0f;
        }
        else
        {
            m_scale = scale;
        }
        
        m_dpi = static_cast<uint32_t>(m_scale * GUISE_DEFAULT_DPI);
    }

    void Canvas::setActiveControl(Control * control)
    {
        if (m_activeControl && control != m_activeControl)
        {
            m_activeControl->onActiveChange(false);
        }

        if (control)
        {
            m_activeControl = control;
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

    void Canvas::queueControlRendering(Control * control)
    {
        auto itLevel = m_renderControls.find(control->getLevel());
        if (itLevel == m_renderControls.end())
        {
            itLevel = m_renderControls.insert({ control->getLevel(),{} }).first;
        }

        itLevel->second.push_back(control);
    }

    void Canvas::registerDpiSensitive(DpiSensitive * object)
    {
        m_dpiSensitiveObjects.insert(object);
    }

    void Canvas::unregisterDpiSensitive(DpiSensitive * object)
    {
        m_dpiSensitiveObjects.erase(object);
    }

    Canvas::Canvas(const Vector2ui32 & size, std::shared_ptr<Style::Sheet> * styleSheet) :
        m_dpi(GUISE_DEFAULT_DPI),
        m_plane(nullptr),
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

        m_plane = Plane::create(*this);
        m_plane->setLevel(1);
    }

    Control * Canvas::queryControlHit(const Vector2f & point) const
    {
        for (auto levelIt = m_renderControls.rbegin(); levelIt != m_renderControls.rend(); levelIt++)
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