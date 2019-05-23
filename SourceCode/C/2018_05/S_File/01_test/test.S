
.global _start
_start:
	stmfd sp! , {r0-r12, lr}

	nop 

	ldmfd sp!, {r0-r12, pc}
