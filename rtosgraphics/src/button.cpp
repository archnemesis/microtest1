/*
 * button.cpp
 *
 *  Created on: May 8, 2017
 *      Author: robin
 */

#include "button.h"

Button::Button() :
	Widget(),
	m_backgroundColor(Color(0xFF666666)),
	m_textColor(Color(0xFF000000)),
	m_borderColor(Color(0xFF333333)),
	m_borderWidth(1),
	m_text("Button")
{
	m_minHeight = 40;
}

Button::~Button()
{

}

void Button::draw(Canvas &canvas)
{
	int x1 = x();
	int y1 = y();

	canvas.setColor(m_backgroundColor);
	canvas.drawFilledRect(x1, y1, width(), height());

	int text_width = m_textFont.stringWidth(m_text);
	int text_x = x() + ((width() - text_width) / 2);
	int text_y = y() + ((height() - m_textFont.stringHeight()) / 2);

	canvas.setColor(m_textColor);
	canvas.drawText(text_x, text_y, m_text.c_str());
}

void Button::setText(const std::string &string)
{
	m_text = string;
}

std::string Button::text() const
{
	return m_text;
}

const Color& Button::backgroundColor() const {
	return m_backgroundColor;
}

void Button::setBackgroundColor(const Color& backgroundColor) {
	m_backgroundColor = backgroundColor;
}

const Color& Button::borderColor() const {
	return m_borderColor;
}

void Button::setBorderColor(const Color& borderColor) {
	m_borderColor = borderColor;
}

int Button::borderWidth() const {
	return m_borderWidth;
}

void Button::setBorderWidth(int borderWidth) {
	m_borderWidth = borderWidth;
}

bool Button::pressed() const {
	return m_pressed;
}

void Button::setPressed(bool pressed) {
	m_pressed = pressed;
}

const Color& Button::textColor() const {
	return m_textColor;
}

void Button::setTextColor(const Color& textColor) {
	m_textColor = textColor;
}

const Font& Button::textFont() const {
	return m_textFont;
}

void Button::setTextFont(const Font& textFont) {
	m_textFont = textFont;
}
