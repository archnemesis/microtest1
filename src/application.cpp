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
 * @file	/microtest1/src/test_thread_1.cpp/test_thread_1.cpp
 * @author	robin
 * @date	Apr 29, 2017
 * @brief	[DESCRIPTION]
 */

#include <application.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_dma2d.h"
#include "hardware_ltdc.h"

#define FRAMEBUFFER_WIDTH	240
#define FRAMEBUFFER_HEIGHT	320

volatile uint32_t _framebuffer_1[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT] __attribute__ ((section (".sram_data")));
volatile uint32_t _framebuffer_2[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT] __attribute__ ((section (".sram_data")));

TestThread testThread1(GPIO_PIN_13, 50);
TestThread testThread2(GPIO_PIN_14, 60);
NotifierThread testThread3(&testThread1);

DMA2D_HandleTypeDef dma2d;

void application_init()
{

}

void test_thread_1_main()
{
	testThread1.start();
	testThread2.start();
	testThread3.start();
}

TestThread::TestThread(unsigned int gpio, unsigned int sleep) : Thread()
{
	m_gpio = gpio;
	m_sleep = sleep;
}

TestThread::~TestThread()
{

}

void TestThread::run()
{
	GPIO_PinState i = GPIO_PIN_SET;
	unsigned int x = 0;

	for (; x < (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT); x++) {
		_framebuffer_1[x] = 0xFF00FF00;
	}

	HW_LTDC_SetFramebuffer((uint32_t *)_framebuffer_1);

	while (1)
    {
		waitForEvent(0xBEEF);
		HAL_GPIO_WritePin(GPIOG, m_gpio, i);
		i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

NotifierThread::NotifierThread(TestThread *target)
{
	m_target = target;
}

NotifierThread::~NotifierThread()
{

}

void NotifierThread::run()
{
	GPIO_PinState i = GPIO_PIN_SET;

	while (1) {
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, i);
		i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
		sleep(1000);
		m_target->notify(0xBEEF);
	}
}
