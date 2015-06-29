//Written by Phuong H. Hoang
/* This file contents functions used to config PWM module */
#define _dsPIC30F4012_
#include <p30F4012.h>
#include <dsp.h>
#include "PWM.h"
#include "hardware_descriptors.h"

/***** Function prototype*****/
void PwmMode(unsigned int pwm_mode);
void PwmTimebaseSetting(unsigned int pwm_prescale, unsigned int pwm_period);
void SetPwmDutycycle(unsigned int duty_cycle);
void SetPwmFreq(unsigned int freq);
void PwmEnable(void);
void SetPwmDutyCycle(unsigned int pwm_dutycycle);
void SetPwmPinout(void);
void EnablePwmPin(unsigned int pin);
void SetOvdPin(unsigned int pin);
void ClearOvdPin(unsigned int pin);
void SetPwmPin(unsigned int pin);
void ClearPwmPin(unsigned int pin);
void DisablePwmPin(unsigned int pin);
void InitPwm(unsigned int pwm_freq, unsigned int pwm_duty);

/***** Function implementation *****/
void PwmMode(unsigned int pwm_mode)
{
	switch (pwm_mode)
	{	
		case (FREE_MODE):
			PTCON = PTCON & 0xFFFC;
			break;
		case (SINGLE_SHOT_MODE):
			PTCON = PTCON & 0xFFFC;
			PTCON = PTCON | 0x0001;
			break;
		case (CONTINUOUS_UP_DOWN_MODE):
			PTCON = PTCON & 0xFFFC;
			PTCON = PTCON | 0x0002;
			break;
		case (DOUBLE_UPDATE_MODE):
			PTCON = PTCON | 0x0003;
			break;
		default: 
			break;
	}
}

void PwmTimebaseSetting(unsigned int pwm_prescale, unsigned int pwm_period)  // duoc goi trong ham SetPwmFreq
{
	switch (pwm_prescale)
	{
		case (1):
			PTCON = PTCON & 0xFFF3;	
			break;
		case (4):
			PTCON = PTCON & 0xFFF3;
			PTCON = PTCON | 0x0004;
			break;
		case (16):
			PTCON = PTCON & 0xFFF3;
			PTCON = PTCON | 0x0008;
			break;
		case (64):
			PTCON = PTCON | 0x000C;
			break;
		default:
			break;
	}
	if (pwm_period < 0x8000)
	{
		PTPER = pwm_period;
	}
}

void RunPositive(void)
{
    OVDCON = 0xFE00;
}

void RunNegative(void)
{
    OVDCON = 0xFD00;
}

void RunStop(void)
{
    OVDCON = 0xFC00;
}

void PwmEnable(void)

{
	PTCON = PTCON | 0x8000;
}

void SetPwmDutyCycle(unsigned int duty_cycle)
{
	unsigned int temp;
	if ((0 <= duty_cycle)&&(duty_cycle <= 100))
	{
		temp = PTPER / 100;								
		temp = temp * duty_cycle;
		PDC1 	= temp * 2;									// chon duty cycle cho khoi PWM
	}
		
}

void SetPwmFreq(unsigned int freq) 							// freq là tan so pwm tinh bang kHz
{
	unsigned int pwm_prescale;
	unsigned int pwm_period;
	unsigned int temp;
	unsigned int check_mode;
	check_mode = PTCON & 0x0003;
	pwm_prescale = 1;
	if ((check_mode == 0x0000) || (check_mode == 0x0001))	// FREE hoac SINGLE_SHOT MODE
	{
		temp = FCY_KHZ/freq;
		pwm_period = temp*pwm_prescale - 1;
		PwmTimebaseSetting(pwm_prescale, pwm_period);
	}
	else 													// center align
	{
		temp = FCY_KHZ/freq;
		pwm_period = temp*pwm_prescale/2 - 1;
		PwmTimebaseSetting(pwm_prescale, pwm_period);
	}
}

void EnablePwmPin(unsigned int pin)
{
	PWMCON1 = PWMCON1 | pin;
}

void DisablePwmPin(unsigned int pin)
{
	PWMCON1 = PWMCON1 & (pin^0xFFFF);
}
void SetOvdPin(unsigned int pin)
{
	OVDCON = OVDCON | pin;
}
void ClearOvdPin(unsigned int pin)
{
	OVDCON = OVDCON & (pin^0xFFFF);
}
void SetPwmPin(unsigned int pin)
{
	OVDCON = OVDCON | pin;
}
void ClearPwmPin(unsigned int pin)
{
	unsigned int temp;
	temp = pin ^ 0xFFFF;
	OVDCON = OVDCON & temp;
}

void SetPwmPinout(void)
{
	TRISEbits.TRISE0 = 0;						// chan ra pwm 1 L
	TRISEbits.TRISE1 = 0;						// chan ra pwm 1 H
	PWMCON1 = PWMCON1 | 0x0700;					// Chon cac khoi chay doc lap
	OVDCON = 0xFE00;
	PWMCON1 = PWMCON1 | 0x0011;
}

void InitPwm(unsigned int pwm_freq, unsigned int pwm_duty)
{
	PwmMode(FREE_MODE);							// chay o che do FREE_MODE
	SetPwmFreq(pwm_freq);						// tan so PWM 10kHz
	SetPwmDutyCycle(pwm_duty);					// 50% PWM	
	SetPwmPinout();								// chon che do chan
	PwmEnable();								// chay PWM
}
