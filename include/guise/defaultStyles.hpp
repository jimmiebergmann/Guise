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

#ifndef GUISE_DEFUALT_STYLES_HPP
#define GUISE_DEFUALT_STYLES_HPP

#include "guise/style.hpp"

namespace Guise
{

    namespace DefaultStyles
    {     

        static const Style button = Style(
            {
                { Style::Property::Padding,         15.0f },
                { Style::Property::BackgroundColor, { 0.25f, 0.25f, 0.25f, 1.0f } },
                { Style::Property::Border,          Style::BorderStyle::Solid },
                { Style::Property::BorderWidth,     10.0f },
                { Style::Property::BorderColor,     { 0.05f, 0.05f, 0.05f, 1.0f } }
            }
        );

        static const Style canvas = Style(
            {
                { Style::Property::Padding,         5.0f },
                { Style::Property::BackgroundColor,{ 0.9f, 0.9f, 0.9f, 1.0f } }
            }
        );

        static const Style window = Style(
            {
                { Style::Property::Padding,         5.0f },
                { Style::Property::BackgroundColor,{ 0.7f, 0.7f, 0.7f, 1.0f } }
            }
        );

        static const Style plane = Style(
            {
                { Style::Property::Padding,         5.0f }
            }
        );


    }

}


#endif