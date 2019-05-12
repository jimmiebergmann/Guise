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

#include "guise/input.hpp"

namespace Guise
{
    // Input event implementations.
    Input::Event::Event() :
        type(EventType::None)
    { }

    Input::Event::Event(const EventType type, const uint8_t value1, const Vector2f &value2) :
        type(type),
        button(value1),
        position(value2)
    { }

    Input::Event::Event(const EventType type, const wchar_t value) :
        type(type),
        character(value)
    { }

    Input::Event::Event(const EventType type, const float value) :
        type(type),
        distance(value)
    { }

    Input::Event::Event(const EventType type, const Key value) :
        type(type),
        key(value)
    { }

    Input::Event::Event(const EventType type, const Vector2f & value) :
        type(type),
        position(value)
    { }


    // Input implementations.
    Input::Input()
    { }

    void Input::update()
    {
        // Key is pressed, but has not been queued.
        for (auto it = m_keysPressed.begin(); it != m_keysPressed.end(); it++)
        {
            if (m_eventKeyPressed.find(*it) == m_eventKeyPressed.end())
            {
                m_eventQueue.push({EventType::KeyboardPress, *it});
            }
        }
        m_eventKeyPressed.clear();

        // Key just pressed events.
        for (auto it = m_keysReleased.begin(); it != m_keysReleased.end(); it++)
        {
            m_eventQueue.push({ EventType::KeyboardRelease, *it });
            m_keysPressed.erase(*it);
        }
        m_keysReleased.clear();

        // Mouse is pressed, but has not been queued.
        for (auto it = m_mousePressed.begin(); it != m_mousePressed.end(); it++)
        {
            if (m_eventMousePressed.find(it->first) == m_eventMousePressed.end())
            {
                m_eventQueue.push({ EventType::MousePress, it->first, m_mousePosition });
            }
        }
        m_eventMousePressed.clear();

        // Mouse just pressed events.
        for (auto it = m_mouseReleased.begin(); it != m_mouseReleased.end(); it++)
        {
            m_eventQueue.push({ EventType::MouseRelease, it->first, it->second });
            m_mousePressed.erase(it->first);
        }
        m_mouseReleased.clear();

    }

    bool Input::peekEvent(Event & e)
    {
        if (m_eventQueue.size())
        {
            e = m_eventQueue.front();
            return true;
        }

        return false;
    }

    bool Input::pollEvent(Event & e)
    {
        if (m_eventQueue.size())
        {
            e = m_eventQueue.front();
            m_eventQueue.pop();
            return true;
        }

        return false;
    }

    void Input::pushEvent(const Event & e)
    {
        // Hardcoded queue size.
        if (m_eventQueue.size() > 8192)
        {
            return;
        }

        switch (e.type)
        {
            case Input::EventType::KeyboardRelease:
                m_keysReleased.insert(e.key);
                return;
            case Input::EventType::KeyboardPress:
                if (m_keysPressed.find(e.key) == m_keysPressed.end())
                {
                    m_eventQueue.push({EventType::KeyboardJustPressed, e.key});
                }
                m_keysPressed.insert(e.key);
                m_eventKeyPressed.insert(e.key);
                break;
            case Input::EventType::MouseRelease:
                m_mouseReleased.insert({ e.button, e.position });
                return;
            case Input::EventType::MousePress:
                if (m_mousePressed.find(e.button) == m_mousePressed.end())
                {
                    m_mousePosition = e.position;
                    m_eventQueue.push({ EventType::MouseJustPressed, e.button, e.position });
                }
                m_mousePressed.insert({ e.button, e.position });
                m_eventMousePressed.insert({ e.button, e.position });
                break;
            case Input::EventType::MouseMove:
                m_mousePosition = e.position;
                break;
            default: break;
        }

        m_eventQueue.push(e);
    }

    size_t Input::queueSize() const
    {
        return m_eventQueue.size();
    }

#if defined(GUISE_PLATFORM_WINDOWS)
    Input::Key Input::transalteWin32Key(const WORD key)
    {
        switch (key)
        {
        case 'A':               return Key::A;
        case 'B':               return Key::B;
        case 'C':               return Key::C;
        case 'D':               return Key::D;
        case 'E':               return Key::E;
        case 'F':               return Key::F;
        case 'G':               return Key::G;
        case 'H':               return Key::H;
        case 'I':               return Key::I;
        case 'J':               return Key::J;
        case 'K':               return Key::K;
        case 'L':               return Key::L;
        case 'M':               return Key::M;
        case 'N':               return Key::N;
        case 'O':               return Key::O;
        case 'P':               return Key::P;
        case 'Q':               return Key::Q;
        case 'R':               return Key::R;
        case 'S':               return Key::S;
        case 'T':               return Key::T;
        case 'U':               return Key::U;
        case 'V':               return Key::V;
        case 'W':               return Key::W;
        case 'X':               return Key::X;
        case 'Y':               return Key::Y;
        case 'Z':               return Key::Z;
        case '0':               return Key::Num0;
        case '1':               return Key::Num1;
        case '2':               return Key::Num2;
        case '3':               return Key::Num3;
        case '4':               return Key::Num4;
        case '5':               return Key::Num5;
        case '6':               return Key::Num6;
        case '7':               return Key::Num7;
        case '8':               return Key::Num8;
        case '9':               return Key::Num9;
        case VK_F1:             return Key::F1;
        case VK_F2:             return Key::F2;
        case VK_F3:             return Key::F3;
        case VK_F4:             return Key::F4;
        case VK_F5:             return Key::F5;
        case VK_F6:             return Key::F6;
        case VK_F7:             return Key::F7;
        case VK_F8:             return Key::F8;
        case VK_F9:             return Key::F9;
        case VK_F10:            return Key::F10;
        case VK_F11:            return Key::F11;
        case VK_F12:            return Key::F12;
        case VK_DOWN:           return Key::Down;
        case VK_LEFT:           return Key::Left;
        case VK_RIGHT:          return Key::Right;
        case VK_UP:             return Key::Up;
        case VK_LMENU:          return Key::AltLeft;
        case VK_RMENU:          return Key::AltRight;
        case VK_BACK:           return Key::Backspace;
        //case XK_braceleft:    return Key::Brace_Left;
        //case XK_braceright:   return Key::Brace_Right;
        case VK_OEM_4:          return Key::BracketLeft;
        case VK_OEM_6:          return Key::BracketRight;
        case VK_PAUSE:          return Key::Break;
        case VK_CAPITAL:        return Key::Capslock;
        //case XK_colon:        return Key::Colon;
        case VK_LCONTROL:       return Key::ControlLeft;
        case VK_RCONTROL:       return Key::ControlRight;
        case VK_OEM_COMMA:      return Key::Comma;
        case VK_DELETE:         return Key::Delete;
        case VK_END:            return Key::End;
        case VK_ESCAPE:         return Key::Escape;
        //case XK_greater:      return Key::Greater;
        case VK_HOME:           return Key::Home;
        case VK_INSERT:         return Key::Insert;
        //case XK_less:         return Key::Less;
        case VK_OEM_MINUS:      return Key::Minus;
        case VK_NUMLOCK:        return Key::NumLock;
        case VK_NEXT:           return Key::PageDown;
        case VK_PRIOR:          return Key::PageUp;
        case VK_OEM_PERIOD:     return Key::Period;
        case VK_ADD:            return Key::Plus;
        case VK_SNAPSHOT:       return Key::Print;
        case VK_RETURN:         return Key::Return;
        case VK_SCROLL:         return Key::ScrollLock;
        case VK_OEM_1:          return Key::SemiColon;
        case VK_LSHIFT:         return Key::ShiftLeft;
        case VK_RSHIFT:         return Key::ShiftRight;
        case VK_SPACE:          return Key::Space;
        case VK_LWIN:           return Key::SuperLeft;
        case VK_RWIN:           return Key::SuperRight;
        case VK_TAB:            return Key::Tab;
        //case XK_underscore:   return Key::Underscore
        default: break;
        }

        return Key::Unkown;
    }
#endif

}