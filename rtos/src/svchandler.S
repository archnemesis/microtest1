.thumb
.syntax unified
.align 4

_svc_exc_return:
	.word svc_exc_return

SVC_Handler:
	TST		LR, #4
	ITE		EQ
	MRSEQ	R0, MSP
	MRSNE	R0, PSP
	LDR		R1, _svc_exc_return
	STR		LR, [R1]
	BL		_syscall_handler
	LDR		R1, _svc_exc_return
	LDR		LR, [R1]
	BX		LR
