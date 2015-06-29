
// Written by Phuong H. Hoang, April, 2013
#include <p30f4012.h>
#include "typedef.h"  // Thu+' tu+. luo^n la` file thu+ vie^.n chip, typedef.h, "name.h" trong file name.c
#include "xprintf.h"
#include "UART.h"
#include "protocol.h"

BYTE pCircularBuffer[SIZE_MAX_OF_CIRCULAR] = {0};// Khai báo vùng nh? ??m quay vòng
BYTE g_nEnd = 0;   // Ch? s? c?a cho bi?t v? trí byte cu?i cùng nh?n ???c trong Circular Buffer
BYTE g_nStart = 0;
RESULT UartOpenPort(DWORD nFCY, DWORD nBaudRate)     // setup mode cho UART 1
{
    // This is an EXAMPLE, so brutal typing goes into explaining all bit sets

    // The HPC16 board has a DB9 connector wired to UART1, so we will
    // be configuring this port only

    // configure U1MODE
    U1MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
    //U1MODEbits.notimplemented;	// Bit14
    U1MODEbits.USIDL = 0;	// Bit13 Continue in Idle
    //U1MODEbits.IREN = 0;	// Bit12 No IR translation
    //U1MODEbits.RTSMD = 0;	// Bit11 Simplex Mode
    //U1MODEbits.notimplemented;	// Bit10
    //U1MODEbits.UEN = 0;		// Bits8,9 TX,RX enabled, CTS,RTS not
    U1MODEbits.WAKE = 0;	// Bit7 No Wake up (since we don't sleep here)
    U1MODEbits.LPBACK = 0;	// Bit6 No Loop Back
    U1MODEbits.ABAUD = 0;	// Bit5 No Autobaud (would require sending '55')
    //U1MODEbits.URXINV = 0;	// Bit4 IdleState = 1  (for dsPIC)
    //U1MODEbits.BRGH = 0;	// Bit3 16 clocks per bit period
    U1MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
    U1MODEbits.STSEL = 0;	// Bit0 One Stop Bit

    // Load a value into Baud Rate Generator.  Example is for 9600.
    // See section 19.3.1 of datasheet.
    U1BRG = (WORD)((nFCY / (16 * nBaudRate)) - 1);
    // Load all values in for U1STA SFR
    //U1STAbits.UTXISEL1 = 0;	//Bit15 Int when Char is transferred (1/2 config!)
    //U1STAbits.UTXINV = 0;	//Bit14 N/A, IRDA config
    //U1STAbits.UTXISEL0 = 0;	//Bit13 Other half of Bit15
    U1STAbits.UTXISEL = 0;
    //U1STAbits.notimplemented = 0;	//Bit12
    U1STAbits.UTXBRK = 0;	//Bit11 Disabled
    U1STAbits.UTXEN = 0;	//Bit10 TX pins controlled by periph
    U1STAbits.UTXBF = 0;	//Bit9 *Read Only Bit*
    U1STAbits.TRMT = 0;	//Bit8 *Read Only bit*
    U1STAbits.URXISEL = 0;	//Bits6,7 Int. on character recieved
    U1STAbits.ADDEN = 0;	//Bit5 Address Detect Disabled
    U1STAbits.RIDLE = 0;	//Bit4 *Read Only Bit*
    U1STAbits.PERR = 0;		//Bit3 *Read Only Bit*
    U1STAbits.FERR = 0;		//Bit2 *Read Only Bit*
    U1STAbits.OERR = 0;		//Bit1 *Read Only Bit*
    U1STAbits.URXDA = 0;	//Bit0 *Read Only Bit*

    //IPC7 = 0x4400;	//dspic33f Mid Range Interrupt Priority level, no urgent reason
    IPC2bits.U1RXIP = 7; // Set priority = 4 (medium)
    IPC2bits.U1TXIP = 1; // Set priority = 4 (medium)
    IFS0bits.U1TXIF = 0;	// Clear the Transmit Interrupt Flag
    IEC0bits.U1TXIE = 1;	// Enable Transmit Interrupts
    IFS0bits.U1RXIF = 0;	// Clear the Recieve Interrupt Flag
    IEC0bits.U1RXIE = 1;	// Enable Recieve Interrupts

    U1MODEbits.UARTEN = 1;	// And turn the peripheral on
    U1STAbits.UTXEN = 1;
    // I think I have the thing working now.
    return SUCCESS;						// using primary pins for UART1 module
}

VOID UartSendByteXprintf(BYTE byData)  //Hàm g?i 1 byte
{
    //if (IFS0bits.U1TXIF == 0)
    {
        U1TXREG = byData;       // gán d? li?u c?n g?i cho thanh ghi g?i
    }
    while (IFS0bits.U1TXIF == 0);// Ch? khi nào g?i xong (c? ng?t on)
    IFS0bits.U1TXIF = 0;         // Xóa c? ng?t g?i
}

VOID UartSendByte(PBYTE pData)  //Hàm g?i 1 byte
{
    //if (IFS0bits.U1TXIF == 0)
    {
        U1TXREG = *pData;       // gán d? li?u c?n g?i cho thanh ghi g?i
    }
    while (IFS0bits.U1TXIF == 0);// Ch? khi nào g?i xong (c? ng?t on)
    IFS0bits.U1TXIF = 0;         // Xóa c? ng?t g?i
}

VOID UartSendByteConst(BYTE byData)  //Hàm g?i 1 byte
{
    //if (IFS0bits.U1TXIF == 0)
    {
        U1TXREG = byData;       // gán d? li?u c?n g?i cho thanh ghi g?i
    }
    while (IFS0bits.U1TXIF == 0);// Ch? khi nào g?i xong (c? ng?t on)
    IFS0bits.U1TXIF = 0;         // Xóa c? ng?t g?i
}

RESULT UartSendBuffer(PBYTE pBuffer, BYTE nLength)  //Hàm g?i 1 m?ng ?? dài nLength
{
    WORD nIndex;
    for (nIndex = 0; nIndex < nLength; nIndex++)
    {
        UartSendByte(pBuffer + nIndex);              //B?n ch?t là g?i nhi?u l?n 1 byte
    }
    return SUCCESS;
}

RESULT UartGetByte(PBYTE pByte)
{
    if (g_nStart != g_nEnd)
    {
	*pByte = pCircularBuffer[g_nStart];
	g_nStart = (g_nStart + 1) % SIZE_MAX_OF_CIRCULAR;
	return SUCCESS;
    }
    else
        return UNSUCCESS;
}

void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void) // Hàm ng?t nh?n
{
    BYTE byData;
    byData = U1RXREG;                   //L?u tr? t?m th?i byte v?a nh?n
    pCircularBuffer[g_nEnd] = byData;   //??a ngay byte v?a nh?n vào Circular buffer
    //xprintf("\nPhan hoi thanh cong : %d", byData);     //Test
    //UartSendByte(&byData);              //Test
    g_nEnd = (g_nEnd + 1) % SIZE_MAX_OF_CIRCULAR;
    //g_bRXFlag = 1;
    IFS0bits.U1RXIF = 0;                // Xóa c? ng?t nh?n UART1
}
