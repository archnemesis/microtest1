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
 * @file	/microtest1/rtosgraphics/include/widget.h/widget.h
 * @author	robin
 * @date	May 3, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_WIDGET_H_
#define INCLUDE_WIDGET_H_

#include <sigslot.h>
#include "canvas.h"

class InputEvent;

class Widget {
public:
	enum class SizePolicy {
		MinimumSizePolicy,
		ExpandingSizePolicy,
		FixedSizePolicy,
		PreferredSizePolicy
	};

	Widget();
	~Widget();

	void setX(int x);
	void setY(int y);
	void setWidth(int width);
	void setMinWidth(int width);
	void setMaxWidth(int width);
	void setHeight(int height);
	void setMinHeight(int height);
	void setMaxHeight(int height);
	void setVerticalSizePolicy(SizePolicy policy);
	void setHorizontalSizePolicy(SizePolicy policy);

	int x() const;
	int y() const;
	int height() const;
	int width() const;

	virtual int minHeight() const;
	virtual int maxHeight() const;
	virtual int minWidth() const;
	virtual int maxWidth() const;
	virtual int widthHint() const;
	virtual int heightHint() const;
	virtual int heightForWidth(int width) const;
	virtual bool hasHeightForWidth() const;
	SizePolicy horizontalSizePolicy() const;
	SizePolicy verticalSizePolicy() const;

	virtual void draw(Canvas& canvas);
	virtual void processInputEvent(InputEvent *event);

	Signal0<void> clicked;

protected:
	int m_x;
	int m_y;
	int m_width;
	int m_minWidth;
	int m_maxWidth;
	int m_height;
	int m_minHeight;
	int m_maxHeight;
	SizePolicy m_vSizePolicy;
	SizePolicy m_hSizePolicy;
};


#endif /* INCLUDE_WIDGET_H_ */
