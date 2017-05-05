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

#include "canvas.h"

class Widget {
public:
	enum SizePolicy {
		MinimumSizePolicy,
		ExpandingSizePolicy,
		FixedSizePolicy,
		PreferredSizePolicy
	};

	Widget();
	virtual ~Widget();

	void setX(int x);
	void setY(int y);
	void setWidth(int width);
	void setHeight(int height);
	void setVerticalSizePolicy(SizePolicy policy);
	void setHorizontalSizePolicy(SizePolicy policy);

	int x() const;
	int y() const;
	int width() const;
	int height() const;
	int widthHint() const;
	int heightHint() const;
	SizePolicy horizontalSizePolicy() const;
	SizePolicy verticalSizePolicy() const;

	virtual void draw(Canvas& canvas) = 0;

protected:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	SizePolicy m_vSizePolicy;
	SizePolicy m_hSizePolicy;
};


#endif /* INCLUDE_WIDGET_H_ */
