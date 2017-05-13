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
 * @file	/microtest1/rtosgraphics/include/button.h/button.h
 * @author	robin
 * @date	Apr 30, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_BUTTON_H_
#define INCLUDE_BUTTON_H_


#include "widget.h"
#include "color.h"
#include "font.h"
#include "inputevent.h"
#include <string>

class Button : public Widget
{
public:
	Button();
	~Button();

	virtual void draw(Canvas &canvas);
	int heightHint() const;
	void setText(const std::string &string);
	std::string text() const;
	const Color& backgroundColor() const;
	void setBackgroundColor(const Color& backgroundColor);
	const Color& borderColor() const;
	void setBorderColor(const Color& borderColor);
	int borderWidth() const;
	void setBorderWidth(int borderWidth);bool pressed() const;
	void setPressed(bool pressed = false);
	const Color& textColor() const;
	void setTextColor(const Color& textColor);
	const Font& textFont() const;
	void setTextFont(const Font& textFont);
	void processInputEvent(InputEvent *event);

protected:
	std::string m_text;
	bool m_pressed = false;
	Color m_textColor;
	Color m_backgroundColor;
	Color m_backgroundColorPressed;
	Color m_borderColor;
	int m_borderWidth;
	Font m_textFont;
};


#endif /* INCLUDE_BUTTON_H_ */
