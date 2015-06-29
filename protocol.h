#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

typedef RESULT (*UARTCALLBACK)(PVOID);

typedef struct tagPACKAGE
{
	BYTE nStart;  // 0xAA
	BYTE nLength; // ddo^. da`i go'i tin ke^? byte ba('t dda^`u
	//BYTE nLowByte;	// Low byte of package low
	//BYTE nHighByte;	// High byte of package length
	BYTE nCommand;
	PBYTE pData;
	BYTE nCheckSum;	// to^?ng tra`n cu?a ta^'t ca? ca'c byte tru+o+'c ddo', kho^ng ke^? byte ba('t dda^`u
	BYTE nStop;   // 0x55 
} PACKAGE, *PPACKAGE;

#define SIZE_MAX 100

#define PROCESS 1
#define CHECK 0

#ifndef FINISH
#define FINISH 3
#endif


#define START_BYTE 0xAA
#define STOP_BYTE 0x55

#ifndef RUN
#define RUN 1
#endif

#ifndef WAIT
#define WAIT 2
#endif

#ifndef EQUAL
#define EQUAL 0
#endif

#ifndef UNEQUAL
#define UNEQUAL 1
#endif

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16
#endif

extern VOID RegisterUartCallBack(UARTCALLBACK fnCallBack);

extern VOID UartMakePackage(PPACKAGE pPackage, BYTE nLengthData, BYTE nCommand, PBYTE pSendData);

extern VOID UartMakePackageNotCheckSum(PPACKAGE pPackage, BYTE nLengthData, BYTE nCommand, PBYTE pSendData);

extern VOID UartSendPackage(PPACKAGE pPackage);

extern VOID UartSendPackageNotCheckSum(PPACKAGE pPackage);

extern RESULT ProcessUartPackage();



#endif
