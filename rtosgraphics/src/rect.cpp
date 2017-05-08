/*
 * rect.cpp
 *
 *  Created on: May 7, 2017
 *      Author: robin
 */

#include "rect.h"

int Rect::height() const {
	return m_height;
}

void Rect::setHeight(int height) {
	m_height = height;
}

int Rect::width() const {
	return m_width;
}

void Rect::setWidth(int width) {
	m_width = width;
}

int Rect::x() const {
	return m_x;
}

void Rect::setX(int x) {
	m_x = x;
}

int Rect::y() const {
	return m_y;
}

void Rect::setY(int y) {
	m_y = y;
}
