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
#include <optional>

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

            enum class DataType : uint32_t
            {
                Boolean,
                BorderStyle,
                Float,
                Integer,
                Overflow,
                String,
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
            Property(const int value);
            Property(const Overflow value);
            Property(const std::string & value);
            Property(const Vector2f & value);
            Property(const Vector3f & value);
            Property(const Vector4f & value);

            ~Property();

            DataType getDataType() const;

            bool getBool() const;
            BorderStyle getBorderStyle() const;
            float getFloat() const;
            int getInteger() const;
            Overflow getOverflow() const;
            std::string getString() const;
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
                bool            m_valueBoolean;
                BorderStyle     m_valueBorderStyle;
                float           m_valueFloat;
                int             m_valueInteger;
                Overflow        m_valueOverflow;
                std::string *   m_valueString;
                Vector2f        m_valueVector2f;
                Vector3f        m_valueVector3f;
                Vector4f        m_valueVector4f;
            };

            DataType    m_dataType;

        };


        class GUISE_API Selector
        {

        public:

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

            PaddingStyle(PaddingStyle * parent = nullptr);
            PaddingStyle(const std::shared_ptr<Selector> & selector, PaddingStyle * parent = nullptr);

            Vector4f getPadding() const;
            Vector2f getPaddingLow() const;
            Vector2f getPaddingHigh() const;

            void setPadding(const Vector4f & padding);
            void setPadding(const Vector2f & padding);
            void setPadding(const float & padding);

        protected:

            PaddingStyle *          m_parent;

            std::optional<Vector4f> m_padding;

        };


        class GUISE_API BoxStyle : public PaddingStyle
        {

        public:

            BoxStyle(BoxStyle * parent = nullptr);
            BoxStyle(const std::shared_ptr<Selector> & selector, BoxStyle * parent = nullptr);

            const Vector4f getBackgroundColor() const;
            const Vector4f getBorderColor() const;
            Property::BorderStyle getBorderStyle() const;
            float getBorderWidth() const;
            const Vector2f getPosition() const;
            const Vector2f getSize() const;
            Property::Overflow getOverflow() const;

            void setBackgroundColor(const Vector4f & color);
            void setBorderColor(const Vector4f & color);
            void setBorderStyle(const Property::BorderStyle borderStyle);
            void setBorderWidth(const float width);
            void setPosition(const Vector2f & position);
            void setSize(const Vector2f & size);
            void setOverflow(const Property::Overflow overflow);

        protected:

            BoxStyle *                                  m_parent;

            std::optional<Vector4f>                     m_backgroundColor;
            std::optional<Vector4f>                     m_borderColor;
            std::optional<float>                        m_borderWidth;
            std::optional<Style::Property::BorderStyle> m_borderStyle;
            std::optional<Vector2f>                     m_position;
            std::optional<Vector2f>                     m_size;
            std::optional<Vector4f>                     m_padding;
            std::optional<Style::Property::Overflow>    m_overflow;

        };


        class GUISE_API LabelStyle
        {

        public:

            LabelStyle() :
                m_fontFamily("Arial"),
                m_fontSize(12),
                m_overflow(Style::Property::Overflow::hidden)
            { }

            LabelStyle(const std::shared_ptr<Selector> & selector) :
                m_fontFamily("Arial"),
                m_fontSize(12),
                m_overflow(Style::Property::Overflow::hidden)
            {
                if (!selector)
                {
                    return;
                }

                auto position = selector->getProperty("font-family");
                if (position && position->getDataType() == Property::DataType::String)
                {
                    m_fontFamily = position->getString();
                }
                auto size = selector->getProperty("font-size");
                if (size && size->getDataType() == Property::DataType::Integer)
                {
                    m_fontSize = size->getInteger();
                }
                auto overflow = selector->getProperty("overflow");
                if (overflow && overflow->getDataType() == Property::DataType::Overflow)
                {
                    m_overflow = overflow->getOverflow();
                }
            }

           
            const std::string & getFontFamily() const
            {
                return m_fontFamily;
            }
            const int getFontSize() const
            {
                return m_fontSize;
            }
            Property::Overflow getOverflow() const
            {
                return m_overflow;
            }

            void setFontFamily(const std::string & family)
            {
                m_fontFamily = family;
            }
            void setFontSize(const int size)
            {
                m_fontSize = size;
            }
            void setOverflow(const Property::Overflow overflow)
            {
                m_overflow = overflow;
            }

        protected:

            std::string m_fontFamily;
            int m_fontSize;
            Property::Overflow m_overflow;

        };
        
    }

}

#endif