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

    inline Semaphore::Semaphore() :
        m_value(0)
    { }

    inline Semaphore::~Semaphore()
    { }

    inline void Semaphore::wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (!m_value)
        {
            m_condition.wait(lock);
        }
        --m_value;
    }

    inline bool Semaphore::tryWait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_value)
        {
            --m_value;
            return true;
        }

        return false;
    }

   /* inline void Semaphore::notifyAll()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_value = 0;
        m_condition.notify_all();
    }*/

    inline void Semaphore::notifyOne()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        ++m_value;
        m_condition.notify_one();
    }

    inline void Semaphore::reset()
    {
        m_value = 0;
    }

}
