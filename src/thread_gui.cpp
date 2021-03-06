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

#include "stm32f4xx.h"
#include "hardware_ltdc.h"
#include "thread_gui.h"
#include "label.h"
#include "button.h"
#include "view.h"
#include "vlayout.h"
#include "canvas.h"
#include "list.h"
#include "inputevent.h"

uint32_t _framebuffer_1[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT] __attribute__ ((section (".sram_data")));
uint32_t _framebuffer_2[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT] __attribute__ ((section (".sram_data")));
volatile uint32_t *framebuffer;

GuiThread::GuiThread() :
		Thread(100),
		m_canvas(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT)
{
	m_canvas.setFramebufferAddress((uint32_t *)&_framebuffer_2[0]);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	mutex_init(&m_refreshMutex);
	mutex_init(&m_inputEventMutex);
	framebuffer = (uint32_t *)&_framebuffer_1[0];

	m_testLabel1 = new Label();
	m_testLabel1->setText("Red Label");
	m_testLabel1->setColor(Color(255,0,0,255));
	m_testLabel1->setVerticalSizePolicy(Widget::SizePolicy::ExpandingSizePolicy);

	m_testLabel2 = new Label();
	m_testLabel2->setText("Green Label");
	m_testLabel2->setColor(Color(0,255,0,255));

	m_testLabel3 = new Label();
	m_testLabel3->setText("Blue Label");
	m_testLabel3->setColor(Color(0,0,255,255));

	m_testButton1 = new Button();
	m_testButton1->setText("Test Button");

	m_testButton2 = new Button();
	m_testButton2->setText("Another Button!");

	m_testLayout = new VLayout();
	m_testLayout->setMarginLeft(5);
	m_testLayout->setMarginRight(5);
	m_testLayout->setMarginTop(5);
	m_testLayout->setMarginBottom(5);
	m_testLayout->setSpacing(5);
	m_testLayout->addWidget(m_testLabel1);
	m_testLayout->addWidget(m_testButton2);
	m_testLayout->addWidget(m_testButton1);

	m_testView = new View();
	m_testView->setBackgroundColor(Color(255,255,255,255));
	m_testView->setTitleColor(Color(0,0,0,255));
	m_testView->setTitleTextColor(Color(255,255,255,255));
	m_testView->setTitleFont(Font("Roboto Regular 16"));
	m_testView->setTitleText("STM32");
	m_testView->setWidth(FRAMEBUFFER_WIDTH);
	m_testView->setHeight(FRAMEBUFFER_HEIGHT);
	m_testView->setMainWidget(m_testLayout);

	m_testButton1->clicked.Connect(this, &GuiThread::buttonClicked);

	pushView(m_testView);
}

GuiThread::~GuiThread()
{
	delete m_testView;
	delete m_testLabel1;
	delete m_testLabel2;
	delete m_testLayout;
}

void GuiThread::run()
{
	View *activeView = nullptr;
	Widget *eventClaimedBy = nullptr;
	static GPIO_PinState i = GPIO_PIN_RESET;

	while (1) {
		if (m_viewStack.at(0) != activeView) {
			//
			// make a transition
			//
			if (activeView != nullptr) {
				activeView->deactivated();
			}

			activeView = m_viewStack.at(0);
			activeView->activated();
		}

		if (activeView == nullptr) continue;

		//
		// check for input events
		//
		while (m_inputEventQueue.size() > 0) {
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, i);
			i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
			if (mutex_lock_nonblocking(&m_inputEventMutex) == E_OK) {
				InputEvent *event = m_inputEventQueue.front();
				m_inputEventQueue.pop();
				mutex_unlock(&m_inputEventMutex);

				char buf[255];
				sprintf(buf, "X: %d, Y: %d", event->x(), event->y());
				m_testLabel1->setText(buf);

				if (event->type() != InputEvent::EventType::TouchDown) {
					if (eventClaimedBy != nullptr) {
						eventClaimedBy->processInputEvent(event);
					}
					else {
						activeView->processInputEvent(event);
					}
				}
				else {
					eventClaimedBy = nullptr;
					activeView->processInputEvent(event);
				}

				if (event->claimedBy() != nullptr) {
					eventClaimedBy = event->claimedBy();
				}

				delete event;
			}
		}

		//
		// draw the active view
		//
		activeView->draw(m_canvas);

		//
		// rotate the framebuffers
		//
		m_canvas.setFramebufferAddress((uint32_t *)framebuffer);
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

		sleep(5);
	}
}

void GuiThread::pushView(View *view)
{
	m_viewStack.insert(0, view);
}

void GuiThread::popView()
{
	m_viewStack.remove(0);
}

bool GuiThread::takeRefreshMutex() {
	static GPIO_PinState i = GPIO_PIN_RESET;

	if (mutex_lock_nonblocking(&m_refreshMutex) == E_OK) {
		HW_LTDC_SetFramebuffer(framebuffer);

//		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, i);
//		i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
		return true;
	}
	return false;
}

void GuiThread::registerTouchEvent(int x, int y, bool state) {
	//
	// 0,0 - 3750,300
	// 240,320 - 300,3785
	//
	int x_min = 3750;
	int x_max = 300;
	int y_min = 300;
	int y_max = 3785;
	InputEvent *event = nullptr;
	static bool last_state = false;
	static int last_x = 0;
	static int last_y = 0;

	int x_adj = (x - x_min) * m_canvas.width() / (x_max - x_min);
	int y_adj = (y - y_min) * m_canvas.height() / (y_max - y_min);

	if (last_state != state) {
		if (state) {
			event = new InputEvent(
					InputEvent::EventType::TouchDown,
					x_adj,
					y_adj);

			last_x = x_adj;
			last_y = y_adj;
		}
		else {
			event = new InputEvent(
					InputEvent::EventType::TouchUp,
					last_x,
					last_y);
		}
		last_state = state;
		mutex_lock_wait(&m_inputEventMutex, 0);
		m_inputEventQueue.push(event);
		mutex_unlock(&m_inputEventMutex);
	}
	else if (state == true) {
		event = new InputEvent(
				InputEvent::EventType::Drag,
				x_adj,
				y_adj);

		last_x = x_adj;
		last_y = y_adj;
		mutex_lock_wait(&m_inputEventMutex, 0);
		m_inputEventQueue.push(event);
		mutex_unlock(&m_inputEventMutex);
	}
}

void GuiThread::buttonClicked() {
	m_testLabel2->setText("Clicked!");
}

#endif /* THREAD_GUI_CPP_ */
