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
	while (1) {

	}
}

uint32_t thread_tick(void);

void SysTick_Handler()
{
	HAL_IncTick();

	if (thread_tick() != 0x0) {
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
}

void PendSV_Handler()
{
	__asm__ __volatile__(
			" MRS 		R0, PSP						\n\t"	// Save PSP to R0
			" ISB									\n\t"	// Instruction Sync Barrier
			"										\n\t"
			" MOV		R2, LR						\n\t"
			" MRS		R3, CONTROL					\n\t"
			" STMDB 	R0!, {R2-R11}				\n\t"	// Store R4-R11 onto the process stack
			" 										\n\t"
			" TST		R14, #0x10					\n\t"	// Save FPU Context
			" IT		EQ							\n\t"
			" VSTMDBEQ	R0!, {S16-S31}				\n\t"
			" 										\n\t"
			" LDR 		R3, =active_thread			\n\t"	// Get current task pointer
			" LDR		R2, [R3]					\n\t"
			" STR		R0, [R2]					\n\t"	// Update the current task stack pointer
			" STMDB		SP!, {R14}					\n\t"	// Save our EXC_RETURN value
			" 										\n\t"
			" MOV 		R0, #10						\n\t"	// Block lower-priority interrupts
			" CPSID		I							\n\t"	// Errata Workaround
			" MSR		BASEPRI, R0					\n\t"
			" 										\n\t"
			" DSB									\n\t"
			" ISB									\n\t"
			" CPSIE		I							\n\t"	// Errata Workaround
			" 										\n\t"
			" BL 		thread_switch_context		\n\t"
			" 										\n\t"
			" MOV		R0, #0						\n\t"
			" MSR		BASEPRI, R0					\n\t"
			" LDMIA		SP!, {R14}					\n\t"	// Restore our EXC_RETURN value
			" 										\n\t"
			" LDR 		R3, =active_thread			\n\t"	// Get next task pointer
			" LDR		R1, [R3]					\n\t"	// Load the address of the Stack Pointer
			" LDR		R0, [R1]					\n\t"	// Load the Stack Pointer
			" 										\n\t"
			" TST		R14, #0x10					\n\t"	// Restore FPU Context
			" IT		EQ							\n\t"
			" VLDMIAEQ	R0!, {S16-S31}				\n\t"
			" 										\n\t"
			" LDMIA		R0!, {R2-R11}				\n\t"
			" MOV		LR, R2						\n\t"
			" MSR		CONTROL, R3					\n\t"
			" ISB									\n\t"
			" 										\n\t"
			" MSR		PSP, R0						\n\t"
			" BX		LR							\n\t"
			".align 4								\n\t"
	);
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
		if (thread_list[i] == NULL) break;

		thread = thread_list[i];

		if (thread != NULL) {
			switch (thread->state) {
			case STATE_READY:
				if (thread->running_priority > highest_priority) {
					highest_priority = thread->running_priority;
					top = thread;
				}

				break;
			case STATE_TERMINATED:
				//
				// thread was terminated, remove it from the thread list
				//
				thread_list[i] = NULL;
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
					case WAIT_MEMORY:
					{
						//
						// waiting on a DMA transfer...
						//
						break;
					}
					case WAIT_MUTEX:
					{
						//
						// mutex timed out, set a flag indicating this so when it
						// wakes it can check
						//
						if (thread->mutex_timeout > 0 && (HAL_GetTick() - thread->mutex_counter) >= thread->mutex_timeout) {
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
					case WAIT_EVENT:
					{
						if ((thread->event_flags & thread->event_wait_mask) == thread->event_wait_mask) {
							thread->state = STATE_READY;
							thread->wait_condition = WAIT_NONE;

							//
							// clear the awaited event bits from the event
							// register with LDREX/STREX to make sure we don't
							// clear bits set for other events in ISRs
							//
							__asm__ __volatile__(
									"	LDR		R0, %[t]				\n\t"
									"1:	LDREX	R2, [R0, #4]			\n\t"
									"	BIC		R2, R2, R1				\n\t"
									"	STREX	R3, R2, [R0, #4]		\n\t"
									"	CMP		R3, #1					\n\t"
									"	IT		EQ						\n\t"
									"	BEQ		1b						\n\t"
									".align 4"
								: : [t] "m" (thread), [e] "r" (thread->event_wait_mask) : "r0", "r2", "r3"
							);

							//
							// clear the wait mask
							//
							thread->event_wait_mask = 0;

							if (thread->running_priority > highest_priority) {
								highest_priority = thread->running_priority;
								top = thread;
							}
						}
						break;
					}
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

#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_notify(struct thread_t *thread, uint32_t event_bits)
{
	__asm__ __volatile__(
			"1:	LDREX	R2, [R0, #4]			\n\t"
			"	ORR		R2, R2, R1				\n\t"
			"	STREX	R3, R2, [R0, #4]		\n\t"
			"	CMP		R3, #1					\n\t"
			"	IT		EQ						\n\t"
			"	BEQ		1b						\n\t"
			".align 4"
	::: "r2", "r3", "cc");

	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_wait_event(uint32_t event_mask)
{
	syscall(SYSCALL_THREAD_WAIT_EVENT);
}

uint32_t thread_wait_event_syscall_handler(uint32_t *args)
{
	struct thread_t *thread = active_thread;
	unsigned int event_mask = args[0];

//	if ((thread->event_flags & event_mask) == event_mask) {
//
//		//
//		// clear the awaited event bits from the event
//		// register with LDREX/STREX to make sure we don't
//		// clear bits set for other events in ISRs
//		//
//		__asm__ __volatile__(
//				"	LDR		R1, %[t]				\n\t"
//				"1:	LDREX	R2, [R1, #4]			\n\t"
//				"	BIC		R2, R2, R3				\n\t"
//				"	STREX	R3, R2, [R1, #4]		\n\t"
//				"	CMP		R3, #1					\n\t"
//				"	IT		EQ						\n\t"
//				"	BEQ		1b						\n\t"
//				".align 4"
//			: : [t] "m" (thread), [e] "r" (thread->event_wait_mask)
//		);
//
//		return E_OK;
//	}

	thread->state = STATE_WAITING;
	thread->wait_condition = WAIT_EVENT;
	thread->event_wait_mask = event_mask;
	thread_yield_syscall_handler(NULL);

	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_start(struct thread_t *thread)
{
	syscall(SYSCALL_THREAD_START);
}

uint32_t thread_start_syscall_handler(uint32_t *args)
{
	struct thread_t *thread = (struct thread_t *)args[0];
	unsigned int i = 0;

	for (; i < THREAD_MAX_THREADS; i++) {
		if (thread_list[i] == NULL) {
			thread_list[i] = thread;
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

	thread_initialize(thread, data);

	handle->thread = thread;

	return E_OK;
}

int thread_create_static(
		struct thread_t *thread,
		void *data)
{
	thread_initialize(thread, data);
	return E_OK;
}

void thread_initialize(struct thread_t *thread, void *data)
{
	thread->running_priority = thread->attr.priority;
	thread->state = STATE_READY;
	thread->wait_condition = WAIT_NONE;
	thread->event_flags = 0;
	thread->event_wait_mask = 0;

	//
	// The stack must be set up such that the processor will unstack it
	// properly before executing the thread. We set up the initial PC, SP and
	// pass arguments via the stacked r0-r3 registers.
	//
	thread->stack_ptr = (uint32_t *)((uint32_t)thread->stack + sizeof(thread->stack) - 72);

	HW32_REG((uint32_t)thread->stack_ptr + 64) 	= (uint32_t)thread->attr.main;	// program counter
	HW32_REG((uint32_t)thread->stack_ptr + 68) 	= 0x01000000;					// initial xPSR
	HW32_REG((uint32_t)thread->stack_ptr + 40) 	= (uint32_t)data;				// pass context as first argument
	HW32_REG((uint32_t)thread->stack_ptr + 4) 	= 0x3;							// unprivileged thread
	HW32_REG((uint32_t)thread->stack_ptr) 		= 0xFFFFFFFDUL;					// initial EXC_RETURN
}

int thread_destroy(struct thread_t *thread)
{
	if (thread == NULL) {
		return E_INVALID;
	}

	heap_free(thread);
	thread = NULL;
	return E_OK;
}

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__ ((noinline))
int thread_terminate(struct thread_t *thread)
{
	syscall(SYSCALL_THREAD_TERMINATE);
}

uint32_t thread_terminate_syscall_handler(uint32_t *args)
{
	struct thread_t *thread = (struct thread_t *)args[0];

	unsigned int i = 0;

	for (; i < THREAD_MAX_THREADS; i++) {
		if (thread_list[i] == thread) {
			thread_list[i]->state = STATE_TERMINATED;

			if (active_thread == thread) {
				thread_tick();
				SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
			}

			return E_OK;
		}
	}

	return E_OK;
}

void thread_exit()
{
	thread_terminate(active_thread);
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
	unsigned int i = 0;
	idler_thread.attr.main = &idler;
	idler_thread.attr.priority = 1;
	strcpy((char *)idler_thread.attr.name, "0:idler");

	if (E_OK == thread_create_static(&idler_thread, NULL)) {
		for (; i < THREAD_MAX_THREADS; i++) {
			if (thread_list[i] == NULL) {
				thread_list[i] = &idler_thread;
				break;
			}
		}

		active_thread = &idler_thread;
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
	active_thread->sleep_counter = HAL_GetTick();

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
		active_thread->state = STATE_WAITING;
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

		struct thread_t *thread = NULL;
		struct thread_t *tmp;
		unsigned int i = 0;
		unsigned int p = 0;

		for (; i < THREAD_MAX_THREADS; i++) {
			tmp = thread_list[i];

			if (tmp->state == STATE_WAITING) {
				if (tmp->wait_condition == WAIT_MUTEX) {
					if (tmp->attr.priority > p) {
						thread = tmp;
						p = tmp->attr.priority;
					}
				}
			}
		}

		if (thread != NULL) {
			thread->state = STATE_READY;
			thread->wait_condition = WAIT_NONE;
			thread->mutex_waiting = NULL;
			thread->mutex_counter = 0;
			thread->mutex_timeout = 0;
			thread->mutex_priority = 0;

			if (thread_tick()) {
				SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
			}
		}
	}

	return E_OK;
}
