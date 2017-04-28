//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

#include "micrortos.h"
#include "thread.h"

static struct thread_t test_thread_1;
static struct thread_t test_thread_2;

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void test_thread_1_main(void *data)
{
	GPIO_PinState i = GPIO_PIN_RESET;

	while (1)
    {
       HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, i);
       i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
       thread_sleep(200);
    }
}

void test_thread_2_main(void *data)
{
	GPIO_PinState i = GPIO_PIN_SET;

	while (1)
    {
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, i);
		i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
		thread_sleep(100);
    }
}

int
main(int argc, char* argv[])
{
	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	//
	// Green (G13) and Red (G14)
	//
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	test_thread_1.attr.priority = 10;
	test_thread_1.attr.main = &test_thread_1_main;
	strcpy(test_thread_1.attr.name, "1:test-thread-1");
	thread_create_static(&test_thread_1, NULL);
	thread_start(&test_thread_1);

	test_thread_2.attr.priority = 10;
	test_thread_2.attr.main = &test_thread_2_main;
	strcpy(test_thread_2.attr.name, "1:test-thread-2");
	thread_create_static(&test_thread_2, NULL);
	thread_start(&test_thread_2);

	thread_start_scheduler();

	GPIO_PinState i = GPIO_PIN_RESET;

	while (1)
    {
       HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, i);
       i = (i == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET);
       HAL_Delay(100);
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
