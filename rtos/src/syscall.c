/**
 * micrortos
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
 * @file	/micrortos/src/syscall.c/syscall.c
 * @author	robin
 * @date	Apr 19, 2017
 * @brief	[DESCRIPTION]
 */

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <stddef.h>
#include <stdint.h>
#include <syscall.h>
#include <private/syscall_p.h>
#include <thread.h>

/**
 * This is used to bootstrap us into thread mode, by returning
 * the stack pointer of the first thread in the SVC_Handler.
 */
extern uint32_t _svc_exc_return;

static syscall_callback syscall_table[] = {
		&thread_start_scheduler_syscall_handler,
		&thread_start_syscall_handler,
		&thread_terminate_syscall_handler,
		&thread_sleep_syscall_handler,
		&thread_yield_syscall_handler
};

void syscall_handler(uint32_t *svc_args);

void SVC_Handler() {
	__asm__ __volatile__(
			" TST	LR, #4								\n\t"
			" ITE	EQ									\n\t"
			" MRSEQ	R0, MSP								\n\t"
			" MRSNE	R0, PSP								\n\t"
			" LDR	LR, _svc_exc_return					\n\t"
//			" BKPT										\n\t"
			" STR	LR, [R1]							\n\t"
			" BL	syscall_handler  					\n\t"
			" BKPT										\n\t"
			" LDR	LR, _svc_exc_return							\n\t"
			" BX	LR									\n\t"
			".align 4									\n\t"
			::: "r0");
}

void syscall_handler(uint32_t *svc_args)
{
	uint8_t svc_id = ((char *) svc_args[6])[-2];

	if (svc_id == 0) {
		// special case to start scheduler
		// modify svc_exc_return to be the stack pointer
		uint32_t *stack_pointer = (uint32_t *)thread_start_scheduler_syscall_handler(svc_args);

		if (stack_pointer != NULL) {
			_svc_exc_return = HW32_REG((stack_pointer));
			__set_PSP(((uint32_t)stack_pointer + 10*4));
			__set_CONTROL(0x3);
			__ISB();
			return;
		}
		else {
			svc_args[0] = -1;
		}
	}
	else {
		if (svc_id >= (sizeof(syscall_table) / sizeof(syscall_table[0]))) {
			svc_args[0] = -1;
		}
		else {
			syscall_callback handler = syscall_table[svc_id];
			handler(svc_args);
		}
	}
}

