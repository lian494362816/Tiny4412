
@printf 函数的只在uboot包 System.map 里面查找出来的
@地址位0xc3e11ad0

.global _start
_start:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =String
	ldr r1, =0xc3e11ad0
	
	mov lr, pc
	mov pc, r1

	ldmfd sp!, {r0-r12, pc}

String:
	.string "hello uboot printf...\n"
	.align 2
