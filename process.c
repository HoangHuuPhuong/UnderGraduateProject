//Written by Phuong H. Hoang
#include <p30F4012.h>
#include <dsp.h>
#include "typedef.h"
#include "hal_uart.h"
#include "PWM.h"
#include "protocol.h"
#include "process.h"
#include "xprintf.h"
#include "timer1.h"

extern long int		real_pos;
extern long int 	last_pos;
extern long int 	error;
extern long int 	begin_pos;
extern long int 	set_pos;
//extern long int         abc;
extern fractional 	set_v;
extern fractional 	real_v;
extern fractional	set_i;
extern fractional	real_i;
extern unsigned int 	qei_buf;
extern BOOL g_bRunFlag;
extern BOOL g_bUartFlag;
extern fractional PIDGain_v[3];
extern fractional PIDGain_i[3];
//extern double g_dKpp;
extern tPID	strPID_v;
extern tPID	strPID_i;
extern tPID	*strPID;
extern fractional abcCoeff_v[3] __attribute__ ((section (".xbss, bss, xmemory")));
extern fractional abcCoeff_i[3] __attribute__ ((section (".xbss, bss, xmemory")));
extern fractional ControlSample_i[3] __attribute__ ((section (".ybss, bss, ymemory")));
extern fractional ControlSample_v[3] __attribute__ ((section (".ybss, bss, ymemory")));
RESULT UartFeedback(PVOID pFeedback)
{
    UartSendPackage(pFeedback);
    return SUCCESS;
}

WORD wDuty;
WORD nI;

RESULT ProcessDataPackage(PVOID pDataPackage)
{
	PPACKAGE pPackage = (PPACKAGE)pDataPackage;
        PACKAGE stPackage;

	switch (pPackage -> nCommand)
	{
	case SET_POSITION_POSITIVE:
            //xprintf("\nNhan du lieu thanh cong positive");   //Test
            //RunStop();
            //g_bRunFlag = OFF;
            //g_bUartFlag = OFF;
            set_pos = (LONG)(*((PWORD)(pPackage->pData)));
            //xprintf("\n%d", *(pPackage->pData));
            //xprintf("\n%d", pPackage->pData[1]);
            //g_sDirection = POSITIVE;
            break;
//        case SET_POSITION_NEGATIVE:
            //xprintf("\nNhan du lieu thanh cong negative");   //Test
            //g_lSetPosition = -(*(pPackage->pData) + 256 * pPackage->pData[1]);
//          break;
        case CHECKCOM:
#ifdef PROGRAM_JOINT_1
            UartMakePackageNotCheckSum(&stPackage, 0, PORT_COM_JOINT_1, NULL);
            UartSendPackageNotCheckSum(&stPackage);
            for (nI = 0; nI < 60000; nI++);
            UartSendPackageNotCheckSum(&stPackage);
#endif
#ifdef PROGRAM_JOINT_2
            UartMakePackageNotCheckSum(&stPackage, 0, PORT_COM_JOINT_2, NULL);
            UartSendPackageNotCheckSum(&stPackage);
            for (nI = 0; nI < 60000; nI++);
            UartSendPackageNotCheckSum(&stPackage);
#endif
            break;
        case START:
            g_bRunFlag = ON;
            //g_bUartFlag = ON;
            //if (g_lSetPosition >= g_lPresentPosition)
//            {
//                //RunPositive();
//            }
//            else
//            {
//               //RunNegative();
//            }
            break;
        case STOP:
            g_bRunFlag = OFF;
            //g_bUartFlag = OFF;
            //RunStop();
            break;
        /*case KP_POS:
            g_dKpp = *((PDOUBLE)(pPackage->pData));
            break;
        case KP_V:
            PIDGain_v[0] = Q15(*((PDOUBLE)(pPackage->pData)));
            PIDCoeffCalc(PIDGain_v, strPID);
            break;
        case KI_V:
            PIDGain_v[1] = Q15(*((PDOUBLE)(pPackage->pData)));
            break;
        case KP_I:
            PIDGain_i[0] = Q15(*((PDOUBLE)(pPackage->pData)));
            break;
        case KI_I:
            PIDGain_i[1] = Q15(*((PDOUBLE)(pPackage->pData)));
            break;*/
        case STATE_SEND_DATA:
            strPID = &strPID_v;
            strPID->abcCoefficients = abcCoeff_v;
            PIDCoeffCalc(PIDGain_v, strPID);
            strPID->controlHistory = ControlSample_v;
            strPID->controlReference = set_v;
            strPID->measuredOutput = real_v;
            PIDInit(strPID);
            strPID = &strPID_i;
            strPID->abcCoefficients = abcCoeff_i;
            PIDCoeffCalc(PIDGain_i, strPID);
            strPID->controlHistory = ControlSample_i;
            strPID->controlReference = set_i;
            strPID->measuredOutput = real_i;
            PIDInit(strPID);
            UartMakePackageNotCheckSum(&stPackage, 0, SEND_DATA_FEEDBACK, NULL);
            UartSendPackageNotCheckSum(&stPackage);
            break;
	}
	return SUCCESS;
}
