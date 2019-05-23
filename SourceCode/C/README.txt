
2018_05
	asm_File
		asm_test.c 
			最简单的汇编程序，实现100+200
		asm_add_sub_mul.c
			用汇编实现加 减 乘
		asm_mov.c
			简单的赋值指令
		asm_ldr.c
			大于0xff 就需要使用ldr来赋值
		asm_bic.c
			清除某些位	
		asm_add_orr_mvn.c 
			与 或 按位取反
		asm_b.c 
			简答的跳转
		asm_cmp.c
			比较 两个寄存器
		asm_loop.c 
			1加到100循环	
		asm_pc.c
			检查pc指针的作用
		asm_stack.c
			push pop 命令
		asm_lr.c 
			链接寄存器，stmfd ldmfd 命令
		asm_mrs.c
			操作特殊寄存器CPSR 
		asm_shift.c
			位移，算数左右，逻辑左 
		asm_ldr_str.c    
			将指针传给寄存器
		asm_string.c
			计算字符串的长度
		asm_copy.c 
			复制字符串
	S_File
		板子：在uboot下输入 dnw  0x40008000
		PC  ：dnw test.bin
		板子: go 0x40008000
			
		01_test
			简单的测试程序，跑空指令	

		02_printf
			打印字符串

		03_led
			点亮4个led

		04_led_advance
			4个led闪烁

		05_buzzer
			启动蜂鸣器

		06_key
			按下键，打印字符串

		07_led_buzzer_key
			按下键，led点亮, 蜂鸣器启动，打印字符串
			
	C_File
		01_print 
			使用C语言实现打印

		02_led  
			使用C语言配置led	

		03_key 
			使用C语言配置按键

		04_led_key_buzzer
			使用C语言配置 led 按键 蜂鸣器

2018_06			
	01_template
		简单的C程序， 将led key buzzer 合并起来

	02_rtc
		rtc 程序, Makefile写法向标准修改
			

