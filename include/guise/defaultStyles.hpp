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

    namespace Style
    {

        namespace DefaultStyles
        {

            static const Selector button = Selector(
                {
                    { "padding",                { 15.0f } },
                    { "background-color",   {   { 0.25f, 0.25f, 0.25f, 1.0 } } },
                    { "padding",                { 15.0f } },
                    { "border-style",           { Property::BorderStyle::Solid } },
                    { "border-width",           { 10.0f } },
                    { "border-color",       {   { 0.05f, 0.05f, 0.05f, 1.0f } } }
                }
            );

            static const Selector canvas = Selector(
                {
                    { "padding",                { 5.0f }  },
                    { "background-color",   {   { 0.9f, 0.9f, 0.9f, 1.0f } } }
                }
            );

            static const Selector window = Selector(
                {
                    { "padding",                { 5.0f } },
                    { "background-color",   {   { 0.7f, 0.7f, 0.7f, 1.0f } } }
                }
            );

            static const Selector plane = Selector(
                {
                    { "padding",                { 5.0f }  }
                }
            );

        }

    }

}


#endif