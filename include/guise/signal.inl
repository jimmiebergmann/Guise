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

namespace Guise
{

    // Implementation of Signal class with no template parameters.
    inline Signal<>::~Signal()
    {
        for (auto it = m_callbacksNoParams.begin(); it != m_callbacksNoParams.end(); it++)
        {
            if (it->second)
            {
                delete it->second;
            }
        }
    }

    inline Signal<> & Signal<>::connect(const CallbackNoParams & callback)
    {
        m_callbacksNoParams.push_back({ callback, nullptr });
        return *this;
    }

    inline Signal<> & Signal<>::connect(const DependenciesWeak & dependencies, const CallbackNoParams & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_callbacksNoParams.push_back({ callback, deps });
        return *this;
    }

    inline Signal<> & Signal<>::connectAnonymously(const CallbackNoParams & callback)
    {
        m_anonymousCallbacksNoParams.push_back({ callback, nullptr });
        return *this;
    }

    inline Signal<> & Signal<>::connectAnonymously(const DependenciesWeak & dependencies, const CallbackNoParams & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_anonymousCallbacksNoParams.push_back({ callback, deps });
        return *this;
    }

    inline void Signal<>::disconnectAll()
    {
        m_callbacksNoParams.clear();
    }

    inline size_t Signal<>::getAnonymousConnectionCount() const
    {
        return m_anonymousCallbacksNoParams.size();
    }
    inline size_t Signal<>::getConnectionCount() const
    {
        return m_callbacksNoParams.size();
    }

    inline size_t Signal<>::getTotalConnectionCount() const
    {
        return getConnectionCount() + getAnonymousConnectionCount();
    }

    inline Signal<> & Signal<>::operator =(const CallbackNoParams & callback)
    {
        return connect(callback);
    }

    inline Signal<> & Signal<>::operator =(const AssignStructNoParams & assignStruct)
    {
        return connect(assignStruct.dependencies, assignStruct.callback);
    }

    inline Signal<> & Signal<>::operator()()
    {
        call(m_anonymousCallbacksNoParams);
        call(m_callbacksNoParams);
        return *this;
    }

    template<typename U>
    inline void Signal<>::call(U & callback)
    {
        for (auto it = callback.begin(); it != callback.end();)
        {
            bool deleted = false;

            Dependencies dependencies;
            if (it->second)
            {
                for (auto & dep : *it->second)
                {
                    auto locked = dep.lock();
                    if (!locked)
                    {
                        delete it->second;
                        it = callback.erase(it);
                        deleted = true;
                        break;
                    }

                    dependencies.push_back(locked);
                }
            }

            if (!deleted)
            {
                it->first();
                ++it;
            }
        }
    }


    // Implementation of Signal class.
    template<typename ... T>
    inline Signal<T...>::~Signal()
    {
        for (auto it = m_callbacks.begin(); it != m_callbacks.end(); it++)
        {
            if (it->second)
            {
                delete it->second;
            }
        }
        for (auto it = m_callbacksNoParams.begin(); it != m_callbacksNoParams.end(); it++)
        {
            if (it->second)
            {
                delete it->second;
            }
        }
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::connect(const Callback & callback)
    {
        m_callbacks.push_back({ callback, nullptr });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::connect(const CallbackNoParams & callback)
    {
        m_callbacksNoParams.push_back({ callback, nullptr });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::connect(const DependenciesWeak & dependencies, const Callback & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_callbacks.push_back({ callback, deps });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::connect(const DependenciesWeak & dependencies, const CallbackNoParams & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_callbacksNoParams.push_back({ callback, deps });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::connectAnonymously(const Callback & callback)
    {
        m_anonymousCallbacks.push_back({ callback, nullptr });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::connectAnonymously(const CallbackNoParams & callback)
    {
        m_anonymousCallbacksNoParams.push_back({ callback, nullptr });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::connectAnonymously(const DependenciesWeak & dependencies, const Callback & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_anonymousCallbacks.push_back({ callback, deps });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::connectAnonymously(const DependenciesWeak & dependencies, const CallbackNoParams & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_anonymousCallbacksNoParams.push_back({ callback, deps });
        return *this;
    }

    template<typename ... T>
    inline void Signal<T...>::disconnectAll()
    {  
        m_callbacks.clear();
        m_callbacksNoParams.clear();
    }

    template<typename ... T>
    inline size_t Signal<T...>::getAnonymousConnectionCount() const
    {
        return m_anonymousCallbacks.size() + m_anonymousCallbacksNoParams.size();
    }

    template<typename ... T>
    inline size_t Signal<T...>::getConnectionCount() const
    {
        return m_callbacks.size() + m_callbacksNoParams.size();
    }

    template<typename ... T>
    inline size_t Signal<T...>::getTotalConnectionCount() const
    {
        return getConnectionCount() + getAnonymousConnectionCount();
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator =(const Callback & callback)
    {
        return connect(callback);
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator =(const CallbackNoParams & callback)
    {
        return connect(callback);
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator =(const AssignStruct & assignStruct)
    {
        return connect(assignStruct.dependencies, assignStruct.callback);
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator =(const AssignStructNoParams & assignStruct)
    {
        return connect(assignStruct.dependencies, assignStruct.callback);
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator()(T ... params)
    {
        call(m_anonymousCallbacks, params...);
        call(m_anonymousCallbacksNoParams);
        call(m_callbacks, params...);
        call(m_callbacksNoParams);       
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator()()
    {
        call(m_anonymousCallbacksNoParams);
        call(m_callbacksNoParams);      
        return *this;
    }

    template<typename ... T>
    template<typename U>
    inline void Signal<T...>::call(U & callback)
    {
        for (auto it = callback.begin(); it != callback.end();)
        {
            bool deleted = false;

            Dependencies dependencies;
            if (it->second)
            {
                for (auto & dep : *it->second)
                {
                    auto locked = dep.lock();
                    if (!locked)
                    {
                        delete it->second;
                        it = callback.erase(it);
                        deleted = true;
                        break;
                    }

                    dependencies.push_back(locked);
                }
            }

            if (!deleted)
            {
                it->first();
                ++it;
            }
        }
    }

    template<typename ... T>
    template<typename U>
    inline void Signal<T...>::call(U & callback, T ... params)
    {
        for (auto it = callback.begin(); it != callback.end();)
        {
            bool deleted = false;

            Dependencies dependencies;
            if (it->second)
            {
                for (auto & dep : *it->second)
                {
                    auto locked = dep.lock();
                    if (!locked)
                    {
                        delete it->second;
                        it = callback.erase(it);
                        deleted = true;
                        break;
                    }

                    dependencies.push_back(locked);
                }
            }

            if (!deleted)
            {
                it->first(params...);
                ++it;
            }
        }
    }
 
}
