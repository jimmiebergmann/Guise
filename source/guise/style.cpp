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
                case Property::DataType::Boolean:       m_valueBoolean = property.m_valueBoolean; break;
                case Property::DataType::BorderStyle:   m_valueBorderStyle = property.m_valueBorderStyle; break;
                case Property::DataType::Float:         m_valueFloat = property.m_valueFloat; break;
                case Property::DataType::Overflow:      m_valueOverflow = property.m_valueOverflow; break;
                case Property::DataType::Vector2f:      m_valueVector2f = property.m_valueVector2f; break;
                case Property::DataType::Vector3f:      m_valueVector3f = property.m_valueVector3f; break;
                case Property::DataType::Vector4f:      m_valueVector4f = property.m_valueVector4f; break;
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
                case Property::DataType::Overflow:      m_valueOverflow = property->m_valueOverflow; break;
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
        Property::Property(const Overflow value) :
            m_dataType(DataType::Overflow),
            m_valueOverflow(value)
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

        Property::Overflow Property::getOverflow() const
        {
            return m_valueOverflow;
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

        std::shared_ptr<Property> Selector::getProperty(const std::string & name)
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
                    { "canvas", DefaultStyles::canvas },
                    { "plane",  DefaultStyles::plane },
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

    }

}