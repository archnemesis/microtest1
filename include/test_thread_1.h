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
 * @file	/microtest1/include/test_thread_1.h/test_thread_1.h
 * @author	robin
 * @date	Apr 29, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_TEST_THREAD_1_H_
#define INCLUDE_TEST_THREAD_1_H_

#include <thread.h>

#ifdef __cplusplus
extern "C" {
#endif

void test_thread_1_main();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class TestThread : public Thread
{
public:
	TestThread(unsigned int gpio, unsigned int sleep);
	virtual ~TestThread();
protected:
	virtual void run();
	unsigned int m_gpio;
	unsigned int m_sleep;
};

class NotifierThread : public Thread
{
public:
	NotifierThread(TestThread *target);
	virtual ~NotifierThread();
protected:
	virtual void run();
	TestThread *m_target;
};

#endif

#endif /* INCLUDE_TEST_THREAD_1_H_ */