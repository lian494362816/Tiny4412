#ifndef __MY_ERROR_H_
#define __MY_ERROR_H_

#define FG_BLACK     "\033[30m"
#define FG_RED       "\033[31m"
#define FG_GREEN     "\033[32m"
#define FG_YELLOW    "\033[33m"
#define FG_BLUE      "\033[34m"
#define FG_VIOLET    "\033[35m"
#define FG_DARKGREE  "\033[36m"
#define FG_WHITE     "\033[37m"

#define BG_BLACK     "\033[40m"
#define BG_RED       "\033[41m"
#define BG_GREEN     "\033[42m"
#define BG_YELLOW    "\033[43m"
#define BG_BLUE      "\033[44m"
#define BG_VIOLET    "\033[45m"
#define BG_DARKGREE  "\033[46m"
#define BG_WHITE     "\033[47m"

#define NONE         "\033[0m"

#ifdef __KERNEL__
#define my_print printk
#else
#define my_print printf
#endif

#define _STRING(x) #x
#define STRING(x) _STRING(x)

#define PRINT_INFO(...) do{\
    my_print(FG_GREEN "[%s]:","INF");\
    my_print(__VA_ARGS__);\
    my_print(BG_BLACK NONE);\
}while(0)

#define PRINT_WRN(...) do{\
    my_print(FG_YELLOW "[%s] [%s] [%d]:","WAR", __FILE__, __LINE__);\
    my_print(__VA_ARGS__);\
    my_print(BG_BLACK NONE);\
}while(0)

#define PRINT_ERR(...) do{\
    my_print(FG_RED "[%s] [%s] [%d]:","ERR", __FILE__, __LINE__);\
    my_print(__VA_ARGS__);\
    my_print(BG_BLACK NONE);\
}while(0)

#define PRINT_BUG(...) do{\
    my_print(FG_RED "[%s] [%s] [%s] [%d]:","BUG", __FILE__, __FUNCTION__, __LINE__);\
    my_print(__VA_ARGS__);\
    my_print(BG_BLACK NONE);\
    while(1);\
}while(0)

#define PRINT_FUNC_ERR(func) do{\
    my_print(FG_RED "[%s] [%s] [%d]: [%s] fail \n", "ERR", __FILE__, __LINE__, STRING(func));\
    my_print(BG_BLACK NONE);\
}while(0)

#endif //MY_ERROR_H_
