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
 * @file	/microtest1/rtosgraphics/include/canvas.h/canvas.h
 * @author	robin
 * @date	Apr 30, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_CANVAS_H_
#define INCLUDE_CANVAS_H_

#include <stdint.h>

#include "color.h"
#include "font.h"

class Canvas {
public:
	Canvas(int width, int height);
	Canvas(uint32_t *framebuffer, int width, int height);
	~Canvas();

	void setFramebufferAddress(uint32_t *framebuffer);
	void clear();
	void setColor(Color& color);
	void setFont(Font& font);
	void drawPixel(int x, int y);
	void putPixel(int x, int y, uint32_t pixeldata);
	void drawLine(int x1, int y1, int x2, int y2, int width);
	void drawRect(int x, int y, int width, int height);
	void drawFilledRect(int x, int y, int width, int height);
	void drawText(int x, int y, const char *text);
	void blit(Canvas& dest, int destX, int destY, int destWidth, int destHeight);

	uint32_t getPixel(int x, int y);
	int width() const { return m_width; }
	int height() const { return m_height; }
	Color &getColor();

	friend void _drawText_fontPixelCallback(int16_t x, int16_t y, uint8_t count, uint8_t alpha, void *state);

protected:
	volatile uint32_t *m_framebuffer;
	int m_my_framebuffer;
	int m_width;
	int m_height;
	Font m_drawingFont;
	Color m_drawingColor;

	int m_drawText_charX;
	int m_drawText_charY;
};


#endif /* INCLUDE_CANVAS_H_ */
