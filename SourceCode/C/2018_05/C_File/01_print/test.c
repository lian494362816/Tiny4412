

//program: 使用C语言测试printf函数
//date:2018.05.29

//函数指针
int (*printf)(const char *format, ...) = (void *) (0xc3e11ad0);

int _start(void)
{

	printf("hello world %d %d\n", 1, 2);

	return 0;
}



		
