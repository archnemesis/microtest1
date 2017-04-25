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
 * @file	/micrortos/include/thread.h/thread.h
 * @author	robin
 * @date	Apr 16, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_THREAD_H_
#define INCLUDE_THREAD_H_

#include <stdint.h>
#include <micrortos.h>
#include <mutex.h>

enum thread_state {
	STATE_INVALID = 0,
	STATE_READY,
	STATE_RUNNING,
	STATE_SUSPENDED,
	STATE_WAITING,
	STATE_SLEEPING,
	STATE_TERMINATED
};

enum thread_wait_condition {
	WAIT_NONE = 0,
	WAIT_SEMAPHORE,
	WAIT_MUTEX,
	WAIT_EVENT,
	WAIT_MEMORY
};

struct thread_def_t {
	char		name[32];
	uint32_t	priority;
	void		(*main)(void *);
};

struct thread_t {
	uint32_t 					*stack_ptr;
	enum thread_state 			state;
	enum thread_wait_condition	wait_condition;
	struct thread_def_t			attr;
	uint32_t 					sleep_counter;
	uint32_t 					sleep_duration;
	uint64_t 					stack[1024];
	struct mutex_t				*mutex_waiting;
	uint32_t					mutex_timeout;
	uint32_t					mutex_counter;
};

struct thread_handle_t {
	struct thread_t	*thread;
};

struct thread_list_t {
	struct thread_t *thread;
	struct thread_list_t *next;
	uint32_t priority;
};

/**
 * Thread Management Routines
 * To be called only from handler mode!
 */

/**
 * Create a new entry in the thread list for the given thread_attr_t
 * and start the thread.
 *
 * @param handle the new thread pointer will be stored here
 * @param attr the new thread will use these attributes
 * @param data pointer to thread data passed as first argument
 * @return integer error code
 */
__attribute__ ((noinline))
int thread_create(struct thread_handle_t *handle, struct thread_def_t *attr, void *data);

/**
 * Terminate a thread and remove it from the thread list.
 *
 * @param handle the thread handle
 * @return integer error code
 */
int thread_destroy(struct thread_handle_t *handle);

/**
 * Wait for a thread to finish.
 */
int thread_join(struct thread_handle_t *handle, uint32_t timeout);

int thread_def_setname(struct thread_def_t *def, char *name);
int thread_def_getname(struct thread_def_t *def, char *buffer);
int thread_def_setpriority(struct thread_def_t *def, uint32_t priority);
int thread_def_getpriority(struct thread_def_t *def, uint32_t *priority);
int thread_def_setmain(struct thread_def_t *def, void (*main)(void *));

/**
 * These functions are called from within threads.
 */

/**
 * Put the current thread to sleep for msecs milliseconds.
 */
void thread_sleep(time_t time);

/**
 * Trigger an early context switch.
 */
void thread_yield(void);

/**
 * Exit a thread, removing it from the thread list.
 */
void thread_exit(void);

/**
 * Wait for a mutex to become available.
 */
void thread_wait_for_mutex(struct mutex_t *mutex, uint32_t timeout);

/**
 * Get handle for a thread.
 */
int thread_get_current(struct thread_handle_t *handle);

#endif /* INCLUDE_THREAD_H_ */
