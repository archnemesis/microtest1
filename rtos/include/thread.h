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

#ifdef __cplusplus
extern "C" {
#endif

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
	volatile uint32_t *stack_ptr;
	volatile uint32_t event_flags;
	uint32_t event_wait_mask;
	enum thread_state state;
	enum thread_wait_condition wait_condition;
	uint32_t running_priority;
	struct thread_def_t attr;
	uint32_t sleep_counter;
	uint32_t sleep_duration;
	struct mutex_t *mutex_waiting;
	uint32_t mutex_timeout;
	uint32_t mutex_counter;
	uint32_t mutex_priority;
	uint32_t stack[512];
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

__attribute__ ((noinline))
int thread_start_scheduler();

/**
 * Create a new entry in the thread list for the given thread_attr_t.
 *
 * @param handle the new thread pointer will be stored here
 * @param attr the new thread will use these attributes
 * @param data pointer to thread data passed as first argument
 * @return integer error code
 */
int thread_create(struct thread_handle_t *handle, struct thread_def_t *attr, void *data);

/**
 * Create a static thread.
 */
int thread_create_static(struct thread_t *thread, void *data);

/**
 * Terminate a thread and remove it from the thread list.
 *
 * @param handle the thread handle
 * @return integer error code
 */
int thread_destroy(struct thread_t *thread);

/**
 * Add a thread to the global thread queue and run it.
 *
 * @param handle
 * @return integer error code
 */
__attribute__ ((noinline))
int thread_start(struct thread_t *thread);

/**
 * Remove a thread from the global thread queue.
 *
 * @param handle
 * @return integer error code
 */
__attribute__ ((noinline))
int thread_terminate(struct thread_t *thread);

/**
 * Wait for a thread to finish.
 */
//int thread_join(struct thread_handle_t *handle, uint32_t timeout);

/**
 * These functions are called from within threads.
 */

/**
 * Put the current thread to sleep for msecs milliseconds.
 */
__attribute__ ((noinline))
void thread_sleep(unsigned long time);

/**
 * Trigger an early context switch.
 */
__attribute__ ((noinline))
void thread_yield(void);

/**
 * Exit a thread, removing it from the thread list.
 */
void thread_exit(void);

/**
 * Wait for a mutex to become available.
 */
__attribute__ ((noinline))
int thread_wait_mutex(struct mutex_t *mutex, unsigned long timeout);

/**
 * Wait for an event.
 */
__attribute__ ((noinline))
int thread_wait_event(uint32_t event_mask);

/**
 * Release a mutex and trigger higher priority waiting tasks.
 */
__attribute__ ((noinline))
int thread_release_mutex(struct mutex_t *mutex);

/**
 * Send a notification to a thread.
 */
int thread_notify(struct thread_t *thread, uint32_t event_bits);

/**
 * Get handle for a thread.
 */
struct thread_t *thread_get_current();

#ifdef __cplusplus
}

class Thread {
public:
	Thread(unsigned int priority = 1);
	virtual ~Thread();
	void start();
	void terminate();
	void notify(uint32_t event_mask);

protected:
	virtual void run() = 0;
	void sleep(unsigned int time = 0);
	void wrapper();
	void waitForEvent(uint32_t event_mask);

	struct thread_t m_thread;
	unsigned int m_priority;
private:
	static void start_wrapper(void *ptr);
};

#endif

#endif /* INCLUDE_THREAD_H_ */
