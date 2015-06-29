// RobotScaraDlg.cpp : implementation file
// Written by Phuong H. Hoang and Ha M. Doan, April 2013

#include "stdafx.h"
#include "RobotScara.h"
#include "RobotScaraDlg.h"
#include "serialport.h"
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const double dR1 = 0.25;
const double dR2 = 0.15;
const double dX0 = 0.095;
const double dY0 = 0.115;
const long lMaxEncoder1 = 6400;
const long lMaxEncoder2 = 16000;
const double dMaxDeg1 = 200.0;
const float fMaxDeg1 = 200.0;
const double dMaxDeg2 = 250.0;
const float fMaxDeg2 = 250.0;
const double dJoint0Deg = 50.44;

double g_dT11 = 0;
double g_dT12 = 0;
double g_dT21 = 0;
double g_dT22 = 0;
double g_dSaveRealQ1 = 0;
double g_dSaveRealQ2 = 0;
int g_nCountTime = 0;

COMMTIMEOUTS stTimeouts = {MAXDWORD, 0, 0, 0, 0};

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRobotScaraDlg dialog




CRobotScaraDlg::CRobotScaraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRobotScaraDlg::IDD, pParent)
	, m_strEditStatusJoint1(_T(""))
	, m_dLastX(0)
	, m_dLastY(0)
	, m_dRealX(0)
	, m_dRealY(0)
	, m_hThread(INVALID_HANDLE_VALUE)
	, m_dLastQ1(0)
	, m_dLastQ2(0)
	, m_dRealQ1(0)
	, m_dRealQ2(0)
	, m_lLastQe1(0)
	, m_lLastQe2(0)
	, m_lRealQe2(0)
	, m_lRealQe1(0)
	, m_dLastQ1Deg(0)
	, m_dLastQ2Deg(0)
	, m_strPathName(_T(""))
	, m_strFileName(_T(""))
	, m_bSaveFile(false)
	, m_nMode(0)
	, m_dRealSpeedRad1(0)
	, m_dRealSpeedRad2(0)
	, m_lRealSpeedQe1(0)
	, m_lRealSpeedQe2(0)
	, m_dRealQ1Deg(0)
	, m_dRealQ2Deg(0)
	, m_dSetTime(0)
	, m_dDdq1(0)
	, m_dDdq2(0)
	, m_strStateSendData(_T(""))
	, m_fKppJoint1(0)
	, m_fKpvJoint1(0)
	, m_fKivJoint1(0)
	, m_fKpiJoint1(0)
	, m_fKiiJoint1(0)
	, m_fKppJoint2(0)
	, m_fKpvJoint2(0)
	, m_fKivJoint2(0)
	, m_fKpiJoint2(0)
	, m_fKiiJoint2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobotScaraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BMTDH, m_ctrlStaticText1);
	DDX_Control(pDX, IDC_STATIC_VD, m_ctrlStaticText2);
	DDX_Control(pDX, IDC_STATIC_DHBKHN, m_ctrlStaticText3);
	DDX_Control(pDX, IDC_STATIC_HTDKCH, m_ctrlStaticText4);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_ctrlComboBaudrate);
	DDX_Text(pDX, IDC_EDIT_STATUS_JOINT1, m_strEditStatusJoint1);
	DDX_Text(pDX, IDC_EDIT_STATUS_JOINT2, m_strEditStatusJoint2);
	DDX_Text(pDX, IDC_EDIT_LAST_X, m_dLastX);
	DDX_Text(pDX, IDC_EDIT_LAST_Y, m_dLastY);
	DDX_Text(pDX, IDC_EDIT_REAL_X, m_dRealX);
	DDX_Text(pDX, IDC_EDIT_REAL_Y, m_dRealY);
	DDX_Text(pDX, IDC_EDIT_LAST_Q1_DEG, m_dLastQ1Deg);
	DDX_Text(pDX, IDC_EDIT_LAST_Q2_DEG, m_dLastQ2Deg);
	DDV_MinMaxDouble(pDX, m_dLastQ2Deg, -125.0, 125.0);
	DDV_MinMaxDouble(pDX, m_dLastQ1Deg, -100.0, 100.0);
	DDX_Control(pDX, IDC_EDIT_REAL_X, m_ctrlEditRealX);
	DDX_Control(pDX, IDC_EDIT_REAL_Y, m_ctrlEditRealY);
	DDX_Control(pDX, IDC_EDIT_REAL_Q1, m_ctrlEditQ1);
	DDX_Control(pDX, IDC_EDIT_REAL_Q2, m_ctrlEditQ2);
	DDX_Control(pDX, IDC_STATIC_BK_LOGO, m_ctrlBKLogo);
	DDX_Text(pDX, IDC_EDIT_SET_TIME, m_dSetTime);
	DDV_MinMaxDouble(pDX, m_dSetTime, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_STATE_SEND_DATA, m_strStateSendData);
	DDX_Text(pDX, IDC_EDIT_KP_POS_JOINT_1, m_fKppJoint1);
	DDX_Text(pDX, IDC_EDIT_KPV_JOINT_1, m_fKpvJoint1);
	DDX_Text(pDX, IDC_EDIT_KIV_JOINT_1, m_fKivJoint1);
	DDX_Text(pDX, IDC_EDIT_KPI_JOINT_1, m_fKpiJoint1);
	DDX_Text(pDX, IDC_EDIT_KII_JOINT_1, m_fKiiJoint1);
		DDX_Text(pDX, IDC_EDIT_KP_POS_JOINT_2, m_fKppJoint2);
	DDX_Text(pDX, IDC_EDIT_KPV_JOINT_2, m_fKpvJoint2);
	DDX_Text(pDX, IDC_EDIT_KIV_JOINT_2, m_fKivJoint2);
	DDX_Text(pDX, IDC_EDIT_KPI_JOINT_2, m_fKpiJoint2);
	DDX_Text(pDX, IDC_EDIT_KII_JOINT_2, m_fKiiJoint2);
}

BEGIN_MESSAGE_MAP(CRobotScaraDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CRobotScaraDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND_DATA, &CRobotScaraDlg::OnBnClickedButtonSendData)
	ON_BN_CLICKED(IDC_BUTTON_START, &CRobotScaraDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CRobotScaraDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_RADIO_SAVE_FILE, &CRobotScaraDlg::OnBnClickedRadioSaveFile)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRobotScaraDlg message handlers

/*
*/

BOOL CRobotScaraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// TODO: Add extra initialization here
//**********************************Set font*************************************
	int fontHeight = 25;
	CFont *font = new CFont();
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));		//Clear out structure
	lf.lfHeight = fontHeight;				//Request a 20 - pixel-high font
	_tcscpy_s(lf.lfFaceName, _T("Arial"));	//with face name "Arial"
	font->CreateFontIndirect(&lf);			//Create the font
	m_ctrlStaticText1.SetFont(font);
	m_ctrlStaticText2.SetFont(font);
	m_ctrlStaticText3.SetFont(font);
	m_ctrlStaticText4.SetFont(font);
//*******************************************************************************
//**********************************Set BaudRate*********************************
	m_ctrlComboBaudrate.AddString(_T("1200"));
	m_ctrlComboBaudrate.InsertString(1, _T("4800"));
	m_ctrlComboBaudrate.InsertString(2, _T("9600"));
	m_ctrlComboBaudrate.InsertString(3, _T("115200"));
	m_ctrlComboBaudrate.InsertString(4, _T("230400"));
	m_ctrlComboBaudrate.InsertString(5, _T("460800"));
	m_ctrlComboBaudrate.InsertString(6, _T("921600"));
	m_ctrlComboBaudrate.SetCurSel(3);
//*******************************************************************************
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRobotScaraDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRobotScaraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRobotScaraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CRobotScaraDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		if ((pWnd == GetDlgItem(IDC_STATIC_DHBKHN)) || (pWnd == GetDlgItem(IDC_STATIC_VD)) || (pWnd == GetDlgItem(IDC_STATIC_BMTDH)))
		{
			pDC->SetTextColor(RGB(255, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
		if (pWnd == GetDlgItem(IDC_STATIC_HTDKCH))
		{
			pDC->SetTextColor(RGB(0, 0, 255));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CRobotScaraDlg::ConvertXYToQ(double* pX, double* pY, double* pQ1, double* pQ2)
{
	double dRealX = *pX - dX0;
	double dRealY = *pY - dY0;
	double dA = 2 * dRealX * dR1;
	double dB = 2 * dRealY * dR2;
	double dD = dRealX * dRealX + dRealY * dRealY + dR1 * dR1 - dR2 * dR2;
	double dS1 = (dD * dB - dA * sqrt(fabs(dA * dA + dB * dB - dD * dD))) / (dA * dA + dB * dB);
	double dC1 = (dD * dA + dB * sqrt(fabs(dA * dA + dB * dB - dD * dD))) / (dA * dA + dB * dB);
	*pQ1 = atan2(dS1, dC1);
	double dS12 = (dRealY - dR1 * dS1) / dR2;
	double dC12 = (dRealX - dR1 * dC1) / dR2;
	*pQ2 = atan2(dS12, dC12) - *pQ1; 
}

void CRobotScaraDlg::ConvertQtoXY(double* pQ1, double* pQ2, double* pX, double* pY)
{
	*pX = dX0 + dR1 * cos(*pQ1) + dR2 * cos(*pQ1 + *pQ2);
	*pY = dY0 + dR1 * sin(*pQ1) + dR2 * sin(*pQ1 + *pQ2);
}

void CRobotScaraDlg::ConvertQtoE(double* pQ1, double* pQ2, long* pQe1, long* pQe2)
{
	long lQe01 = (long)((dMaxDeg1 / 2 - dJoint0Deg) * (double)lMaxEncoder1 / dMaxDeg1);
	long lQe02 = (long)((dMaxDeg2 / 2 - dJoint0Deg) * (double)lMaxEncoder2 / dMaxDeg2);
	*pQe1 = (long)(((double)lMaxEncoder1  * *pQ1 * 360.0) / (2 * M_PI * dMaxDeg1)) + lQe01;
	*pQe2 = (long)(((double)lMaxEncoder2  * *pQ2 * 360.0) / (2 * M_PI * dMaxDeg2)) + lQe02;
}

void CRobotScaraDlg::ConvertEtoQ(long* pQe1, long* pQe2, double* pQ1, double* pQ2)
{
	long lQe01 = (long)((dMaxDeg1 / 2 - dJoint0Deg) * (double)lMaxEncoder1 / dMaxDeg1);
	long lQe02 = (long)((dMaxDeg2 / 2 - dJoint0Deg) * (double)lMaxEncoder2 / dMaxDeg2);
	*pQ1 = (double)(*pQe1 - lQe01) * (2 * M_PI * dMaxDeg1) / ((double)lMaxEncoder1 * 360.0);
	*pQ2 = (double)(*pQe2 - lQe02) * (2 * M_PI * dMaxDeg2) / ((double)lMaxEncoder2 * 360.0);
}

void CRobotScaraDlg::ConvertWeToWq(long* pWe1, long* pWe2, double* pWq1, double* pWq2)
{
	*pWq1 = (double)(*pWe1) * (2 * M_PI * dMaxDeg1) / ((double)lMaxEncoder1 * 360.0);
	*pWq2 = (double)(*pWe2) * (2 * M_PI * dMaxDeg2) / ((double)lMaxEncoder2 * 360.0);	
}

double CRobotScaraDlg::ConvertDegToRad(double dDeg)
{
	return (dDeg * 2 * M_PI / 360.0) ;
}

double CRobotScaraDlg::ConvertRadToDeg(double dRad)
{
	return (dRad * 360.0 / (2 * M_PI));
}

void CRobotScaraDlg::OnBnClickedButtonConnect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//**********************************Open COM PORT*****************************************************
	BYTE nIndex;
	BYTE nIndexBuffer = 0;
	BYTE nIndexJoint1 = 0;
	BYTE nIndexJoint2 = 0;
	BYTE pTempBuffer[255];
	CString strBaudrate;
	int nBaudrate;
	char strError;
	char* pError = &strError;
	BYTE byTempByte;

	if (m_stPortJoint1.IsOpen() == TRUE)
	{
		m_stPortJoint1.Close();
	}
	if (m_stPortJoint2.IsOpen() == TRUE)
	{
		m_stPortJoint2.Close();
	}
	nBaudrate = m_ctrlComboBaudrate.GetCurSel();
	m_ctrlComboBaudrate.GetLBText(nBaudrate, strBaudrate);
	nBaudrate = _tstoi(strBaudrate);
	for (nIndex = 1; nIndex <= 100; nIndex++)
	{
		try
		{
			m_stPort.Open(nIndex, nBaudrate, CSerialPort :: NoParity, 8, CSerialPort :: OneStopBit, CSerialPort :: NoFlowControl, FALSE);
			if (m_stPort.IsOpen() == TRUE)	//Search com port
			{
				//m_stPort.Close();
				//m_stPort.Open(nIndex, nBaudrate, CSerialPort :: NoParity, 8, CSerialPort :: OneStopBit, CSerialPort :: NoFlowControl, FALSE);
				BOOL bCheckFlag = OFF;
				BOOL bStatus = OFF;
				m_stPort.SetTimeouts(stTimeouts);
				//m_stPort.Set0ReadTimeout();
				m_stPort.ClearWriteBuffer();
				m_stPort.ClearReadBuffer();
				m_stPort.Flush();
				SendBuffer(&m_stPort, NULL, CHECKCOM, 0);
				//******************************************Set Timeout************************************

				DWORD nBreakTime = GetTickCount() + TIMEOUT_CHECKCOM;	// Time = TIMEOUT_CHECKCOM (ms)
			
				//*****************************************************************************************
				while (bStatus == OFF)
				{
					// Read file from KIT continuously
					//SendBuffer(&m_stPort, NULL, CHECKCOM, 0);
					if (m_stPort.Read((PBYTE)(&byTempByte), 1) == 1)
					{
						if (byTempByte == START_BYTE)
							bCheckFlag = ON;
						if (byTempByte == STOP_BYTE)
						{
							bCheckFlag = OFF;
							//bStatus = ON;
							nIndexBuffer = 0;
							if (pTempBuffer[2] == PORT_COM_JOINT_1)
							{
								nIndexJoint1 = nIndex;	
								break;
							}
							if (pTempBuffer[2] == PORT_COM_JOINT_2)
							{
								nIndexJoint2 = nIndex;
								break;
							}
							//if (pTempBuffer[2] == 0xFF)
							//{
								//long nTest = *((long*)(pTempBuffer + 3));
								//long nTest1 = 1;
							//}
						}
						if (bCheckFlag == ON)
						{
							pTempBuffer[nIndexBuffer] = byTempByte;
							nIndexBuffer++;
						}
					}
					if (nBreakTime < GetTickCount())
					{
						//MessageBox(_T("Search file error!"));
						break;
					}
				}	
				//*****************************************************************************************
				m_stPort.Close();
			}
		}
		catch(...)
		{
		}
	}
	if (nIndexJoint1 != 0)
	{
		m_stPortJoint1.Open(nIndexJoint1, nBaudrate, CSerialPort :: NoParity, 8, CSerialPort :: OneStopBit, CSerialPort :: NoFlowControl, FALSE);
		if(m_stPortJoint1.IsOpen() == TRUE)
		{
			m_strEditStatusJoint1 = _T("Kết nối khớp 1 thành công!");
			m_stPortJoint1.SetTimeouts(stTimeouts);
			m_stPortJoint1.ClearWriteBuffer();
			m_stPortJoint1.ClearReadBuffer();
			m_stPortJoint1.Flush();
		}
		else
			m_strEditStatusJoint1 = _T("Kết nối khớp 1 không thành công!");
	}
	else
	{
		m_strEditStatusJoint1 = _T("Kết nối khớp 1 không thành công!");
	}
	if (nIndexJoint2 != 0)
	{
		m_stPortJoint2.Open(nIndexJoint2, nBaudrate, CSerialPort :: NoParity, 8, CSerialPort :: OneStopBit, CSerialPort :: NoFlowControl, FALSE);
		if(m_stPortJoint2.IsOpen() == TRUE)
		{
			m_strEditStatusJoint2 = _T("Kết nối khớp 2 thành công!");
			m_stPortJoint2.SetTimeouts(stTimeouts);
			m_stPortJoint2.ClearWriteBuffer();
			m_stPortJoint2.ClearReadBuffer();
			m_stPortJoint2.Flush();
		}
		else
			m_strEditStatusJoint2 = _T("Kết nối khớp 2 không thành công!");
	}
	else
	{
		m_strEditStatusJoint2 = _T("Kết nối khớp 2 không thành công!");
	}	
//*******************************************************************************
	UpdateData(FALSE);
}

void CRobotScaraDlg::SendBuffer(PBYTE pBuffer, BYTE nCommand, BYTE nLengthBuffer)
{

}

void CRobotScaraDlg::SendBuffer(CSerialPort* pPort, PBYTE pBuffer, BYTE nCommand, BYTE nLengthBuffer)
{
	BYTE byStart = START_BYTE;
	BYTE nLength = nLengthBuffer + 5;
	BYTE byStop = STOP_BYTE;
	BYTE byCheckSum = 0;
	BYTE nIndex;
	byCheckSum += nLength + nCommand;
	for (nIndex = 0; nIndex < nLengthBuffer; nIndex++)
		byCheckSum = byCheckSum + pBuffer[nIndex];
	pPort->ClearWriteBuffer();
	pPort->Flush();
	pPort->Write(&byStart, 1);
	pPort->Write(&nLength, 1);
	pPort->Write(&nCommand, 1);
	pPort->Write(pBuffer, nLengthBuffer);
	pPort->Write(&byCheckSum, 1);
	pPort->Write(&byStop, 1);
	//pPort->ClearWriteBuffer();
}



void CRobotScaraDlg::OnBnClickedButtonSendData()
{
	//*********************************Check COM********************************************************
	CHECK_COM_JOINT1();
	CHECK_COM_JOINT2();
	//**************************************************************************************************
	UpdateData(TRUE);
	m_stPortJoint1.ClearReadBuffer();
	m_stPortJoint1.Flush();
	m_stPortJoint2.ClearReadBuffer();
	m_stPortJoint2.Flush();
	if ((m_fKppJoint1 != 0) || (m_fKpvJoint1 != 0) || (m_fKivJoint1 != 0) || (m_fKpiJoint1 != 0) || (m_fKiiJoint1 != 0) || (m_fKppJoint2 != 0) || (m_fKpvJoint2 != 0) || (m_fKivJoint2 != 0) || (m_fKpiJoint2 != 0) || (m_fKiiJoint2 != 0))
	{
		//float fKp1 = m_dKp1 * 5 * fMaxDeg1 * 2 * (float)M_PI * 10000 / (lMaxEncoder1 * 360) ;
		//float fKd1 = m_dKd1 * 5 * fMaxDeg1 * 2 * (float)M_PI * 10000 / (lMaxEncoder1 * 360) ;
		//float fKp2 = m_dKp2 * 5 * fMaxDeg2 * 2 * (float)M_PI * 10000 / (lMaxEncoder2 * 360) ;
		//float fKd2 = m_dKd2 * 5 * fMaxDeg2 * 2 * (float)M_PI * 10000 / (lMaxEncoder2 * 360) ;
		SendBuffer(&m_stPortJoint1, (PBYTE)(&m_fKppJoint1), KP_POS, 4);
		SendBuffer(&m_stPortJoint2, (PBYTE)(&m_fKppJoint2), KP_POS, 4);
		SendBuffer(&m_stPortJoint1, (PBYTE)(&m_fKpvJoint1), KP_V, 4);
		SendBuffer(&m_stPortJoint2, (PBYTE)(&m_fKpvJoint2), KP_V, 4);
		SendBuffer(&m_stPortJoint1, (PBYTE)(&m_fKivJoint1), KI_V, 4);
		SendBuffer(&m_stPortJoint2, (PBYTE)(&m_fKivJoint2), KI_V, 4);
		SendBuffer(&m_stPortJoint1, (PBYTE)(&m_fKpiJoint1), KP_I, 4);
		SendBuffer(&m_stPortJoint2, (PBYTE)(&m_fKpiJoint2), KP_I, 4);
		SendBuffer(&m_stPortJoint1, (PBYTE)(&m_fKiiJoint1), KI_I, 4);
		SendBuffer(&m_stPortJoint2, (PBYTE)(&m_fKiiJoint2), KI_I, 4);

	}
	SendBuffer(&m_stPortJoint1, NULL, STATE_SEND_DATA, 0);
	SendBuffer(&m_stPortJoint2, NULL, STATE_SEND_DATA, 0);
//******************************************Set Timeout************************************

	DWORD nBreakTime = GetTickCount() + TIMEOUT_SEND_DATA;	// Time = TIMEOUT_CHECKCOM (ms)

//*****************************************************************************************
	BOOL bSendDataFlagJoin1 = UNSUCCESS;
	BOOL bSendDataFlagJoin2 = UNSUCCESS;
	BYTE pTempBuffer[255];
	BYTE byTempByte;
	int nIndexBuffer = 0;
	BOOL bCheckFlag = OFF;
	BOOL bStatus = OFF;
	while (bStatus == OFF)
	{
		if (m_stPortJoint1.Read((PBYTE)(&byTempByte), 1) == 1)
		{
			if (byTempByte == START_BYTE)
				bCheckFlag = ON;
			if (byTempByte == STOP_BYTE)
			{
				bCheckFlag = OFF;
				//bStatus = ON;
				nIndexBuffer = 0;
				if (pTempBuffer[2] == SEND_DATA_FEEDBACK)
				{
					bSendDataFlagJoin1 = SUCCESS;	
					break;
				}
			}
			if (bCheckFlag == ON)
			{
				pTempBuffer[nIndexBuffer] = byTempByte;
				nIndexBuffer++;
			}
		}
		if (nBreakTime < GetTickCount())
		{
			break;
		}
	}
	nBreakTime = GetTickCount() + TIMEOUT_CHECKCOM;	// Time = TIMEOUT_CHECKCOM (ms)
	nIndexBuffer = 0;
	bCheckFlag = OFF;
	bStatus = OFF;
//*****************************************************************************************
	while (bStatus == OFF)
	{
		if (m_stPortJoint2.Read((PBYTE)(&byTempByte), 1) == 1)
		{
			if (byTempByte == START_BYTE)
				bCheckFlag = ON;
			if (byTempByte == STOP_BYTE)
			{
				bCheckFlag = OFF;
				//bStatus = ON;
				nIndexBuffer = 0;
				if (pTempBuffer[2] == SEND_DATA_FEEDBACK)
				{
					bSendDataFlagJoin2 = SUCCESS;	
					break;
				}
			}
			if (bCheckFlag == ON)
			{
				pTempBuffer[nIndexBuffer] = byTempByte;
				nIndexBuffer++;
			}
		}
		if (nBreakTime < GetTickCount())
		{
			break;
		}
	}	
	if ((bSendDataFlagJoin1 == SUCCESS) && (bSendDataFlagJoin2 == SUCCESS))
	{
		m_strStateSendData = _T("Gửi dữ liệu thành công");
	}
	else 
		m_strStateSendData = _T("Gửi dữ liệu không thành công");
	UpdateData(FALSE);
}

void CRobotScaraDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	//*********************************Check COM********************************************************
	CHECK_COM_JOINT1();
	CHECK_COM_JOINT2();
	//**************************************************************************************************
	UpdateData(TRUE);
	m_stPortJoint1.ClearReadBuffer();
	m_stPortJoint2.ClearReadBuffer();
	m_stPortJoint1.Flush();
	m_stPortJoint2.Flush();
	if ((m_dLastX != 0) || (m_dLastY != 0))
	{
		ConvertXYToQ(&m_dLastX, &m_dLastY, &m_dLastQ1, &m_dLastQ2);
		//ConvertQtoE(&m_dLastQ1, &m_dLastQ2, &m_lLastQe1, &m_lLastQe2);	// Truyen lan luot khong can chuyen doi vi tri cuoi
	}
	else
	{
		if ((m_dLastQ1Deg != 0) || (m_dLastQ2Deg != 0))
		{
			m_dLastQ1 = ConvertDegToRad(m_dLastQ1Deg + dJoint0Deg);
			m_dLastQ2 = ConvertDegToRad(m_dLastQ2Deg + dJoint0Deg);
			//ConvertQtoE(&m_dLastQ1, &m_dLastQ2, &m_lLastQe1, &m_lLastQe2);
		}
	}	
	SendBuffer(&m_stPortJoint1, NULL, START, 0);
	SendBuffer(&m_stPortJoint2, NULL, START, 0);
	if (m_nMode == SAVE_FILE_MODE)
	{
		m_fsFileJoint.open(m_strPathName, ios::out);
		m_fsFileJoint << fixed;
		m_fsFileJoint << setprecision(4);
		if (m_hThread != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hThread);
		}
		m_hThread = CreateThread(NULL, 0, &CRobotScaraDlg::ProcessData, this, 0, NULL);
	}
	UINT_PTR nTimer = SetTimer(TIMER_EVENT, SET_TIME, NULL);
	g_nCountTime = 0;
	m_dDdq1 = (4 * fabs(m_dLastQ1 - m_dRealQ1) / (m_dSetTime * m_dSetTime)) * 1.6;
	g_dT11 = m_dSetTime / 2 - (sqrt(fabs((m_dSetTime * m_dSetTime * m_dDdq1 - 4 * m_dLastQ1 + 4 * m_dRealQ1) / m_dDdq1))) / 2;
	g_dT12 = m_dSetTime - g_dT11;
	m_dDdq2 = (4 * fabs(m_dLastQ2 - m_dRealQ2) / (m_dSetTime * m_dSetTime)) * 1.6;
	g_dT21 = m_dSetTime / 2 - (sqrt(fabs((m_dSetTime * m_dSetTime * m_dDdq2 - 4 * m_dLastQ2 + 4 * m_dRealQ2) / m_dDdq2))) / 2;
	g_dT22 = m_dSetTime - g_dT21;
	g_dSaveRealQ1 = m_dRealQ1;
	g_dSaveRealQ2 = m_dRealQ2;

	UpdateData(FALSE);
}

void CRobotScaraDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	//*********************************Check COM********************************************************
	CHECK_COM_JOINT1();
	CHECK_COM_JOINT2();
	//**************************************************************************************************
	KillTimer(TIMER_EVENT);
	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		TerminateThread(m_hThread, 0);
		CloseHandle(m_hThread);
	}
	if (!m_fsFileJoint.fail())
	{
		m_fsFileJoint.close();
	}
	SendBuffer(&m_stPortJoint1, NULL, STOP, 0);
	SendBuffer(&m_stPortJoint2, NULL, STOP, 0);
}

DWORD CALLBACK CRobotScaraDlg::ProcessData(LPVOID lpParameter)
{
	CRobotScaraDlg* pDialog = (CRobotScaraDlg*)lpParameter;
	DWORD nTimeRunMiliSec = 0;
	BYTE nStateWriteFile = WAIT;
	BYTE byDataJoint1;
	BYTE byDataJoint2;
	BOOL bFinishFlag1 = FALSE;
	BOOL bFinishFlag2 = FALSE;
	BOOL bCheckFlag = OFF;
	BOOL bReadDataFlag1 = OFF;
	BOOL bReadDataFlag2 = OFF;
	BYTE nIndex1 = 0;
	BYTE nIndex2 = 0;
	BYTE pBuffer1[255];
	BYTE pBuffer2[255];
	BYTE byCommand1;
	BYTE byCommand2;
	BYTE nLength1;
	BYTE nLength2;

	long nRealQe1;
	long nRealQe2;
	
	long nSetQe1;
	long nSetQe2;

	double dFeedbackSetQ1;
	double dFeedbackSetQ2;

	long nRealSpeedQe1;
	long nRealSpeedQe2;

	WORD nRealCurrent1;
	WORD nRealCurrent2;
	while(1)
	{
		switch(nStateWriteFile)
		{
		case WAIT:
			bCheckFlag = OFF;
			if (bFinishFlag1 == FALSE)
			{
				if (pDialog->m_stPortJoint1.Read((PBYTE)&byDataJoint1, 1) == 1)
				{
					nStateWriteFile = READ_JOINT_1;
					bCheckFlag = ON;
				}
			}
			if ((bFinishFlag2 == FALSE) && (bCheckFlag == OFF))
			{
				if (pDialog->m_stPortJoint2.Read((PBYTE)&byDataJoint2, 1) == 1)
				{
					nStateWriteFile = READ_JOINT_2;
				}
			}
			if ((bFinishFlag1 == ON) && (bFinishFlag2 == ON))
			{
				nStateWriteFile = WRITE_FILE;
			}
			break;
		case READ_JOINT_1:
			if ((byDataJoint1 == START_BYTE) && (bReadDataFlag1 == OFF))
			{
				bReadDataFlag1 = ON;
			}
			if (bReadDataFlag1 == ON)
			{
				pBuffer1[nIndex1++] = byDataJoint1;
				if (byDataJoint1 == STOP_BYTE)
				{
					bFinishFlag1 = TRUE;
					bReadDataFlag1 = OFF;
				}
			}
			nStateWriteFile = WAIT;
			break;
		case READ_JOINT_2:
			if ((byDataJoint2 == START_BYTE) && (bReadDataFlag2 == OFF))
			{
				bReadDataFlag2 = ON;
			}
			if (bReadDataFlag2 == ON)
			{
				pBuffer2[nIndex2++] = byDataJoint2;
				if (byDataJoint2 == STOP_BYTE)
				{
					bFinishFlag2 = TRUE;
					bReadDataFlag2 = OFF;
				}
			}
			nStateWriteFile = WAIT;
			break;
		case WRITE_FILE:
			nLength1 = *(pBuffer1 + 1);
			nLength2 = *(pBuffer2 + 1);
			if ((nLength1 == nIndex1) && (nLength2 == nIndex2))
			{
				byCommand1 = pBuffer1[2];
				byCommand2 = pBuffer2[2];
				nRealQe1 = *((long*)(pBuffer1 + 3));
				nRealQe2 = *((long*)(pBuffer2 + 3));
				nRealSpeedQe1 = *((long*)(pBuffer1 + 7));
				nRealSpeedQe2 = *((long*)(pBuffer2 + 7));
				nRealCurrent1 = *((PWORD)(pBuffer1 + 11));
				nRealCurrent2 = *((PWORD)(pBuffer2 + 11));
				nSetQe1 = *((long*)(pBuffer1 + 13));
				nSetQe2 = *((long*)(pBuffer2 + 13));

				//Tranfer Position
				//if ((byCommand1 & POSITION_POSITIVE) == POSITION_POSITIVE)
				pDialog->m_lRealQe1 = nRealQe1;
				//else
				//	pDialog->m_lRealQe1 = -nRealQe1;
				//if ((byCommand2 & POSITION_POSITIVE) == POSITION_POSITIVE)
				pDialog->m_lRealQe2 = nRealQe2;
				//else
				//	pDialog->m_lRealQe2 = -nRealQe2;
				////Tranfer Speed
				//if ((byCommand1 & SPEED_POSITIVE) == SPEED_POSITIVE)
				pDialog->m_lRealSpeedQe1 = nRealSpeedQe1;
				//else
				//	pDialog->m_lRealSpeedQe1 = -(long)nRealSpeedQe1;
				//if ((byCommand2 & SPEED_POSITIVE) == SPEED_POSITIVE)
				pDialog->m_lRealSpeedQe2 = nRealSpeedQe2;
				//else
				//	pDialog->m_lRealSpeedQe2 = -(long)nRealSpeedQe2;
				//Tranfer Current
				if ((byCommand1 & CURRENT_POSITIVE) == CURRENT_POSITIVE)
					pDialog->m_sRealCurrent1 = nRealCurrent1;
				else
					pDialog->m_sRealCurrent1 = -(short)nRealCurrent1;

				if ((byCommand2 & CURRENT_POSITIVE) == CURRENT_POSITIVE)
					pDialog->m_sRealCurrent2 = nRealCurrent2;
				else
					pDialog->m_sRealCurrent2 = -(short)nRealCurrent2;
				//
				pDialog->ConvertEtoQ(&(pDialog->m_lRealQe1), &(pDialog->m_lRealQe2), &(pDialog->m_dRealQ1), &(pDialog->m_dRealQ2));
				pDialog->ConvertWeToWq(&(pDialog->m_lRealSpeedQe1), &(pDialog->m_lRealSpeedQe2), &(pDialog->m_dRealSpeedRad1), &(pDialog->m_dRealSpeedRad2));
				//pDialog->m_dRealQ1
				
				pDialog->ConvertEtoQ(&nSetQe1, &nSetQe2, &dFeedbackSetQ1, &dFeedbackSetQ2);

				pDialog->m_fsFileJoint << nTimeRunMiliSec << " ";

				pDialog->m_fsFileJoint << pDialog->m_dRealQ1 << " ";
				pDialog->m_fsFileJoint << pDialog->m_dRealSpeedRad1 << " ";
				pDialog->m_fsFileJoint << pDialog->m_sRealCurrent1 << " ";
				pDialog->m_fsFileJoint << dFeedbackSetQ1 << " ";

				pDialog->m_fsFileJoint << pDialog->m_dRealQ2 << " ";
				pDialog->m_fsFileJoint << pDialog->m_dRealSpeedRad2 << " "; 
				pDialog->m_fsFileJoint << pDialog->m_sRealCurrent2 << " ";
				pDialog->m_fsFileJoint << dFeedbackSetQ2 << endl;
				
				//pDialog->UpdateData(true);
				pDialog->ConvertQtoXY(&(pDialog->m_dRealQ1), &(pDialog->m_dRealQ2), &(pDialog->m_dRealX), &(pDialog->m_dRealY));
				//pDialog->UpdateData(false);
				CString strTemp;
				strTemp.Format(_T("%2.4f"), pDialog->m_dRealX); 
				pDialog->m_ctrlEditRealX.SetSel(0, 7);
				pDialog->m_ctrlEditRealX.ReplaceSel((LPCTSTR)(strTemp), FALSE);
				strTemp.Format(_T("%2.4f"), pDialog->m_dRealY); 				
				pDialog->m_ctrlEditRealY.SetSel(0, 7);
				pDialog->m_ctrlEditRealY.ReplaceSel((LPCTSTR)(strTemp), FALSE);	

				pDialog->m_dRealQ1Deg = pDialog->ConvertRadToDeg(pDialog->m_dRealQ1);
				pDialog->m_dRealQ2Deg = pDialog->ConvertRadToDeg(pDialog->m_dRealQ2);
				strTemp.Format(_T("%4.2f"), pDialog->m_dRealQ1Deg); 
				pDialog->m_ctrlEditQ1.SetSel(0, 7);
				pDialog->m_ctrlEditQ1.ReplaceSel((LPCTSTR)(strTemp), FALSE);
				strTemp.Format(_T("%4.2f"), pDialog->m_dRealQ2Deg); 				
				pDialog->m_ctrlEditQ2.SetSel(0, 7);
				pDialog->m_ctrlEditQ2.ReplaceSel((LPCTSTR)(strTemp), FALSE);
				nTimeRunMiliSec++;
			}
			bFinishFlag1 = FALSE;
			bFinishFlag2 = FALSE;
			nIndex1 = 0;
			nIndex2 = 0;
			nStateWriteFile = WAIT;
			break;
		}
	}
	return 0;
}

void CRobotScaraDlg::OnBnClickedRadioSaveFile()
{
	// TODO: Add your control notification handler code here
	CFileDialog ctrlDlg(FALSE, _T("txt"));
	ctrlDlg.m_ofn.lpstrTitle = _T("Save file");
	ctrlDlg.m_ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0 All Files (*.*)\0*.*\0\0");	//Filt file txt
	if(ctrlDlg.DoModal() == IDOK)
	{
		m_strPathName = ctrlDlg.GetPathName(); // return full path and filename
		m_strFileName = ctrlDlg.GetFileName();
		//m_bFileFlag = ON;
	}
	m_nMode = SAVE_FILE_MODE;
}



void CRobotScaraDlg::OnTimer(UINT_PTR nIDEvent)
{
	double dTime;
	double dSetQ1;
	double dSetQ2;
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	if (nIDEvent == TIMER_EVENT)
	{
		//Calculate set point
		g_nCountTime++;
		dTime = (double)g_nCountTime * (double)SET_TIME * 0.001;	
		if (dTime <= g_dT11)
			dSetQ1 = g_dSaveRealQ1 + m_dDdq1 * dTime * dTime / 2;
		else
			if ((g_dT11 < dTime) && (dTime < g_dT12))
				dSetQ1 = g_dSaveRealQ1 + m_dDdq1 * g_dT11 * (dTime - g_dT11 / 2);
			else
				if ((g_dT12 < dTime) && (dTime < m_dSetTime))
					dSetQ1 = m_dLastQ1 - m_dDdq1 * (m_dSetTime - dTime) * (m_dSetTime - dTime) / 2;
				else
					dSetQ1 = m_dLastQ1;
		if (dTime <= g_dT21)
			dSetQ2 = g_dSaveRealQ2 + m_dDdq2 * dTime * dTime / 2;
		else
			if ((g_dT21 < dTime) && (dTime < g_dT22))
				dSetQ2 = g_dSaveRealQ2 + m_dDdq2 * g_dT21 * (dTime - g_dT21 / 2);
			else
				if ((g_dT22 < dTime) && (dTime < m_dSetTime))
					dSetQ2 = m_dLastQ2 - m_dDdq2 * (m_dSetTime - dTime) * (m_dSetTime - dTime) / 2;
				else
					dSetQ2 = m_dLastQ2;
		ConvertQtoE(&dSetQ1, &dSetQ2, &m_lLastQe1, &m_lLastQe2);
		WORD wTemp;
		wTemp = (WORD)m_lLastQe1;
		SendBuffer(&m_stPortJoint1, (PBYTE)(&wTemp), SET_POSITION_POSITIVE, 2);
		wTemp = (WORD)m_lLastQe2;
		SendBuffer(&m_stPortJoint2, (PBYTE)(&wTemp), SET_POSITION_POSITIVE, 2);
	}
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}


BOOL CRobotScaraDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_fsFileJoint.fail())
	{
		m_fsFileJoint.close();
	}
	return CDialog::DestroyWindow();
}
