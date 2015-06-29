/* On the internet many sources */


#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#define SET_BIT(Addr, Bit)      ((Addr) |= (1 << (Bit))) //Set bit
#define CLEAR_BIT(Addr, Bit)    ((Addr) &= ~(1 << (Bit))) //Clear bit
#define CHECK_BIT(Addr, Bit)    (((Addr) & (1 << (Bit))) != 0)//== (1 << (Bit)))

#ifndef DOUBLE
typedef double DOUBLE;
#endif

#ifndef PDOUBLE
typedef double* PDOUBLE;
#endif

#ifndef FLOAT
typedef float FLOAT;
#endif

#ifndef CHAR
typedef char CHAR;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef PCHAR
typedef char* PCHAR;
#endif

#ifndef PBYTE
typedef unsigned char* PBYTE;
#endif

#ifndef SHORT
typedef short SHORT;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef PSHORT
typedef short* PSHORT;
#endif

#ifndef PWORD
typedef unsigned short* PWORD;
#endif

#ifndef LONG
typedef long LONG;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef PLONG
typedef long* PLONG;
#endif

#ifndef PDWORD
typedef unsigned long* PDWORD;
#endif

#ifndef VOID
typedef void VOID;
#endif

#ifndef PVOID
typedef void* PVOID;
#endif

#ifndef HANDLE
typedef void* HANDLE;
#endif

#ifndef BOOL
typedef char BOOL;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

typedef unsigned char RESULT;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef SUCCESS
#define SUCCESS 1
#endif

#ifndef UNSUCCESS
#define UNSUCCESS 0
#endif

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif
typedef RESULT (*SYSTEMCALLBACK)(PVOID);

//#define PROGRAM_JOINT_1
//#define PROGRAM_JOINT_2

#endif
