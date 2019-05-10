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

#include "guise/control/button.hpp"

namespace Guise
{
    std::shared_ptr<Button> Button::create()
    {
        return std::shared_ptr<Button>(new Button());
    }

    ControlType Button::getType() const
    {
        return ControlType::Button;
    }

    void Button::render(RendererInterface & renderer, const Vector2f & canvasPosition, const Vector2f & canvasSize)
    {
        Vector2f position = getPosition();
        position.x = position.x > 0.0f ? position.x : 0.0f;
        position.y = position.y > 0.0f ? position.y : 0.0f;

        Vector2f size = getSize();
        
        if (size.x > 0.0f)
        {
            if (size.x + position.x > canvasSize.x)
            {
                size.x = canvasSize.x - position.x;
            }
            else
            {
                size.x = size.x;
            }
        }
        else
        {
            size.x = canvasSize.x - position.x;
        }

        if (size.y > 0.0f)
        {
            if (size.y + position.y > canvasSize.y)
            {
                size.y = canvasSize.y - position.y;
            }
            else
            {
                size.y = size.y;
            }
        }
        else
        {
            size.y = canvasSize.y - position.y;
        }


        position += canvasPosition;

        //size.x = size.x > 0.0f ? (size.x < canvasSize.x ? size.x : canvasSize.x - position.x) : 0.0f;
        //size.y = size.y > 0.0f ? (size.y < canvasSize.y ? size.y : canvasSize.y - position.y) : 0.0f;

       if (size.x > 0.0f && size.y > 0.0f)
       {
           renderer.drawQuad(position, size);

           auto child = getChilds()[0];
           if (child)
           {
               Vector2f childPos = position + getPaddingLow();
               Vector2f childSize = size - getPaddingLow() - getPaddingHigh();

               if (childSize.x > 0.0f && childSize.y > 0.0f)
               {
                   child->render(renderer, childPos, childSize);
               }
           }
       }
    }

    Button::Button()
    { }

}