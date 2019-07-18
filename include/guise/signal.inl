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
        for (auto it = m_callbacks.begin(); it != m_callbacks.end(); it++)
        {
            if (it->second)
            {
                delete it->second;
            }
        }
    }

    inline Signal<> & Signal<>::add(const DependenciesWeak & dependencies, const Callback & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_callbacks.push_back({ callback, deps });
        return *this;
    }

    inline Signal<> & Signal<>::operator =(const Callback & callback)
    {
        m_callbacks.push_back({ callback, nullptr });
        return *this;
    }

    inline Signal<> & Signal<>::operator =(const AssignStruct & assignStruct)
    {
        return add(assignStruct.dependencies, assignStruct.callback);
    }

    inline Signal<> & Signal<>::operator()()
    {
        for (auto it = m_callbacks.begin(); it != m_callbacks.end();)
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
                        it = m_callbacks.erase(it);
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
        return *this;
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
    inline Signal<T...> & Signal<T...>::add(const DependenciesWeak & dependencies, const Callback & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_callbacks.push_back({ callback, deps });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::add(const DependenciesWeak & dependencies, const CallbackNoParams & callback)
    {
        DependenciesWeak * deps = new DependenciesWeak;
        *deps = dependencies;
        m_callbacksNoParams.push_back({ callback, deps });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator =(const Callback & callback)
    {
        m_callbacks.push_back({ callback, nullptr });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator =(const CallbackNoParams & callback)
    {
        m_callbacksNoParams.push_back({ callback, nullptr });
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator =(const AssignStruct & assignStruct)
    {
        return add(assignStruct.dependencies, assignStruct.callback);
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator =(const AssignStructNoParams & assignStruct)
    {
        return add(assignStruct.dependencies, assignStruct.callback);
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator()(T ... params)
    {
        call(m_callbacks, params...);
        call(m_callbacksNoParams);
        return *this;
    }

    template<typename ... T>
    inline Signal<T...> & Signal<T...>::operator()()
    {
        call(m_callbacks);
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
