// RobotScaraDlg.h : header file
// Written by Phuong H. Hoang and Ha M. Doan, April 2013

#pragma once
#include "afxwin.h"
#include "serialport.h"

#define TIMEOUT_CHECKCOM					5000
#define TIMEOUT_SEND_DATA						1500

#define ON									1
#define OFF									0
#define START_BYTE							0xAA
#define STOP_BYTE							0x55
#define SAVE_FILE_MODE						1		

#define WAIT								1
#define	WRITE_FILE							2
#define	READ_JOINT_1						3
#define READ_JOINT_2						4

#define CHECKCOM							0x01
#define SET_POSITION_POSITIVE				0x02
#define START								0x03
#define STOP								0x04
#define KP_PARAMETER						0x05
#define KD_PARAMETER						0x06
#define STATE_SEND_DATA						0x07
#define KP_POS								0x08
#define KP_V								0x09
#define KI_V								0x0A
#define KP_I								0x0B
#define KI_I								0x0C

#define DATA_OF_JOINT_1_SPEED_POSITIVE		0x80
#define DATA_OF_JOINT_1_SPEED_NEGATIVE		0x81
#define DATA_OF_JOINT_2_SPEED_POSITIVE		0x82
#define DATA_OF_JOINT_2_SPEED_NEGATIVE		0x83
#define PORT_COM_JOINT_1					0x84
#define PORT_COM_JOINT_2					0x85

//#define POSITION_NEGATIVE					0x90
#define POSITION_POSITIVE					0x91
//#define POSITION_NEGATIVE					0x90
#define SPEED_POSITIVE						0x92
//#define POSITION_NEGATIVE					0x90
#define CURRENT_POSITIVE					0x94
#define SEND_DATA_FEEDBACK					0xA0
#define TIMER_EVENT							1

#define SET_TIME							50		//50ms

#define CHECK_COM_JOINT1() if(!m_stPortJoint1.IsOpen()) {MessageBox(_T("Chưa kết nối khớp 1!"), _T("Thông báo"), MB_ICONWARNING); return;}

#define CHECK_COM_JOINT2() if(!m_stPortJoint2.IsOpen()) {MessageBox(_T("Chưa kết nối khớp 2!"), _T("Thông báo"), MB_ICONWARNING); return;}


class CRobotScaraDlg : public CDialog
{
// Construction
public:
	CRobotScaraDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ROBOTSCARA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CStatic m_ctrlStaticText1;
	CStatic m_ctrlStaticText2;
	CStatic m_ctrlStaticText3;
	CStatic m_ctrlStaticText4;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CComboBox m_ctrlComboBaudrate;
	CString m_strEditStatusJoint1;
	CString m_strEditStatusJoint2;
	void ConvertXYToQ(double* pX, double* pY, double* pQ1, double* pQ2);
	void ConvertQtoXY(double* pQ1, double* pQ2, double* pX, double* pY);
	void ConvertQtoE(double* pQ1, double* pQ2, long* pQe1, long* pQe2);
	void ConvertEtoQ(long* pQe1, long* pQe2, double* pQ1, double* pQ2);
	double ConvertDegToRad(double dDeg);
	double ConvertRadToDeg(double dRad);
	double m_dLastX;
	double m_dLastY;
	double m_dRealX;
	double m_dRealY;
	afx_msg void OnBnClickedButtonConnect();
	CSerialPort m_stPortJoint1;
	CSerialPort m_stPortJoint2;
	void SendBuffer(PBYTE pBuffer, BYTE nCommand, BYTE nLengthBuffer);
	void SendBuffer(CSerialPort* pPort, PBYTE pBuffer, BYTE nCommand, BYTE nLengthBuffer);
	afx_msg void OnBnClickedButtonSendData();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	static DWORD CALLBACK ProcessData(LPVOID lpParameter);
	HANDLE m_hThread;
	double m_dLastQ1;
	double m_dLastQ2;
	double m_dRealQ1;
	double m_dRealQ2;
	long m_lLastQe1;
	long m_lLastQe2;
	long m_lRealQe1;
	long m_lRealQe2;
	double m_dLastQ1Deg;
	double m_dLastQ2Deg;
	fstream m_fsFileJoint;
	fstream m_fsFileJoint1;
	fstream m_fsFileJoint2;
	afx_msg void OnBnClickedRadioSaveFile();
	CString m_strPathName;
	CString m_strFileName;
	bool m_bSaveFile;
	BYTE m_nMode;
	double m_dRealSpeedRad1;
	double m_dRealSpeedRad2;
	long m_lRealSpeedQe1;
	SHORT m_sRealCurrent;
	long m_lRealSpeedQe2;
	SHORT m_sRealCurrent1;
	SHORT m_sRealCurrent2;
	void ConvertWeToWq(long* pWe1, long* pWe2, double* pWq1, double* pWq2);
	CEdit m_ctrlEditRealX;
	CEdit m_ctrlEditRealY;
	CEdit m_ctrlEditQ1;
	CEdit m_ctrlEditQ2;
	double m_dRealQ1Deg;
	double m_dRealQ2Deg;
	CStatic m_ctrlBKLogo;
	CSerialPort m_stPort;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double m_dSetTime;
	double m_dDdq1;
	double m_dDdq2;
	virtual BOOL DestroyWindow();
	CString m_strStateSendData;
	float m_fKppJoint1;
	float m_fKpvJoint1;
	float m_fKivJoint1;
	float m_fKpiJoint1;
	float m_fKiiJoint1;
	float m_fKppJoint2;
	float m_fKpvJoint2;
	float m_fKivJoint2;
	float m_fKpiJoint2;
	float m_fKiiJoint2;
};
