.thumb
.syntax unified
.align 4

#define THREAD_MAX_INTERRUPT_PRIORITY 5

_current_thread:
	.word current_thread

.global PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
	MRS 		R0, PSP				// Save PSP to R0
	ISB								// Instruction Sync Barrier
	MOV			R2, LR
	MRS			R3, CONTROL
	STMDB 		R0!, {R2-R11}		// Store R4-R11 onto the process stack
	TST			R14, #0x10			// Save FPU Context
	IT			EQ
	VSTMDBEQ	R0!, {S16-S31}
	LDR 		R3, _current_thread	// Get current task pointer
	LDR			R2, [R3]
	STR			R0, [R2]			// Update the current task stack pointer
	STMDB		SP!, {R14}			// Save our EXC_RETURN value
	MOV 		R0, #THREAD_MAX_INTERRUPT_PRIORITY				// Block lower-priority interrupts
	CPSID		I					// Errata Workaround
	MSR			BASEPRI, R0
	DSB
	ISB
	CPSIE		I					// Errata Workaround
	BL 			_thread_switch_context
	MOV			R0, #0
	MSR			BASEPRI, R0
	LDMIA		SP!, {R14}			// Restore our EXC_RETURN value
	LDR 		R3, _current_thread	// Get next task pointer
	LDR			R1, [R3]			// Load the address of the Stack Pointer
	LDR			R0, [R1]			// Load the Stack Pointer
	TST			R14, #0x10			// Restore FPU Context
	IT			EQ
	VLDMIAEQ	R0!, {S16-S31}
	LDMIA		R0!, {R2-R11}
	MOV			LR, R2
	MSR			CONTROL, R3
	ISB
	MSR			PSP, R0
	BX			LR
