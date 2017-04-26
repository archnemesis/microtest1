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
 * @file	/micrortos/include/mutex.h/mutex.h
 * @author	robin
 * @date	Apr 21, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_MUTEX_H_
#define INCLUDE_MUTEX_H_

#include <thread.h>

struct mutex_t {
	uint8_t locked;
	struct thread_t *holder;
};

void mutex_init(struct mutex_t *mutex);
void __mutex_unlock(struct mutex_t *mutex);
void mutex_lock(struct mutex_t *mutex);
int mutex_lock_nonblocking(struct mutex_t *mutex);
void mutex_unlock(struct mutex_t *mutex);

#endif /* INCLUDE_MUTEX_H_ */
