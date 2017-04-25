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
 * @file	/micrortos/include/syscall.h/syscall.h
 * @author	robin
 * @date	Apr 19, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_SYSCALL_H_
#define INCLUDE_SYSCALL_H_

#include <micrortos.h>
#include <thread.h>
#include <private/thread_p.h>

typedef void (*syscall_handler)(uint32_t *args);

#define syscall(code) asm volatile("SVC %[immediate]"::[immediate] "I" (code))

syscall_handler syscall_table[] = {
		&thread_start_scheduler_syscall_handler,
		&thread_create_syscall_handler,
		&thread_destroy_syscall_handler,
		&thread_sleep_syscall_handler,
		&thread_yield_syscall_handler
};

#endif /* INCLUDE_SYSCALL_H_ */
