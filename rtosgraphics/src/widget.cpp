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
 * @file	/microtest1/rtosgraphics/src/widget.cpp/widget.cpp
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */

#include "widget.h"

Widget::Widget() :
		m_x(0),
		m_y(0),
		m_width(0),
		m_height(0),
		m_minWidth(0),
		m_maxWidth(0),
		m_minHeight(0),
		m_maxHeight(0),
		m_vSizePolicy(SizePolicy::PreferredSizePolicy),
		m_hSizePolicy(SizePolicy::PreferredSizePolicy)
{

}

Widget::~Widget()
{

}

Widget::SizePolicy Widget::horizontalSizePolicy() const {
	return m_hSizePolicy;
}

Widget::SizePolicy Widget::verticalSizePolicy() const {
	return m_vSizePolicy;
}

void Widget::setVerticalSizePolicy(Widget::SizePolicy policy)
{
	m_vSizePolicy = policy;
}

void Widget::setHorizontalSizePolicy(Widget::SizePolicy policy)
{
	m_hSizePolicy = policy;
}

void Widget::setMinWidth(int width) {
}

void Widget::setMaxWidth(int width) {
}

void Widget::setMinHeight(int height) {
}

void Widget::setMaxHeight(int height) {
}

int Widget::minHeight() const {
	return 0;
}

int Widget::minWidth() const {
	return 0;
}

void Widget::draw(Canvas &canvas)
{

}

void Widget::setX(int x)
{
	m_x = x;
}

void Widget::setY(int y)
{
	m_y = y;
}

void Widget::setWidth(int width)
{
	m_width = width;
}

void Widget::setHeight(int height)
{
	m_height = height;
}

int Widget::x() const
{
	return m_x;
}

int Widget::y() const
{
	return m_y;
}

int Widget::width() const
{
	return m_width;
}

int Widget::height() const
{
	return m_height;
}

int Widget::widthHint() const
{
	if (m_minWidth > 0) {
		return m_minWidth;
	}
	return -1;
}

int Widget::heightHint() const
{
	if (m_minHeight > 0) {
		return m_minHeight;
	}

	return -1;
}

bool Widget::hasHeightForWidth() const
{
	return false;
}

int Widget::heightForWidth(int width) const
{
	return -1;
}

int Widget::maxHeight() const
{
	return INT16_MAX;
}

int Widget::maxWidth() const
{
	return INT16_MAX;
}
