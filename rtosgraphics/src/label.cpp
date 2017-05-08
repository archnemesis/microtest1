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
 * @file	/microtest1/rtosgraphics/src/label.cpp/label.cpp
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */

#include <string.h>
#include "label.h"
#include "heap.h"

Label::Label() :
		m_align(Font::TextAlignment::Left),
		m_textWrapping(false)
{

}

Label::Label(const char *labelText) :
		m_align(Font::TextAlignment::Left),
		m_textWrapping(false),
		m_text(labelText)
{

}

void Label::setText(std::string const& text)
{
	m_text = text;
}

void Label::setText(const char *text)
{
	m_text = text;
}

void Label::draw(Canvas& canvas)
{
	int text_width = m_font.stringWidth(m_text.c_str());
	int text_height = m_font.stringHeight();
	int text_y = (height() - text_height) / 2;
	int text_x = x();

	switch (m_align) {
	case Font::TextAlignment::Left:
		break;
	case Font::TextAlignment::Right:
		text_x = width() - text_width;
		break;
	case Font::TextAlignment::Center:
		text_x = (width() - text_width) / 2;
		break;
	}

	canvas.setFont(m_font);
	canvas.setColor(m_color);
	canvas.drawText(x() + text_x, y() + text_y, m_text.c_str());
}

void Label::setFont(Font& font)
{
	m_font = font;
}

void Label::setColor(const Color& color)
{
	m_color = color;
}

void Label::setAlignment(Font::TextAlignment align)
{
	m_align = align;
}

bool Label::hasHeightForWidth() const {
	return m_textWrapping;
}

int Label::heightForWidth(int width) const {
	if (textWrapping()) {
		//
		// calculate preferred height based on width
		//
		Rect boundingRect = m_font.boundingRect(width, m_text, m_align);
		return boundingRect.height();
	}
	else {
		return m_font.stringHeight();
	}
}

bool Label::textWrapping() const {
	return m_textWrapping;
}

int Label::widthHint() const {
	if (!m_textWrapping) {
		return m_font.stringWidth(m_text);
	}
	return -1;
}

int Label::heightHint() const {
	return m_font.stringHeight();
}

void Label::setTextWrapping(bool textWrapping) {
	m_textWrapping = textWrapping;
}
