//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include "diag/Trace.h"
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

#include "hardware_gpio.h"
#include "hardware_fmc.h"
#include "hardware_spi.h"
#include "hardware_ltdc.h"
#include "hardware_ili9341.h"
#include "hardware_i2c.h"
#include "hardware_stmpe811.h"

#include "micrortos.h"
#include "thread.h"
#include "error.h"
#include "application.h"

// ----- main() ---------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
	HW_GPIO_Init();
	HW_FMC_Init();
	HW_SPI_Init();
	HW_LTDC_Init();
	HW_ILI9341_Init();
	HW_I2C_Init();

	HAL_Delay(100);
	HW_STMPE811_Init();

	application_init();
	thread_start_scheduler();

	GPIO_PinState i = GPIO_PIN_RESET;

	while (1)
    {
       HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, i);
       i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
       HAL_Delay(100);
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
	trace_printf("Error caught!\n");

	while(1)
	{

	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
