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
#include <private/thread_p.h>

static struct thread_handle_t idle_thread_handle;
static struct thread_def_t idle_thread_def;

struct thread_list_t thread_list = {
		.thread = NULL,
		.next = NULL
};
struct thread_t *active_thread = 0;
struct thread_t *queued_thread = 0;

#pragma GCC diagnostic ignored "-Wunused-parameter"
void idler(void *data) {
	while (1) {}
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
	struct thread_list_t *ptr = &thread_list;
	struct thread_list_t *top = NULL;
	uint32_t highest_priority = 0;

	if (ptr->next == NULL) {
		return 0;
	}

	ptr = ptr->next;

	/*
	 * Loop thread list and find the highest priority
	 * thread.
	 */
	do {
		switch (ptr->thread->state) {
		case STATE_READY:
			if (ptr->thread->running_priority > highest_priority) {
				highest_priority = ptr->thread->running_priority;
				top = ptr;
			}

			break;
		case STATE_SLEEPING:
			/* Wake thread */
			if ((HAL_GetTick() - ptr->thread->sleep_counter) >= ptr->thread->sleep_duration) {
				ptr->thread->state = STATE_READY;
				ptr->thread->sleep_counter = 0;
				ptr->thread->sleep_duration = 0;

				if (ptr->thread->running_priority > highest_priority) {
					highest_priority = ptr->thread->running_priority;
					top = ptr;
				}
			}

			break;
		case STATE_WAITING:
			switch (ptr->thread->wait_condition) {
			case WAIT_MUTEX:
				//
				// mutex timed out, set a flag indicating this so when it
				// wakes it can check
				//
				if ((HAL_GetTick() - ptr->thread->mutex_counter) >= ptr->thread->mutex_timeout) {
					ptr->thread->state = STATE_READY;
					ptr->thread->wait_condition = WAIT_NONE;
					ptr->thread->mutex_waiting = NULL;
					ptr->thread->mutex_counter = 0;
					ptr->thread->mutex_timeout = 0;
					ptr->thread->mutex_priority = 0;

					if (ptr->thread->running_priority > highest_priority) {
						highest_priority = ptr->thread->running_priority;
						top = ptr;
					}
				}
				break;
			}

			break;
		}

		ptr->thread->running_priority++;
		ptr = ptr->next;
	} while (ptr);

	//
	// if this thread's priority was raised by a mutex held, we
	// want to remain at that priority until the mutex is released
	// and priority is reset to the running value
	//
	if (ptr->thread->mutex_priority > 0) {
		top->thread->running_priority = ptr->thread->mutex_priority;
	}
	else {
		top->thread->running_priority = top->thread->attr.priority;
	}

	if (top->thread != active_thread) {
		queued_thread = top->thread;
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

void thread_start_syscall_handler(uint32_t *args)
{
	struct thread_handle_t *handle = (struct thread_handle_t *)args[0];
	struct thread_list_t *head = &thread_list;

	while (head->next != NULL) {
		head = head->next;
	}

	struct thread_list_t *item = (struct thread_list_t *)malloc(sizeof(struct thread_list_t));
	if (item == NULL) {
		args[0] = E_MALLOC;
	}

	item->thread = handle->thread;
	item->next = NULL;
	head->next = item;

	args[0] = E_OK;
}

int thread_create(struct thread_handle_t *handle, struct thread_def_t *attr, void *data)
{
	struct thread_t *thread = (struct thread_t *)malloc(sizeof(struct thread_t));
	if (thread == NULL) {
		return E_THREAD_NOMEM;
	}

	memcpy(&thread->attr, attr, sizeof(struct thread_def_t));
	memset(&thread->stack, 0, sizeof(thread->stack));
	thread->state = STATE_READY;
	thread->wait_condition = WAIT_NONE;

	// Set stackPointer to end of stack - 64
	thread->stack_ptr = (uint32_t *)((uint32_t)thread->stack + sizeof(thread->stack) - 64);
	HW32_REG((uint32_t)thread->stack_ptr + 64) 	= (uint32_t)thread->attr.main;	// program counter
	HW32_REG((uint32_t)thread->stack_ptr + 68) 	= 0x01000000;					// initial xPSR
	HW32_REG((uint32_t)thread->stack_ptr + 40) 	= (uint32_t)data;				// pass context as first argument
	HW32_REG((uint32_t)thread->stack_ptr + 4) 	= 0x3;							// unprivileged thread
	HW32_REG((uint32_t)thread->stack_ptr) 		= 0xFFFFFFFDUL;					// initial EXC_RETURN

	handle->thread = thread;

	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_terminate(struct thread_handle_t *handle)
{
	syscall(SYSCALL_THREAD_TERMINATE);
}

void thread_terminate_syscall_handler(uint32_t *args)
{
	struct thread_handle_t *handle = (struct thread_handle_t *)args[0];
	struct thread_list_t *ptr = &thread_list;
	struct thread_list_t *tmp = NULL;

	//
	// move our pointer to the item before our thread
	//
	while (ptr->next != NULL && ptr->next->thread != handle->thread) {
		ptr = ptr->next;
	}

	//
	// reached end of list without finding our thread
	//
	if (ptr->next == NULL) {
		args[0] = E_OK;
	}

	if (ptr->next->next != NULL) {
		tmp = ptr->next;				// save our item
		ptr->next = ptr->next->next;	// move item after ours to present spot
		free(tmp);						// free that thread_list_t item
	}

	//
	// if the thread we just removed is currently running,
	// we force a context switch to get a new one going
	//
	if (active_thread == handle->thread) {
		thread_tick();
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}

	args[0] = E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_start_scheduler()
{
	syscall(SYSCALL_INIT);
}

uint32_t thread_start_scheduler_syscall_handler(uint32_t *args)
{
	idle_thread_def.main = &idler;
	idle_thread_def.priority = 1;
	strcpy((char *)idle_thread_def.name, "0:idler");

	if (E_OK == thread_create(&idle_thread_handle, &idle_thread_def, NULL)) {
		args[0] = (uint32_t)idle_thread_handle.thread->stack_ptr;
		return;
	}

	args[0] = 0;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
void thread_sleep(time_t time)
{
	syscall(SYSCALL_THREAD_SLEEP);
}

void thread_sleep_syscall_handler(uint32_t *args)
{
	unsigned long time = (unsigned long)args[0];

	active_thread->state = STATE_SLEEPING;
	active_thread->sleep_duration = time;
	active_thread->sleep_counter = 0;

	thread_yield_syscall_handler(NULL);
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
void thread_yield()
{
	syscall(SYSCALL_THREAD_SLEEP);
}

void thread_yield_syscall_handler(uint32_t *args)
{
	if (thread_tick()) {
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_wait_mutex(struct mutex_t *mutex, unsigned long timeout)
{
	syscall(SYSCALL_THREAD_WAIT_MUTEX);
}

void thread_wait_mutex_syscall_handler(uint32_t *args)
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

	args[0] = E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_release_mutex(struct mutex_t *mutex)
{
	syscall(SYSCALL_THREAD_RELEASE_MUTEX);
}

void thread_release_mutex_syscall_handler(uint32_t *args)
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

	args[0] = E_OK;
}
