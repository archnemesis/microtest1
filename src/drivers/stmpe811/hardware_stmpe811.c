/**
 * PDCL_OS
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
 * @file	/PDCL_OS/src/hardware/stmpe811/hardware_stmpe811.c
 * @author	robin
 * @date	Feb 1, 2017
 * @brief	[DESCRIPTION]
 */

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_i2c.h"
#include "hardware_i2c.h"
#include "hardware_stmpe811.h"

#define STMPE811_ADDRESS 0x82
static GPIO_PinState i = GPIO_PIN_RESET;
static unsigned char sample_buffer[64];

void HW_STMPE811_Init()
{
	//
	// SYS_CTRL1
	// - Issue Soft Reset
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_SYS_CTRL1,
			(uint8_t)(STMPE811_SYS_CTRL1_SOFT_RESET));

	HAL_Delay(100);

	//
	// SYS_CTRL2
	// - Enable ADC Clock
	// - Enable TSC Clock
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_SYS_CTRL2,
			(uint8_t)STMPE811_SYS_CTRL2_GPIO_OFF | STMPE811_SYS_CTRL2_TS_OFF);

	//
	// INT_EN
	// - Enable FIFO_TH
	// - Enable TOUCH_DET
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_INT_EN,
			(uint8_t)(STMPE811_INT_EN_TOUCH_DET | STMPE811_INT_EN_FIFO_TH));

	//
	// ADC_CTRL1
	// - Sample Time: 80 cycles
	// - 12-bit ADC
	// - Internal Reference
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_ADC_CTRL1,
			(uint8_t)(STMPE811_ADC_CTRL1_MOD_12B | STMPE811_ADC_CTRL1_SAMPLE_TIME_80));

	//
	// GPIO_AF
	// - Set GPIO Pins to ADC/TSC Mode
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_GPIO_AF,
			(uint8_t)0);

	//
	// TSC Configuration
	// - 4 Samples
	// - 50us Touch Detect Delay
	// - 100us Settling Time
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_TSC_CFG,
			(uint8_t)(STMPE811_TSC_CFG_AVE_CTRL_4_SAMPLES | STMPE811_TSC_CFG_TOUCH_DET_DELAY_50US | STMPE811_TSC_CFG_SETTLING_100US));

	//
	// FIFO_TH
	// - 64 samples?
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_FIFO_TH,
			(uint8_t)(1));

	//
	// TSC Control
	// - No Window Tracking
	// - XY-Only Operating Mode
	// - Enable TSC
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_TSC_CTRL,
			(uint8_t)(STMPE811_TSC_CTRL_OP_MOD_XY | STMPE811_TSC_CTRL_EN));

	//
	// INT_CTRL
	// - Interrupts Active Low
	// - Enable Global Interrupts
	// - Edge Interrupt Type
	//

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_INT_CTRL,
			(uint8_t)0);
}

void HW_STMPE811_IRQHandler()
{
	uint8_t int_sta = HW_I2C_ReadReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_INT_STA);

	i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, i);

//		HW_I2C_WriteReg8(
//				(uint16_t)STMPE811_ADDRESS,
//				(uint16_t)STMPE811_REG_INT_CTRL,
//				(uint8_t)0);
//
//		uint8_t fifo_level = HW_I2C_ReadReg8(
//				(uint16_t)STMPE811_ADDRESS,
//				(uint16_t)STMPE811_REG_FIFO_SIZE);
//
//		if (fifo_level > 0) {
//			//
//			// Disable the TSC while reading data, due to silicon
//			// bug that causes the FIFO to crash sometimes...
//			//
//			HW_I2C_WriteReg8(
//					(uint16_t)STMPE811_ADDRESS,
//					(uint16_t)STMPE811_REG_TSC_CTRL,
//					(uint8_t)(STMPE811_TSC_CTRL_OP_MOD_XY));
//
//			HW_I2C_ReadReg(
//					(uint16_t)STMPE811_ADDRESS,
//					(uint16_t)0xD7,
//					(uint8_t *)&sample_buffer[0],
//					fifo_level * 3);
//
//			//
//			// Re-enable the TSC (which should reset and clear FIFO)
//			//
//			HW_I2C_WriteReg8(
//					(uint16_t)STMPE811_ADDRESS,
//					(uint16_t)STMPE811_REG_TSC_CTRL,
//					(uint8_t)(STMPE811_TSC_CTRL_OP_MOD_XY | STMPE811_TSC_CTRL_EN));
//		}

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_INT_CTRL,
			(uint8_t)0);

	uint8_t fifo_level = HW_I2C_ReadReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_FIFO_SIZE);

	if (fifo_level > 0) {
		//
		// Disable the TSC while reading data, due to silicon
		// bug that causes the FIFO to crash sometimes...
		//
		HW_I2C_WriteReg8(
				(uint16_t)STMPE811_ADDRESS,
				(uint16_t)STMPE811_REG_TSC_CTRL,
				(uint8_t)(STMPE811_TSC_CTRL_OP_MOD_XY));

		HW_I2C_ReadReg(
				(uint16_t)STMPE811_ADDRESS,
				(uint16_t)0xD7,
				(uint8_t *)&sample_buffer[0],
				3);

		//
		// Re-enable the TSC (which should reset and clear FIFO)
		//
		HW_I2C_WriteReg8(
				(uint16_t)STMPE811_ADDRESS,
				(uint16_t)STMPE811_REG_FIFO_STA,
				(uint8_t)1);
		HW_I2C_WriteReg8(
				(uint16_t)STMPE811_ADDRESS,
				(uint16_t)STMPE811_REG_FIFO_STA,
				(uint8_t)0);
		HW_I2C_WriteReg8(
				(uint16_t)STMPE811_ADDRESS,
				(uint16_t)STMPE811_REG_TSC_CTRL,
				(uint8_t)(STMPE811_TSC_CTRL_OP_MOD_XY | STMPE811_TSC_CTRL_EN));
	}
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_INT_STA,
			(uint8_t)int_sta);

	HW_I2C_WriteReg8(
			(uint16_t)STMPE811_ADDRESS,
			(uint16_t)STMPE811_REG_INT_CTRL,
			(uint8_t)(STMPE811_INT_CTRL_GLOBAL_INT | STMPE811_INT_CTRL_INT_TYPE));
}
