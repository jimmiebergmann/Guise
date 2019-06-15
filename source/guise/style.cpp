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

    namespace Style
    {

        // Helper functions and data
        /*static const std::string g_emptyString = "";

        static const std::string g_PropBackgroundColorName = "background-color";
        static const std::string g_PropBackgroundImageName = "background-image";
        static const std::string g_PropBorderColorName = "border-color";
        static const std::string g_PropBorderStyleName = "border-style";
        static const std::string g_PropBorderWidthName = "border-width";
        static const std::string g_PropOverflowName = "overflow";
        static const std::string g_PropPaddingName = "padding";
        static const std::string g_PropPositionName = "position";
        static const std::string g_PropSizeName = "size";

        static const std::string g_SelButtonName = "button";
        static const std::string g_SelCanvasName = "canvas";
        static const std::string g_SelPlaneName = "plane";
        static const std::string g_SelWindowName = "window";

        static const std::string & getPropertyName(const Property::Type type)
        {
            switch (type)
            {
            case Property::Type::BackgroundColor: return g_PropBackgroundColorName;
                case Property::Type::BackgroundImage: return g_PropBackgroundImageName;
                case Property::Type::BorderColor: return g_PropBorderColorName;
                case Property::Type::BorderStyle: return g_PropBorderStyleName;
                case Property::Type::BorderWidth: return g_PropBorderWidthName;
                case Property::Type::Overflow: return g_PropOverflowName;
                case Property::Type::Padding: return g_PropPaddingName;
                case Property::Type::Position: return g_PropPositionName;
                case Property::Type::Size: return g_PropSizeName;
                default: break;
            }
            return g_emptyString;
        }

        static const std::string & getSelectorName(const Selector::Type type)
        {
            switch (type)
            {
                case Selector::Type::Button: return g_SelButtonName;
                case Selector::Type::Canvas: return g_SelCanvasName;
                case Selector::Type::Plane: return g_SelPlaneName;
                case Selector::Type::Window: return g_SelWindowName;
                default: break;
            }
            return g_emptyString;
        }*/


        // Property implementations.
        Property::Property(const Property & property) :
            m_dataType(property.m_dataType)
        {
            switch (property.m_dataType)
            {
                case Property::DataType::Boolean:       m_valueBoolean      = property.m_valueBoolean; break;
                case Property::DataType::BorderStyle:   m_valueBorderStyle  = property.m_valueBorderStyle; break;
                case Property::DataType::Float:         m_valueFloat        = property.m_valueFloat; break;
                case Property::DataType::Integer:       m_valueInteger      = property.m_valueInteger; break;
                case Property::DataType::Overflow:      m_valueOverflow     = property.m_valueOverflow; break;
                case Property::DataType::String:        m_valueString       = new std::string(*property.m_valueString); break;
                case Property::DataType::Vector2f:      m_valueVector2f     = property.m_valueVector2f; break;
                case Property::DataType::Vector3f:      m_valueVector3f     = property.m_valueVector3f; break;
                case Property::DataType::Vector4f:      m_valueVector4f     = property.m_valueVector4f; break;
                default: break;
            }
        }

        Property::Property(const std::shared_ptr<Property> & property) :
            m_dataType(property->m_dataType)
        {
            if (!property)
            {
                return;
            }

            switch (property->m_dataType)
            {
                case Property::DataType::Boolean:       m_valueBoolean = property->m_valueBoolean; break;
                case Property::DataType::BorderStyle:   m_valueBorderStyle = property->m_valueBorderStyle; break;
                case Property::DataType::Float:         m_valueFloat = property->m_valueFloat; break;
                case Property::DataType::Integer:       m_valueInteger = property->m_valueInteger; break;
                case Property::DataType::Overflow:      m_valueOverflow = property->m_valueOverflow; break;
                case Property::DataType::String:        m_valueString = new std::string(*property->m_valueString); break;
                case Property::DataType::Vector2f:      m_valueVector2f = property->m_valueVector2f; break;
                case Property::DataType::Vector3f:      m_valueVector3f = property->m_valueVector3f; break;
                case Property::DataType::Vector4f:      m_valueVector4f = property->m_valueVector4f; break;
                default: break;
            }
        }

        Property::Property(const bool value) :
            m_dataType(DataType::Boolean),
            m_valueBoolean(value)
        { }
        Property::Property( const BorderStyle value) :
            m_dataType(DataType::BorderStyle),
            m_valueBorderStyle(value)
        { }
        Property::Property(const float value) :
            m_dataType(DataType::Float),
            m_valueFloat(value)
        { }
        Property::Property(const int value) :
            m_dataType(DataType::Integer),
            m_valueInteger(value)
        { }
        Property::Property(const Overflow value) :
            m_dataType(DataType::Overflow),
            m_valueOverflow(value)
        { }
        Property::Property(const std::string & value) :
            m_dataType(DataType::String),
            m_valueString(new std::string(value))
        { }
        Property::Property(const Vector2f & value) :
            m_dataType(DataType::Vector2f),
            m_valueVector2f(value)
        { }
        Property::Property(const Vector3f & value) :
            m_dataType(DataType::Vector3f),
            m_valueVector3f(value)
        { }
        Property::Property(const Vector4f & value) :
            m_dataType(DataType::Vector4f),
            m_valueVector4f(value)
        { }

        Property::~Property()
        {
            if (m_dataType == DataType::String)
            {
                delete m_valueString;
            }
        }


        Property::DataType Property::getDataType() const
        {
            return m_dataType;
        }

        bool Property::getBool() const
        {
            return m_valueBoolean;
        }

        Property::BorderStyle Property::getBorderStyle() const
        {
            return m_valueBorderStyle;
        }
        float Property::getFloat() const
        {
            return m_valueFloat;
        }

        int Property::getInteger() const
        {
            return m_valueInteger;
        }

        Property::Overflow Property::getOverflow() const
        {
            return m_valueOverflow;
        }

        std::string Property::getString() const
        {
            if (m_dataType != DataType::String)
            {
                return "";
            }
            return *m_valueString;
        }

        const Vector2f & Property::getVector2f() const
        {
            return m_valueVector2f;
        }

        const Vector3f & Property::getVector3f() const
        {
            return m_valueVector3f;
        }

        const Vector4f & Property::getVector4f() const
        {
            return m_valueVector4f;
        }

        void Property::setBool(const bool value)
        {
            m_valueBoolean = value;
            m_dataType = DataType::Boolean;
        }

        void Property::setBorderStyle(const BorderStyle value)
        {
            m_valueBorderStyle = value;
            m_dataType = DataType::BorderStyle;
        }

        void Property::setFloat(const float value)
        {
            m_valueFloat = value;
            m_dataType = DataType::Float;
        }

        void Property::setOverflow(const Overflow value)
        {
            m_valueOverflow = value;
            m_dataType = DataType::Overflow;
        }

        void Property::setVector2f(const Vector2f & value)
        {
            m_valueVector2f = value;
            m_dataType = DataType::Vector2f;
        }

        void Property::setVector3f(const Vector3f & value)
        {
            m_valueVector3f = value;
            m_dataType = DataType::Vector3f;
        }

        void Property::setVector4f(const Vector4f & value)
        {
            m_valueVector4f = value;
            m_dataType = DataType::Vector4f;
        }

        Property & Property::operator = (const bool value)
        {
            m_valueBoolean = value;
            m_dataType = DataType::Boolean;
            return *this;
        }

        Property & Property::operator = (const BorderStyle value)
        {
            m_valueBorderStyle = value;
            m_dataType = DataType::BorderStyle;
            return *this;
        }

        Property & Property::operator = (const float value)
        {
            m_valueFloat = value;
            m_dataType = DataType::Float;
            return *this;
        }

        Property & Property::operator = (const Overflow value)
        {
            m_valueOverflow = value;
            m_dataType = DataType::Overflow;
            return *this;
        }

        Property & Property::operator = (const Vector2f & value)
        {
            m_valueVector2f = value;
            m_dataType = DataType::Vector2f;
            return *this;
        }

        Property & Property::operator = (const Vector3f & value)
        {
            m_valueVector3f = value;
            m_dataType = DataType::Vector3f;
            return *this;
        }

        Property & Property::operator = (const Vector4f & value)
        {
            m_valueVector4f = value;
            m_dataType = DataType::Vector4f;
            return *this;
        }


        // Selector implementations.
        Selector::PropertyNameValue::PropertyNameValue(const std::string & name, const Property & property) :
            name(name),
            property(property)
        { }

        Selector::Selector()
        { }

        Selector::Selector(const Selector & selector)
        {
            for (auto it = selector.m_properties.begin(); it != selector.m_properties.end(); it++)
            {
                m_properties.insert({ it->first, std::make_shared<Property>(it->second) });
            }
        }

        Selector::Selector(const std::shared_ptr<Selector> & selector)
        {
            for (auto it = selector->m_properties.begin(); it != selector->m_properties.end(); it++)
            {
                m_properties.insert({ it->first, std::make_shared<Property>(*it->second) });
            }
        }

        Selector::Selector(const std::initializer_list<PropertyNameValue> & properties)
        {
            for (auto it = properties.begin(); it != properties.end(); it++)
            {
                auto itProp = m_properties.find(it->name);
                if (itProp == m_properties.end())
                {
                    m_properties.insert({ it->name, std::make_shared<Property>(it->property) });
                }
                else
                {
                    *(itProp->second) = it->property;
                }
            }
        }

        std::shared_ptr<Property> Selector::getProperty(const std::string & name) const
        {
            auto it = m_properties.find(name);
            if (it != m_properties.end())
            {
                return it->second;
            }
            return nullptr;
        }

        Properties & Selector::getProperties()
        {
            return m_properties;
        }
        const Properties & Selector::getProperties() const
        {
            return m_properties;
        }

        // Sheet implementations.
        Sheet::SelectorNameValue::SelectorNameValue(const std::string & name, const Selector & selector) :
            name(name),
            selector(selector)
        { }

        std::shared_ptr<Sheet> Sheet::create(const std::initializer_list<SelectorNameValue> & selectors)
        {
            return std::shared_ptr<Sheet>(new Sheet(selectors));
        }

        std::shared_ptr<Sheet> Sheet::createDefault()
        {
            return std::shared_ptr<Sheet>(new Sheet(
                {
                    { "button", DefaultStyles::button },
                    { "button:active", DefaultStyles::buttonActive },
                    { "button:disabled", DefaultStyles::buttonDisabled },
                    { "button:hover", DefaultStyles::buttonHover },
                    { "canvas", DefaultStyles::canvas },
                    { "label", DefaultStyles::label },
                    { "plane",  DefaultStyles::plane },
                    { "vertical-grid", DefaultStyles::verticalGrid },
                    { "vertical-grid-slot", DefaultStyles::verticalGridSlot },
                    { "window", DefaultStyles::window }
                }
            ));
        }


        std::shared_ptr<Selector> Sheet::getSelector(const std::string & name) const
        {
            auto it = m_selectors.find(name);
            if (it != m_selectors.end())
            {
                return it->second;
            }
            return nullptr;
        }

        Sheet::Sheet(const std::initializer_list<SelectorNameValue> & selectors)
        {
            for (auto it = selectors.begin(); it != selectors.end(); it++)
            {
                auto itSel = m_selectors.find(it->name);
                if (itSel == m_selectors.end())
                {
                    m_selectors.insert({ it->name, std::make_shared<Selector>(it->selector) });
                }
                else
                {
                    *(itSel->second) = it->selector;
                }
            }
        }


        // Padding style implementations.
        PaddingStyle::PaddingStyle(PaddingStyle * parent) :
            m_parent(parent)
        { }

        PaddingStyle::PaddingStyle(const std::shared_ptr<Selector> & selector, PaddingStyle * parent) :
            m_parent(parent)
        {
            if (!selector)
            {
                return;
            }

            auto padding = selector->getProperty("padding");
            if (padding)
            {
                switch (padding->getDataType())
                {
                    case Property::DataType::Float:
                    {
                        auto val = padding->getFloat();
                        m_padding = { val, val, val, val };
                    }
                    break;
                    case Property::DataType::Vector2f:
                    {
                        m_padding = { padding->getVector2f(), 0.0f, 0.0f };
                    }
                    break;
                    case Property::DataType::Vector4f:
                    {
                        m_padding = padding->getVector4f();
                    }
                    break;
                    default: break;
                }
            }
        }

        Vector4f PaddingStyle::getPadding() const
        {
            return m_padding.has_value() ? m_padding.value() : (m_parent ? m_parent->getPadding() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }
        Vector2f PaddingStyle::getPaddingLow() const
        {
            return m_padding.has_value() ? Vector2f{ m_padding.value().x, m_padding.value().y } : (m_parent ? m_parent->getPaddingLow() : Vector2f{ 0.0f, 0.0f });
        }
        Vector2f PaddingStyle::getPaddingHigh() const
        {
            return m_padding.has_value() ? Vector2f{ m_padding.value().z, m_padding.value().w } : (m_parent ? m_parent->getPaddingHigh() : Vector2f{ 0.0f, 0.0f });
        }

        void PaddingStyle::setPadding(const Vector4f & padding)
        {
            m_padding = padding;
        }

        void PaddingStyle::setPadding(const Vector2f & padding)
        {
            if (!m_padding.has_value())
            {
                m_padding = { 0.0f, 0.0f, 0.0f, 0.0f };
            }

            m_padding.value().x = padding.x;
            m_padding.value().y = padding.y;
        }

        void PaddingStyle::setPadding(const float & padding)
        {
            m_padding = { padding, padding, padding, padding };
        }


        // Box style implementations.
        BoxStyle::BoxStyle(BoxStyle * parent) :
            m_parent(parent)
        { }

        BoxStyle::BoxStyle(const std::shared_ptr<Selector> & selector, BoxStyle * parent) :
            m_parent(parent),
            PaddingStyle(selector, parent)
        {
            if (!selector)
            {
                return;
            }

            auto position = selector->getProperty("position");
            if (position && position->getDataType() == Property::DataType::Vector2f)
            {
                m_position = position->getVector2f();
            }
            auto size = selector->getProperty("size");
            if (size && size->getDataType() == Property::DataType::Vector2f)
            {
                m_size = size->getVector2f();
            }
            auto overflow = selector->getProperty("overflow");
            if (overflow && overflow->getDataType() == Property::DataType::Overflow)
            {
                m_overflow = overflow->getOverflow();
            }
            auto backgroundColor = selector->getProperty("background-color");
            if (backgroundColor && backgroundColor->getDataType() == Property::DataType::Vector4f)
            {
                m_backgroundColor = backgroundColor->getVector4f();
            }
            auto borderColor = selector->getProperty("border-color");
            if (borderColor && borderColor->getDataType() == Property::DataType::Vector4f)
            {
                m_borderColor = borderColor->getVector4f();
            }
            auto borderStyle = selector->getProperty("border-style");
            if (borderStyle && borderStyle->getDataType() == Property::DataType::BorderStyle)
            {
                m_borderStyle = borderStyle->getBorderStyle();
            }
            auto borderWidth = selector->getProperty("border-width");
            if (borderWidth && borderWidth->getDataType() == Property::DataType::Float)
            {
                m_borderWidth = borderWidth->getFloat();
            }
        }

        const Vector4f BoxStyle::getBackgroundColor() const
        {
            return m_backgroundColor.has_value() ? m_backgroundColor.value() : (m_parent ? m_parent->getBackgroundColor() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }
        const Vector4f BoxStyle::getBorderColor() const
        {
            return m_borderColor.has_value() ? m_borderColor.value() : (m_parent ? m_parent->getBorderColor() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }
        Property::BorderStyle BoxStyle::getBorderStyle() const
        {
            return m_borderStyle.has_value() ? m_borderStyle.value() : (m_parent ? m_parent->getBorderStyle() : Property::BorderStyle::None);
        }
        float BoxStyle::getBorderWidth() const
        {
            return m_borderWidth.has_value() ? m_borderWidth.value() : (m_parent ? m_parent->getBorderWidth() : 0.0f);
        }
        const Vector2f BoxStyle::getPosition() const
        {
            return m_position.has_value() ? m_position.value() : (m_parent ? m_parent->getPosition() : Vector2f{ 0.0f, 0.0f });
        }
        const Vector2f BoxStyle::getSize() const
        {
            return m_size.has_value() ? m_size.value() : (m_parent ? m_parent->getSize() : Vector2f{ 0.0f, 0.0f });
        }
        Property::Overflow BoxStyle::getOverflow() const
        {
            return m_overflow.has_value() ? m_overflow.value() : (m_parent ? m_parent->getOverflow() : Property::Overflow::hidden);
        }

        void BoxStyle::setBackgroundColor(const Vector4f & color)
        {
            m_backgroundColor = color;
        }
        void BoxStyle::setBorderColor(const Vector4f & color)
        {
            m_borderColor = color;
        }
        void BoxStyle::setBorderStyle(const Property::BorderStyle borderStyle)
        {
            m_borderStyle = borderStyle;
        }
        void BoxStyle::setBorderWidth(const float width)
        {
            m_borderWidth = width;
        }
        void BoxStyle::setPosition(const Vector2f & position)
        {
            m_position = position;
        }
        void BoxStyle::setSize(const Vector2f & size)
        {
            m_size = size;
        }
        void BoxStyle::setOverflow(const Property::Overflow overflow)
        {
            m_overflow = overflow;
        }

    }

}