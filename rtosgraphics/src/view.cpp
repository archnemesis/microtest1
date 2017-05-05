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
 * @file	/microtest1/rtosgraphics/src/view.cpp/view.cpp
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */


#include "view.h"
#include "color.h"
#include "font.h"
#include "heap.h"

View::View() :
		Widget(),
		m_marginTop(0),
		m_marginBottom(0),
		m_marginLeft(0),
		m_marginRight(0),
		m_titleSeparatorWidth(1)
{
	m_mainWidget = 0;
	m_title[0] = 0;
}

void View::draw(Canvas& canvas)
{
	unsigned int titleHeight = 28;

	canvas.setColor(m_backgroundColor);
	canvas.clear();
	canvas.setColor(m_titleColor);
	canvas.drawFilledRect(0, 0, m_width, titleHeight);

	canvas.setColor(m_titleSeparatorColor);
	canvas.drawLine(0, titleHeight, m_width, titleHeight, 1);

	canvas.setColor(m_titleTextColor);
	canvas.setFont(m_titleFont);
	canvas.drawText(5, 2, "PDCL OS");

	if (m_mainWidget != 0) {
		int mainWidget_width = width() - (marginLeft() + marginRight());
		int mainWidget_height = height() - (28 + marginTop() + marginLeft());
		int mainWidget_x = marginLeft();
		int mainWidget_y = marginTop() + 28;
		m_mainWidget->setX(mainWidget_x);
		m_mainWidget->setY(mainWidget_y);
		m_mainWidget->setWidth(mainWidget_width);
		m_mainWidget->setHeight(mainWidget_height);
		//Canvas widget_canvas(mainWidget_width, mainWidget_height);
		m_mainWidget->draw(canvas);
		//widget_canvas.blit(canvas, mainWidget_x, mainWidget_y, mainWidget_width, mainWidget_height);
	}
}

const Color& View::backgroundColor() const {
	return m_backgroundColor;
}

void View::setBackgroundColor(const Color& backgroundColor) {
	m_backgroundColor = backgroundColor;
}

Widget* View::mainWidget() {
	return m_mainWidget;
}

void View::setMainWidget(Widget* mainWidget) {
	m_mainWidget = mainWidget;
}

int View::marginBottom() const {
	return m_marginBottom;
}

void View::setMarginBottom(int marginBottom) {
	m_marginBottom = marginBottom;
}

int View::marginLeft() const {
	return m_marginLeft;
}

void View::setMarginLeft(int marginLeft) {
	m_marginLeft = marginLeft;
}

int View::marginRight() const {
	return m_marginRight;
}

void View::setMarginRight(int marginRight) {
	m_marginRight = marginRight;
}

int View::marginTop() const {
	return m_marginTop;
}

void View::setMarginTop(int marginTop) {
	m_marginTop = marginTop;
}

void View::setMargins(int top, int left, int bottom, int right)
{
	setMarginTop(top);
	setMarginBottom(bottom);
	setMarginLeft(left);
	setMarginRight(right);
}

void View::setMargins(int margins)
{
	setMarginTop(margins);
	setMarginBottom(margins);
	setMarginLeft(margins);
	setMarginRight(margins);
}

const char* View::title() const {
	return m_title;
}

const Color& View::titleColor() const {
	return m_titleColor;
}

void View::setTitleColor(const Color& titleColor) {
	m_titleColor = titleColor;
}

const Color& View::titleSeparatorColor() const {
	return m_titleSeparatorColor;
}

void View::setTitleSeparatorColor(const Color& titleSeparatorColor) {
	m_titleSeparatorColor = titleSeparatorColor;
}

int View::titleSeparatorWidth() const {
	return m_titleSeparatorWidth;
}

void View::setTitleSeparatorWidth(int titleSeparatorWidth) {
	m_titleSeparatorWidth = titleSeparatorWidth;
}

const Color& View::titleTextColor() const {
	return m_titleTextColor;
}

void View::setTitleTextColor(const Color& titleTextColor) {
	m_titleTextColor = titleTextColor;
}

const Font& View::titleFont() const
{
	return m_titleFont;
}

void View::setTitleFont(const Font& font)
{
	m_titleFont = font;
}
