OUTPUT_FORMAT("elf32-littlearm", "elf32-bitarm", "elf32-littelarm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x40008000;
	.text :
	{
		test.o(*);
		*.o(*);
	}
	align = 4;

}
