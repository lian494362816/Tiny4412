#ifndef __DATETYPE_H__
#define __DATETYPE_H__

#define FALSE  (0)
#define TRUE   (1)
#define SUCCESS (0)
#define FAIL (-1)
#define NULL (0)

typedef int (*PRINT_T) (const char *format, ...);

typedef volatile unsigned int    VU32;
typedef volatile unsigned short  VU16;
typedef volatile unsigned char   VU8;

typedef unsigned long long       U64;
typedef unsigned int             U32;
typedef unsigned short           U16;
typedef unsigned char            U8;
typedef unsigned long            ulong;
typedef float                    F32;
typedef double                   F64;

typedef long long                S64;
typedef signed int               S32;
typedef signed short             S16;
typedef signed char              S8;


#endif // __DATETYPE_H__
