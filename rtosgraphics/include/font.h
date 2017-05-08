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
 * @file	/microtest1/rtosgraphics/include/font.h/font.h
 * @author	robin
 * @date	Apr 30, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_FONT_H_
#define INCLUDE_FONT_H_

#include <string>
#include "mcufont.h"
#include "mf_font.h"
#include "mf_scaledfont.h"
#include "rect.h"

class Canvas;

class Font {
	friend class Canvas;
public:
	enum class TextAlignment {
		Left,
		Right,
		Center,
		Justified
	};

	Font();
	Font(const char *fontName);

	void setFont(const char *fontName);
	void setScaling(float scale);
	int stringWidth(const char *string) const;
	int stringWidth(const std::string &string) const;
	int stringHeight() const;

	friend void mf_wordwrap_callback(mf_str line, uint16_t count, void *state);

	Rect boundingRect(int width, const char *text, TextAlignment alignment) const;
	Rect boundingRect(int width, const std::string &text, TextAlignment alignment) const;
	Rect boundingRect(const char *text) const;

protected:
	char m_fontName[16];
	const struct mf_font_s *m_font;
	struct mf_scaledfont_s *m_font_scaled;
	float m_scale;
};


#endif /* INCLUDE_FONT_H_ */
