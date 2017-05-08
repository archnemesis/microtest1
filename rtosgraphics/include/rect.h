/*
 * rect.h
 *
 *  Created on: May 7, 2017
 *      Author: robin
 */

#ifndef INCLUDE_RECT_H_
#define INCLUDE_RECT_H_


class Rect
{
public:
	Rect(int x, int y, int width, int height) :
		m_x(x),
		m_y(y),
		m_width(width),
		m_height(height) { }

	int height() const;
	void setHeight(int height);
	int width() const;
	void setWidth(int width);
	int x() const;
	void setX(int x);
	int y() const;
	void setY(int y);

protected:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
};


#endif /* INCLUDE_RECT_H_ */
