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
 * @file	/microtest1/rtosgraphics/include/color.h/color.h
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_COLOR_H_
#define INCLUDE_COLOR_H_


class Color {
public:
	Color();
	Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
	Color(unsigned int argb);
	unsigned int red() const;
	unsigned int green() const;
	unsigned int blue() const;
	unsigned int alpha() const;
	void setRed(unsigned int red);
	void setGreen(unsigned int green);
	void setBlue(unsigned int blue);
	void setAlpha(unsigned int alpha);
	Color blendWith(Color& color2) const;
	unsigned int toInt() const;
protected:
	unsigned int m_red;
	unsigned int m_green;
	unsigned int m_blue;
	unsigned int m_alpha;
	unsigned int m_argb;
};


#endif /* INCLUDE_COLOR_H_ */
