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
 * @file	/microtest1/include/thread_gui.h/thread_gui.h
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef INCLUDE_THREAD_GUI_H_
#define INCLUDE_THREAD_GUI_H_

#include <queue>
#include "thread.h"
#include "mutex.h"
#include "canvas.h"
#include "view.h"
#include "list.h"

class Button;
class Label;
class VLayout;
class InputEvent;

class GuiThread : public Thread
{
public:
	GuiThread();
	~GuiThread();
	void run();
	bool takeRefreshMutex();
	void pushView(View *view);
	void popView();
	void registerTouchEvent(int x, int y, bool state);
	void buttonClicked();
private:
	std::queue<InputEvent*> m_inputEventQueue;
	Canvas m_canvas;
	struct mutex_t m_refreshMutex;
	struct mutex_t m_inputEventMutex;

	List<View*> m_viewStack;

	/* ---- Widgets ---- */
	View	*m_testView;
	VLayout	*m_testLayout;
	Label	*m_testLabel1;
	Label	*m_testLabel2;
	Label	*m_testLabel3;
	Button	*m_testButton1;
	Button	*m_testButton2;
};


#endif /* INCLUDE_THREAD_GUI_H_ */
