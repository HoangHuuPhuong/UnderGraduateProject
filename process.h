#ifndef __PROCESS_H__
#define __PROCESS_H__

//***********************************define byte command************************
//#define SET_POSITION_NEGATIVE   1
#define CHECKCOM                                0x01
#define SET_POSITION_POSITIVE                   0x02
#define START					0x03
#define STOP					0x04
#define KP_PARAMETER                            0x05
#define KD_PARAMETER				0x06
#define STATE_SEND_DATA				0x07
#define KP_POS                                  0x08
#define KP_V					0x09
#define KI_V					0x0A
#define KP_I					0x0B
#define KI_I					0x0C

#define DATA_OF_JOINT_1_SPEED_POSITIVE		0x80
#define DATA_OF_JOINT_1_SPEED_NEGATIVE		0x81
#define DATA_OF_JOINT_2_SPEED_POSITIVE		0x82
#define DATA_OF_JOINT_2_SPEED_NEGATIVE		0x83
#define PORT_COM_JOINT_1			0x84
#define PORT_COM_JOINT_2			0x85
#define POSITION_POSITIVE			0x91
#define SPEED_POSITIVE				0x92
#define CURRENT_POSITIVE			0x94
#define SEND_DATA_FEEDBACK                      0xA0

//******************************************************************************
//#define POSITIVE    1
//#define NEGATIVE    -1

extern RESULT ProcessDataPackage(PVOID pPackage);

#endif
