#ifndef _HARDWARE_DESCRIPTORS_H_
#define _HARDWARE_DESCRIPTORS_H_
#include <dsp.h>
/**** Global define ****/
#define FCY				16000000                //24000000			// intruction circle
#define FCY_KHZ				16000                   //24000
#define FCY_MHZ                         16                      //24
// UART1 definitions
#define	BAUDRATE				9600
#define UART1_RINT_PRIOR		1
#define UART1_TINT_PRIOR		7
#define TRANSMIT_ENABLE()		_LATD1 = 1;
#define TRANSMIT_DISABLE()		_LATD1 = 0;
#define RECEIVE_LENGTH			4
// Timer 1 definitions - for 50ms interrupt
// The formula to calculate the cycle of the timer 1 is: T = T1_PER*T1_PRESCALE/Fcy 
#define T1_PER 					25000//100ms
#define T1_PRESCALE				64
#define T1_INT_PRIOR                            2
// Timer 2 definitions - for 5ms interrupt
// The formula to calculate the cycle of the timer 1 is: T = T2_PER*T2_PRESCALE/Fcy 
#define T2_PER 					20000//10ms
#define T2_PRESCALE				8
#define T2_INT_PRIOR                            3
// Timer 2 definitions - for 0.5ms interrupt
// The formula to calculate the cycle of the timer 1 is: T = T3_PER*T3_PRESCALE/Fcy 
#define T3_PER 					2000// 1ms
#define T3_PRESCALE				8
#define T3_INT_PRIOR                            4
// QEI definitions
#define QEI_MAX					60000
#define QEI_INT_NUM				1
#define	QEI_INT_PRIOR                           5
// PWM definitions
#define PWM_FREQ				5
#define PWM_DUTY				0
// ADC definitions
#define ADC_INT_PRIOR                           6

// NODE ID define
#define NODE_ID					2

// SPEDD define
#define SPEED_1					20
#define SPEED_2					70
#define SPEED_3					40
#define SPEED_4					50
#define SPEED_5					70
#define SPEED_6					20
#define SPEED_7					40
#define SPEED_8					50
#define SPEED_9					20
#define SPEED_10				70

// opcode definitions
#define OPCODE_SET			0x00
#define OPCODE_P_REQUEST		0x01
#define OPCODE_V_REQUEST		0x02
#define OPCODE_I_REQUEST		0x03
//PID parametters
#define KPP  				0.1
	


#endif // _HARDWARE_DESCRIPTORS_H_
