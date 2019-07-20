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
#include "guise/control.hpp"
#include <algorithm>
#include <iostream>

namespace Guise
{

    namespace Style
    {

        static const std::string g_emptyString = "";

        // External definitions.
        float const FitParent = -1.0f;
        float const FitContent = 0.0f;

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


        // Linear Gradient implementations.
        LinearGradient::LinearGradient() :
            m_angle(0.0f),
            m_colorA(0.0f, 0.0f, 0.0f, 0.0f),
            m_colorB(0.0f, 0.0f, 0.0f, 0.0f)
        { }

        LinearGradient::LinearGradient(const Vector4f & colorA, const Vector4f & colorB) :
            m_angle(0.0f),
            m_colorA(colorA),
            m_colorB(colorB)
        { }

        LinearGradient::LinearGradient(const float angle, const Vector4f & colorA, const Vector4f & colorB) :
            m_angle(angle),
            m_colorA(colorA),
            m_colorB(colorB)
        { }

        void LinearGradient::setAngle(const float angle)
        {
            m_angle = angle;
        }

        void LinearGradient::setColorA(const Vector4f & color)
        {
            m_colorA = color;
        }

        void LinearGradient::setColorB(const Vector4f & color)
        {
            m_colorB = color;
        }

        float LinearGradient::getAngle() const
        {
            return m_angle;
        }

        const Vector4f & LinearGradient::getColorA() const
        {
            return m_colorA;
        }

        const Vector4f & LinearGradient::getColorB() const
        {
            return m_colorB;
        }


        // Size implementations.
        Size::Size() :
            Vector2f(0.0f, 0.0f),
            fit(NoFit, NoFit)
        { }

        Size::Size(const float x, const float y) :
            Vector2f(x, y),
            fit(NoFit, NoFit)
        { }

        Size::Size(const float x, const Fit yFit) :
            Vector2f(x, 0.0f),
            fit(NoFit, yFit)
        { }

        Size::Size(const Fit xFit, const float y) :
            Vector2f(0.0f, y),
            fit(xFit, NoFit)
        { }

        Size::Size(const Fit xFit, const Fit yFit) :
            Vector2f(0.0f, 0.0f),
            fit(xFit, yFit)
        { }

        Size::Size(const Vector2f & vector) :
            Vector2f(vector),
            fit(NoFit, NoFit)
        { }


        // Property implementations.
        Property::Property(const Property & property) :
            m_dataType(property.m_dataType)
        {
            switch (property.m_dataType)
            {
            case Property::DataType::Boolean:       m_valueBoolean = property.m_valueBoolean; break;
            case Property::DataType::BorderStyle:   m_valueBorderStyle = property.m_valueBorderStyle; break;
            case Property::DataType::Float:         m_valueFloat = property.m_valueFloat; break;
            case Property::DataType::Integer:       m_valueInteger = property.m_valueInteger; break;
            case Property::DataType::Overflow:      m_valueOverflow = property.m_valueOverflow; break;
            case Property::DataType::String:        m_valueString = new std::string(*property.m_valueString); break;
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
        Property::Property(const BorderStyle value) :
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
        Property::Property(const LinearGradient & value) :
            m_dataType(DataType::LinearGradient),
            m_valueLinearGradient(new LinearGradient(value))
        { }
        Property::Property(const Overflow value) :
            m_dataType(DataType::Overflow),
            m_valueOverflow(value)
        { }
        Property::Property(const Size & value) :
            m_dataType(DataType::Size),
            m_valueSize(value)
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
        Property::Property(const HorizontalAlign value) :
            m_dataType(DataType::HorizontalAlign),
            m_valueHorizontalAlign(value)
        { }
        Property::Property(const VerticalAlign value) :
            m_dataType(DataType::VerticalAlign),
            m_valueVerticalAlign(value)
        { }

        Property::~Property()
        {
            deallocate();
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

        LinearGradient Property::getLinearGradient() const
        {
            if (m_dataType != DataType::LinearGradient)
            {
                return LinearGradient();
            }
            return *m_valueLinearGradient;
        }

        Property::Overflow Property::getOverflow() const
        {
            return m_valueOverflow;
        }

        const Size & Property::getSize() const
        {
            return m_valueSize;
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

        Property::HorizontalAlign Property::getHorizontalAlign() const
        {
            return m_valueHorizontalAlign;
        }
        Property::VerticalAlign Property::getVerticalAlign() const
        {
            return m_valueVerticalAlign;
        }

        void Property::setBool(const bool value)
        {
            deallocate();
            m_dataType = DataType::Boolean;
            m_valueBoolean = value;  
        }

        void Property::setBorderStyle(const BorderStyle value)
        {
            deallocate();
            m_dataType = DataType::BorderStyle;
            m_valueBorderStyle = value;           
        }

        void Property::setFloat(const float value)
        {
            deallocate();
            m_dataType = DataType::Float;
            m_valueFloat = value;           
        }

        void Property::setInteger(const int value)
        {
            deallocate();
            m_dataType = DataType::Integer;
            m_valueInteger = value;
        }

        void Property::setLinearGradient(const LinearGradient & value)
        {
            deallocate();
            m_dataType = DataType::LinearGradient;
            m_valueLinearGradient = new LinearGradient(value);
        }

        void Property::setOverflow(const Overflow value)
        {
            deallocate();
            m_dataType = DataType::Overflow;
            m_valueOverflow = value;   
        }

        void Property::setSize(const Size & value)
        {
            deallocate();
            m_dataType = DataType::Size;
            m_valueSize = value;
        }

        void Property::setString(const std::string & value)
        {
            deallocate();
            m_dataType = DataType::String;
            m_valueString = new std::string(value);
        }

        void Property::setVector2f(const Vector2f & value)
        {
            deallocate();
            m_dataType = DataType::Vector2f;
            m_valueVector2f = value;
        }

        void Property::setVector3f(const Vector3f & value)
        {
            deallocate();
            m_dataType = DataType::Vector3f;
            m_valueVector3f = value;
        }

        void Property::setVector4f(const Vector4f & value)
        {
            deallocate();
            m_dataType = DataType::Vector4f;
            m_valueVector4f = value;
        }

        void Property::setHorizontalAlign(const HorizontalAlign value)
        {
            deallocate();
            m_dataType = DataType::HorizontalAlign;
            m_valueHorizontalAlign = value;
        }
        void Property::setVerticalAlign(const VerticalAlign value)
        {
            deallocate();
            m_dataType = DataType::VerticalAlign;
            m_valueVerticalAlign = value;         
        }

        Property & Property::operator = (const bool value)
        {
            deallocate();
            m_dataType = DataType::Boolean;
            m_valueBoolean = value;
            return *this;
        }

        Property & Property::operator = (const BorderStyle value)
        {
            deallocate();
            m_dataType = DataType::BorderStyle;
            m_valueBorderStyle = value;
            return *this;
        }

        Property & Property::operator = (const float value)
        {
            deallocate();
            m_dataType = DataType::Float;
            m_valueFloat = value;
            return *this;
        }

        Property & Property::operator = (const int value)
        {
            deallocate();
            m_dataType = DataType::Integer;
            m_valueInteger = value;
            return *this;
        }

        Property & Property::operator = (const LinearGradient & value)
        {
            deallocate();
            m_dataType = DataType::LinearGradient;
            m_valueLinearGradient = new LinearGradient(value);
            return *this;
        }

        Property & Property::operator = (const Overflow value)
        {
            deallocate();
            m_dataType = DataType::Overflow;
            m_valueOverflow = value;
            return *this;
        }

        Property & Property::operator = (const std::string & value)
        {
            deallocate();
            m_dataType = DataType::String;
            m_valueString = new std::string(value);
            return *this;
        }

        Property & Property::operator = (const Vector2f & value)
        {
            deallocate();
            m_dataType = DataType::Vector2f;
            m_valueVector2f = value;
            return *this;
        }

        Property & Property::operator = (const Vector3f & value)
        {
            deallocate();
            m_dataType = DataType::Vector3f;
            m_valueVector3f = value;
            return *this;
        }

        Property & Property::operator = (const Vector4f & value)
        {
            deallocate();
            m_dataType = DataType::Vector4f;
            m_valueVector4f = value;
            return *this;
        }

        Property & Property::operator = (const HorizontalAlign value)
        {
            deallocate();
            m_dataType = DataType::HorizontalAlign;
            m_valueHorizontalAlign = value;
            return *this;
        }
        Property & Property::operator = (const VerticalAlign value)
        {
            deallocate();
            m_dataType = DataType::VerticalAlign;
            m_valueVerticalAlign = value;
            return *this;
        }

        void Property::deallocate()
        {
            switch (m_dataType)
            {
                case DataType::String: delete m_valueString; break;
                case DataType::LinearGradient: delete m_valueLinearGradient; break;
                default: break;
            }
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
                    { "checkbox", DefaultStyles::checkbox },
                    { "checkbox:checked", DefaultStyles::checkboxChecked },
                    { "checkbox:checkedHover", DefaultStyles::checkboxCheckedHover },
                    { "checkbox:checkedDisabled", DefaultStyles::checkboxCheckedDisabled },
                    { "checkbox:hover", DefaultStyles::checkboxHover },
                    { "checkbox:disabled", DefaultStyles::checkboxDisabled },
                    { "horizontal-grid", DefaultStyles::horizontalGrid },
                    { "horizontal-grid-slot", DefaultStyles::horizontalGridSlot },
                    { "label", DefaultStyles::label },
                    { "plane",  DefaultStyles::plane },
                    { "text-box", DefaultStyles::textBox },
                    { "text-box-text", DefaultStyles::textBoxText },
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


        // Parent style implementations.
        ParentStyle::ParentStyle(ParentStyle * parent) :
            m_parent(parent)
        { }

        /*ParentStyle::ParentStyle(const std::shared_ptr<Selector> & selector, ParentStyle * parent) :
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
                    m_padding = { padding->getVector2f(), padding->getVector2f() };
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
        }*/

        Vector4f ParentStyle::getPadding() const
        {
            return m_padding.has_value() ? m_padding.value() : (m_parent ? m_parent->getPadding() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }
        Vector2f ParentStyle::getPaddingLow() const
        {
            return m_padding.has_value() ? Vector2f{ m_padding.value().x, m_padding.value().y } : (m_parent ? m_parent->getPaddingLow() : Vector2f{ 0.0f, 0.0f });
        }
        Vector2f ParentStyle::getPaddingHigh() const
        {
            return m_padding.has_value() ? Vector2f{ m_padding.value().z, m_padding.value().w } : (m_parent ? m_parent->getPaddingHigh() : Vector2f{ 0.0f, 0.0f });
        }

        void ParentStyle::setPadding(const Vector4f & padding)
        {
            m_padding = padding;
        }
        void ParentStyle::setPadding(const Vector2f & padding)
        {
            m_padding = { padding.x, padding.y, padding.x, padding.y };
        }
        void ParentStyle::setPadding(const float & padding)
        {
            m_padding = { padding, padding, padding, padding };
        }
        void ParentStyle::setPaddingLow(const Vector2f & paddingLow)
        {
            if (!m_padding.has_value())
            {
                m_padding = { 0.0f, 0.0f, 0.0f, 0.0f };
            }

            m_padding.value().x = paddingLow.x;
            m_padding.value().y = paddingLow.y;
        }
        void ParentStyle::setPaddingHigh(const Vector2f & paddingHigh)
        {
            if (!m_padding.has_value())
            {
                m_padding = { 0.0f, 0.0f, 0.0f, 0.0f };
            }

            m_padding.value().z = paddingHigh.x;
            m_padding.value().w = paddingHigh.y;
        }

        void ParentStyle::updateEmptyProperties(const std::shared_ptr<Selector> & selector)
        {
            if (!selector)
            {
                return;
            }

            if(!m_padding.has_value())
            {
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
                            m_padding = { padding->getVector2f(), padding->getVector2f() };
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
        }


        // Align style implementations.
        AlignStyle::AlignStyle(AlignStyle * parent) :
            m_parent(parent)
        {}

        /*AlignStyle::AlignStyle(const std::shared_ptr<Selector> & selector, AlignStyle * parent) :
            m_parent(parent)
        {
            if (!selector)
            {
                return;
            }

            auto vertAlign = selector->getProperty("vertical-align");
            if (!vertAlign)
            {
                vertAlign = selector->getProperty("vert-align");
            }
            if (vertAlign)
            {
                switch (vertAlign->getDataType())
                {
                case Property::DataType::VerticalAlign:
                {
                    m_verticalAlign = vertAlign->getVerticalAlign();
                }
                break;
                default: break;
                }
            }

            auto horiAlign = selector->getProperty("horizontal-align");
            if (!horiAlign)
            {
                horiAlign = selector->getProperty("hori-align");
            }
            if (horiAlign)
            {
                switch (horiAlign->getDataType())
                {
                case Property::DataType::HorizontalAlign:
                {
                    m_horizontalAlign = horiAlign->getHorizontalAlign();
                }
                break;
                default: break;
                }
            }
        }*/

        Property::HorizontalAlign AlignStyle::getHorizontalAlign() const
        {
            return m_horizontalAlign.has_value() ? m_horizontalAlign.value() : (m_parent ? m_parent->getHorizontalAlign() : Property::HorizontalAlign::Left);
        }
        Property::VerticalAlign AlignStyle::getVerticalAlign() const
        {
            return m_verticalAlign.has_value() ? m_verticalAlign.value() : (m_parent ? m_parent->getVerticalAlign() : Property::VerticalAlign::Top);
        }

        void AlignStyle::setHorizontalAlign(const Property::HorizontalAlign horizontalAlign)
        {
            m_horizontalAlign = horizontalAlign;
        }
        void AlignStyle::setVerticalAlign(const Property::VerticalAlign verticalAlign)
        {
            m_verticalAlign = verticalAlign;
        }

        void AlignStyle::updateEmptyProperties(const std::shared_ptr<Selector> & selector)
        {
            if (!selector)
            {
                return;
            }

            if (!m_verticalAlign.has_value())
            {
                auto vertAlign = selector->getProperty("vertical-align");
                if (!vertAlign)
                {
                    vertAlign = selector->getProperty("vert-align");
                }
                if (vertAlign)
                {
                    switch (vertAlign->getDataType())
                    {
                        case Property::DataType::VerticalAlign:
                        {
                            m_verticalAlign = vertAlign->getVerticalAlign();
                        }
                        break;
                        default: break;
                    }
                }
            }

            if (!m_horizontalAlign.has_value())
            {
                auto horiAlign = selector->getProperty("horizontal-align");
                if (!horiAlign)
                {
                    horiAlign = selector->getProperty("hori-align");
                }
                if (horiAlign)
                {
                    switch (horiAlign->getDataType())
                    {
                    case Property::DataType::HorizontalAlign:
                    {
                        m_horizontalAlign = horiAlign->getHorizontalAlign();
                    }
                    break;
                    default: break;
                    }
                }
            }
        }


        // Rect style implementations.
        RectStyle::RectStyle(RectStyle * parent, Control * control) :
            AlignStyle(parent),
            m_parent(parent)
        {
            if (control)
            {
                onChange.connectAnonymously([control]()
                {
                    control->resize();
                });
            }
        }

        /*RectStyle::RectStyle(const std::shared_ptr<Selector> & selector, RectStyle * parent) :
            AlignStyle(selector, parent),
            m_parent(parent)
        {
            if (!selector)
            {
                return;
            }

            auto margin = selector->getProperty("margin");
            if (margin)
            {
                switch (margin->getDataType())
                {
                case Property::DataType::Float:
                {
                    auto val = margin->getFloat();
                    m_margin = { val, val, val, val };
                }
                break;
                case Property::DataType::Vector2f:
                {
                    m_margin = { margin->getVector2f(), margin->getVector2f() };
                }
                break;
                case Property::DataType::Vector4f:
                {
                    m_margin = margin->getVector4f();
                }
                break;
                default: break;
                }
            }
            auto overflow = selector->getProperty("overflow");
            if (overflow && overflow->getDataType() == Property::DataType::Overflow)
            {
                m_overflow = overflow->getOverflow();
            }
            auto position = selector->getProperty("position");
            if (position && position->getDataType() == Property::DataType::Vector2f)
            {
                m_position = position->getVector2f();
            }

            auto size = selector->getProperty("size");
            if (size)
            {
                switch (size->getDataType())
                {
                    case Property::DataType::Size:
                    {
                        m_size = size->getSize();
                    }
                    break;
                    case Property::DataType::Vector2f:
                    {
                        m_size = size->getVector2f();
                    }
                    break;
                    default: break;
                }
            }
        }*/
        
        Property::Overflow RectStyle::getOverflow() const
        {
            return m_overflow.has_value() ? m_overflow.value() : (m_parent ? m_parent->getOverflow() : Property::Overflow::Hidden);
        }
        Vector4f RectStyle::getMargin() const
        {
            return m_margin.has_value() ? m_margin.value() : (m_parent ? m_parent->getMargin() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }
        Vector2f RectStyle::getMarginLow() const
        {
            return m_margin.has_value() ? Vector2f{ m_margin.value().x, m_margin.value().y } : (m_parent ? m_parent->getMarginLow() : Vector2f{ 0.0f, 0.0f });
        }
        Vector2f RectStyle::getMarginHigh() const
        {
            return m_margin.has_value() ? Vector2f{ m_margin.value().z, m_margin.value().w } : (m_parent ? m_parent->getMarginHigh() : Vector2f{ 0.0f, 0.0f });
        }
        const Vector2f RectStyle::getPosition() const
        {
            return m_position.has_value() ? m_position.value() : (m_parent ? m_parent->getPosition() : Vector2f{ 0.0f, 0.0f });
        }
        const Size RectStyle::getSize() const
        {
            return m_size.has_value() ? m_size.value() : (m_parent ? m_parent->getSize() : Size{ 0.0f, 0.0f });
        }

        void RectStyle::setMargin(const Vector4f & margin)
        {
            m_margin = { margin.x, margin.y, margin.x, margin.y };
        }
        void RectStyle::setMargin(const Vector2f & margin)
        {
            m_margin = { margin.x, margin.y, margin.x, margin.y };
        }
        void RectStyle::setMargin(const float margin)
        {
            m_margin = { margin, margin, margin, margin };
        }
        void RectStyle::setMarginLow(const Vector2f & marginLow)
        {
            if (!m_margin.has_value())
            {
                m_margin = { 0.0f, 0.0f, 0.0f, 0.0f };
            }

            m_margin.value().x = marginLow.x;
            m_margin.value().y = marginLow.y;
        }
        void RectStyle::setMarginHigh(const Vector2f & marginHigh)
        {
            if (!m_margin.has_value())
            {
                m_margin = { 0.0f, 0.0f, 0.0f, 0.0f };
            }

            m_margin.value().z = marginHigh.x;
            m_margin.value().w = marginHigh.y;
        }
        void RectStyle::setOverflow(const Property::Overflow overflow)
        {
            m_overflow = overflow;
        }
        void RectStyle::setPosition(const Vector2f & position)
        {
            m_position = position;

            if (position != m_position)
            {
                m_position = position;
                onChange();
            }
        }
        void RectStyle::setSize(const Size & size)
        {
            if (size != m_size)
            {
                m_size = size;
                onChange();
            }
        }

        void RectStyle::updateEmptyProperties(const std::shared_ptr<Selector> & selector)
        {
            if (!selector)
            {
                return;
            }

            AlignStyle::updateEmptyProperties(selector);

            if (!m_margin.has_value())
            {
                auto margin = selector->getProperty("margin");
                if (margin)
                {
                    switch (margin->getDataType())
                    {
                    case Property::DataType::Float:
                    {
                        auto val = margin->getFloat();
                        m_margin = { val, val, val, val };
                    }
                    break;
                    case Property::DataType::Vector2f:
                    {
                        m_margin = { margin->getVector2f(), margin->getVector2f() };
                    }
                    break;
                    case Property::DataType::Vector4f:
                    {
                        m_margin = margin->getVector4f();
                    }
                    break;
                    default: break;
                    }
                }
            }

            if (!m_overflow.has_value())
            {
                auto overflow = selector->getProperty("overflow");
                if (overflow && overflow->getDataType() == Property::DataType::Overflow)
                {
                    m_overflow = overflow->getOverflow();
                }
            }

            if (!m_position.has_value())
            {
                auto position = selector->getProperty("position");
                if (position && position->getDataType() == Property::DataType::Vector2f)
                {
                    m_position = position->getVector2f();
                }
            }

            if (!m_size.has_value())
            {
                auto size = selector->getProperty("size");
                if (size)
                {
                    switch (size->getDataType())
                    {
                        case Property::DataType::Size:
                        {
                            m_size = size->getSize();
                        }
                        break;
                        case Property::DataType::Vector2f:
                        {
                            m_size = size->getVector2f();
                        }
                        break;
                        default: break;
                    }
                }
            }
        }


        // Border style implementations.
        BorderStyle::BorderStyle(BorderStyle * parent) :
            m_parent(parent)
        { }

        /*BorderStyle::BorderStyle(const std::shared_ptr<Selector> & selector, BorderStyle * parent) :
            m_parent(parent)
        {
            if (!selector)
            {
                return;
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
        }*/

        const Vector4f BorderStyle::getBorderColor() const
        {
            return m_borderColor.has_value() ? m_borderColor.value() : (m_parent ? m_parent->getBorderColor() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }
        Property::BorderStyle BorderStyle::getBorderStyle() const
        {
            return m_borderStyle.has_value() ? m_borderStyle.value() : (m_parent ? m_parent->getBorderStyle() : Property::BorderStyle::None);
        }
        float BorderStyle::getBorderWidth() const
        {
            return m_borderWidth.has_value() ? m_borderWidth.value() : (m_parent ? m_parent->getBorderWidth() : 0.0f);
        }

        void BorderStyle::setBorderColor(const Vector4f & color)
        {
            m_borderColor = color;
        }
        void BorderStyle::setBorderStyle(const Property::BorderStyle style)
        {
            m_borderStyle = style;
        }
        void BorderStyle::setBorderWidth(const float width)
        {
            m_borderWidth = width;
        }

        void BorderStyle::updateEmptyProperties(const std::shared_ptr<Selector> & selector)
        {
            if (!selector)
            {
                return;
            }

            if (!m_borderColor.has_value())
            {
                auto borderColor = selector->getProperty("border-color");
                if (borderColor && borderColor->getDataType() == Property::DataType::Vector4f)
                {
                    m_borderColor = borderColor->getVector4f();
                }
            }
            if (!m_borderStyle.has_value())
            {
                auto borderStyle = selector->getProperty("border-style");
                if (borderStyle && borderStyle->getDataType() == Property::DataType::BorderStyle)
                {
                    m_borderStyle = borderStyle->getBorderStyle();
                }
            }
            if (!m_borderWidth.has_value())
            {
                auto borderWidth = selector->getProperty("border-width");
                if (borderWidth && borderWidth->getDataType() == Property::DataType::Float)
                {
                    m_borderWidth = borderWidth->getFloat();
                }
            }          
        }


        // Paint rect style
        PaintRectStyle::PaintRectStyle(PaintRectStyle * parent, Control * control) :
            RectStyle(parent, control),
            BorderStyle(parent),
            m_parent(parent)
        { }

       /* PaintRectStyle::PaintRectStyle(const std::shared_ptr<Selector> & selector, PaintRectStyle * parent) :
            RectStyle(selector, parent),
            BorderStyle(selector, parent),
            m_parent(parent)
        {
            if (!selector)
            {
                return;
            }

            auto bgColor = selector->getProperty("background-color");
            if (bgColor)
            {
                switch (bgColor->getDataType())
                {
                case Property::DataType::Float:
                {
                    auto val = bgColor->getFloat();
                    m_backgroundColor = { val, val, val, 1.0f };
                }
                break;
                case Property::DataType::Vector3f:
                {
                    m_backgroundColor = { bgColor->getVector3f(), 1.0f };
                }
                break;
                case Property::DataType::Vector4f:
                {
                    m_backgroundColor = bgColor->getVector4f();
                }
                break;
                default: break;
                }
            }
        }*/

        const Vector4f PaintRectStyle::getBackgroundColor() const
        {
            return m_backgroundColor.has_value() ? m_backgroundColor.value() : (m_parent ? m_parent->getBackgroundColor() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }

        void PaintRectStyle::setBackgroundColor(const Vector4f & color)
        {
            m_backgroundColor = color;
        }

        void PaintRectStyle::updateEmptyProperties(const std::shared_ptr<Selector> & selector)
        {
            if (!selector)
            {
                return;
            }

            Style::RectStyle::updateEmptyProperties(selector);
            Style::BorderStyle::updateEmptyProperties(selector);

            if (!m_backgroundColor.has_value())
            {
                auto bgColor = selector->getProperty("background-color");
                if (bgColor)
                {
                    switch (bgColor->getDataType())
                    {
                        case Property::DataType::Float:
                        {
                            auto val = bgColor->getFloat();
                            m_backgroundColor = { val, val, val, 1.0f };
                        }
                        break;
                        case Property::DataType::Vector3f:
                        {
                            m_backgroundColor = { bgColor->getVector3f(), 1.0f };
                        }
                        break;
                        case Property::DataType::Vector4f:
                        {
                            m_backgroundColor = bgColor->getVector4f();
                        }
                        break;
                        default: break;
                    }
                }
            }
        }


        // Font style implementations.
        FontStyle::FontStyle(FontStyle * parent) :
            m_parent(parent)
        { }

        /*FontStyle::FontStyle(const std::shared_ptr<Selector> & selector, FontStyle * parent) :
            m_parent(parent)
        {
            if (!selector)
            {
                return;
            }

            auto fontBgColor = selector->getProperty("font-background-color");
            if (fontBgColor)
            {
                switch (fontBgColor->getDataType())
                {
                case Property::DataType::Integer:
                {
                    float val = static_cast<float>(fontBgColor->getInteger());
                    m_fontBackgroundColor = { val, val, val, 1.0f };
                }
                break;
                case Property::DataType::Float:
                {
                    float val = static_cast<float>(fontBgColor->getFloat());
                    m_fontBackgroundColor = { val, val, val, 1.0f };
                }
                case Property::DataType::Vector3f:
                {
                    m_fontBackgroundColor = { fontBgColor->getVector3f(), 1.0f };
                }
                break;
                case Property::DataType::Vector4f:
                {
                    m_fontBackgroundColor = fontBgColor->getVector4f();
                }
                break;
                default: break;
                }
            }
            auto fontColor = selector->getProperty("font-color");
            if (fontColor)
            {
                switch (fontColor->getDataType())
                {
                case Property::DataType::Integer:
                {
                    float val = static_cast<float>(fontColor->getInteger());
                    m_fontColor = { val, val, val, 1.0f };
                }
                break;
                case Property::DataType::Float:
                {
                    float val = static_cast<float>(fontColor->getFloat());
                    m_fontColor = { val, val, val, 1.0f };
                }
                case Property::DataType::Vector3f:
                {
                    m_fontColor = { fontColor->getVector3f(), 1.0f };
                }
                break;
                case Property::DataType::Vector4f:
                {
                    m_fontColor = fontColor->getVector4f();
                }
                break;
                default: break;
                }
            }
            auto fontFamily = selector->getProperty("font-family");
            if (fontFamily && fontFamily->getDataType() == Property::DataType::String)
            {
                m_fontFamily = fontFamily->getString();
            }
            auto fontSize = selector->getProperty("font-size");
            if (fontSize && fontSize->getDataType() == Property::DataType::Integer)
            {
                m_fontSize = fontSize->getInteger();
            }
            auto overflow = selector->getProperty("overflow");
            if (overflow && overflow->getDataType() == Property::DataType::Overflow)
            {
                m_overflow = overflow->getOverflow();
            }
        }*/

        const Vector4f FontStyle::getFontBackgroundColor() const
        {
            return m_fontBackgroundColor.has_value() ? m_fontBackgroundColor.value() : (m_parent ? m_parent->getFontBackgroundColor() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }
        const Vector4f FontStyle::getFontColor() const
        {
            return m_fontColor.has_value() ? m_fontColor.value() : (m_parent ? m_parent->getFontColor() : Vector4f{ 0.0f, 0.0f, 0.0f, 0.0f });
        }
        const std::string FontStyle::getFontFamily() const
        {
            return m_fontFamily.has_value() ? m_fontFamily.value() : (m_parent ? m_parent->getFontFamily() : "");
        }
        const int32_t FontStyle::getFontSize() const
        {
            return m_fontSize.has_value() ? m_fontSize.value() : (m_parent ? m_parent->getFontSize() : 0);
        }  

        void FontStyle::setFontBackgroundColor(const Vector4f & color)
        {
            m_fontBackgroundColor = color;
        }
        void FontStyle::setFontColor(const Vector4f & color)
        {
            m_fontColor = color;
        }
        void FontStyle::setFontFamily(const std::string & family)
        {
            m_fontFamily = family;
        }
        void FontStyle::setFontSize(const int32_t size)
        {
            m_fontSize = size;
        }

        void FontStyle::updateEmptyProperties(const std::shared_ptr<Selector> & selector)
        {
            if (!selector)
            {
                return;
            }

            if (!m_fontBackgroundColor.has_value())
            {
                auto fontBgColor = selector->getProperty("font-background-color");
                if (fontBgColor)
                {
                    switch (fontBgColor->getDataType())
                    {
                    case Property::DataType::Integer:
                    {
                        float val = static_cast<float>(fontBgColor->getInteger());
                        m_fontBackgroundColor = { val, val, val, 1.0f };
                    }
                    break;
                    case Property::DataType::Float:
                    {
                        float val = static_cast<float>(fontBgColor->getFloat());
                        m_fontBackgroundColor = { val, val, val, 1.0f };
                    }
                    case Property::DataType::Vector3f:
                    {
                        m_fontBackgroundColor = { fontBgColor->getVector3f(), 1.0f };
                    }
                    break;
                    case Property::DataType::Vector4f:
                    {
                        m_fontBackgroundColor = fontBgColor->getVector4f();
                    }
                    break;
                    default: break;
                    }
                }
            }

            if (!m_fontColor.has_value())
            {
                auto fontColor = selector->getProperty("font-color");
                if (fontColor)
                {
                    switch (fontColor->getDataType())
                    {
                    case Property::DataType::Integer:
                    {
                        float val = static_cast<float>(fontColor->getInteger());
                        m_fontColor = { val, val, val, 1.0f };
                    }
                    break;
                    case Property::DataType::Float:
                    {
                        float val = static_cast<float>(fontColor->getFloat());
                        m_fontColor = { val, val, val, 1.0f };
                    }
                    case Property::DataType::Vector3f:
                    {
                        m_fontColor = { fontColor->getVector3f(), 1.0f };
                    }
                    break;
                    case Property::DataType::Vector4f:
                    {
                        m_fontColor = fontColor->getVector4f();
                    }
                    break;
                    default: break;
                    }
                }
            }

            if (!m_fontFamily.has_value())
            {
                auto fontFamily = selector->getProperty("font-family");
                if (fontFamily && fontFamily->getDataType() == Property::DataType::String)
                {
                    m_fontFamily = fontFamily->getString();
                }
            }

            if (!m_fontSize.has_value())
            {
                auto fontSize = selector->getProperty("font-size");
                if (fontSize && fontSize->getDataType() == Property::DataType::Integer)
                {
                    m_fontSize = fontSize->getInteger();
                }
            }

            if (!m_overflow.has_value())
            {
                auto overflow = selector->getProperty("overflow");
                if (overflow && overflow->getDataType() == Property::DataType::Overflow)
                {
                    m_overflow = overflow->getOverflow();
                }
            }
        }


        // Parent rect style.
        Bounds2f ParentRectStyle::calcStyledBounds(const ParentRectStyle & style, const Bounds2f & canvasBounds, const float scale)
        {
            const Vector2f newPos = style.getPosition() * scale;
            const Vector2f newSize = style.getSize() * scale;

            const bool clamp = style.getOverflow() == Style::Property::Overflow::Hidden;
            Bounds2f bounds = { canvasBounds.position + newPos, newSize };

            if (clamp)
            {
                bounds.position = Vector2f::clamp(bounds.position, canvasBounds.position,
                                                  Vector2f::max(canvasBounds.position, canvasBounds.position + canvasBounds.size));
            }

            Vector2f lower = bounds.position;
            Vector2f higherCanvas = canvasBounds.position + canvasBounds.size;
            Vector2f higherThis = bounds.position + Vector2f::max({ 0.0f, 0.0f }, newSize);

            if ((clamp && higherThis.x > higherCanvas.x) || newSize.x <= 0.0f)
            {
                bounds.size.x = higherCanvas.x - lower.x;
            }
            if ((clamp && higherThis.y > higherCanvas.y) || newSize.y <= 0.0f)
            {
                bounds.size.y = higherCanvas.y - lower.y;
            }

            bounds.size.x = std::max(0.0f, bounds.size.x);
            bounds.size.y = std::max(0.0f, bounds.size.y);

            bounds.position.x = std::floor(bounds.position.x);
            bounds.position.y = std::floor(bounds.position.y);
            bounds.size.x = std::floor(bounds.size.x);
            bounds.size.y = std::floor(bounds.size.y);

            return bounds;
        }

        ParentRectStyle::ParentRectStyle(ParentRectStyle * parent, Control * control) :
            Style::RectStyle(parent, control),
            Style::ParentStyle(parent)
        { }

        /*ParentRectStyle::ParentRectStyle(const std::shared_ptr<Style::Selector> & selector, ParentRectStyle * parent) :
            Style::RectStyle(selector, parent),
            Style::ParentStyle(selector, parent)
        { }*/

        void ParentRectStyle::updateEmptyProperties(const std::shared_ptr<Selector> & selector)
        {
            Style::RectStyle::updateEmptyProperties(selector);
            Style::ParentStyle::updateEmptyProperties(selector);
        }


        // Parent paint rect style.
        Bounds2f ParentPaintRectStyle::calcStyledBounds(const ParentPaintRectStyle & style, const Bounds2f & canvasBounds, const float scale)
        {   
            const Vector2f newPos = style.getPosition() * scale;
            const Vector2f newSize = style.getSize() * scale;

            const bool clamp = style.getOverflow() == Style::Property::Overflow::Hidden;
            Bounds2f bounds = { canvasBounds.position + newPos, newSize };

            if (clamp)
            {
                bounds.position = Vector2f::clamp(bounds.position, canvasBounds.position,
                                                  Vector2f::max(canvasBounds.position, canvasBounds.position + canvasBounds.size));
            }

            Vector2f lower = bounds.position;
            Vector2f higherCanvas = canvasBounds.position + canvasBounds.size;
            Vector2f higherThis = bounds.position + Vector2f::max({ 0.0f, 0.0f }, newSize);

            if ((clamp && higherThis.x > higherCanvas.x) || newSize.x <= 0.0f)
            {
                bounds.size.x = higherCanvas.x - lower.x;
            }
            if ((clamp && higherThis.y > higherCanvas.y) || newSize.y <= 0.0f)
            {
                bounds.size.y = higherCanvas.y - lower.y;
            }

            bounds.size.x = std::max(0.0f, bounds.size.x);
            bounds.size.y = std::max(0.0f, bounds.size.y);

            bounds.position.x = std::floor(bounds.position.x);
            bounds.position.y = std::floor(bounds.position.y);
            bounds.size.x = std::floor(bounds.size.x);
            bounds.size.y = std::floor(bounds.size.y);

            return bounds;
        }

        ParentPaintRectStyle::ParentPaintRectStyle(ParentPaintRectStyle * parent, Control * control) :
            Style::PaintRectStyle(parent, control),
            Style::ParentStyle(parent)
        { }
        /*ParentPaintRectStyle::ParentPaintRectStyle(const std::shared_ptr<Style::Selector> & selector, ParentPaintRectStyle * parent) :
            Style::PaintRectStyle(selector, parent),
            Style::ParentStyle(selector, parent)
        { }*/

        void ParentPaintRectStyle::updateEmptyProperties(const std::shared_ptr<Selector> & selector)
        {
            Style::PaintRectStyle::updateEmptyProperties(selector);
            Style::ParentStyle::updateEmptyProperties(selector);
        }

    }

}