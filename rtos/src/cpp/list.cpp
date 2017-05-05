/**
 * microtest1
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
 * @file	/microtest1/rtos/src/cpp/list_t.cpp/list_t.cpp
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */

#include "list.h"

template <typename T>
List<T>::List()
{
	m_head = 0;
	m_tail = 0;
}

template <typename T>
List<T>::~List()
{
	if (!isEmpty()) {
		ListNode<T> *current = m_head;
		ListNode<T> *temp = 0;

		while (current != 0) {
			temp = current;
			current = current->m_next;
			delete temp;
		}
	}
}

template <typename T>
bool List<T>::isEmpty()
{
	return (m_head == 0 && m_tail == 0);
}

template <typename T>
void List<T>::append(T data)
{
	if (isEmpty()) {
		ListNode<T> *ptr = new ListNode<T>(data);
		m_head = ptr;
		m_tail = ptr;
	}
	else {
		ListNode<T> *ptr = new ListNode<T>(data);
		m_tail->m_next = ptr;
		m_tail = ptr;
	}
}

template <typename T>
void List<T>::remove(T data)
{
	if (!isEmpty()) {
		ListNode<T> *ptr = m_head;
		ListNode<T> *tmp;

		if (m_head->data() == data) {
			tmp = m_head;
			m_head = m_head->m_next;
			delete tmp;
			return;
		}

		while (ptr->m_next != 0) {
			if (ptr->m_next->data() == data) {
				tmp = ptr->m_next;
				ptr->m_next = tmp->m_next;
				delete tmp;
				return;
			}
			ptr = ptr->m_next;
		}
	}
}

template <typename T>
unsigned int List<T>::length()
{
	unsigned int i = 0;
	ListNode<T> *ptr = m_head;

	while (ptr != 0) {
		i++;
		ptr = ptr->m_next;
	}

	return i;
}

template <typename T>
T List<T>::at(int index)
{
	int i = 0;
	ListNode<T> *ptr = m_head;

	for (; i < length(); i++) {
		if (i == index) {
			return ptr->data();
		}
		ptr = ptr->m_next;
	}
	//
	// TODO: call error handler
	//
}
