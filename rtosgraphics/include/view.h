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
 * @file	/microtest1/rtosgraphics/include/view.h/view.h
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_VIEW_H_
#define INCLUDE_VIEW_H_


#include <sigslot.h>
#include "widget.h"
#include "canvas.h"
#include "color.h"
#include "font.h"
#include "inputevent.h"

class View : public Widget {
public:
	View();

	void draw(Canvas& canvas);
	int exec();
	const Color& backgroundColor() const;
	Widget* mainWidget();
	int marginBottom() const;
	int marginLeft() const;
	int marginRight() const;
	int marginTop() const;
	const char* title() const;
	const Color& titleColor() const;
	const Color& titleSeparatorColor() const;
	int titleSeparatorWidth() const;
	const Color& titleTextColor() const;
	const Font& titleFont() const;

	void setBackgroundColor(const Color& backgroundColor);
	void setMainWidget(Widget* mainWidget);
	void setMarginBottom(int marginBottom);
	void setMarginLeft(int marginLeft);
	void setMarginRight(int marginRight);
	void setMarginTop(int marginTop);
	void setMargins(int top, int left, int bottom, int right);
	void setMargins(int margins);
	void setTitleColor(const Color& titleColor);
	void setTitleSeparatorColor(const Color& titleSeparatorColor);
	void setTitleSeparatorWidth(int titleSeparatorWidth);
	void setTitleTextColor(const Color& titleTextColor);
	void setTitleFont(const Font& font);
	void setTitleText(const char *text);

	void processInputEvent(InputEvent *event);

	Signal0<void> activated;
	Signal0<void> deactivated;

protected:
	Widget *m_mainWidget;

	Color m_backgroundColor;
	Color m_titleColor;
	Color m_titleTextColor;
	Color m_titleSeparatorColor;
	Font  m_titleFont;

	int m_marginTop;
	int m_marginBottom;
	int m_marginLeft;
	int m_marginRight;

	int   m_titleSeparatorWidth;
	char  m_title[32];
};


#endif /* INCLUDE_VIEW_H_ */
