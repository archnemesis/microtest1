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
 * @file	/microtest1/rtosgraphics/include/label.h/label.h
 * @author	robin
 * @date	Apr 30, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_LABEL_H_
#define INCLUDE_LABEL_H_

#include <string>
#include "widget.h"
#include "font.h"
#include "color.h"
#include "canvas.h"

class Label : public Widget
{
public:
	Label();
	Label(const char *labelText);

	void setText(std::string const& text);
	void setText(const char *text);
	void setFont(Font& font);
	void setColor(const Color& color);
	void setAlignment(Font::TextAlignment align);
	void draw(Canvas& canvas);

	int widthHint() const;
	int heightHint() const;
	bool hasHeightForWidth() const;
	int heightForWidth(int width) const;
	bool textWrapping() const;
	void setTextWrapping(bool textWrapping);

protected:
	std::string m_text;
	Font m_font;
	Color m_color;
	Font::TextAlignment m_align;
	bool m_textWrapping;
};


#endif /* INCLUDE_LABEL_H_ */
