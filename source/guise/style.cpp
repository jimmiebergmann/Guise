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

#include "guise/style.hpp"
#include "guise/defaultStyles.hpp"
#include <algorithm>
#include <iostream>

namespace Guise
{
    static Style g_dummyStyle;

    // Style PropertyPair implementations.
    Style::PropertyPair::PropertyPair(const Property property, const float value) :
        property(property),
        f(value),
        valueType(ValueType::Float)
    {}

    Style::PropertyPair::PropertyPair(const Property property, const bool value) :
        property(property),
        b(value),
        valueType(ValueType::Boolean)
    {}

    Style::PropertyPair::PropertyPair(const Property property, const Vector2f & value) :
        property(property),
        vec2(value),
        valueType(ValueType::Vector2f)
    { }

    Style::PropertyPair::PropertyPair(const Property property, const Vector3f & value) :
        property(property),
        vec3(value),
        valueType(ValueType::Vector3f)
    {}

    Style::PropertyPair::PropertyPair(const Property property, const Vector4f & value) :
        property(property),
        vec4(value),
        valueType(ValueType::Vector4f)
    {}

    Style::PropertyPair::PropertyPair(const Property property, const BorderStyle value) :
        property(property),
        borderStyle(value),
        valueType(ValueType::BorderStyle)
    {}


    // Style implementations.
    Style::Style() :
        m_clamp(true),
        m_position(0.0f, 0.0f),
        m_size(0.0f, 0.0f),
        m_padding(0.0f, 0.0f, 0.0f, 0.0f),
        m_backgroundColor(0.0f, 0.0f, 0.0f, 0.0f),
        m_borderStyle(BorderStyle::None),
        m_borderWidth(0.0f),
        m_borderColor(0.0f, 0.0f, 0.0f, 0.0f)
    { }

    Style::Style(const std::initializer_list<PropertyPair> & properties) :
        Style()
    {
        setStyleProperties(properties);
    }

    Style::Style(const Style & style) :
        m_clamp(style.getClamp()),
        m_position(style.getPosition()),
        m_size(style.getSize()),
        m_padding(style.getPadding()),
        m_backgroundColor(style.getBackgroundColor()),
        m_borderStyle(style.getBorder()),
        m_borderWidth(style.getBorderWidth()),
        m_borderColor(style.getBorderColor())
    { }

    Style & Style::operator =(const Style & style)
    {
        m_clamp = style.getClamp();
        m_position = style.getPosition();
        m_size = style.getSize();
        m_padding = style.getPadding();
        m_backgroundColor = style.getBackgroundColor();
        m_borderStyle = style.getBorder();
        m_borderWidth = style.getBorderWidth();
        m_borderColor = style.getBorderColor();
        return *this;
    }

    void Style::setStyle(const Style & style)
    {
        *this = style;
    }

    void Style::setStyleProperties(const std::initializer_list<PropertyPair> & properties)
    {
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            switch (it->property)
            {
                case Property::BackgroundColor: m_backgroundColor = it->vec4; break;
                case Property::Border: m_borderStyle = it->borderStyle; break;
                case Property::BorderColor:
                    switch (it->valueType)
                    {
                        case PropertyPair::ValueType::Vector3f: m_borderColor = { it->vec3, 1.0f }; break;
                        case PropertyPair::ValueType::Vector4f: m_borderColor = it->vec4; break;
                        default: break;
                    }
                    break;
                case Property::BorderWidth: m_borderWidth = it->f; break;
                case Property::Clamp: m_clamp = it->b; break;
                case Property::Padding:
                    switch (it->valueType)
                    {
                        case PropertyPair::ValueType::Float: m_padding = { it->f , it->f , it->f , it->f }; break;
                        case PropertyPair::ValueType::Vector2f: m_padding = { it->vec2.x, it->vec2.y, it->vec2.x, it->vec2.y }; break;
                        case PropertyPair::ValueType::Vector4f: m_padding = { it->vec4.x, it->vec4.y, it->vec4.z, it->vec4.w }; break;
                        default: break;
                    }
                    break;
                case Property::Position: m_position = it->vec2; break;
                case Property::Size: m_size = it->vec2; break;
                    
                    
                default: break;
            }
        }
    }

    bool Style::getClamp() const
    {
        return m_clamp;
    }

    void Style::setClamp(bool clamp)
    {
        m_clamp = clamp;
    }

    const Vector2f & Style::getPosition() const
    {
        return m_position;
    }

    void Style::setPosition(const Vector2f & position)
    {
        m_position = position;
    }

    const Vector2f & Style::getSize() const
    {
        return m_size;
    }

    void Style::setSize(const Vector2f & size)
    {
        m_size = size;
    }

    const Vector4f & Style::getPadding() const
    {
        return m_padding;
    }

    Vector2f Style::getPaddingLow() const
    {
        return { m_padding.x, m_padding.y };
    }

    Vector2f Style::getPaddingHigh() const
    {
        return { m_padding.z, m_padding.w };
    }

    void Style::setPadding(const float padding)
    {
        m_padding = { padding, padding, padding, padding };
    }

    void Style::setPadding(const Vector2f & padding)
    {
        m_padding = { padding.x, padding.y, padding.x, padding.y };
    }

    void Style::setPadding(const Vector4f & padding)
    {
        m_padding = padding;
    }

    void Style::setPaddingLow(const Vector2f & low)
    {
        m_padding.x = low.x;
        m_padding.y = low.y;
    }

    void Style::setPaddingHigh(const Vector2f & high)
    {
        m_padding.z = high.x;
        m_padding.w = high.y;
    }

    Vector4f Style::getBackgroundColor() const
    {
        return m_backgroundColor;
    }

    void Style::setBackgroundColor(const Vector4f & color)
    {
        m_backgroundColor = color;
    }

    Style::BorderStyle Style::getBorder() const
    {
        return m_borderStyle;
    }

    void Style::setBorder(const BorderStyle style)
    {
        m_borderStyle = style;
    }

    float Style::getBorderWidth() const
    {
        return m_borderWidth;
    }

    void Style::setBorderWidth(const float width)
    {
        m_borderWidth = width;
    }

    Vector4f Style::getBorderColor() const
    {
        return m_borderColor;
    }

    void Style::setBorderColor(const Vector4f color)
    {
        m_borderColor = color;
    }

    Bounds2f Style::calcRenderBounds(const Bounds2f & canvasBound)
    {
        Bounds2f bounds = { canvasBound.position + m_position, m_size};
        
        if (m_clamp)
        {
            bounds.position = Vector2f::clamp(bounds.position, canvasBound.position, Vector2f::max(canvasBound.position, canvasBound.position + canvasBound.size));
        }

        Vector2f lower = bounds.position;
        Vector2f higherCanvas = canvasBound.position + canvasBound.size;
        Vector2f higherThis = bounds.position + Vector2f::max({ 0.0f, 0.0f }, m_size);

        if ((m_clamp && higherThis.x > higherCanvas.x) || m_size.x <= 0.0f)
        {
            bounds.size.x = higherCanvas.x - lower.x;
        }
        if ((m_clamp && higherThis.y > higherCanvas.y) || m_size.y <= 0.0f)
        {
            bounds.size.y = higherCanvas.y - lower.y;
        }
        
        bounds.size.x = std::max(0.0f, bounds.size.x);
        bounds.size.y = std::max(0.0f, bounds.size.y);

        return bounds;
    }

  
    // StyleSheet EntryPair implementations.
    StyleSheet::EntryPair::EntryPair(const Entry entry, const Style & style) :
        entry(entry),
        name(""),
        style(style)
    { }

    StyleSheet::EntryPair::EntryPair(const std::string & name, const Style & style) :
        entry(entry),
        name(name),
        style(style)
    { }


    // StyleSheet implementations.
    std::shared_ptr<StyleSheet> StyleSheet::create(const std::initializer_list<EntryPair> & entries)
    {
        return std::shared_ptr<StyleSheet>(new StyleSheet(entries));
    }

    std::shared_ptr<StyleSheet> StyleSheet::createDefault()
    {
        return std::shared_ptr<StyleSheet>(new StyleSheet(
            {
                { StyleSheet::Entry::Button, DefaultStyles::button },
                { StyleSheet::Entry::Canvas, DefaultStyles::canvas },
                { StyleSheet::Entry::Plane,  DefaultStyles::plane },
                { StyleSheet::Entry::Window, DefaultStyles::window }
            }
        ));
    }

    const Style & StyleSheet::getStyle(const Entry entry) const
    {
        switch (entry)
        {
            case Entry::Canvas: return *m_canvasStyle;
            case Entry::Button: return *m_buttonStyle;
            case Entry::Plane:  return *m_planeStyle;
            case Entry::Window: return *m_windowStyle;
            default: break;
        }

        // Dummy.
        return g_dummyStyle;
    }

    Style & StyleSheet::getStyle(const Entry entry)
    {
        switch (entry)
        {
            case Entry::Button: return *m_buttonStyle;
            case Entry::Canvas: return *m_canvasStyle;
            case Entry::Plane:  return *m_planeStyle;
            case Entry::Window: return *m_windowStyle;
            default: break;
        }

        // Dummy.
        return g_dummyStyle;
    }

    const Style * StyleSheet::getStyle(const std::string & name) const
    {
        auto it = m_styleMap.find(name);
        if (it != m_styleMap.end())
        {
            return it->second.get();
        }

        return nullptr;
    }
    Style * StyleSheet::getStyle(const std::string & name)
    {
        auto it = m_styleMap.find(name);
        if (it != m_styleMap.end())
        {
            return it->second.get();
        }

        return nullptr;
    }


    StyleSheet::StyleSheet(const std::initializer_list<EntryPair> & entries) :
        m_buttonStyle(std::make_shared<Style>()),
        m_canvasStyle(std::make_shared<Style>()),
        m_planeStyle(std::make_shared<Style>()),
        m_windowStyle(std::make_shared<Style>()),
        m_styleMap{
            { "canvas", m_canvasStyle },
            { "button", m_buttonStyle },
            { "plane",  m_planeStyle } ,
            { "window", m_windowStyle } }
    {
        for (auto it = entries.begin(); it != entries.end(); it++)
        {
            // Add entry, based on name.
            if (it->name.size())
            {
                if (it->name == "canvas")
                {
                    *m_canvasStyle = it->style;
                    continue;
                }
                else if (it->name == "button")
                {
                    *m_buttonStyle = it->style;
                    continue;
                }
                else if (it->name == "plane")
                {
                    *m_planeStyle = it->style;
                    continue;
                }
                else if (it->name == "window")
                {
                    *m_windowStyle = it->style;
                    continue;
                }

                auto smIt = m_styleMap.find(it->name);
                if(smIt == m_styleMap.end())
                {
                    m_styleMap.insert({ it->name, std::make_shared<Style>(it->style) });
                }
                else
                {
                    *smIt->second = it->style;
                }
                
                continue;
            }

            // Add entry, based on type.
            switch (it->entry)
            {
                case StyleSheet::Entry::Button: *m_buttonStyle = it->style; break;
                case StyleSheet::Entry::Canvas: *m_canvasStyle = it->style; break;
                case StyleSheet::Entry::Plane:  *m_planeStyle = it->style; break;
                case StyleSheet::Entry::Window: *m_windowStyle = it->style; break;
                default: break;
            }
        }
    }

}