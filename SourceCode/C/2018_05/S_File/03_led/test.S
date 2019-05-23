
#define GPM4 (0x11000000 + 0x02E0)

.global _start
_start:
	stmfd sp!, {r0-r12, lr}
	ldr r0, =GPM4
	@1 set to output mode

	@1.1 read
	ldr r1, [r0]

	@1.2 clear
	ldr r2, =0xFFFF
	bic r1, r2

	@1.3 set Output
	ldr r2, =0x1111
	orr r1, r2

	@1.4 write
	str r1, [r0]

	@2 set led light, 0 for ON , 1 for OFF

	@2.1 read
	ldr r0, =GPM4
	ldr r1, [r0 , #4] @PGM4 + 4

	@2.2 set value to zero
	bic r1, #0xF

	@2.3 write
	str r1, [r0, #4]


	ldmfd sp!, {r0-r12, pc}




