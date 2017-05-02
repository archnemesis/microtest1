/**
 * microtest1
 * ----------------------------------------
 *
 * MIT License
 *
 * Copyright (c) 2017 robin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file	/microtest1/rtos/src/cpp/thread_cpp.cpp/thread_cpp.cpp
 * @author	robin
 * @date	Apr 29, 2017
 * @brief	[DESCRIPTION]
 */

#include <thread.h>

Thread::Thread(unsigned int priority)
{
	m_priority = priority;
	m_thread.attr.main = Thread::start_wrapper;
	m_thread.attr.priority = m_priority;
	thread_create_static(&m_thread, (void *)this);
}

Thread::~Thread()
{

}

void Thread::start_wrapper(void *ptr)
{
	Thread *thiz = static_cast<Thread *>(ptr);
	thiz->wrapper();
}

void Thread::start()
{
	thread_start(&m_thread);
}

void Thread::wrapper()
{
	run();
	terminate();
}

void Thread::terminate()
{
	thread_terminate(&m_thread);
}

void Thread::sleep(unsigned int time)
{
	thread_sleep(time);
}

void Thread::waitForEvent(uint32_t event_mask)
{
	thread_wait_event(event_mask);
}

void Thread::notify(uint32_t event_mask)
{
	thread_notify(&this->m_thread, event_mask);
}
