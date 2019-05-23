
#define GPX3 (0x11000000 + 0x0C60) @key
#define GPD0 (0x11400000 + 0x00A0) @buzzer
#define GPM4 (0x11000000 + 0x02E0) @led
#define PRINT (0xc3e11ad0)

.global _start
_start:
	@use r0 to get key status
	stmfd sp!, {r1-r12, lr}

	
	bl key_init
	bl led_init
	bl buzzer_init

	main_loop:

		bl led_off
		bl buzzer_off

		loop:
			bl key_status
			ldr r1, =0xF
			and r1, r0
			cmp r1, #0xF
		beq loop

		bl led_on
		bl buzzer_on

		ldr r0, =String
		ldr r1, =PRINT
		mov lr, pc
		mov pc, r1
		bl delay

	b main_loop

	ldmfd sp!, {r1-r12, pc}


.global key_init
key_init:
	stmfd sp!, {r0-r12, lr}

	@set GPX3_2 GPX3_3 GPX3_4 GPX3_5 to input mode
	ldr r0, =GPX3
	ldr r1, [r0]
	ldr r2, =(0xFFFF << 8)
	bic r1, r2
	str r1, [r0]

	ldmfd sp!, {r0-r12, pc}


.global key_status
key_status:
	@ use r0 to store key status
	stmfd sp!, {r1-r12, lr}

	ldr r0, =GPX3
	ldr r1, [r0, #4]
	lsr r1, #2
	mov r0, r1

	ldmfd sp!, {r1-r12, pc}

.global led_init
led_init:
	stmfd sp!, {r0-r12, lr}

	@set GPM4_0 GPM4_1 GPM4_2 GPM4_3 to output mode
	ldr r0, =GPM4
	ldr r1, [r0]
	ldr r2, =0xFFFF
	bic r1, r2
	ldr r2, =0x1111
	orr r1, r2
	str r1, [r0]

	bl led_off

	ldmfd sp!, {r0-r12, pc}

.global led_on
led_on:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =GPM4
	ldr r1, [r0, #4]
	bic r1, #0xF
	str r1, [r0, #4]

	ldmfd sp!, {r0-r12, pc}

.global led_off
led_off:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =GPM4
	ldr r1, [r0, #4]
	orr r1, #0xF
	str r1, [r0, #4]

	ldmfd sp!, {r0-r12, pc}


.global buzzer_init
buzzer_init:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =GPD0
	ldr r1, [r0]
	bic r1, #0xF
	orr r1, #0x1
	str r1, [r0]

	bl buzzer_off

	ldmfd sp!, {r0-r12, pc}

.global buzzer_on
buzzer_on:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =GPD0
	ldr r1, [r0, #4]
	orr r1, #0x1
	str r1, [r0, #4]

	ldmfd sp!, {r0-r12, pc}

.global buzzer_off
buzzer_off:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =GPD0
	ldr r1, [r0, #4]
	bic r1, #0x1
	str r1, [r0, #4]

	ldmfd sp!, {r0-r12, pc}

.global delay
delay:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =0x7FFFFFF

	delay_loop:

	sub r0, #1
	cmp r0, #0

	bne delay_loop

	ldmfd sp!, {r0-r12, pc}

.global String
String:
	.string "Key !\n"
	.align 2

	

