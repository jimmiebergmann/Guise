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
                Boolean = 0,
                BorderStyle,
                Float,
                Integer,
                Overflow,
                String,
                Vector2f,
                Vector3f,
                Vector4f,
                VerticalAlign,
                HorizontalAlign
            };

            enum class BorderStyle : uint32_t
            {
                None,
                Solid
            };

            enum class Overflow : uint32_t
            {
                Hidden = 0,
                Visible
            };

            enum class HorizontalAlign : uint32_t
            {
                Left = 0,
                Center,
                Right
            };

            enum class VerticalAlign : uint32_t
            {
                Top = 0,
                Center,
                Bottom
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
            Property(const HorizontalAlign value);
            Property(const VerticalAlign value);           

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
            HorizontalAlign getHorizontalAlign() const;
            VerticalAlign getVerticalAlign() const;

            void setBool(const bool value);
            void setBorderStyle(const BorderStyle value);
            void setFloat(const float value);
            void setOverflow(const Overflow value);
            void setVector2f(const Vector2f & value);
            void setVector3f(const Vector3f & value);
            void setVector4f(const Vector4f & value);
            void setHorizontalAlign(const HorizontalAlign value);
            void setVerticalAlign(const VerticalAlign value);
            
            Property & operator = (const bool value);
            Property & operator = (const BorderStyle value);
            Property & operator = (const float value);
            Property & operator = (const Overflow value);
            Property & operator = (const Vector2f & value);
            Property & operator = (const Vector3f & value);
            Property & operator = (const Vector4f & value);
            Property & operator = (const HorizontalAlign value);
            Property & operator = (const VerticalAlign value);

        private:

            DataType    m_dataType;

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
                VerticalAlign   m_valueVerticalAlign;
                HorizontalAlign m_valueHorizontalAlign;
            };

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


        class GUISE_API ParentStyle
        {

        public:

            ParentStyle(ParentStyle * parent = nullptr);
            ParentStyle(const std::shared_ptr<Selector> & selector, ParentStyle * parent = nullptr);

            Vector4f getPadding() const;
            Vector2f getPaddingLow() const;
            Vector2f getPaddingHigh() const;

            void setPadding(const Vector4f & padding);
            void setPadding(const Vector2f & padding);
            void setPadding(const float & padding);
            void setPaddingLow(const Vector2f & paddingLow);
            void setPaddingHigh(const Vector2f & paddingHigh);

        protected:

            ParentStyle *          m_parent;

            std::optional<Vector4f> m_padding;

        };


        class GUISE_API AlignStyle
        {

        public:

            AlignStyle(AlignStyle * parent = nullptr);
            AlignStyle(const std::shared_ptr<Selector> & selector, AlignStyle * parent = nullptr);

            Property::HorizontalAlign getHorizontalAlign() const;
            Property::VerticalAlign getVerticalAlign() const;

            void setHorizontalAlign(const Property::HorizontalAlign horizontalAlign);
            void setVerticalAlign(const Property::VerticalAlign verticalAlign);

        protected:

            AlignStyle * m_parent;

            std::optional<Property::HorizontalAlign> m_horizontalAlign;
            std::optional<Property::VerticalAlign> m_verticalAlign;

        };


        class GUISE_API RectStyle : public AlignStyle
        {

        public:

            RectStyle(RectStyle * parent = nullptr);
            RectStyle(const std::shared_ptr<Selector> & selector, RectStyle * parent = nullptr);

            Property::Overflow getOverflow() const;
            Vector4f getMargin() const;
            Vector2f getMarginLow() const;
            Vector2f getMarginHigh() const;
            const Vector2f getPosition() const;
            const Vector2f getSize() const;
           
            void setMargin(const Vector4f & margin);
            void setMargin(const Vector2f & margin);
            void setMargin(const float margin);
            void setMarginLow(const Vector2f & marginLow);
            void setMarginHigh(const Vector2f & marginHigh);
            void setOverflow(const Property::Overflow overflow);
            void setPosition(const Vector2f & position);
            void setSize(const Vector2f & size);
            
        protected:

            RectStyle * m_parent;

            std::optional<Vector4f>                     m_margin;
            std::optional<Style::Property::Overflow>    m_overflow;
            std::optional<Vector2f>                     m_position;
            std::optional<Vector2f>                     m_size;

        };


        class GUISE_API BorderStyle
        {

        public:

            BorderStyle(BorderStyle * parent = nullptr);
            BorderStyle(const std::shared_ptr<Selector> & selector, BorderStyle * parent = nullptr);

            const Vector4f getBorderColor() const;
            Property::BorderStyle getBorderStyle() const;
            float getBorderWidth() const;

            void setBorderColor(const Vector4f & color);
            void setBorderStyle(const Property::BorderStyle style);
            void setBorderWidth(const float width);

        protected:

            BorderStyle * m_parent;

            std::optional<Vector4f>                     m_borderColor;
            std::optional<Style::Property::BorderStyle> m_borderStyle;
            std::optional<float>                        m_borderWidth;            

        };


        class GUISE_API PaintRectStyle : public RectStyle, public BorderStyle
        {

        public:

            PaintRectStyle(PaintRectStyle * parent = nullptr);
            PaintRectStyle(const std::shared_ptr<Selector> & selector, PaintRectStyle * parent = nullptr);

            const Vector4f getBackgroundColor() const;
   
            void setBackgroundColor(const Vector4f & color);

        protected:

            PaintRectStyle * m_parent;

            std::optional<Vector4f>                     m_backgroundColor;

        };


        class GUISE_API FontStyle : RectStyle
        {

        public:

            FontStyle(FontStyle * parent = nullptr);
            FontStyle(const std::shared_ptr<Selector> & selector, FontStyle * parent = nullptr);

            const Vector4f getFontBackgroundColor() const;
            const Vector4f getFontColor() const;
            const std::string getFontFamily() const;
            const int32_t getFontSize() const;

            void setFontBackgroundColor(const Vector4f & color);
            void setFontColor(const Vector4f & color);
            void setFontFamily(const std::string & family);
            void setFontSize(const int32_t size);

        protected:

            FontStyle * m_parent;

            std::optional<Vector4f>     m_fontBackgroundColor;
            std::optional<Vector4f>     m_fontColor;
            std::optional<std::string>  m_fontFamily;
            std::optional<int32_t>          m_fontSize;

        };


        class GUISE_API ParentRectStyle : public Style::RectStyle, public Style::ParentStyle
        {

        public:

            ParentRectStyle(ParentRectStyle * parent = nullptr);
            ParentRectStyle(const std::shared_ptr<Style::Selector> & selector, ParentRectStyle * parent = nullptr);

        };


        class GUISE_API ParentPaintRectStyle : public Style::PaintRectStyle, public Style::ParentStyle
        {

        public:

            ParentPaintRectStyle(ParentPaintRectStyle * parent = nullptr);
            ParentPaintRectStyle(const std::shared_ptr<Style::Selector> & selector, ParentPaintRectStyle * parent = nullptr);

        };       

    }

}

#endif