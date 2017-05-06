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
 * @file	/microtest1/rtos/include/list.h/list.h
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_LIST_H_
#define INCLUDE_LIST_H_

#include <iterator>
#include "list_node.h"

template<class T>
class List {
public:
	List();
	~List();

	class iterator
	{
		friend class ListNode<T>;

		public:
			typedef iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;
			iterator(ListNode<T> *node) : m_node(node) { }
			self_type operator++() { m_node = m_node->m_next; return *this; }
			self_type operator++(int junk) { m_node = m_node->next; return *this; }
			reference operator*() { return m_node->data(); }
			pointer operator->() { return m_node->data(); }
			bool operator==(const self_type& rhs) { return m_node->data() == rhs.m_node->data(); }
			bool operator!=(const self_type& rhs) { return m_node->data() != rhs.m_node->data(); }
		private:
			ListNode<T> *m_node;
	};

	class const_iterator
	{
		friend class ListNode<T>;

		public:
			typedef const_iterator self_type;
			typedef T value_type;
			typedef T& reference;
			typedef T* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;
			const_iterator(ListNode<T> *node) : m_node(node) { }
			self_type operator++() { m_node = m_node->m_next; return *this; }
			self_type operator++(int junk) { m_node = m_node->m_next; return *this; }
			const reference operator*() { return m_node->data(); }
			const pointer operator->() { return m_node->data(); }
			bool operator==(const self_type& rhs) { return m_node->data() == rhs.m_node->data(); }
			bool operator!=(const self_type& rhs) { return m_node->data() != rhs.m_node->data(); }
		private:
			ListNode<T> *m_node;
	};

	bool isEmpty() const;
	bool empty() const { return isEmpty(); }
	unsigned int length();
	unsigned int size() { return length(); }
	void insert(int index, T data);
	void insert(T data) { insert(0, data); }
	void append(T data);
	void remove(T data);
	void remove(int index);
	void erase(T data) { remove(data); }
	void clear();
	T at(int index);
	iterator begin() { return iterator(m_head); }
	iterator end() { return iterator(m_tail); }
	const_iterator begin() const { return const_iterator(m_head); }
	const_iterator end() const { return const_iterator(m_tail); }

protected:
	ListNode<T> *m_head;
	ListNode<T> *m_tail;
};

template <class T>
List<T>::List()
{
	m_head = 0;
	m_tail = 0;
}

template <class T>
List<T>::~List()
{
	clear();
}

template <class T>
void List<T>::clear()
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

	m_head = 0;
	m_tail = 0;
}

template <class T>
bool List<T>::isEmpty() const
{
	return (m_head == 0 && m_tail == 0);
}

template <class T>
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

template <class T>
void List<T>::insert(int index, T data)
{
	ListNode<T> *ptr = new ListNode<T>(data);

	if (index == 0) {
		ptr->m_next = m_head;
		m_head = ptr;
	}
	else {
		int i = 1;
		ListNode<T> *tmp = m_head;
		while (i < index) {
			tmp = m_head->m_next;
			i++;
		}
		ptr->m_next = tmp->m_next;
		tmp->m_next = ptr;
	}
}

template <class T>
void List<T>::remove(int index)
{
	int i = 0;
	ListNode<T> *ptr = m_head;

	if (index == 0) {
		m_head = m_head->m_next;
		delete ptr;
	}
	else {
		ListNode<T> *tmp;
		for (i = 1; i < index; i++) {
			ptr = ptr->m_next;
		}

		tmp = ptr->m_next;
		if (ptr->m_next != 0) {
			//
			// if there are more items after the one we want to remove
			//
			ptr->m_next = ptr->m_next->m_next;
		}
		else {
			//
			// this was the second-to-last item, now is last
			//
			ptr->m_next = 0;
		}

		delete tmp;
	}
}

template <class T>
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

template <class T>
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

template <class T>
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

#endif /* INCLUDE_LIST_H_ */
