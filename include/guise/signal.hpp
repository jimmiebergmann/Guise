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
#include <memory>

namespace Guise
{

    /*
    *
    * Anonymous connections are not disconnected when calling disconnectAll.
    *
    *
    */
    template<typename ... T>
    class Signal;

    template<>
    class Signal<>
    {

    public:

        using CallbackNoParams  = std::function<void()>;
        using Dependencies      = std::vector<std::shared_ptr<void> >;
        using DependenciesWeak  = std::vector<std::weak_ptr<void> >;

        struct AssignStructNoParams
        {
            DependenciesWeak    dependencies;
            CallbackNoParams    callback;
        };

        ~Signal();

        Signal & connect(const CallbackNoParams & callback);

        Signal & connect(const DependenciesWeak & dependencies, const CallbackNoParams & callback);

        Signal & connectAnonymously(const CallbackNoParams & callback);

        Signal & connectAnonymously(const DependenciesWeak & dependencies, const CallbackNoParams & callback);

        void disconnectAll();

        size_t getAnonymousConnectionCount() const;

        size_t getConnectionCount() const;

        size_t getTotalConnectionCount() const;

        Signal & operator =(const CallbackNoParams & callback);

        Signal & operator =(const AssignStructNoParams & assignStruct);

        Signal & operator()();

    private:

        template<typename U>
        void call(U & callback);

        template<typename T>
        using Callbacks = std::vector<std::pair<T, DependenciesWeak*> >;

        Callbacks<CallbackNoParams> m_callbacksNoParams;
        Callbacks<CallbackNoParams> m_anonymousCallbacksNoParams;

    };

    template<typename ... T>
    class Signal
    {

    public:

        using Callback          = std::function<void(T...)>;
        using CallbackNoParams  = std::function<void()>;
        using Dependencies      = std::vector<std::shared_ptr<void> >;
        using DependenciesWeak  = std::vector<std::weak_ptr<void> >;

        struct AssignStruct
        {
            DependenciesWeak    dependencies;
            Callback            callback;    
        };
        struct AssignStructNoParams
        {
            DependenciesWeak    dependencies;
            CallbackNoParams    callback;   
        };

        ~Signal();

        Signal & connect(const Callback & callback);

        Signal & connect(const CallbackNoParams & callback);

        Signal & connect(const DependenciesWeak & dependencies, const Callback & callback);

        Signal & connect(const DependenciesWeak & dependencies, const CallbackNoParams & callback);

        Signal & connectAnonymously(const Callback & callback);

        Signal & connectAnonymously(const CallbackNoParams & callback);

        Signal & connectAnonymously(const DependenciesWeak & dependencies, const Callback & callback);

        Signal & connectAnonymously(const DependenciesWeak & dependencies, const CallbackNoParams & callback);

        void disconnectAll();

        size_t getAnonymousConnectionCount() const;

        size_t getConnectionCount() const;

        size_t getTotalConnectionCount() const;

        Signal & operator =(const Callback & callback);

        Signal & operator =(const CallbackNoParams & callback);

        Signal & operator =(const AssignStruct & assignStruct);

        Signal & operator =(const AssignStructNoParams & assignStruct);

        Signal & operator()(T ... params);

        Signal & operator()();

    private:

        template<typename U>
        void call(U & callback);

        template<typename U>
        void call(U & callback, T ... params);

        template<typename U>
        using Callbacks = std::vector<std::pair<U, DependenciesWeak*> >;

        Callbacks<Callback>         m_callbacks;
        Callbacks<CallbackNoParams> m_callbacksNoParams;
        Callbacks<Callback>         m_anonymousCallbacks;
        Callbacks<CallbackNoParams> m_anonymousCallbacksNoParams;

    };
}

#include "guise/signal.inl"

#endif