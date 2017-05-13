/*
 * inputevent.cpp
 *
 *  Created on: May 11, 2017
 *      Author: robin
 */

#include "inputevent.h"
#include "widget.h"

InputEvent::InputEvent() :
	m_type(EventType::Misc),
	m_x(0),
	m_y(0)
{

}

InputEvent::InputEvent(EventType type, int x, int y) :
	m_type(type),
	m_x(x),
	m_y(y)
{

}

int InputEvent::x() {
	return m_x;
}

int InputEvent::y() {
	return m_y;
}

InputEvent::EventType InputEvent::type() {
	return m_type;
}

void InputEvent::claim(Widget *widget)
{
	m_claimedBy = widget;
}

Widget *InputEvent::claimedBy() const
{
	return m_claimedBy;
}
