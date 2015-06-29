//Written by Phuong H. Hoang
/* This file contents functions used for configurating QEI module */
#define _dsPIC30F4012_
#include <p30F4012.h>
#include <dsp.h>
#include "typedef.h"
#include "QEI.h"
#include "hardware_descriptors.h"
#include "hal_uart.h"

//cac ham cau hinh QEICON
void SetQeiMode(unsigned int mode);
void EnableQeiIdle(void);
void DisableQeiIdle(void);
void EnableIndexToResetCounter(void);
void DisableIndexToResetCounter(void);

// Cau hnh QFLTCON
void EnableDigitalFilter(void);
void DisableDigitalFilter(void);
void EnableQeiErrorChecking(void);
void DisableQeiErrorChecking(void);

// cau hinh ngat
void EnableQeiInterrupt(void);
void DisableQeiInterrupt(void);
void SetQeiInterruptPriority(unsigned int prior);

// cau hinh ADPCFG
void SetQeiPins(void);

// ghi gia tri max count
void SetCounterMaxCount(unsigned int max);

// init QEI
void InitQei(void);

// QEI interrupt routine 
void __attribute__ ((interrupt, no_auto_psv)) _QEIInterrupt(void);

/****** Function Implemetation  *****/
void SetQeiMode(unsigned int mode)
{	
	QEICON = QEICON & 0xF8FF; 	// xoa cac bit QEIM
	QEICON = QEICON | mode;
}

void EnableQeiIdle(void)
{
	QEICON = QEICON & 0xDFFF;
}

void DisableQeiIdle(void)
{
	QEICON = QEICON | 0x2000;
}

void EnableIndexToResetCounter(void)
{
	QEICON = QEICON | 0x1000;
}

void DisableIndexToResetCounter(void)
{
	QEICON = QEICON & 0xEFFF;
}

void EnableDigitalFilter(void)
{
	DFLTCON = DFLTCON | 0x00F0;
}

void DisableDigitalFilter(void)
{
	DFLTCON = DFLTCON & 0xFF0F;
}

void EnableQeiErrorChecking(void)
{
	DFLTCON = DFLTCON | 0x0100;
}

void DisableQeiErrorChecking(void)
{
	DFLTCON = DFLTCON &0xFEFF;
}

void EnableQeiInterrupt(void)
{
	IEC2bits.QEIIE = 1;
}

void DisableQeiInterrupt(void)
{
	IEC2bits.QEIIE = 0;
}
void SetQeiInterruptPriority(unsigned int prior)
{
	IPC10 = IPC10 & 0xFFF8;
	IPC10 = IPC10 | prior;// bit 0 den 2 QEIIP
}

void SetQeiPins(void)
{
	_TRISB3 = 1;
	_TRISB4 = 1;
	_TRISB5 = 1;
	ADPCFG = ADPCFG | 0x0038;
}

void SetCounterMaxCount(unsigned int max)
{
	MAXCNT = max;
}

void InitQei(void)
{
	SetQeiMode(MODEx4);
	SetQeiPins();
	DisableQeiIdle();
	DisableIndexToResetCounter();
	EnableDigitalFilter();
	DisableQeiErrorChecking();
	EnableQeiInterrupt();
	SetCounterMaxCount(QEI_MAX);
	SetQeiInterruptPriority(QEI_INT_PRIOR);
	EnableQeiInterrupt();
	POSCNT = 0x0000;
}

// ham ngat QEI
void __attribute__ ((interrupt, no_auto_psv)) _QEIInterrupt(void)
{
	IFS2bits.QEIIF = 0;
	if (QEICONbits.UPDN != 0)
	{
		last_pos = real_pos;
		real_pos = real_pos + QEI_MAX - qei_buf + 1;
		qei_buf = 0; 	
		POSCNT = 0;
	}
	else 
	{
		last_pos = real_pos;
		real_pos = real_pos - qei_buf - 1;
		qei_buf = QEI_MAX;
		POSCNT = QEI_MAX;
	}		
}
