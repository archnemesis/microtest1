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
 * @file	/micrortos/src/thread/thread.c/thread.c
 * @author	robin
 * @date	Apr 19, 2017
 * @brief	[DESCRIPTION]
 */

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>
#include <thread.h>
#include <heap.h>
#include <private/thread_p.h>

static struct thread_t idler_thread;

struct thread_t *active_thread = 0;
struct thread_t *queued_thread = 0;
struct thread_t *thread_list[THREAD_MAX_THREADS] = { 0 };

void thread_initialize(struct thread_t *thread, void *data);

#pragma GCC diagnostic ignored "-Wunused-parameter"
void idler(void *data) {
	while (1) { __BKPT(); }
}

uint32_t thread_tick(void);

void SysTick_Handler()
{
	HAL_IncTick();

	if (thread_tick() != 0x0) {
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
}

/**
 * Simply set queued thread to active, clear
 * the queued thread pointer.
 */
void thread_switch_context()
{
	active_thread = queued_thread;
	queued_thread = NULL;
}

struct thread_t *thread_get_current()
{
	return active_thread;
}

uint32_t thread_tick()
{
	struct thread_t *thread = NULL;
	struct thread_t *top = NULL;
	uint32_t highest_priority = 0;

	uint32_t i = 0;

	for (; i < THREAD_MAX_THREADS; i++) {
		thread = thread_list[i];

		if (thread != NULL) {
			switch (thread->state) {
			case STATE_READY:
				if (thread->running_priority > highest_priority) {
					highest_priority = thread->running_priority;
					top = thread;
				}

				break;
			case STATE_SLEEPING:
				/* Wake thread */
				if ((HAL_GetTick() - thread->sleep_counter) >= thread->sleep_duration) {
					thread->state = STATE_READY;
					thread->sleep_counter = 0;
					thread->sleep_duration = 0;

					if (thread->running_priority > highest_priority) {
						highest_priority = thread->running_priority;
						top = thread;
					}
				}

				break;
			case STATE_WAITING:
				switch (thread->wait_condition) {
				case WAIT_MUTEX:
					//
					// mutex timed out, set a flag indicating this so when it
					// wakes it can check
					//
					if ((HAL_GetTick() - thread->mutex_counter) >= thread->mutex_timeout) {
						thread->state = STATE_READY;
						thread->wait_condition = WAIT_NONE;
						thread->mutex_waiting = NULL;
						thread->mutex_counter = 0;
						thread->mutex_timeout = 0;
						thread->mutex_priority = 0;

						if (thread->running_priority > highest_priority) {
							highest_priority = thread->running_priority;
							top = thread;
						}
					}
					break;
				}

				break;
			}

			thread->running_priority++;
		}
	}

	if (top == NULL) {
		return 0;
	}

	//
	// if this thread's priority was raised by a mutex held, we
	// want to remain at that priority until the mutex is released
	// and priority is reset to the running value
	//
	if (top->mutex_priority > 0) {
		top->running_priority = top->mutex_priority;
	}
	else {
		top->running_priority = top->attr.priority;
	}

	if (top != active_thread) {
		queued_thread = top;
		return 1;
	}

	return 0;
}


#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_start(struct thread_handle_t *handle)
{
	syscall(SYSCALL_THREAD_START);
}

uint32_t thread_start_syscall_handler(uint32_t *args)
{
	struct thread_t *thread = NULL;
	struct thread_handle_t *handle = (struct thread_handle_t *)args[0];
	uint32_t i;

	for (; i < THREAD_MAX_THREADS; i++) {
		if (thread_list[i] == NULL) {
			thread_list[i] = handle->thread;
			break;
		}
	}

	if (i == THREAD_MAX_THREADS) {
		return E_INVALID;
	}

	return E_OK;
}

int thread_create(struct thread_handle_t *handle, struct thread_def_t *attr, void *data)
{
	struct thread_t *thread = (struct thread_t *)heap_malloc(sizeof(struct thread_t));
	if (thread == NULL) {
		return E_THREAD_NOMEM;
	}

	memcpy(&thread->attr, attr, sizeof(struct thread_def_t));
	memset(&thread->stack, 0, sizeof(thread->stack));
	thread->state = STATE_READY;
	thread->wait_condition = WAIT_NONE;

	thread_initialize(thread, data);

	handle->thread = thread;

	return E_OK;
}

int thread_create_static(
		struct thread_t *thread,
		void *data)
{
	thread->state = STATE_READY;
	thread->wait_condition = WAIT_NONE;
	thread_initialize(thread, data);
	return E_OK;
}

void thread_initialize(struct thread_t *thread, void *data)
{
	// Set stackPointer to end of stack - 72
	thread->stack_ptr = (uint32_t *)((uint32_t)thread->stack + sizeof(thread->stack) - 72);
	HW32_REG((uint32_t)thread->stack_ptr + 64) 	= (uint32_t)thread->attr.main;	// program counter
	HW32_REG((uint32_t)thread->stack_ptr + 68) 	= 0x01000000;					// initial xPSR
	HW32_REG((uint32_t)thread->stack_ptr + 40) 	= (uint32_t)data;				// pass context as first argument
	HW32_REG((uint32_t)thread->stack_ptr + 4) 	= 0x3;							// unprivileged thread
	HW32_REG((uint32_t)thread->stack_ptr) 		= 0xFFFFFFFDUL;					// initial EXC_RETURN
}

int thread_destroy(struct thread_handle_t *handle)
{
	if (handle->thread == NULL) {
		return E_INVALID;
	}

	heap_free(handle->thread);
	handle->thread = NULL;
	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_terminate(struct thread_handle_t *handle)
{
	syscall(SYSCALL_THREAD_TERMINATE);
}

uint32_t thread_terminate_syscall_handler(uint32_t *args)
{
	struct thread_handle_t *handle = (struct thread_handle_t *)args[0];
	struct thread_list_t *ptr = &thread_list;
	struct thread_list_t *tmp = NULL;

	unsigned int i = 0;

	for (i; i < THREAD_MAX_THREADS; i++) {
		if (thread_list[i] == handle->thread) {
			thread_list[i] = NULL;

			if (active_thread == handle->thread) {
				thread_tick();
				SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
			}

			return E_OK;
		}
	}

	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_start_scheduler()
{
	syscall(SYSCALL_INIT);
}

uint32_t *thread_start_scheduler_syscall_handler(uint32_t *args)
{
	idler_thread.attr.main = &idler;
	idler_thread.attr.priority = 1;
	strcpy((char *)idler_thread.attr.name, "0:idler");

	if (E_OK == thread_create_static(&idler_thread, NULL)) {
		thread_list[0] = &idler_thread;
		return idler_thread.stack_ptr;
	}

	return NULL;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
void thread_sleep(unsigned long time)
{
	syscall(SYSCALL_THREAD_SLEEP);
}

uint32_t thread_sleep_syscall_handler(uint32_t *args)
{
	unsigned long time = (unsigned long)args[0];

	active_thread->state = STATE_SLEEPING;
	active_thread->sleep_duration = time;
	active_thread->sleep_counter = 0;

	thread_yield_syscall_handler(NULL);
	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
void thread_yield()
{
	syscall(SYSCALL_THREAD_SLEEP);
}

uint32_t thread_yield_syscall_handler(uint32_t *args)
{
	if (thread_tick()) {
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_wait_mutex(struct mutex_t *mutex, unsigned long timeout)
{
	syscall(SYSCALL_THREAD_WAIT_MUTEX);
}

uint32_t thread_wait_mutex_syscall_handler(uint32_t *args)
{
	struct mutex_t *mutex = (struct mutex_t *)args[0];
	unsigned long timeout = args[1];

	//
	// if threading is enabled we set this thread to waiting state
	//
	if (active_thread) {
		active_thread->wait_condition = WAIT_MUTEX;
		active_thread->mutex_waiting = mutex;
		active_thread->mutex_timeout = timeout;
		active_thread->mutex_counter = HAL_GetTick();

		//
		// if a lower-priority task is holding the mutex, we raise that
		// thread's priority so it finishes with the mutex, allowing this
		// task to complete faster. in the forced context switch below,
		// it should come up next
		//
		struct thread_t *holder = mutex->holder;
		if (holder != NULL && active_thread->attr.priority > holder->attr.priority) {
			// this check makes sure that another higher-priority task hasn't
			// already done this exact thing
			if (active_thread->attr.priority > holder->mutex_priority) {
				holder->mutex_priority = active_thread->attr.priority;
			}
		}

		//
		// trigger a context switch
		// this should always happen because this thread is now suspended
		// and the next should run
		//
		thread_yield_syscall_handler(NULL);
	}

	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_release_mutex(struct mutex_t *mutex)
{
	syscall(SYSCALL_THREAD_RELEASE_MUTEX);
}

uint32_t thread_release_mutex_syscall_handler(uint32_t *args)
{
	struct mutex_t *mutex = (struct mutex_t *)args[0];

	__mutex_unlock(mutex);

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
}
