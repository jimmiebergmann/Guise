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

            std::shared_ptr<Property> getProperty(const std::string & name);

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
        
    }

}

#endif