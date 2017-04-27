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
 * @file	/micrortos/include/micrortos.h/micrortos.h
 * @author	robin
 * @date	Apr 22, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_MICRORTOS_H_
#define INCLUDE_MICRORTOS_H_

#define SYSCALL_INIT				      0
#define SYSCALL_THREAD_START     1
#define SYSCALL_THREAD_TERMINATE		2
#define SYSCALL_THREAD_SLEEP		  3
#define SYSCALL_THREAD_WAIT_MUTEX 4
#define SYSCALL_THREAD_YIELD      5
#define SYSCALL_THREAD_RELEASE_MUTEX 6

#define HW32_REG(ADDRESS) (*((volatile uint32_t *)(ADDRESS)))

#define E_OK 0
#define E_THREAD_INVALID 	1
#define E_THREAD_NOMEM   	2
#define E_THREAD_OOM		3
#define E_MALLOC			4
#define E_EMPTY				5
#define E_LOCKED			6
#define E_INVALID			7

#define THREAD_MAX_THREADS				10
#define THREAD_MAX_INTERRUPT_PRIORITY	5
#define THREAD_IDLE_NAME				"0:idler"

#endif /* INCLUDE_MICRORTOS_H_ */
