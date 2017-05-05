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
 * @file	/PDCL_OS/src/gfx/canvas.cpp/canvas.cpp
 * @author	robin
 * @date	Feb 3, 2017
 * @brief	[DESCRIPTION]
 */

#include "canvas.h"
#include "heap.h"
#include "mcufont.h"
#include "mf_font.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define sign(x) ((x > 0)? 1 : ((x < 0)? -1: 0))

Canvas::Canvas(int width, int height) :
		m_framebuffer(0),
		m_width(width),
		m_height(height),
		m_drawingColor(255,0,0,0),
		m_drawingFont("Ubuntu 12"),
		m_my_framebuffer(1),
		m_drawText_charX(0),
		m_drawText_charY(0)
{
	m_framebuffer = (uint32_t*)heap_malloc(m_width * m_height * 4);
}

Canvas::Canvas(uint32_t *framebuffer, int width, int height) :
		m_framebuffer(framebuffer),
		m_width(width),
		m_height(height),
		m_drawingColor(255,0,0,0),
		m_drawingFont("Ubuntu 12"),
		m_my_framebuffer(0),
		m_drawText_charX(0),
		m_drawText_charY(0)
{

}

Canvas::~Canvas()
{

}

void Canvas::setColor(Color& color)
{
	m_drawingColor = color;
}

void Canvas::setFont(Font& font)
{
	m_drawingFont = font;
}

void Canvas::blit(Canvas& dest, int destX, int destY, int destWidth, int destHeight)
{
	int x, y;

	for (x = 0; x < destWidth; x++) {
		for (y = 0; y < destHeight; y++) {
			dest.putPixel(destX + x, destY + y, m_framebuffer[(y * m_width) + x]);
		}
	}
}

void Canvas::clear()
{
	int i = 0;
	uint32_t c = m_drawingColor.toInt();
	for (; i < (m_width * m_height); i++) {
		m_framebuffer[i] = c;
	}
}

void Canvas::drawPixel(int x, int y)
{
	if (x < 0 || x >= m_width) {
		return;
	}
	if (y < 0 || y >= m_height) {
		return;
	}

	unsigned int pos = (y * m_width) + x;

	if (m_drawingColor.alpha() == 255) {
		m_framebuffer[pos] = m_drawingColor.toInt();
		return;
	}

	Color bg(m_framebuffer[pos]);
	Color blended = m_drawingColor.blendWith(bg);
	m_framebuffer[pos] = blended.toInt();
}

void Canvas::putPixel(int x, int y, uint32_t pixeldata)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
	uint32_t pos = (y * m_width) + x;
	m_framebuffer[pos] = pixeldata;
}

uint32_t Canvas::getPixel(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return 0;
	uint32_t pos = (y * m_width) + x;
	return m_framebuffer[pos];
}

void Canvas::setFramebufferAddress(uint32_t* framebuffer) {
	m_framebuffer = framebuffer;
}

Color& Canvas::getColor()
{
	return m_drawingColor;
}

void _drawText_fontPixelCallback(int16_t x, int16_t y, uint8_t count, uint8_t alpha, void *state)
{
	Canvas *canvas = (Canvas*)state;

	uint32_t color1 = canvas->getColor().toInt();
	uint32_t color2;
	uint32_t rgba;
	uint32_t rb, g;

	uint8_t i = 0;
	for (; i < count; i++) {
		if (alpha == 255) {
			canvas->putPixel(x+i, y, color1);
		}
		else {
			color2 = canvas->getPixel(x+i, y);

			int rA = (color1 & 0x000000FF);
			int gA = (color1 & 0x0000FF00) >> 8;
			int bA = (color1 & 0x00FF0000) >> 16;
			int aA = alpha;

			int rB = (color2 & 0x000000FF);
			int gB = (color2 & 0x0000FF00) >> 8;
			int bB = (color2 & 0x00FF0000) >> 16;
			int aB = (color2 & 0xFF000000) >> 24;

			int rOut, gOut, bOut, aOut;

			rOut = (rA * aA / 255) + (rB * aB * (255 - aA) / (255*255));
			gOut = (gA * aA / 255) + (gB * aB * (255 - aA) / (255*255));
			bOut = (bA * aA / 255) + (bB * aB * (255 - aA) / (255*255));
			aOut = aA + (aB * (255 - aA) / 255);
			rgba = (aOut << 24) | (bOut << 16) | (gOut << 8) | rOut;

			canvas->putPixel(x+i, y, rgba);
		}
	}
}

void Canvas::drawText(int x, int y, const char *text)
{
	m_drawText_charX = x;
	m_drawText_charY = y;

	int w = 0;
	int i = 0;
	for (; i < strlen(text); i++) {
		w = mf_render_character(m_drawingFont.m_font,
				m_drawText_charX,
				m_drawText_charY,
				text[i],
				(mf_pixel_callback_t)_drawText_fontPixelCallback,
				(void*)this);
		m_drawText_charX += w;
	}
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, int width)
{
	int i = 0;

	int x = x1;
	int y = y1;

	int delta_x = abs(x2 - x1);
	int delta_y = abs(y2 - y1);

	int s1 = sign(x2 - x1);
	int s2 = sign(y2 - y1);
	int swap = 0;

	if (delta_y > delta_x) {
		int temp = delta_x;
		delta_x = delta_y;
		delta_y = temp;
		swap = 1;
	}

	int D = 2*delta_y - delta_x;

	for (i = 0; i < delta_x; i++) {
		drawPixel(x, y);
		while (D >= 0) {
			D = D - 2 * delta_x;
			if (swap) {
				x += s1;
			}
			else {
				y += s2;
			}
		}
		D = D + 2 * delta_y;
		if (swap) {
			y += s2;
		}
		else {
			x += s1;
		}
	}
}

void Canvas::drawRect(int x, int y, int width, int height)
{
	drawLine(x, y, x + width, y, 1);
	drawLine(x + width, y, x + width, y + height, 1);
	drawLine(x + width, y + width, x, y + height, 1);
	drawLine(x, y + height, x, y, 1);
}

void Canvas::drawFilledRect(int x, int y, int width, int height)
{
	int x1;
	int y1;

	for (x1 = x; x1 < (x + width); x1++) {
		for (y1 = y; y1 < (y + height); y1++) {
			drawPixel(x1, y1);
		}
	}
}
