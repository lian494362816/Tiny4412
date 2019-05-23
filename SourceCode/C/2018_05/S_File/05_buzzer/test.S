
#define GPD0 (0x11400000 + 0x00A0)

.global _start
_start:

stmfd sp!, {r0-r12, lr}
	bl buzzer_init

	loop:
	bl buzzer_on
	bl buzzer_delay

	bl buzzer_off
	bl buzzer_delay

	b loop

ldmfd sp!, {r0-r12, pc}


.global buzzer_init
buzzer_init:
	
stmfd sp!, {r0-r12, lr}

	@set output mode
	ldr r0, =GPD0
	ldr r1, [r0]
	orr r1, #0x1
	str r1, [r0]

ldmfd sp!, {r0-r12, pc}


.global buffzer_on
buzzer_on:

stmfd sp!, {r0-r12, lr}

	@set value to 1
	ldr r0, =GPD0
	ldr r1, [r0, #4] 
	orr r1, #0x1
	str r1, [r0, #4]

ldmfd sp!, {r0-r12, pc}


.global buffzer_off
buzzer_off:
	
stmfd sp!, {r0-r12, lr}

	@set value to 0
	ldr r0, =GPD0
	ldr r1, [r0, #4]
	bic r1, #0x1
	str r1, [r0, #4]

ldmfd sp!, {r0-r12, pc}


.global buzzer_delay
buzzer_delay:

stmfd sp!, {r0-r12, lr}

	@clock 1.4G
	ldr r0, =0x7FFFFFF

	buzzer_loop:

	sub r0, #1
	cmp r0, #0
	bne buzzer_loop

ldmfd sp!, {r0-r12, pc}
