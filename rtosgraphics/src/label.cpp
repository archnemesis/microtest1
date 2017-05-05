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
		m_align(TextAlignLeft)
{

}

Label::Label(const char *labelText) :
		m_align(TextAlignLeft),
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
	case TextAlignLeft:
		break;
	case TextAlignRight:
		text_x = width() - text_width;
		break;
	case TextAlignCenter:
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

void Label::setAlignment(TextAlignment align)
{
	m_align = align;
}
