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
 * @file	/PDCL_OS/src/hardware/stm32f4xx/hardware_fmc.c/hardware_fmc.c
 * @author	robin
 * @date	Jan 31, 2017
 * @brief	[DESCRIPTION]
 */

#include "hardware_fmc.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

/**
 * Initialize following GPIO pins for FMC:
 *
 * PB5    ------> FMC_SDCKE1
 * PB6    ------> FMC_SDNE1
 * PC0    ------> FMC_SDNWE
 * PD0    ------> FMC_D2
 * PD1    ------> FMC_D3
 * PD8    ------> FMC_D13
 * PD9    ------> FMC_D14
 * PD10   ------> FMC_D15
 * PD14   ------> FMC_D0
 * PD15   ------> FMC_D1
 * PE0    ------> FMC_NBL0
 * PE1    ------> FMC_NBL1
 * PE7    ------> FMC_D4
 * PE8    ------> FMC_D5
 * PE9    ------> FMC_D6
 * PE10   ------> FMC_D7
 * PE11   ------> FMC_D8
 * PE12   ------> FMC_D9
 * PE13   ------> FMC_D10
 * PE14   ------> FMC_D11
 * PE15   ------> FMC_D12
 * PF0    ------> FMC_A0
 * PF1    ------> FMC_A1
 * PF2    ------> FMC_A2
 * PF3    ------> FMC_A3
 * PF4    ------> FMC_A4
 * PF5    ------> FMC_A5
 * PF11   ------> FMC_SDNRAS
 * PF12   ------> FMC_A6
 * PF13   ------> FMC_A7
 * PF14   ------> FMC_A8
 * PF15   ------> FMC_A9
 * PG0    ------> FMC_A10
 * PG1    ------> FMC_A11
 * PG4    ------> FMC_BA0
 * PG5    ------> FMC_BA1
 * PG8    ------> FMC_SDCLK
 * PG15   ------> FMC_SDNCAS
 */
void HW_FMC_GPIO_Init()
{
	__HAL_RCC_FMC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = \
			GPIO_PIN_0  | \
			GPIO_PIN_1  | \
			GPIO_PIN_8  | \
			GPIO_PIN_9  | \
			GPIO_PIN_10 | \
			GPIO_PIN_14 | \
			GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = \
			GPIO_PIN_0  | \
			GPIO_PIN_1  | \
			GPIO_PIN_7  | \
			GPIO_PIN_8  | \
			GPIO_PIN_9  | \
			GPIO_PIN_10 | \
			GPIO_PIN_11 | \
			GPIO_PIN_12 | \
			GPIO_PIN_13 | \
			GPIO_PIN_14 | \
			GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = \
			GPIO_PIN_0  | \
			GPIO_PIN_1  | \
			GPIO_PIN_2  | \
			GPIO_PIN_3  | \
			GPIO_PIN_4  | \
			GPIO_PIN_5  | \
			GPIO_PIN_11 | \
			GPIO_PIN_12 | \
			GPIO_PIN_13 | \
			GPIO_PIN_14 | \
			GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = \
			GPIO_PIN_0  | \
			GPIO_PIN_1  | \
			GPIO_PIN_4  | \
			GPIO_PIN_5  | \
			GPIO_PIN_8  | \
			GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void HW_FMC_GPIO_DeInit()
{
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5 | GPIO_PIN_6);
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);
	HAL_GPIO_DeInit(GPIOD, \
			GPIO_PIN_0   | \
			GPIO_PIN_1   | \
			GPIO_PIN_8   | \
			GPIO_PIN_9   | \
			GPIO_PIN_10  | \
			GPIO_PIN_14  | \
			GPIO_PIN_15);
	HAL_GPIO_DeInit(GPIOE, \
			GPIO_PIN_0   | \
			GPIO_PIN_1   | \
			GPIO_PIN_7   | \
			GPIO_PIN_8   | \
			GPIO_PIN_9   | \
			GPIO_PIN_10  | \
			GPIO_PIN_11  | \
			GPIO_PIN_12  | \
			GPIO_PIN_13  | \
			GPIO_PIN_14  | \
			GPIO_PIN_15);
	HAL_GPIO_DeInit(GPIOF, \
			GPIO_PIN_0   | \
			GPIO_PIN_1   | \
			GPIO_PIN_2   | \
			GPIO_PIN_3   | \
			GPIO_PIN_4   | \
			GPIO_PIN_5   | \
			GPIO_PIN_11  | \
			GPIO_PIN_12  | \
			GPIO_PIN_13  | \
			GPIO_PIN_14  | \
			GPIO_PIN_15);
	HAL_GPIO_DeInit(GPIOG, \
			GPIO_PIN_0   | \
			GPIO_PIN_1   | \
			GPIO_PIN_4   | \
			GPIO_PIN_5   | \
			GPIO_PIN_8   | \
			GPIO_PIN_15);
}

#define TMRD(x) (x << 0)  /* Load Mode Register to Active */
#define TXSR(x) (x << 4)  /* Exit Self-refresh delay */
#define TRAS(x) (x << 8)  /* Self refresh time */
#define TRC(x)  (x << 12) /* Row cycle delay */
#define TWR(x)  (x << 16) /* Recovery delay */
#define TRP(x)  (x << 20) /* Row precharge delay */
#define TRCD(x) (x << 24) /* Row to column delay */
void HW_FMC_Init()
{
	HW_FMC_GPIO_Init();

	unsigned int tmp = 0;

	// Enable clock
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;

	// Init Step 1
	FMC_SDRAM_DEVICE->SDCR[0] = FMC_SDCR1_SDCLK_1 | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE_1;
	FMC_SDRAM_DEVICE->SDCR[1] = FMC_SDCR2_NR_0 | FMC_SDCR2_MWID_0 | FMC_SDCR2_NB | FMC_SDCR2_CAS;

	// Init Step 2
	FMC_SDRAM_DEVICE->SDTR[0] = TRC(7) | TRP(2);
	FMC_SDRAM_DEVICE->SDTR[1] = TMRD(2) | TXSR(7) | TRAS(4) | TWR(2) | TRCD(2);

	// Init Step 3
	while (FMC_SDRAM_DEVICE->SDSR & FMC_SDSR_BUSY);
	FMC_SDRAM_DEVICE->SDCMR = 1 | FMC_SDCMR_CTB2 | (1 << 5);

	// Init Step 4 (wait)
	for(tmp = 0; tmp < 1000000; tmp++);

	// Init Step 5
	while (FMC_SDRAM_DEVICE->SDSR & FMC_SDSR_BUSY);
	FMC_SDRAM_DEVICE->SDCMR = 2 | FMC_SDCMR_CTB2 | (1 << 5);

	// Init Step 6
	while (FMC_SDRAM_DEVICE->SDSR & FMC_SDSR_BUSY);
	FMC_SDRAM_DEVICE->SDCMR = 3 | FMC_SDCMR_CTB2 | (4 << 5);

	// Init Step 7
	while (FMC_SDRAM_DEVICE->SDSR & FMC_SDSR_BUSY);
	FMC_SDRAM_DEVICE->SDCMR = 4 | FMC_SDCMR_CTB2 | (1 << 5) | (0x231 << 9);

	// Init Step 8
	while(FMC_SDRAM_DEVICE->SDSR & FMC_SDSR_BUSY);
	FMC_SDRAM_DEVICE->SDRTR |= (683 << 1);
	while(FMC_SDRAM_DEVICE->SDSR & FMC_SDSR_BUSY);

	// Initialize memory region to 0
	for(tmp = 0xD0000000; tmp < (0xD0000000 + 0x00800000); tmp += 4)
	  *((volatile uint32_t *)tmp) = 0x00000000;
}

void HW_FMC_DeInit()
{
	HW_FMC_GPIO_DeInit();
}
