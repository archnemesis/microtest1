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
	thread_get_current(&mutex->holder);
}

int mutex_lock_blocking(struct mutex_t *mutex, unsigned long timeout)
{
	unsigned long start = HAL_GetTick();
	while (mutex_lock(mutex) == 0) {
		if (timeout > 0 && (HAL_GetTick() - start) > timeout) {
			return 0;
		}
		else {
			thread_yield();
		}
	}
	return 1;
}

int mutex_lock_wait(struct mutex_t *mutex, uint32_t timeout)
{
	int r = 0;

	while (!mutex_lock_nonblocking(mutex)) {
		thread_wait_on_mutex(mutex, timeout);
	}

	return E_OK;
}

int mutex_lock_wait_syscall_handler(uint32_t *args)
{
	struct mutex_t *mutex = (struct mutex_t *)args[0];
	uint32_t timeout = args[1];

	//
	// try a quick lock to save time
	//
	while (!mutex_lock_nonblocking(mutex)) {
		//
		// set this thread to waiting
		//
		if (active_thread) {
			active_thread->wait_condition = WAIT_MUTEX;
			active_thread->mutex_waiting = mutex;
			active_thread->mutex_timeout = timeout;
		}

		//
		// trigger a context switch
		//
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}


	//
	// at this point the mutex was locked
	//
	return E_OK;
}
