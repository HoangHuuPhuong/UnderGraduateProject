
//Phuong H. Hoang
/* This file contents functions used to config ADC module */
#define _dsPIC30F4012_
#include <p30F4012.h>
#include <dsp.h>
#include "typedef.h"
#include "adc.h"
#include "hardware_descriptors.h"
#include "hal_uart.h"
#include "timer3.h"

/***** Global variables *****/
//unsigned long 	k = 0;
//unsigned int 	adc_value = 0;

/***** Function prototype *****/
void ADInputSelect(unsigned int pin);		// cau hinh thanh ghi ADPCFG
void Channel0ScanPin (unsigned int pin);	// cau hinh thanh ghi ADCSSL

//config ADCON1
void TurnAdcOn(void);
void TurnAdcOff(void);
void TurnAdcIdleOn(void);
void TurnAdcIdleOff(void);
void SelectDataType(unsigned int data_type);
void EnableAdcSimSam(void);
void DisableAdcSimSam(void);
void EnableAdcAutoSampling(void);
void DisableAdcAutoSampling(void);
void AdcSoftwareStartSample(void);
void AdcSoftwareStartConversion(void);
void AutoStartMode(void);
void SoftwareMode(void);
void Timer3Mode(void);
void Int0Mode(void);
void PwmStartMode(void);

//config ADCON2
void AdcReferenceVoltage(unsigned int voltage_source);
void Channel0ScanOn(void);
void Channel0ScanOff(void);
void SelectAdcChannel(unsigned int channel);
void Use8WordsBuffer(void);
void Use16WordsBuffer(void); 
void UseAltPins(void);
void DoNotUseAltPins(void);
void NumberOfConversionBeforeInterrupt(unsigned int number);

// Config ADCON3
void SetAdcClock (unsigned int freq);						// Adc clock min bang Fcy/32
void AdcClockSource (unsigned int source);
void NumberOfClockBetweenSampleConversion(unsigned int number);

// enable AD interrupt
void EnableAdcInterrupt(void);
void DisableAdcInterrupt(void);
void SetAdcInterruptPriority(unsigned int prior);
// doc ADC BUFFER
unsigned int ReadAdcBuffer (unsigned int buffer);			// gia tri truyen vao la ten ADCBUF0-ADCBUFF

// Initialize ADC module
void InitAdc(void);

// AD interrupt routine
void __attribute__ ((interrupt, no_auto_psv)) _ADCInterrupt(void);

/**** Funtion implementation  *****/
void ADInputSelect(unsigned int pin)
{
	switch (pin)
	{
		case PIN_B0:
			_TRISB0 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B0);			
			break;
		case PIN_B1:
			_TRISB1 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B1);						
			break;
		case PIN_B2:
			_TRISB2 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B2);				
			break;
		case PIN_B3:
			_TRISB1 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B3);				
			break;
		case PIN_B4:
			_TRISB4 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B4);				
			break;
		case PIN_B5:
			_TRISB5 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B5);				
			break;
/*
		case PIN_B6:
			_TRISB6 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B6);
			break;
		case PIN_B7:
			_TRISB7 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B7);
			break;
		case PIN_B8:
			_TRISB8 = 1;
			ADPCFG = ADPCFG & (0xFFFF^PIN_B5);
			break;
*/			// these pins are only included in dsPIC30F4011
		default:
			break;
	}
}

void Channel0ScanPin(unsigned int pin)  // call after CSCNA = 1
{
	switch (pin)
	{
		case PIN_B0:			
			ADCSSL= ADCSSL | PIN_B0;			
			break;
		case PIN_B1:
			ADCSSL= ADCSSL | PIN_B1;			
			break;
		case PIN_B2:
			ADCSSL= ADCSSL | PIN_B2;			
			break;
		case PIN_B3:
			ADCSSL= ADCSSL | PIN_B3;			
			break;
		case PIN_B4:
			ADCSSL= ADCSSL | PIN_B4;			
			break;			
		case PIN_B5:
			ADCSSL= ADCSSL | PIN_B5;			
			break;
		case PIN_B6:
			ADCSSL= ADCSSL | PIN_B6;			
			break;
		case PIN_B7:
			ADCSSL= ADCSSL | PIN_B7;			
			break;
		case PIN_B8:
			ADCSSL= ADCSSL | PIN_B8;			
			break;
		default:
			break;
	}
}

void SelectDataType(unsigned int data_type)
{
	ADCON1 = ADCON1 & 0xFCFF;					// xoa cac bit FORM
	ADCON1 = ADCON1 | data_type;
}

void AdcReferenceVoltage (unsigned int voltage_source)
{
	ADCON2 = ADCON2 & 0x1FFF;			// xoa cac bit VCFG
	ADCON2 = ADCON2 | voltage_source;
}

void SelectAdcChannel(unsigned int channel)
{
	ADCON2 = ADCON2 & 0xFCFF;			// xoa cac bit CHPS
	ADCON2 = ADCON2 | channel;
}

void EnableAdcSimSam(void)
{
	ADCON1 = ADCON1 | 0x0008;
}

void DisableAdcSimSam(void)
{
	ADCON1 = ADCON1 & 0xFFF7;
}
void AdcSoftwareStartSample(void)
{
	ADCON1 = ADCON1 | 0x0002;
}

void AdcSofwareStartConversion(void)
{
	ADCON1 = ADCON1 & 0xFFFD;
}

void EnableAdcAutoSampling(void)
{
	ADCON1 = ADCON1 | 0x0004;
}

void DisableAdcAutoSampling(void)
{
	ADCON1 = ADCON1 & 0xFFFB;
}
 
void TurnAdcOn(void)
{
	ADCON1 = ADCON1 | 0x8000;
}

void TurnAdcOff(void)
{
	ADCON1 = ADCON1 & 0x7FFF;
}

void TurnAdcIdleOn(void)
{
	ADCON1 = ADCON1 | 0x2000;
}

void TurnAdcIdleOff(void)
{
	ADCON1 = ADCON1 & 0xDFFF;
}

void Channel0ScanOn(void)
{
	ADCON2 = ADCON2 | 0x0400;
}
void Channel0ScanOff(void)
{
	ADCON2 = ADCON2 & 0xFBFF;
}

void Use8WordsBuffer(void)
{
	ADCON2 = ADCON2 | 0x0002;
}
void Use16WordsBuffer(void)
{
	ADCON2 = ADCON2 & 0xFFFD;
}

void UseAltPins(void)
{
	ADCON2 = ADCON2 | 0x0001;
}
void DoNotUseAltPins(void)
{
	ADCON2 = ADCON2 & 0xFFFE;
}

void NumberOfConversionBeforeInterrupt(unsigned int number)
{
	ADCON2 = ADCON2 & 0xFFC3;	// xoa cac bit SMPI
	switch (number)
	{
		case (1):	
			ADCON2 = ADCON2 | 0x0000;
			break;
		case (2):	
			ADCON2 = ADCON2 | 0x0004;
			break;
		case (3):	
			ADCON2 = ADCON2 | 0x0008;
			break;
		case (4):	
			ADCON2 = ADCON2 | 0x000C;
			break;
		case (5):	
			ADCON2 = ADCON2 | 0x0010;
			break;
		case (6):
			ADCON2 = ADCON2 | 0x0014;
			break;
		case (7):	
			ADCON2 = ADCON2 | 0x0018;
			break;
		case (8):	
			ADCON2 = ADCON2 | 0x001c;
			break;
		case (9):	
			ADCON2 = ADCON2 | 0x0020;
			break;
		case (10):	
			ADCON2 = ADCON2 | 0x0024;
			break;
		case (11):	
			ADCON2 = ADCON2 | 0x0028;
			break;
		case (12):	
			ADCON2 = ADCON2 | 0x002C;
			break;
		case (13):	
			ADCON2 = ADCON2 | 0x0030;
			break;
		case (14):	
			ADCON2 = ADCON2 | 0x0034;
			break;
		case (15):	
			ADCON2 = ADCON2 | 0x0038;
			break;
		case (16):	
			ADCON2 = ADCON2 | 0x003C;
			break;
		default:
			break;
	}	
}

void AutoStartMode(void)
{
	ADCON1 = ADCON1 | 0x00E0;
}
void SoftwareMode(void)
{
	ADCON1 = ADCON1 & 0xFF1F;
}
void Timer3Mode(void)
{
	ADCON1 = ADCON1 & 0xFF1F;// clear bit 765 SSRC
	ADCON1 = ADCON1 | 0x0040;//010 SSRC(765)
}
void Int0Mode(void)
{
	ADCON1 = ADCON1 & 0xFF1F;
	ADCON1 = ADCON1 | 0x0020;
}
void PwmStartMode(void)
{
	ADCON1 = ADCON1 & 0xFF1F;
	ADCON1 = ADCON1 | 0x0060;
}

void SetAdcClock(unsigned int freq) // in kHz
{
	unsigned int temp;
	if (freq < 12000 )
	{
		temp = FCY_KHZ/freq;
		temp = temp * 2 - 1;
		temp = temp & 0x003F;
		ADCON3 = ADCON3 | temp;
	}
}

void AdcClockSource(unsigned int source)
{
	if (source == INTERNAL)
		ADCON3 = ADCON3 | INTERNAL;
	if (source == EXTERNAL)
		ADCON3 = ADCON3 & EXTERNAL;
}

void NumberOfClockBetweenSampleConversion(unsigned int number)
{
	unsigned int temp;
	temp = number;
	if (number < 32)
	{
		temp = temp << 8;
		temp = temp & 0x1F00;
		ADCON3 = ADCON3 | temp;
	}
}

// enable AD interrupt
void EnableAdcInterrupt(void)
{	
	IEC0bits.ADIE = 1;
}
void DisableAdcInterrupt(void)
{
	IEC0bits.ADIE = 0;
}
void SetAdcInterruptPriority(unsigned int prior)
{
	prior 	= prior << 12;
	IPC2 	= IPC2 & 0x0FFF;
	IPC2 	= IPC2 | prior;
}

// doc ADC buffer
unsigned int ReadAdcBuffer (unsigned int buffer)			// gia tri truyen vao la ten ADCBUF0-ADCBUFF
{
	unsigned int temp;
	temp = buffer;
	return temp;
}

void InitAdc(void)
{
	EnableAdcInterrupt();
	SetAdcInterruptPriority(ADC_INT_PRIOR);
	SelectDataType(UNSIGNED_INT);
	AdcReferenceVoltage(AVDD_AVSS);
	ADInputSelect(PIN_B0);
	SelectAdcChannel(CHANNEL_0);
	ADCHS = 0x0000; 										// Channel 0 doc gia tri gia AN0 va -VRef
	DoNotUseAltPins();
	Channel0ScanOff();
	//Channel0ScanOn();
	Use16WordsBuffer();
	NumberOfConversionBeforeInterrupt(1);
	NumberOfClockBetweenSampleConversion(3);
	AdcClockSource(INTERNAL);
	SetAdcClock(8000);										// Tad = 2*Tcy = 125ns
	EnableAdcAutoSampling();
	TurnAdcIdleOff();
	EnableAdcSimSam();
	Timer3Mode();											// dung timer 3 lam trigger
	//AutoStartMode();        								// co the chon Software start mode nhung de khoi dong 																// can dung 2 ham AdcSoftwareStartSample/conversion
	TurnAdcOn();
	Timer3On();
	AdcSoftwareStartSample();								// set ADCON1bits.SAMP = 1 to start sampling	
}


void __attribute__ ((interrupt, no_auto_psv)) _ADCInterrupt(void)
{		
	fractional temp;
	unsigned int data;
	long int error_pos;
	IFS0bits.ADIF = 0;	// xoa co ngat	

	data = ReadAdcBuffer(ADCBUF0);
	real_i = (fractional)data;
// tinh toan PID
	strPID_i.measuredOutput = real_i;
	strPID_i.controlReference = set_i;
	PID(&strPID_i);
	temp = strPID_i.controlOutput;
// Tinh toan gia tri bam xung
	pwm_duty = (long int)temp;
	real_pos = real_pos - (long int)qei_buf + (long int)POSCNT;
	qei_buf =  POSCNT;
	error_pos = set_pos - real_pos;
	if ((-30 < error_pos) && (error_pos < 30))
		pwm_duty = 0;
	else
		pwm_duty = temp;
	if (pwm_duty < 0)
		pwm_duty = 0;
	if (pwm_duty > PTPER)
		pwm_duty = PTPER;
	PDC1 = 2 * (unsigned int)(pwm_duty);
	ADCON1bits.SAMP = 1; 									// set SAMP = 1 to begin another sample
	TurnAdcOn();
}
