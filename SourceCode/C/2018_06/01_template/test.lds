OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "lef32-littlearm")
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


