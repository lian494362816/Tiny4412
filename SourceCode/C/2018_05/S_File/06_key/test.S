
#define GPX3 (0x11000000 + 0x0C60)
#define PRINT (0xc3e11ad0)

.global _start
_start:

stmfd sp!, {r0-r12, lr}

	bl key_init

	main_loop:

		loop:
		bl key_status
		ldr r1, =0xF
		and r1, r0
		cmp r1, #0xF
		beq loop

		ldr r0, =Str
		ldr r1, =PRINT
		mov lr, pc
		mov pc, r1
	
	b main_loop

ldmfd sp!, {r0-r12, pc}



.global key_init
key_init:

	stmfd sp!, {r0-r12, lr}

	//set GPX3_2 GPX3_3 GPX3_4 GPX3_5 input mode
	ldr r0, =GPX3
	ldr r1, [r0]
	ldr r2, =(0xFFFF << 8)
	bic r1, r2
	str r1, [r0]

	ldmfd sp!, {r0-r12, pc}

.global key_status
key_status:

	//not push r0
	stmfd sp!, {r1-r12, lr}

	ldr r0, =GPX3
	ldr r1, [r0, #4]
	lsr r1, #2
	//get key status to r0
	mov r0, r1

	ldmfd sp!, {r1-r12, pc}

.global Str
Str:
	.string "key !!\n"
	.align 
