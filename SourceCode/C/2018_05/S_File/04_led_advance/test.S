

#define GPM4 (0x11000000 + 0x02E0)

.global _start
_start:

	stmfd sp!, {r0-r12, lr}

	bl led_init

	loop:
	bl led_on
	bl led_delay

	bl led_off
	bl led_delay

	b loop

	ldmfd sp!, {r0-r12, pc}


.global led_init
led_init:
	
	stmfd sp!, {r0-r12, lr}

	@1 set gpio in Output 
	ldr r0, =GPM4
	ldr r1, [r0]
	ldr r2, =0x1111
	orr r1, r2
	str r1, [r0]
	
	@2 set value to 1
	ldr r0, =GPM4	
	ldr r1, [r0, #4]
	orr r1, #0xF
	str r1, [r0, #4]

	ldmfd sp!, {r0-r12, pc}

.global led_on
led_on:
	stmfd sp!, {r0-r12, lr}

	@set value to 0

	ldr r0, =GPM4
	ldr r1, [r0, #4]
	bic r1, #0xF
	str r1, [r0, #4]

	ldmfd sp!, {r0-r12, pc}


.global led_on
led_off:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =GPM4
	ldr r1, [r0, #4]
	orr r1, #0xF 
	str r1, [r0, #4]

	ldmfd sp!, {r0-r12, pc}

.global led_delay
led_delay:

	stmfd sp!, {r0-r12, lr}

	@cpu clock  1.4G 
	ldr r0, =0xF777777
	
	delay_loop:

	sub r0, #1
	cmp r0, #0
	bne delay_loop

	ldmfd sp!, {r0-r12, pc}

