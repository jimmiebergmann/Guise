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

#ifndef GUISE_INPUT_HPP
#define GUISE_INPUT_HPP

#include "guise/build.hpp"
#include <queue>
#include <set>
#include <map>

namespace Guise
{

    /**
    * Input class.
    *
    * Used for feeding input events to canvas.
    *
    */
    class GUISE_API Input
    {

    public:

        enum class EventType
        {
            None,
            MouseJustPressed,
            MouseDoubleClick,
            MouseMove,
            MousePress,
            MouseRelease,
            MouseScroll,
            KeyboardJustPressed,
            KeyboardPress,
            KeyboardRelease,
            Texting
        };

        // Public enum
        enum class Key
        {
            Unkown,

            // Alphabet keys
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,

            // Numeric keys
            Num0,
            Num1,
            Num2,
            Num3,
            Num4,
            Num5,
            Num6,
            Num7,
            Num8,
            Num9,

            // Function Keys
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,

            // Arrow keys
            Down,
            Left,
            Right,
            Up,

            // System keys
            AltLeft,
            AltRight,
            Backspace,
            Brace_Left,
            Brace_Right,
            BracketLeft,
            BracketRight,
            Break,
            Capslock,
            Colon,
            ControlLeft,
            ControlRight,
            Comma,
            Delete,
            End,
            Escape,
            Greater,
            Home,
            Insert,
            Less,
            Minus,
            NumLock,
            PageDown,
            PageUp,
            Period,
            Plus,
            Print,
            Return,
            ScrollLock,
            SemiColon,
            ShiftLeft,
            ShiftRight,
            Space,
            SuperLeft,
            SuperRight,
            Tab,
            Underscore
        };

        struct Event
        {
            Event();
            Event(const EventType type, const uint8_t value1, const Vector2f & value2);
            Event(const EventType type, const wchar_t value);
            Event(const EventType type, const float value);
            Event(const EventType type, const Key value);
            Event(const EventType type, const Vector2f & value);

            EventType   type;
            Vector2f    position;
            union
            {
                uint8_t     button;
                wchar_t     character;
                float       distance;
                Key         key;    
            };
            
        };

        Input();

        void update();

        bool peekEvent(Event & e);

        bool pollEvent(Event & e);

        void pushEvent(const Event & e);

        size_t queueSize() const;

        Vector2f getLastMousePosition() const;

#if defined(GUISE_PLATFORM_WINDOWS)
        static Key transalteWin32Key(const WORD key);
#endif

    private:
        
        Input(const Input &) = delete;

        std::queue<Event>           m_eventQueue; 
        std::set<Key>               m_eventKeyPressed;
        std::set<Key>               m_keysPressed;
        std::set<Key>               m_keysReleased;
        std::map<uint8_t, Vector2f> m_eventMousePressed;
        std::map<uint8_t, Vector2f> m_mousePressed;
        std::map<uint8_t, Vector2f> m_mouseReleased;
        Vector2f                    m_mousePosition;

    };

}

#endif