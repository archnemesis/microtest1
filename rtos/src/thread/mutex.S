.thumb
.syntax unified
.align 4

#include <micrortos.h>

_current_thread:
	.word current_thread

.global mutex_unlock
	.type mutex_unlock, %function
mutex_unlock:
1:	LDREX	R1, [R0]
	MOV		R1, #0
	STREX	R2, R1, [R0]
	CMP		R2, #0
	BNE		1b
	BX		LR

.global mutex_lock
	.type mutex_lock, %function
mutex_lock:
1:	LDREX	R1, [R0]	// load lock flag value
	CMP		R1, #1		// is mutex locked?
	IT		EQ
	BEQ		1b			// locked, try again
	MOV		R1, #1		// set flag to 1
	STREX	R2, R1, [R0]// store exclusive
	CMP		R2, #1		// test for interruption
	IT		EQ
	BEQ		1b			// try again if interrupted
	MOV		R0, #E_OK
	BX		LR

.global mutex_lock_nonblocking
	.type mutex_lock_nonblocking, %function
mutex_lock_nonblocking:
	MOV		R3, #E_LOCKED	// success flag init to E_LOCKED
1:	LDREX	R1, [R0]		// move value at *sem into R1, load-exclusive
	CMP		R1, #1			// is mutex already locked?
	IT		EQ				//
	BEQ		2f				// flag is 1 (locked), return with 0 status (R3)
	MOV		R1, #1			// flag is 0, set to 1
	STREX	R2, R1, [R0]	// store new value, store-exclusive
	CMP		R2, #0			// make sure STREX was successful
	ITE		EQ				//
	MOVEQ	R3, #E_OK		// return status of E_OK
	BNE		1b   			// interrupted between LDREX and STREX, try again
	DMB						//
2:	MOV		R0, R3			// return status
	BX		LR
