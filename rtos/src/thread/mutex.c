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
	//
	// try a quick lock to save time
	//
	if (mutex_lock_nonblocking(mutex) != E_OK) {
		//
		// the mutex is locked, so we make a syscall to put the thread
		// into waiting for mutex state
		//
		return thread_wait_mutex(mutex, timeout);
	}

	mutex->holder = thread_get_current();

	//
	// at this point the mutex was locked
	//
	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int mutex_unlock(struct mutex_t *mutex)
{
		syscall(SYSCALL_THREAD_UNLOCK_MUTEX);
}

int mutex_unlock_syscall_handler(uint32_t *args)
{
	struct mutex_t *mutex = (struct mutex_t *)args[0];

	mutex->locked = 0;

	if (active_thread != NULL) {
		//
		// cycle thread list and unlock highest priority thread
		// waiting on this mutex
		//
		struct thread_list_t *ptr = &thread_list;
		struct thread_t *next_thread = NULL;

		while (ptr->next != NULL) {
			if (ptr->thread->state == STATE_WAITING && ptr->thread->wait_condition == WAIT_MUTEX) {
				if (ptr->thread->mutex_waiting == mutex) {
					if (next_thread == NULL || ptr->thread->attr.priority > next_thread->attr.priority) {
						next_thread = ptr->thread;
					}
				}
			}
			ptr = ptr->next;
		}

		//
		// we found a thread waiting with a higher priority, so we trigger a context
		// switch to let it go
		//
		if (next_thread != NULL && next_thread->attr.priority > active_thread->attr.priority) {
			next_thread->state = STATE_READY;
			next_thread->wait_condition = WAIT_NONE;
			next_thread->mutex_waiting = NULL;
			next_thread->mutex_counter = 0;
			next_thread->mutex_timeout = 0;
			next_thread->mutex_priority = 0;

			if (thread_tick()) {
				SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
			}
		}
	}

	return E_OK;
