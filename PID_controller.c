//Written by Phuong H. Hoang

#define _dsPIC30F4012_
#include <p30F4012.h>
#include <dsp.h>
/***** inclusion *****/
#include <string.h>
#include "typedef.h"
#include "timer1.h"
#include "timer2.h"
#include "timer3.h"
#include "pwm.h"
#include "adc.h"
#include "qei.h"
#include "hal_uart.h"
#include "process.h"
#include "protocol.h"
#include "hardware_descriptors.h"

#define FCY_HZ          16000000    // FCY = FOSC / 4
#define UART_BAUDRATE   9600      //9600

/***** Init hardware configuration *****/
_FOSC(CSW_FSCM_OFF & XT_PLL8);			// set clock source and clock frequency
_FWDT(WDT_OFF);
_FBORPOR(MCLR_EN & PBOR_OFF & PWMxL_ACT_HI & PWMxH_ACT_HI);
_FGS(CODE_PROT_OFF);

/***** variables declaration *****/
//unsigned int	baudrate;
//char            byte_to_transmit;
long int	real_pos;       //Uart send
long int 	last_pos;
long int 	error;
long int 	begin_pos;
long int 	set_pos;        //Uart send
//long int        abc;
fractional 	set_v;
fractional 	real_v;         //Uart send
fractional	set_i;
fractional	real_i;         //Uart send
unsigned int 	qei_buf;
long int	pwm_duty;
//char	output_buffer[8];
//char    first_byte = 0;
//char 	tx_flag = 0;
tPID	strPID_v;
tPID	strPID_i;
tPID	*strPID;

BOOL g_bRunFlag = OFF;
BOOL g_bUartFlag = OFF;

fractional PIDGain_v[3];
fractional PIDGain_i[3];
fractional abcCoeff_v[3] __attribute__ ((section (".xbss, bss, xmemory")));
fractional abcCoeff_i[3] __attribute__ ((section (".xbss, bss, xmemory")));
fractional ControlSample_i[3] __attribute__ ((section (".ybss, bss, ymemory")));
fractional ControlSample_v[3] __attribute__ ((section (".ybss, bss, ymemory")));
void Delay_ms(unsigned int);
//long int z,b,c,d,temp1,temp2,abc=0,xyz=0;
//int h=0;
//unsigned char a[50];
//double g_dKpp = 0.1;
/***** main functions *****/
int main ()
{
//	receive_ok = 0;						// init the receive_ok variable
//	InitUART1();						// init the UART 1 module
	InitQei();						// init QEI module
	InitAdc();                                              // init ADC module
	UartOpenPort(FCY_HZ, UART_BAUDRATE);
        RegisterUartCallBack(ProcessDataPackage);
        InitTimer1(T1_PRESCALE, T1_PER);			// timer1 scale is 8, period = 1000
	InitTimer2(T2_PRESCALE, T2_PER);			// timer1 scale is 8, period = 12500
	InitTimer3(T3_PRESCALE, T3_PER);			// timer1 scale is 8, period = 65536
	InitPwm(PWM_FREQ,PWM_DUTY);				// init PWM module, 5 kHz, 0

// initializing pins
	_TRISD1 = 0;
	_TRISE2 = 0;
	_LATE2 	= 1;
	_TRISB1 = 0;
	_LATB1	= 0;
	_TRISC13 = 1;
	_TRISC14 = 1;
	_TRISD0 = 1;

// initializing UART modules variables
	//receive_ok = 0;
	//receive_temp = 0;
	//rx_count = 0;
	//tx_count = 0;

// initializing
	real_pos = 0;
	last_pos = 0;
	qei_buf = 0;
	set_v = 0;
	real_v = 0;
	set_i = 0;
	real_i = 0;
	pwm_duty = 0;

// cac he so PID
	PIDGain_v[0] = 1;
	PIDGain_v[1] = Q15(0.05);
	PIDGain_v[2] = 0;
	PIDGain_i[0] = 1; //Q15(0.02);
	PIDGain_i[1] = Q15(0.002);
	PIDGain_i[2] = 0;
// khoi tao cac struct PID de tinh toan
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
	while(1)
	{
            ProcessUartPackage();
            if ((g_bUartFlag == ON) && (g_bRunFlag == ON))
            {
                g_bUartFlag = OFF;
                BYTE byCommandUart = 0;
                LONG lSpeed = (LONG)real_v;
                byCommandUart = byCommandUart | CURRENT_POSITIVE;
                UartSendByteConst(START_BYTE);
                UartSendByteConst(18);          //Information package have 18 byte
                UartSendByte(&byCommandUart);
                UartSendBuffer((PBYTE)(&real_pos), 4);
                UartSendBuffer((PBYTE)(&lSpeed), 4);
                UartSendBuffer((PBYTE)(&real_i), 2);
                UartSendBuffer((PBYTE)(&set_pos), 4);
                UartSendByteConst(STOP_BYTE);
            }
	}
        return 1;
}
