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

    /**
    * Style class.
    *
    *
    */
    class GUISE_API Style
    {

    public:

        enum class Property
        {
            BackgroundColor,
            Border,
            BorderWidth,
            BorderColor,
            Clamp,
            Padding,
            Position,
            Size
        };

        enum class BorderStyle
        {
            None,
            Solid
        };

        struct GUISE_API PropertyPair
        {
            PropertyPair(const Property property, const float value);

            PropertyPair(const Property property, const bool value);

            PropertyPair(const Property property, const Vector2f & value);

            PropertyPair(const Property property, const Vector4f & value);

            PropertyPair(const Property property, const Vector3f & value);

            PropertyPair(const Property property, const BorderStyle value);

            Property property;

            union
            {
                bool        b;
                BorderStyle borderStyle;
                float       f;
                Vector2f    vec2;
                Vector3f    vec3;
                Vector4f    vec4;            
            };

            enum class ValueType
            {
                Boolean,
                BorderStyle,
                Float,
                Vector2f,
                Vector3f,
                Vector4f
                
            } valueType;
        };

        Style();
        Style(const std::initializer_list<PropertyPair> & properties);
        Style(const Style & style);

        Style & operator =(const Style & style);

        void setStyle(const Style & style);

        void setStyleProperties(const std::initializer_list<PropertyPair> & properties);

        bool getClamp() const;
        void setClamp(bool clamp);

        const Vector2f & getPosition() const;
        void setPosition(const Vector2f & position);

        const Vector2f & getSize() const;
        void setSize(const Vector2f & size);

        const Vector4f & getPadding() const;
        Vector2f getPaddingLow() const;
        Vector2f getPaddingHigh() const;
        void setPadding(const float padding);
        void setPadding(const Vector2f & padding);
        void setPadding(const Vector4f & padding);
        void setPaddingLow(const Vector2f & low);
        void setPaddingHigh(const Vector2f & high);

        Vector4f getBackgroundColor() const;
        void setBackgroundColor(const Vector4f & color);

        BorderStyle getBorder() const;
        void setBorder(const BorderStyle style);

        float getBorderWidth() const;
        void setBorderWidth(const float width);

        Vector4f getBorderColor() const;
        void setBorderColor(const Vector4f color);
     
    protected:

        Bounds2f calcRenderBounds(const Bounds2f & canvasBound);

    private:

        bool                m_clamp;
        Vector2f            m_position;
        Vector2f            m_size;
        Vector4f            m_padding;
        Vector4f            m_backgroundColor;
        BorderStyle         m_borderStyle;
        float               m_borderWidth;
        Vector4f            m_borderColor;

    };


    class GUISE_API StyleSheet
    {

    public:

        enum class Entry
        {
            Button,
            Canvas,
            Plane,
            Window
        };

        struct GUISE_API EntryPair
        {
            EntryPair(const Entry entry, const Style & style);
            EntryPair(const std::string & name, const Style & style);

            Entry entry;
            std::string name;
            const Style & style;
        };

        static std::shared_ptr<StyleSheet> create(const std::initializer_list<EntryPair> & entries = {});
        static std::shared_ptr<StyleSheet> createDefault();

        const Style & getStyle(const Entry entry) const;
        Style & getStyle(const Entry entry);

        const Style * getStyle(const std::string & name) const;
        Style * getStyle(const std::string & name);

    private:

        StyleSheet(const std::initializer_list<EntryPair> & entries = {});

        // Styles for default controls.
        std::shared_ptr<Style> m_buttonStyle;
        std::shared_ptr<Style> m_canvasStyle;
        std::shared_ptr<Style> m_planeStyle;
        std::shared_ptr<Style> m_windowStyle;

        // Custom styles.
        std::map<std::string, std::shared_ptr<Style> > m_styleMap;

    };

}

#endif