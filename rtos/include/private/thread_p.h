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
 * @file	/micrortos/include/private/thread_p.h/thread_p.h
 * @author	robin
 * @date	Apr 20, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_PRIVATE_THREAD_P_H_
#define INCLUDE_PRIVATE_THREAD_P_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <thread.h>

extern struct thread_t *thread_list[THREAD_MAX_THREADS];
extern struct thread_t *active_thread;

uint32_t thread_tick();
uint32_t *thread_start_scheduler_syscall_handler(uint32_t *args);
uint32_t thread_start_syscall_handler(uint32_t *args);
uint32_t thread_terminate_syscall_handler(uint32_t *args);
uint32_t thread_sleep_syscall_handler(uint32_t *args);
uint32_t thread_yield_syscall_handler(uint32_t *args);
uint32_t thread_wait_mutex_syscall_handler(uint32_t *args);
uint32_t thread_release_mutex_syscall_handler(uint32_t *args);
uint32_t thread_wait_event_syscall_handler(uint32_t *args);
void thread_switch_context();

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_PRIVATE_THREAD_P_H_ */
