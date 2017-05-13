/*
 * inputevent.h
 *
 *  Created on: May 11, 2017
 *      Author: robin
 */

#ifndef INCLUDE_INPUTEVENT_H_
#define INCLUDE_INPUTEVENT_H_

class Widget;

class InputEvent
{
public:
	enum class EventType {
		TouchDown,
		TouchUp,
		Drag,
		Misc
	};

	InputEvent();
	InputEvent(EventType type, int x, int y);

	int x();
	int y();
	EventType type();
	void claim(Widget *widget);
	Widget *claimedBy() const;
protected:
	int m_x;
	int m_y;
	EventType m_type;
	Widget *m_claimedBy = nullptr;
};


#endif /* INCLUDE_INPUTEVENT_H_ */
