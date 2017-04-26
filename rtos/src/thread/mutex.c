/**
 * micrortos
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
 * @file	/micrortos/src/thread/mutex.c/mutex.c
 * @author	robin
 * @date	Apr 22, 2017
 * @brief	[DESCRIPTION]
 */

#include "stm32f4xx.h"
#include <mutex.h>
#include <thread.h>
#include <private/thread_p.h>

void mutex_init(struct mutex_t *mutex)
{
	mutex->locked = 0;
	mutex->holder = NULL;
}

int mutex_lock_wait(struct mutex_t *mutex, unsigned long timeout)
{
	unsigned long t = HAL_GetTick();

	//
	// try a quick lock to save time
	//
	while (!mutex_lock_nonblocking(mutex)) {
		//
		// timed out waiting for lock
		//
		if (timeout > 0 && (HAL_GetTick() - t) > timeout) {
			return E_LOCKED;
		}

		//
		// the mutex is locked, so we make a syscall to put the thread
		// into waiting for mutex state. the thread will only be woken
		// up if the mutex is freed or if it times out, at which point
		// we check again
		//
		thread_wait_mutex(mutex, timeout);
	}

	mutex->holder = thread_get_current();

	//
	// at this point the mutex was locked
	//
	return E_OK;
}

void mutex_unlock(struct mutex_t *mutex)
{
	thread_release_mutex(mutex);
}
