/**
 * DiscoTest1
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
 * @file	/DiscoTest1/src/lib/gfx/ui/color.cpp/color.cpp
 * @author	robin
 * @date	Jan 31, 2017
 * @brief	[DESCRIPTION]
 */

#include "color.h"
#include <stdint.h>


Color::Color() :
	m_red(0),
	m_green(0),
	m_blue(0),
	m_alpha(0)
{
	m_argb = (m_alpha << 24) | (m_blue << 16) | (m_green << 8) | m_red;
}

Color::Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) {
	m_red = red;
	m_green = green;
	m_blue = blue;
	m_alpha = alpha;
	m_argb = (m_alpha << 24) | (m_blue << 16) | (m_green << 8) | m_red;
}

Color::Color(unsigned int color) {
	m_argb   = color;
	m_red 	=  color & 0x000000FF;
	m_green 	= (color & 0x0000FF00) >> 8;
	m_blue 	= (color & 0x00FF0000) >> 16;
	m_alpha 	= (color & 0xFF000000) >> 24;
}

unsigned int Color::red() const {
	return m_red;
}

unsigned int Color::green() const {
	return m_green;
}

unsigned int Color::blue() const {
	return m_blue;
}

unsigned int Color::alpha() const {
	return m_alpha;
}

void Color::setRed(unsigned int red) {
	m_red = red;
	m_argb = (m_alpha << 24) | (m_blue << 16) | (m_green << 8) | m_red;
}

void Color::setGreen(unsigned int green) {
	m_green = green;
	m_argb = (m_alpha << 24) | (m_blue << 16) | (m_green << 8) | m_red;
}

void Color::setBlue(unsigned int blue) {
	m_blue = blue;
	m_argb = (m_alpha << 24) | (m_blue << 16) | (m_green << 8) | m_red;
}

void Color::setAlpha(unsigned int alpha) {
	m_alpha = alpha;
	m_argb = (m_alpha << 24) | (m_blue << 16) | (m_green << 8) | m_red;
}

unsigned int Color::toInt() const {
	return m_argb;
}

Color Color::blendWith(Color& color2) const {
	int rgba;
	int rA = m_red;
	int gA = m_green;
	int bA = m_blue;
	int aA = m_alpha;

	int rB = color2.red();
	int gB = color2.green();
	int bB = color2.blue();
	int aB = color2.alpha();

	int rOut, gOut, bOut, aOut;

	rOut = (rA * aA / 255) + (rB * aB * (255 - aA) / (255*255));
	gOut = (gA * aA / 255) + (gB * aB * (255 - aA) / (255*255));
	bOut = (bA * aA / 255) + (bB * aB * (255 - aA) / (255*255));
	aOut = aA + (aB * (255 - aA) / 255);
	rgba = (aOut << 24) | (bOut << 16) | (gOut << 8) | rOut;

	return Color(rgba);
}
