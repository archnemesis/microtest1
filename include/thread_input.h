/*
 * thread_input.h
 *
 *  Created on: May 11, 2017
 *      Author: robin
 */

#ifndef INCLUDE_THREAD_INPUT_H_
#define INCLUDE_THREAD_INPUT_H_


#include "thread.h"
#include "thread_gui.h"

class InputThread : public Thread
{
public:
	InputThread(GuiThread *guiThread) : Thread(10), m_guiThread(guiThread) { }
	void run();

private:
	bool m_touchDetected = false;
	unsigned long m_lastTouchData = 0;
	uint8_t m_dataBuffer[512];
	GuiThread *m_guiThread = nullptr;
	unsigned int m_lastX = 0;
	unsigned int m_lastY = 0;
};


#endif /* INCLUDE_THREAD_INPUT_H_ */
