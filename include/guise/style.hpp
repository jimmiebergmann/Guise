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

#ifndef GUISE_STYLE_HPP
#define GUISE_STYLE_HPP

#include "guise/build.hpp"
#include <initializer_list>
#include <mutex>
#include <map>
#include <string>
#include <memory>

namespace Guise
{

    namespace Style
    {

        class Selector;
        class Property;

        using Properties = std::map<std::string, std::shared_ptr<Property> >;
        using Selectors = std::map<std::string, std::shared_ptr<Selector> >;

        class GUISE_API Property
        {

        public:

            /*enum class Type
            {
                Unkown,

                BackgroundColor,
                BackgroundImage,
                BorderColor,
                BorderStyle,
                BorderWidth,
                Overflow,
                Padding,
                Position,
                Size
            };*/

            enum class DataType : uint32_t
            {
                Boolean,
                BorderStyle,
                Float,
                Overflow,
                Vector2f,
                Vector3f,
                Vector4f

            };

            enum class BorderStyle : uint32_t
            {
                None,
                Solid
            };

            enum class Overflow : uint32_t
            {
                hidden,
                visible
            };

            
            Property(const Property & property);
            Property(const std::shared_ptr<Property> & property);

            Property(const bool value);
            Property(const BorderStyle value);
            Property(const float value);
            Property(const Overflow value);
            Property(const Vector2f & value);
            Property(const Vector3f & value);
            Property(const Vector4f & value);


            DataType getDataType() const;

            bool getBool() const;
            BorderStyle getBorderStyle() const;
            float getFloat() const;
            Overflow getOverflow() const;
            const Vector2f & getVector2f() const;
            const Vector3f & getVector3f() const;
            const Vector4f & getVector4f() const;

            void setBool(const bool value);
            void setBorderStyle(const BorderStyle value);
            void setFloat(const float value);
            void setOverflow(const Overflow value);
            void setVector2f(const Vector2f & value);
            void setVector3f(const Vector3f & value);
            void setVector4f(const Vector4f & value);

            Property & operator = (const bool value);
            Property & operator = (const BorderStyle value);
            Property & operator = (const float value);
            Property & operator = (const Overflow value);
            Property & operator = (const Vector2f & value);
            Property & operator = (const Vector3f & value);
            Property & operator = (const Vector4f & value);

        private:

            union
            {
                bool        m_valueBoolean;
                BorderStyle m_valueBorderStyle;
                float       m_valueFloat;
                Overflow    m_valueOverflow;
                Vector2f    m_valueVector2f;
                Vector3f    m_valueVector3f;
                Vector4f    m_valueVector4f;
            };

            DataType    m_dataType;

        };


        class GUISE_API Selector
        {

        public:

            /*enum class Type
            {
                Custom,

                Button,
                Canvas,
                Plane,
                Window
            };*/

            struct GUISE_API PropertyNameValue
            {
                PropertyNameValue(const std::string & name, const Property & property);

                const std::string & name;
                const Property & property;

            };

            Selector();
            Selector(const Selector & selector);
            Selector(const std::shared_ptr<Selector> & selector);
            Selector(const std::initializer_list<PropertyNameValue> & properties);

            std::shared_ptr<Property> getProperty(const std::string & name) const;

            Properties & getProperties();
            const Properties & getProperties() const;

        private:

            Properties  m_properties;

        };


        class GUISE_API Sheet
        {

        public:

            /*enum class Entry
            {
                Button,
                Canvas,
                Plane,
                Window
            };*/

            struct GUISE_API SelectorNameValue
            {
                SelectorNameValue(const std::string & name, const Selector & selector);

                const std::string & name;
                const Selector & selector;

            };

            static std::shared_ptr<Sheet> create(const std::initializer_list<SelectorNameValue> & selectors = {});
            static std::shared_ptr<Sheet> createDefault();

            std::shared_ptr<Selector> getSelector(const std::string & name) const;


        private:

            Sheet(const std::initializer_list<SelectorNameValue> & selectors = {});

            Selectors      m_selectors;

        };



        class GUISE_API PaddingStyle
        {

        public:

            PaddingStyle() :
                m_padding(0.0f, 0.0f, 0.0f, 0.0f)
            { }

            PaddingStyle(const std::shared_ptr<Selector> & selector) :
                PaddingStyle()
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
                        m_padding.x = m_padding.y = m_padding.z = m_padding.w = padding->getFloat();
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

            const Vector4f & getPadding() const
            {
                return m_padding;
            }
            const Vector2f getPaddingLow() const
            {
                return { m_padding.x, m_padding.y };
            }
            const Vector2f getPaddingHigh() const
            {
                return { m_padding.z, m_padding.w };
            }

            void setPadding(const Vector4f & padding)
            {
                m_padding = padding;
            }

            void setPadding(const Vector2f & padding)
            {
                m_padding.x = padding.x;
                m_padding.y = padding.y;
                m_padding.z = m_padding.w = 0.0f;
            }

            void setPadding(const float & padding)
            {
                m_padding.x = m_padding.y = m_padding.z = m_padding.w = padding;
            }

        protected:

            Vector4f m_padding;

        };


        class GUISE_API BoxStyle : public PaddingStyle
        {

        public:

            BoxStyle() :
                m_backgroundColor(1.0f, 1.0f, 1.0f, 1.0f),
                m_borderColor(1.0f, 1.0f, 1.0f, 1.0f),
                m_borderWidth(0.0f),
                m_borderStyle(Property::BorderStyle::None),
                m_position(0.0f, 0.0f),
                m_size(0.0f, 0.0f),
                m_overflow(Style::Property::Overflow::hidden)
            { }

            BoxStyle(const std::shared_ptr<Selector> & selector) :
                m_backgroundColor(1.0f, 1.0f, 1.0f, 1.0f),
                m_borderColor(1.0f, 1.0f, 1.0f, 1.0f),
                m_borderWidth(0.0f),
                m_borderStyle(Property::BorderStyle::None),
                m_position(0.0f, 0.0f),
                m_size(0.0f, 0.0f),
                m_overflow(Style::Property::Overflow::hidden),
                PaddingStyle(selector)
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

            const Vector4f & getBackgroundColor() const
            {
                return m_backgroundColor;
            }
            const Vector4f & getBorderColor() const
            {
                return m_borderColor;
            }
            Property::BorderStyle getBorderStyle() const
            {
                return m_borderStyle;
            }
            float getBorderWidth() const
            {
                return m_borderWidth;
            }
            const Vector2f & getPosition() const
            {
                return m_position;
            }
            const Vector2f & getSize() const
            {
                return m_size;
            }
            Property::Overflow getOverflow() const
            {
                return m_overflow;
            }

            void setBackgroundColor(const Vector4f & color)
            {
                m_backgroundColor = color;
            }
            void setBorderColor(const Vector4f & color)
            {
                m_borderColor = color;
            }
            void setBorderStyle(const Style::Property::BorderStyle borderStyle)
            {
                m_borderStyle = borderStyle;
            }
            void setBorderWidth(const float width)
            {
                m_borderWidth = width;
            }
            void setPosition(const Vector2f & position)
            {
                m_position = position;
            }
            void setSize(const Vector2f & size)
            {
                m_size = size;
            }
            void setOverflow(const Style::Property::Overflow overflow)
            {
                m_overflow = overflow;
            }

        protected:

            Vector4f m_backgroundColor;
            Vector4f m_borderColor;
            float m_borderWidth;
            Style::Property::BorderStyle m_borderStyle;
            Vector2f m_position;
            Vector2f m_size;
            Vector4f m_padding;
            Style::Property::Overflow m_overflow;

        };
        
    }

}

#endif