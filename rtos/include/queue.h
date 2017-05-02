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
 * @file	/micrortos/include/queue.h/queue.h
 * @author	robin
 * @date	Apr 22, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_QUEUE_H_
#define INCLUDE_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mutex.h>

struct queue_node_t {
	uint32_t item;
	struct queue_node_t *next;
};

struct queue_t {
	struct queue_node_t *head;
	struct queue_node_t *tail;
	struct mutex_t lock_head;
	struct mutex_t lock_tail;
};

int queue_init(struct queue_t *queue);
int queue_enqueue(struct queue_t *queue, uint32_t item);
int queue_dequeue(struct queue_t *queue, uint32_t *item);
int queue_destroy(struct queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_QUEUE_H_ */
