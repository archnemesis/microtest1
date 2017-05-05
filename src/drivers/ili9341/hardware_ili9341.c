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
 * @file	/PDCL_OS/src/hardware/ili9341/hardware_ili9341.c/hardware_ili9341.c
 * @author	robin
 * @date	Feb 1, 2017
 * @brief	[DESCRIPTION]
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_gpio.h"

static const unsigned short ili9341_init_sequence[] = {
	0x0CA, 0x1C3, 0x108, 0x150,
	0x0CF, 0x100, 0x1C1, 0x130,
	0x0ED, 0x164, 0x103, 0x112, 0x181,
	0x0E8, 0x185, 0x100, 0x178,
	0x0CB, 0x139, 0x12C, 0x100, 0x134, 0x102,
	0x0F7, 0x120,
	0x0EA, 0x100, 0x100,
	0x0B1, 0x100, 0x11B,
	0x0B6, 0x10A, 0x1A2,
	0x0C0, 0x110,
	0x0C1, 0x110,
	0x0C5, 0x145, 0x115,
	0x0C7, 0x190,
	0x036, 0x1C8,
	0x0F2, 0x100,
	0x0B0, 0x1C2,
	0x0B6, 0x10A, 0x1A7, 0x127, 0x104,
	0x02A, 0x100, 0x100, 0x100, 0x1EF,
	0x02B, 0x100, 0x100, 0x101, 0x13F,
	0x0F6, 0x101, 0x100, 0x106,
	0x02C, 0x800,
	0x026, 0x101,
	0x0E0, 0x10F, 0x129, 0x124, 0x10C, 0x10E, 0x109, 0x14E, 0x178,
		0x13C, 0x109, 0x113, 0x105, 0x117, 0x111, 0x100,
	0x0E1, 0x100, 0x116, 0x11B, 0x104, 0x111, 0x107, 0x131, 0x133, 0x142, 0x105,
		0x10C, 0x10A, 0x128, 0x12F, 0x10F,
	0x011, 0x800,
	0x035, 0x101,
	0x029,
	0x02C,
	0xFFF
};

void ILI9341_SetDC(uint8_t state) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, state);
}

void ILI9341_SetCS(uint8_t state) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, state);
}

void ILI9341_WriteCommand(uint8_t c) {
	ILI9341_SetDC(0);
	ILI9341_SetCS(0);
	HW_SPI_Send(c);
	ILI9341_SetCS(1);
}

void ILI9341_WriteData(uint8_t c) {
	ILI9341_SetDC(1);
	ILI9341_SetCS(0);
	HW_SPI_Send(c);
	ILI9341_SetCS(1);
}

void HW_ILI9341_Init()
{
    ILI9341_SetCS(1);

	unsigned int i = 0;
	unsigned short d = ili9341_init_sequence[0];

	while (d != 0xFFF) {
		if (d == 0x800) {
			HAL_Delay(1000);
		}
		else if (d & 0x100) {
			ILI9341_WriteData(d & 0xFF);
		}
		else {
			ILI9341_WriteCommand(d);
		}
		d = ili9341_init_sequence[++i];
	}
}
