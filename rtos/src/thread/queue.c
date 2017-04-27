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
 * @file	/micrortos/src/thread/queue.c/queue.c
 * @author	robin
 * @date	Apr 22, 2017
 * @brief	[DESCRIPTION]
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <micrortos.h>
#include <heap.h>
#include <queue.h>

int queue_init(struct queue_t *queue)
{
	mutex_init(&queue->lock_head);
	mutex_init(&queue->lock_tail);

	struct queue_node_t *node = heap_malloc(sizeof(struct queue_node_t));

	if (node == NULL) {
		return E_MALLOC;
	}

	node->next = NULL;
	queue->head = node;
	queue->tail = node;

	return E_OK;
}

int queue_destroy(struct queue_t *queue)
{
	struct queue_node_t *node = queue->head;

	while (node->next != NULL) {
		queue->head = node->next;
		malloc_free(node);
	}

	malloc_free(queue->head);
	queue->head = NULL;
	queue->tail = NULL;

	return E_OK;
}

int queue_enqueue(struct queue_t *queue, uint32_t item)
{
	struct queue_node_t *node = heap_malloc(sizeof(struct queue_node_t));

	if (node == NULL) {
		return E_MALLOC;
	}

	node->item = item;
	node->next = NULL;

	mutex_lock(&queue->lock_tail);
	queue->tail->next = node;
	queue->tail = node;
	mutex_unlock(&queue->lock_tail);

	return E_OK;
}

int queue_dequeue(struct queue_t *queue, uint32_t *item)
{
	mutex_lock(&queue->lock_head);
	struct queue_node_t *node = queue->head;
	struct queue_node_t *new_head = node->next;

	if (new_head == NULL) {
		mutex_unlock(&queue->lock_head);
		return E_EMPTY;
	}

	*item = new_head->item;
	queue->head = new_head;

	mutex_unlock(&queue->lock_head);
	malloc_free(node);

	return E_OK;
}
