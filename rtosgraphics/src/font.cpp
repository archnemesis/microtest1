/**
 * PDCL_OS
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
 * @file	/PDCL_OS/src/gfx/font.cpp/font.cpp
 * @author	robin
 * @date	Feb 3, 2017
 * @brief	[DESCRIPTION]
 */

#include "font.h"
#include <string.h>

Font::Font() :
		m_font(0),
		m_scale(1.0)
{
	setFont("Ubuntu Regular 22");
}

Font::Font(const char *fontName) :
		m_font(0),
		m_scale(1.0)
{
	setFont(fontName);
}

void Font::setFont(const char *fontName)
{
	strcpy(m_fontName, fontName);
	const struct mf_font_s *font = mf_find_font(fontName);
	if (!font) {
		// TODO: error handler called here
	}
	m_font = font;

	//struct mf_font_s *scaled_font = mf_scale_font(m_font_scaled, m_font, )
}

void Font::setScaling(float scale)
{
	m_scale = scale;
}

int Font::stringWidth(const char *string) const
{
	int i;
	int l = strlen(string);
	int w = 0;

	for (i = 0; i < l; i++) {
		w += mf_character_width(m_font, string[i]);
	}

	return w;
}

int Font::stringHeight() const
{
	return (int)m_font->height;
}

Rect Font::boundingRect(int width, const char* text, TextAlignment alignment) const {
	int lines = mf_wordwrap_lines(m_font, width, text);
	int height = lines * m_font->height;

	Rect ret(0, 0, width, height);
	return ret;
}

Rect Font::boundingRect(int width, const std::string &text, TextAlignment alignment) const
{
	return boundingRect(width, text.c_str(), alignment);
}

int Font::stringWidth(const std::string& string) const {
	return stringWidth(string.c_str());
}

Rect Font::boundingRect(const char* text) const {
}
