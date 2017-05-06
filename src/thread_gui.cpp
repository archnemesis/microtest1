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
 * @file	/microtest1/src/thread_gui.cpp/thread_gui.cpp
 * @author	robin
 * @date	May 4, 2017
 * @brief	[DESCRIPTION]
 */
#ifndef THREAD_GUI_CPP_
#define THREAD_GUI_CPP_

#define FRAMEBUFFER_WIDTH	240
#define FRAMEBUFFER_HEIGHT	320

#include "thread_gui.h"
#include "label.h"
#include "view.h"
#include "canvas.h"
#include "list.h"

uint32_t _framebuffer_1[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT] __attribute__ ((section (".sram_data")));
uint32_t _framebuffer_2[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT] __attribute__ ((section (".sram_data")));
uint32_t *framebuffer;

GuiThread::GuiThread() :
		m_canvas(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT)
{
	m_canvas.setFramebufferAddress((uint32_t *)&_framebuffer_2[0]);

	mutex_init(&m_refreshMutex);
	framebuffer = (uint32_t *)&_framebuffer_1[0];

	m_testLabel = new Label();
	m_testLabel->setText("Test Label!");

	m_testView = new View();
	m_testView->setWidth(FRAMEBUFFER_WIDTH);
	m_testView->setHeight(FRAMEBUFFER_HEIGHT);
	m_testView->setMainWidget(m_testLabel);

	pushView(m_testView);
}

GuiThread::~GuiThread()
{

}

void GuiThread::run()
{
	View *activeView;

	while (1) {
		if (m_viewStack.at(0) != activeView) {
			//
			// make a transition
			//
			activeView = m_viewStack.at(0);
		}

		//
		// draw the active view
		//
		activeView->draw(m_canvas);

		//
		// rotate the framebuffers
		//
		m_canvas.setFramebufferAddress(framebuffer);
		if (framebuffer == (uint32_t *)&_framebuffer_1[0]) {
			framebuffer = (uint32_t *)&_framebuffer_2[0];
		}
		else {
			framebuffer = (uint32_t *)&_framebuffer_1[0];
		}

		//
		// signal the interrupt that it's safe to swap the buffers
		// at the next sync
		//
		mutex_unlock(&m_refreshMutex);
	}
}

void GuiThread::pushView(View *view)
{
	m_viewStack.insert(0, view);
}

void GuiThread::popView(View *view)
{
	m_viewStack.remove(0);
}

bool GuiThread::takeRefreshMutex() {
}

#endif /* THREAD_GUI_CPP_ */
