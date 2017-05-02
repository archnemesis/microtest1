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
 * @file	/micrortos/include/ringbuffer.h/ringbuffer.h
 * @author	robin
 * @date	Apr 23, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_RINGBUFFER_H_
#define INCLUDE_RINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <mutex.h>

struct ringbuffer_t {
	uint8_t *buffer;
	uint32_t head;
	uint32_t tail;
	uint32_t size;
	uint32_t elements;
	struct mutex_t head_lock;
	struct mutex_t tail_lock;
};

/**
 * Initialize a ring buffer to default empty state.
 *
 * @return int OS_STATUS
 */
int ringbuffer_init(struct ringbuffer_t *rb, size_t size);

/**
 * Initialize a ring buffer using static memory.
 */
int ringbuffer_init_static(struct ringbuffer_t *rb, size_t size, uint8_t *buffer);

/**
 * Free memory used by the buffer.
 *
 * @return int OS_STATUS
 */
int ringbuffer_destroy(struct ringbuffer_t *rb);

/**
 * Returns true if the buffer is empty.
 *
 * @return bool
 */
bool ringbuffer_empty(struct ringbuffer_t *rb);

/**
 * Returns true if the buffer is full.
 *
 * @return bool
 */
bool ringbuffer_full(struct ringbuffer_t *rb);

/**
 * Returns the number of elements available.
 */
unsigned int ringbuffer_available(struct ringbuffer_t *rb);

/**
 * Get up to size bytes from the buffer and copy to dest.
 *
 * @return int bytes returned
 */
unsigned int ringbuffer_get(struct ringbuffer_t *rb, uint8_t *dest, size_t size);

/**
 * Get up to size bytes from the buffer and copy to dest. This should
 * only be used in an ISR. Multiple ISRs may write to the same buffer,however
 * their priorities should be the same to prevent them interrupting each other
 * during writes.
 *
 * @return bytes returned
 */
unsigned int ringbuffer_get_from_isr(struct ringbuffer_t *rb, uint8_t *dest, size_t size);

/**
 * Put size bytes from in to buffer.
 *
 * @return int bytes written
 */
unsigned int ringbuffer_put(struct ringbuffer_t *rb, uint8_t *in, size_t size);

/**
 * Put size bytes from in to buffer. This should only be used in an ISR.
 * Multiple ISRs may write to the same buffer, however their priorities
 * should be the same to prevent them interrupting each other during writes.
 *
 * @return int bytes written
 */
unsigned int ringbuffer_put_from_isr(struct ringbuffer_t *rb, uint8_t *in, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_RINGBUFFER_H_ */
