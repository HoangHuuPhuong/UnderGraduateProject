#include "typedef.h"
#include "xprintf.h"
#include "hal_uart.h"
#include "protocol.h"

BYTE pDataProcessing[SIZE_MAX];

BYTE g_nState = CHECK;
BYTE g_nReceiveState = WAIT;
BYTE g_nIndex = 0;
BYTE g_nSum = 0;

BYTE nLength = 0;
BYTE byCommand = 0;
BYTE byReceiveByte = 0;

BOOL g_bCheckData = UNSUCCESS;

UARTCALLBACK fnUartCallBack;

//PACKAGE g_stPackage;
//PPACKAGE g_pPackage = &g_stPackage;

VOID UartMakePackage(PPACKAGE pPackage, BYTE nLengthData, BYTE nCommand, PBYTE pSendData)
{
    BYTE nIndex;
    BYTE nLength = nLengthData + 5;
    pPackage->nStart = 0xAA;
    pPackage->nLength = nLength;
    //pPackage->nHighByte = nLength / 256;
    //pPackage->nLowByte = nLength % 256;
    pPackage->nCommand = nCommand;
    pPackage->pData = pSendData;
    //pPackage->nCheckSum = pPackage->nHighByte + pPackage->nLowByte + pPackage->nCommand;
    pPackage->nCheckSum = pPackage->nLength + pPackage->nCommand;
    for (nIndex = 0; nIndex < nLengthData; nIndex++)
	pPackage->nCheckSum += pSendData[nIndex];
    pPackage->nStop = 0x55;
}

VOID UartMakePackageNotCheckSum(PPACKAGE pPackage, BYTE nLengthData, BYTE nCommand, PBYTE pSendData)
{
    pPackage->nStart = 0xAA;
    pPackage->nLength = nLengthData + 4;
    pPackage->nCommand = nCommand;
    pPackage->pData = pSendData;
    pPackage->nStop = 0x55;
}

VOID RegisterUartCallBack(UARTCALLBACK fnCallBack)
{
    fnUartCallBack = fnCallBack;
}

VOID UartSendPackage(PPACKAGE pPackage)
{
    BYTE pTempBuffer[128];	// If replace 256 by 1024, stack over flow
    BYTE nIndex;
    //BYTE nLength = pPackage->nHighByte * 256 + pPackage->nLowByte;
    pTempBuffer[0] = pPackage->nStart;
    pTempBuffer[1] = pPackage->nLength;
    //pTempBuffer[1] = pPackage->nLowByte;
    //pTempBuffer[2] = pPackage->nHighByte;
    pTempBuffer[2] = pPackage->nCommand;
    for (nIndex = 3; nIndex <  pPackage->nLength - 2; nIndex++)
        pTempBuffer[nIndex] = pPackage->pData[nIndex - 3];
    // after this instruction, nIndex = pPackage->nLength - 2
    pTempBuffer[nIndex++] = pPackage->nCheckSum;
    pTempBuffer[nIndex++] = pPackage->nStop;
    UartSendBuffer(pTempBuffer, nIndex);
}

VOID UartSendPackageNotCheckSum(PPACKAGE pPackage)
{
    BYTE pTempBuffer[128];	// If replace 256 by 1024, stack over flow
    BYTE nIndex;
    pTempBuffer[0] = pPackage->nStart;
    pTempBuffer[1] = pPackage->nLength;
    pTempBuffer[2] = pPackage->nCommand;
    for (nIndex = 3; nIndex <  pPackage->nLength - 1; nIndex++)
        pTempBuffer[nIndex] = pPackage->pData[nIndex - 3];
    pTempBuffer[nIndex++] = pPackage->nStop;
    UartSendBuffer(pTempBuffer, nIndex);
}

RESULT ProcessUartPackage()
{
    switch(g_nState)
    {
    case CHECK:
	if (UartGetByte(&byReceiveByte) == SUCCESS)
	{
            g_nState = PROCESS;
            //xprintf("\nCHECK");
	}
	break;
    case PROCESS:
	g_nState = CHECK;
	{
            switch(g_nReceiveState)
            {
            case RUN:
                g_nIndex++;
                switch (g_nIndex)
                {
                    case 1:
                        nLength = byReceiveByte;
                        break;
                    case 2:
                        byCommand = byReceiveByte;
                        break;
                    default:
                        pDataProcessing[g_nIndex - 3] = byReceiveByte;
                        break;

                }
                //xprintf("\nRUN");
                //pDataProcessing[g_nIndex] = *pDataProcessing;
                // calcultate checksum byte
		if ((g_nIndex == nLength - 1) && (g_nIndex > 1))
                {
                    g_nReceiveState = WAIT;
                    g_nState = FINISH;
		}
		if ((g_nIndex < nLength - 2) || (g_nIndex <= 1))
                    g_nSum += byReceiveByte;
                    break;
            case WAIT:
                if (byReceiveByte == START_BYTE)
                    g_nReceiveState = RUN;
                //xprintf("\nWAIT");
		break;
            }
	}
	break;
    case FINISH:
	if ((pDataProcessing[g_nIndex - 3] == STOP_BYTE) && (pDataProcessing[g_nIndex - 4] == g_nSum))
	{
            PACKAGE stPackage;
            PPACKAGE pPackage = &stPackage;
            // Create data package
            UartMakePackage(pPackage, nLength - 5, byCommand,  pDataProcessing);
            // Process data package
            //xprintf("\nFINISH");
            fnUartCallBack(pPackage);
	}
	g_nIndex = 0;
	g_nSum = 0;
	g_nState = CHECK;
	break;
    }
    return SUCCESS;
}
