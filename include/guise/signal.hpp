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

#ifndef GUISE_SIGNAL_HPP
#define GUISE_SIGNAL_HPP

#include "guise/build.hpp"
#include <functional>
#include <vector>

namespace Guise
{

    template<typename ... T>
    class Signal;

    template<>
    class Signal<>
    {

    public:

        using Callback = std::function<void()>;

        Signal & operator =(const Callback & callback)
        {
            m_callbacks.push_back(callback);
            return *this;
        }

        Signal & operator()()
        {
            for (auto & callback : m_callbacks)
            {
                callback();
            }
            return *this;
        }

    private:

        std::vector<Callback> m_callbacks;

        /*
        Signal & operator =(const CallbackNoParams & callback)
        {
            m_callbacksNoParams.push_back(callback);
            return *this;
        }

        
        Signal & operator()(T ... params)
        {
            for (auto & callback : m_callbacks)
            {
                callback(params...);
            }
            for (auto & callback : m_callbacksNoParams)
            {
                callback();
            }
            return *this;
        }

        Signal & operator()()
        {
            for (auto & callback : m_callbacksNoParams)
            {
                callback();
            }
            return *this;
        }

    private:

        std::vector<Callback> m_callbacks;
        std::vector<CallbackNoParams> m_callbacksNoParams;
        */
    };

    template<typename ... T>
    class Signal
    {

    public:

        using Callback = std::function<void(T...)>;
        using CallbackNoParams = std::function<void()>;

        Signal & operator =(const Callback & callback)
        {
            m_callbacks.push_back(callback);
            return *this;
        }

        Signal & operator =(const CallbackNoParams & callback)
        {
            m_callbacksNoParams.push_back(callback);
            return *this;
        }

        Signal & operator()(T ... params)
        {
            for (auto & callback : m_callbacks)
            {
                callback(params...);
            }
            for (auto & callback : m_callbacksNoParams)
            {
                callback();
            }
            return *this;
        }

        Signal & operator()()
        {
            for (auto & callback : m_callbacksNoParams)
            {
                callback();
            }
            return *this;
        }

    private:

        std::vector<Callback> m_callbacks;
        std::vector<CallbackNoParams> m_callbacksNoParams;

    };
}

#include "guise/signal.inl"

#endif