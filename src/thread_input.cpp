/*
 * thread_input.cpp
 *
 *  Created on: May 11, 2017
 *      Author: robin
 */

#include "thread_input.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "hardware_i2c.h"
#include "hardware_stmpe811.h"

#define STMPE811_ADDRESS 0x82

void InputThread::run()
{
	while (true) {
		uint8_t int_sta = HW_I2C_ReadReg8(
				(uint16_t)STMPE811_ADDRESS,
				(uint16_t)STMPE811_REG_INT_STA);

		if ((int_sta & STMPE811_INT_STA_TOUCH_DET) == STMPE811_INT_STA_TOUCH_DET) {
			if (!m_touchDetected) {
				m_touchDetected = true;
			}
			else {
				m_touchDetected = false;
				m_guiThread->registerTouchEvent(m_lastX, m_lastY, m_touchDetected);
				sleep(10);
				continue;
			}

			HW_I2C_WriteReg8(
					(uint16_t)STMPE811_ADDRESS,
					(uint16_t)STMPE811_REG_INT_STA,
					(uint8_t)int_sta);
		}

		uint8_t fifo_level = HW_I2C_ReadReg8(
				(uint16_t)STMPE811_ADDRESS,
				(uint16_t)STMPE811_REG_FIFO_SIZE);

		if (fifo_level > 0) {
			HW_I2C_ReadReg(
					(uint16_t)STMPE811_ADDRESS,
					(uint16_t)0xD7,
					(uint8_t *)&m_dataBuffer[0],
					fifo_level * 3);
			HW_I2C_WriteReg8(
					(uint16_t)STMPE811_ADDRESS,
					(uint16_t)STMPE811_REG_FIFO_STA,
					(uint8_t)1);
			HW_I2C_WriteReg8(
					(uint16_t)STMPE811_ADDRESS,
					(uint16_t)STMPE811_REG_FIFO_STA,
					(uint8_t)0);

			uint16_t xpos = 0;
			uint16_t ypos = 0;
			uint16_t i = 0;

			for (; i < fifo_level; i++) {
				xpos = (m_dataBuffer[(i*3) + 0] << 4) | ((m_dataBuffer[(i*3) + 1] & 0xF0) >> 4);
				ypos = ((m_dataBuffer[(i*3) + 1] & 0x0F) << 8) | m_dataBuffer[(i*3) + 2];

				if (xpos > 0 && ypos > 0) {
					m_lastX = xpos;
					m_lastY = ypos;
				}
			}

			m_lastTouchData = HAL_GetTick();
		}

		//
		// if we have touchDetected = true but no data for > 40ms, then
		// we abort the touch
		//
		if (((HAL_GetTick() - m_lastTouchData) > 20) && m_touchDetected == true) {
			m_touchDetected = false;
			m_guiThread->registerTouchEvent(m_lastX, m_lastY, m_touchDetected);
		}

		if (m_touchDetected) {
			m_guiThread->registerTouchEvent(m_lastX, m_lastY, m_touchDetected);
		}

		sleep(10);
	}
}
