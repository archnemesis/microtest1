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
 * @file	/micrortos/src/thread/ringbuffer.c/ringbuffer.c
 * @author	robin
 * @date	Apr 23, 2017
 * @brief	[DESCRIPTION]
 */

#include <ringbuffer.h>
#include <micrortos.h>
#include <stddef.h>

int ringbuffer_init(struct ringbuffer_t *rb, size_t size, unsigned int elements)
{
	rb->buffer = malloc(size * (elements + 1));

	if (rb->buffer == NULL) {
		return E_MALLOC;
	}

	rb->head = 0;
	rb->tail = 0;
	rb->size = size;
	rb->elements = elements;

	mutex_init(&rb->head_lock);
	mutex_init(&rb->tail_lock);

	return E_OK;
}

bool ringbuffer_empty(struct ringbuffer_t *rb)
{
	if (rb->head == rb->tail) {
		return true;
	}

	return false;
}

bool ringbuffer_full(struct ringbuffer_t *rb)
{
	if ((rb->head + 1) % (rb->elements + 1) == rb->tail) {
		return true;
	}

	return false;
}

int ringbuffer_available(struct ringbuffer_t *rb)
{
	if (rb->head < rb->tail) {
		return (rb->head + (rb->elements + 1)) - rb->tail;
	}
	else {
		return rb->head - rb->tail;
	}
}

int ringbuffer_get(struct ringbuffer_t *rb, uint8_t *dest, size_t size)
{
	mutex_lock(&rb->tail_lock);

	int i = 0;
	int available = ringbuffer_available(rb);

	if (available > 0) {
		for (; i < available && i < size; i++) {
			dest[i] = rb->buffer[rb->tail];
			rb->tail = (rb->tail + 1) % (rb->elements + 1);
		}
	}

	mutex_unlock(&rb->tail_lock);
	return i;
}

int ringbuffer_put(struct ringbuffer_t *rb, uint8_t *in, size_t size)
{
	mutex_lock(&rb->head_lock);

	int i = 0;

	while (!ringbuffer_full(rb)) {
		rb->buffer[rb->head] = in[i++];
		rb->head = (rb->head + 1) % (rb->elements + 1);
		if (i == size) break;
	}

	mutex_unlock(&rb->head_lock);

	return i;
}
