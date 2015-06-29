//Written by Phuong H. Hoang April, 2013
#ifndef __UART_H__
#define __UART_H__

#define UART_BAUD_RATE_4800 4800
#define UART_BAUD_RATE_9600 9600
#define UART_BAUD_RATE_38400 38400
#define UART_BAUD_RATE_57600 57600
#define UART_BAUD_RATE_115200 115200
#define UART_BAUD_RATE_921600 921600
#define UART_BAUD_RATE_460800 460800
#define UART_BAUD_RATE_230400 230400

#define SIZE_MAX_OF_CIRCULAR 20

extern VOID UartSendByte(PBYTE pData);

extern VOID UartSendByteConst(BYTE byData);

extern RESULT UartGetByte(PBYTE pByte);

extern RESULT UartOpenPort(DWORD nFCY, DWORD nBaudrate);    // Open port UART with baudrate

extern RESULT UartSendBuffer(PBYTE pBuffer, BYTE nLength);  // Send an array

extern VOID UartSendByteXprintf(BYTE byData);


#endif
