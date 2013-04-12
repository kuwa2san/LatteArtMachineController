////////////////////////////////////////////////////////////////////////////////////////////////////
// LatteArtMachineDlg.cpp : �����t�@�C��
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LatteArtMachine.h"
#include "LatteArtMachineDlg.h"
#include "ComThread.h"
#include "ServoInitial.h"
#include "locale.h"
#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>

#define _USE_MATH_DEFINES
#include "math.h"
#define		DEG2RAD(deg)		( ( ( deg ) / 360 ) * 2 * M_PI )

#define		DRAW_OFFSET_X		537		//�`��G���A�̃E�B���h�E�I�t�Z�b�g
#define		DRAW_OFFSET_Y		397		//�`��G���A�̃E�B���h�E�I�t�Z�b�g
#define		DRAW_AREA_WIDTH		360		//�v���r���[�`��G���A��
#define		DRAW_AREA_HEIGHT	360		//�v���r���[�`��G���A����
#define		DIV_ACCURACY		0.1		//���`��Ԃ̕������x

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O
////////////////////////////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////
// CLatteArtMachineDlg �_�C�A���O
////////////////////////////////////////////////////////////////////////////////////////////////////
CLatteArtMachineDlg::CLatteArtMachineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLatteArtMachineDlg::IDD, pParent)
	, bConnect(FALSE)
	, bServo1(FALSE)
	, bServo2(FALSE)
	, mDlg_Connect(NULL)
	, iScriptLineCount(0)
	, mDlg_Servo(NULL)
	, bServo3(FALSE)
	, bCreateSettingDlg(FALSE)
	, iCount(0)
	, dX_old(0)
	, dY_old(0)
	, bWrite(FALSE)
	, iDrawCount(0)
	, bReadCsv(FALSE)
	, m_pDC_PreViewArea(NULL)
	, m_pDrawThread(NULL)
	, m_hParentWnd(NULL)
	, m_DrawMessage(0)
	, mDlg_Calibrate(NULL)
	, m_pCommandThread(NULL)
	, m_CommandMessage(0)
	, bCommandThread(FALSE)
	, bEscape(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for( int i = 0 ; i < SCRIPT_COLUMN2 ; i++ )
	{
		pntDraw[i].x = 0;
		pntDraw[i].y = 0;
	}
	for( int i = 0 ; i < DRAW_EVENT_MAX ; i++ )
	{
		m_hDrawEvent[ i ] = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	}
	for( int i = 0 ; i < COMMAND_EVENT_MAX ; i++ )
	{
		m_hCommandEvent[ i ] = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_COM, m_bSetComPort);
	//DDX_Control(pDX, IDC_COMBO_BAUD, m_bSpeed);
	//DDX_Control(pDX, IDC_COMBO_DATABIT, m_bDataBit);
	//DDX_Control(pDX, IDC_COMBO_STOPBIT, m_bStopBit);
	DDX_Control(pDX, IDC_CHECK_SERVO1, m_chkServo1);
	DDX_Control(pDX, IDC_CHECK_SERVO2, m_chkServo2);
	DDX_Control(pDX, IDC_EDIT_SERVO1, m_EditServo1);
	DDX_Control(pDX, IDC_EDIT_SERVO2, m_EditServo2);
	DDX_Control(pDX, IDC_STATIC_CSVPATH, m_bCsvPath);
	//DDX_Control(pDX, IDC_CHECK_SERVO3, m_chkServo3);
	DDX_Control(pDX, IDC_EDIT_VIEWSTATE, m_Edit_ScriptLog);
	DDX_Control(pDX, IDC_PICTURE_LATTEARTMACHINE, mxc_PreViewArea);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLatteArtMachineDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CLatteArtMachineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLatteArtMachineDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CONNECT, &CLatteArtMachineDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, &CLatteArtMachineDlg::OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_TRQUE_ON, &CLatteArtMachineDlg::OnBnClickedTrqueOn)
	ON_BN_CLICKED(IDC_TURN_90DEG, &CLatteArtMachineDlg::OnBnClickedTurn90deg)
	ON_BN_CLICKED(IDC_RETURN_90DEG, &CLatteArtMachineDlg::OnBnClickedReturn90deg)
//	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_SERVO1, &CLatteArtMachineDlg::OnBnClickedCheckServo1)
	ON_BN_CLICKED(IDC_CHECK_SERVO2, &CLatteArtMachineDlg::OnBnClickedCheckServo2)
	ON_BN_CLICKED(IDC_TRQUE_OFF, &CLatteArtMachineDlg::OnBnClickedTrqueOff)
	ON_BN_CLICKED(IDC_DIRECTION_DEG, &CLatteArtMachineDlg::OnBnClickedDirectionDeg)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EXIT, &CLatteArtMachineDlg::OnExit)
	ON_COMMAND(ID_SETTING_CONNECT, &CLatteArtMachineDlg::OnSettingConnect)
	ON_BN_CLICKED(IDC_LOADCSV, &CLatteArtMachineDlg::OnBnClickedLoadcsv)
	ON_BN_CLICKED(IDC_START_LATTEART, &CLatteArtMachineDlg::OnBnClickedStartLatteart)
	ON_COMMAND(ID_SETTING_SERVO, &CLatteArtMachineDlg::OnSettingServo)
	ON_BN_CLICKED(IDC_CHECK_SERVO3, &CLatteArtMachineDlg::OnBnClickedCheckServo3)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_ON, &CLatteArtMachineDlg::OnBnClickedButtonWriteOn)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_OFF, &CLatteArtMachineDlg::OnBnClickedButtonWriteOff)
	ON_COMMAND(ID_SETTING_CALIBRATE, &CLatteArtMachineDlg::OnSettingCalibrate)
	ON_BN_CLICKED(IDC_START_CALIBRATION, &CLatteArtMachineDlg::OnBnClickedStartCalibration)

	ON_MESSAGE( WM_UPDATE_IMAGE ,&CLatteArtMachineDlg::UpdateImage )
	ON_BN_CLICKED(IDC_CLEAR_LOG, &CLatteArtMachineDlg::OnBnClickedClearLog)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////
// CLatteArtMachineDlg ���b�Z�[�W �n���h��
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLatteArtMachineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	setlocale( LC_ALL, "Japanese" );		//���܂��Ȃ�
	//AddStringComPort();
	m_EditServo1.SetWindowText( L"0" );
	m_EditServo2.SetWindowText( L"0" );

	CommandValue.dARM_L1			= 72.0;
	CommandValue.dARM_L2			= 72.0;
	CommandValue.iZeroPoint_X		= 0;
	CommandValue.iZeroPoint_Y		= 0;
	CommandValue.dNowPoint_X		= 0.0;
	CommandValue.dNowPoint_Y		= 0.0;
	CommandValue.iDegPerSec			= 60;	//�������x�w��

	//ini�t�@�C����ǂݍ���
	LoadConnectInitial();

	//�ʐM�ݒ�_�C�A���O
	mDlg_Connect = new CConnectDlg;

	//�T�[�{�ݒ�_�C�A���O
	mDlg_Servo = new CServoDlg;
	mDlg_Servo->Create( CServoDlg::IDD, this );
	mDlg_Servo->ShowWindow( SW_HIDE );

	//�L�����u���[�V�����ݒ�_�C�A���O
	mDlg_Calibrate = new CCalibrationDlg;
	mDlg_Calibrate->Create( CCalibrationDlg::IDD, this );
	mDlg_Calibrate->ShowWindow( SW_HIDE );

	//�v���r���[�G���A�̐ݒ�
	mxc_PreViewArea.SetWindowPos( &CWnd::wndTop, 0, 0, DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT, SWP_NOMOVE | SWP_SHOWWINDOW );
	m_pDC_PreViewArea	= mxc_PreViewArea.GetDC();

	pntDraw[iDrawCount].x	= -1;	//�`��̎n�_��-1�����Ă���
	pntDraw[iDrawCount].y	= -1;
	iDrawCount++;

	//�`��X���b�h�J�n
	DrawThreadStart( this, WM_UPDATE_IMAGE );

	//�R�}���h�X���b�h�J�n
	CommandThreadStart( this, WM_LATTE_COMMAND );

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//�ĕ`�拖��
		mxc_PreViewArea.SetRedraw( TRUE );

		//WindowsDC�Ńv���r���[�G���A��`��(�Ԃ����ႯOpenCV�g������)
		CClientDC	cdc(this);
		CPen		RedPen, BlackPen, BlackPen2, *pOldPen;

		RedPen.CreatePen( PS_SOLID, 1, RGB( 255, 0, 0 ) );
		BlackPen.CreatePen( PS_SOLID, 1, RGB( 0, 0, 0 ) );
		BlackPen2.CreatePen( PS_SOLID, 2, RGB( 0, 0, 0 ) );

		//�`��G���A��\��
		pOldPen = cdc.SelectObject( &BlackPen );
		cdc.Rectangle( DRAW_OFFSET_X - DRAW_AREA_WIDTH / 2,
					DRAW_OFFSET_Y - DRAW_AREA_HEIGHT / 2,
					DRAW_OFFSET_X + DRAW_AREA_WIDTH / 2,
					DRAW_OFFSET_Y + DRAW_AREA_HEIGHT / 2 );
	

		//�R�b�v�G���A�̉~��\��
		pOldPen = cdc.SelectObject( &RedPen );
		cdc.Ellipse( DRAW_OFFSET_X - DRAW_AREA_WIDTH / 2,
					DRAW_OFFSET_Y - DRAW_AREA_HEIGHT / 2,
					DRAW_OFFSET_X + DRAW_AREA_WIDTH / 2,
					DRAW_OFFSET_Y + DRAW_AREA_HEIGHT / 2 );

		//���S����\��
		pOldPen = cdc.SelectObject( &RedPen );
		cdc.MoveTo( DRAW_OFFSET_X, DRAW_OFFSET_Y - DRAW_AREA_HEIGHT / 2 );
		cdc.LineTo( DRAW_OFFSET_X, DRAW_OFFSET_Y + DRAW_AREA_HEIGHT / 2 );
		cdc.MoveTo( DRAW_OFFSET_X - DRAW_AREA_WIDTH / 2, DRAW_OFFSET_Y );
		cdc.LineTo( DRAW_OFFSET_X + DRAW_AREA_WIDTH / 2, DRAW_OFFSET_Y );

		//�v���r���[��ʂ�\��
		if( bReadCsv )
		{
			pOldPen = cdc.SelectObject( &BlackPen2 );

			for( int i = 1 ; i < iDrawCount ; i++ )
			{
				if( ( pntDraw[i].x > ( DRAW_OFFSET_X - DRAW_AREA_WIDTH / 2 ) )
					&& ( pntDraw[i].x < ( DRAW_OFFSET_X + DRAW_AREA_WIDTH / 2 ) )
					&& ( pntDraw[i].y > ( DRAW_OFFSET_Y - DRAW_AREA_HEIGHT / 2 ) )
					&& ( pntDraw[i].y < ( DRAW_OFFSET_Y + DRAW_AREA_HEIGHT / 2 ) )
					)
				{
					if( pntDraw[i].x <= 0 || pntDraw[i].y <= 0 )
					{
						i++;
						cdc.MoveTo( pntDraw[i] );
					}
					else if( pntDraw[i-1].x <= 0 || pntDraw[i].x <= 0 )
					{
						i++;
						//cdc.MoveTo( pntDraw[i] );
					}
					else
					{
						cdc.MoveTo( pntDraw[i-1] );
						cdc.LineTo( pntDraw[i] );
						//TRACE( L"%d	%d\n", pntDraw[i] );
					}
				}
				else
				{
					i++;
				}
			}
		}

		//�ĕ`�拑��
		mxc_PreViewArea.SetRedraw( FALSE );

		CDialog::OnPaint();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
////////////////////////////////////////////////////////////////////////////////////////////////////
HCURSOR CLatteArtMachineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedOk()
{
	//OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedCancel()
{
	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnExit()
{
	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLatteArtMachineDlg::DestroyWindow(void)
{
	//�ʐM�ݒ�̃_�C�A���O�����
	mDlg_Connect->DestroyWindow();
	delete mDlg_Connect;

	//�T�[�{�ݒ�̃_�C�A���O�����
	mDlg_Servo->DestroyWindow();
	delete mDlg_Servo;

	//�L�����u���[�V�����ݒ�̃_�C�A���O�����
	mDlg_Calibrate->DestroyWindow();
	delete mDlg_Calibrate;

	//�v���r���[�G���A���
	mxc_PreViewArea.ReleaseDC( m_pDC_PreViewArea );

	//�T�[�{�g���N�I�t
	OnBnClickedTrqueOff();

	//�ʐM�X���b�h�����
	TerminateThread();
	::CloseHandle( ComThread.hStop );
	::CloseHandle( ComThread.hEnd );

	//�`��X���b�h���
	DrawThreadTerminate();

	//�`��p�C�x���g���
	for( int i = 0 ; i < DRAW_EVENT_MAX ; i++ )
	{
		::CloseHandle( m_hDrawEvent[ i ] );
	}

	//�R�}���h�X���b�h���
	CommandThreadTerminate();

	//�R�}���h�C�x���g���
	for( int i = 0 ; i < COMMAND_EVENT_MAX ; i++ )
	{
		::CloseHandle( m_hCommandEvent[ i ] );
	}
	
	return CDialog::DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �ʐM�ݒ�̃_�C�A���O��\��
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnSettingConnect()
{
	if( !bCreateSettingDlg )
	{
		//�ʐM�ݒ�_�C�A���O
		//mDlg_Connect = new CConnectDlg;
		mDlg_Connect->Create( CConnectDlg::IDD, this );
		mDlg_Connect->ShowWindow( SW_HIDE );

		bCreateSettingDlg = TRUE;
	}

	if( mDlg_Connect->IsWindowVisible() )
	{
		mDlg_Connect->SetActiveWindow();
	}
	else
	{
		mDlg_Connect->ShowWindow( SW_SHOWNORMAL );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �T�[�{�ݒ�̃_�C�A���O��\��
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnSettingServo()
{
	if( mDlg_Servo->IsWindowVisible() )
	{
		mDlg_Servo->SetActiveWindow();
	}
	else
	{
		mDlg_Servo->ShowWindow( SW_SHOWNORMAL );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �L�����u���[�V�����ݒ�̃_�C�A���O��\��
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnSettingCalibrate()
{
	if( mDlg_Calibrate->IsWindowVisible() )
	{
		mDlg_Calibrate->SetActiveWindow();
	}
	else
	{
		mDlg_Calibrate->ShowWindow( SW_SHOWNORMAL );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::AddStringComPort(void)
{
	//m_bSetComPort.AddString( L"COM1" );
	//m_bSetComPort.AddString( L"COM2" );
	//m_bSetComPort.AddString( L"COM3" );
	//m_bSetComPort.AddString( L"COM4" );
	//m_bSetComPort.AddString( L"COM5" );
	//m_bSetComPort.AddString( L"COM6" );
	//m_bSetComPort.AddString( L"COM7" );
	//m_bSetComPort.AddString( L"COM8" );
	//m_bSetComPort.AddString( L"COM9" );
	//m_bSetComPort.AddString( L"COM10" );
	//m_bSetComPort.SetCurSel(4);

	//m_bSpeed.AddString( L"2400" );
	//m_bSpeed.AddString( L"4800" );
	//m_bSpeed.AddString( L"9600" );
	//m_bSpeed.AddString( L"19200" );
	//m_bSpeed.AddString( L"38400" );
	//m_bSpeed.AddString( L"57600" );
	//m_bSpeed.AddString( L"115200" );
	//m_bSpeed.SetCurSel(6);

	//m_bDataBit.AddString( L"4" );
	//m_bDataBit.AddString( L"5" );
	//m_bDataBit.AddString( L"6" );
	//m_bDataBit.AddString( L"7" );
	//m_bDataBit.AddString( L"8" );
	//m_bDataBit.SetCurSel(4);

	//m_bStopBit.AddString( L"�Ȃ�" );
	//m_bStopBit.AddString( L"1" );
	//m_bStopBit.AddString( L"2" );
	//m_bStopBit.SetCurSel(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ���e�A�[�g�}�V���{�̂Ɛڑ�
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedConnect()
{
	UpdateData();
	BOOL	bRet = FALSE;
	CString	strLog;

	if( !bConnect )
	{
		int ComNumber		= ConnectValue.iComNumber;
		int	BaudNumber		= ConnectValue.iBaudNumber;
		int DataBit			= ConnectValue.iDataBit;
		int StopBit			= ConnectValue.iStopBit;

		CString ComPort;
		switch( ComNumber )
		{
			case 0:		ComPort = L"COM1";		break;
			case 1:		ComPort = L"COM2";		break;
			case 2:		ComPort = L"COM3";		break;
			case 3:		ComPort = L"COM4";		break;
			case 4:		ComPort = L"COM5";		break;
			case 5:		ComPort = L"COM6";		break;
			case 6:		ComPort = L"COM7";		break;
			case 7:		ComPort = L"COM8";		break;
			case 8:		ComPort = L"COM9";		break;
			case 9:		ComPort = L"COM10";		break;
		}

		int BaudRate;
		switch( BaudNumber )
		{
			case 0:		BaudRate = 2400;		break;
			case 1:		BaudRate = 4800;		break;
			case 2:		BaudRate = 9600;		break;
			case 3:		BaudRate = 19200;		break;
			case 4:		BaudRate = 38400;		break;
			case 5:		BaudRate = 57600;		break;
			case 6:		BaudRate = 115200;		break;
		}

		int Data;
		switch( DataBit )
		{
			case 0:		Data = 4;		break;
			case 1:		Data = 5;		break;
			case 2:		Data = 6;		break;
			case 3:		Data = 7;		break;
			case 4:		Data = 8;		break;
		}

		BYTE Stop;
		switch( StopBit )
		{
			case 0:		Stop = ONESTOPBIT;		break;
			case 1:		Stop = ONE5STOPBITS;	break;
			case 2:		Stop = TWOSTOPBITS;		break;
		}


		ComThread.hCom = CreateFile( ComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		//ComThread.hCom = CreateFile( ComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL );
		if( ComThread.hCom == INVALID_HANDLE_VALUE )
		{
			m_Edit_ScriptLog.GetWindowText( strLog );
			strLog =  L" > COM�|�[�g�̃I�[�v���Ɏ��s���܂���\r\n" + strLog;
			m_Edit_ScriptLog.SetWindowText( strLog );
			return;
		}

		DCB dcb;
		//memset( &dcb, 0, sizeof( DCB ) );
		dcb.DCBlength		= sizeof( DCB );
		bRet = GetCommState( ComThread.hCom, &dcb );

		dcb.BaudRate		= BaudRate;
		dcb.ByteSize		= Data;
		dcb.Parity			= NOPARITY;
		dcb.StopBits		= Stop;
		bRet = SetCommState( ComThread.hCom, &dcb );
		int	iError = GetLastError();

		if( bRet )
		{
			bConnect = TRUE;
			ComThread.hStop	= ::CreateEvent( NULL, true, false, NULL );
			ComThread.hEnd	= ::CreateEvent( NULL, true, false, NULL );

			m_Edit_ScriptLog.GetWindowText( strLog );
			strLog =  L" > COM�|�[�g�ɐڑ����܂���\r\n" + strLog;
			m_Edit_ScriptLog.SetWindowText( strLog );
		}
		else
		{
			m_Edit_ScriptLog.GetWindowText( strLog );
			strLog =  L" > COM�|�[�g�̃I�[�v���Ɏ��s���܂���\r\n" + strLog;
			m_Edit_ScriptLog.SetWindowText( strLog );
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �`��X���b�h
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::DrawThreadStart(CWnd* hWnd, UINT message)
{
	if( m_pDrawThread != NULL )
	{
		ASSERT( FALSE );
		return;
	}

	ASSERT( hWnd != NULL );
	ASSERT( message >= WM_USER );
	m_hParentWnd	= hWnd;
	m_DrawMessage	= message;

	m_pDrawThread					= ::AfxBeginThread( DrawThreadEntry, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	m_pDrawThread->m_bAutoDelete	= FALSE;
	m_pDrawThread->ResumeThread();

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::DrawThreadTerminate(void)
{
	if( m_pDrawThread == NULL )
	{
		ASSERT( FALSE );
		return;
	}

	::SetEvent( m_hDrawEvent[ DRAW_EVENT_STOP ] );

	if( ::WaitForSingleObject( m_pDrawThread->m_hThread, 10000 ) == WAIT_TIMEOUT )
	{
		::TerminateThread( m_pDrawThread->m_hThread, 0xffffffff );
		::AfxMessageBox( L"Draw Thread Force Terminate" );
	}

	delete m_pDrawThread;
	m_pDrawThread = NULL;

	::ResetEvent( m_hDrawEvent[ DRAW_EVENT_STOP ] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
UINT CLatteArtMachineDlg::DrawThreadEntry(LPVOID pParam)
{
	CLatteArtMachineDlg*	pDlg	= (CLatteArtMachineDlg*)pParam;

	return pDlg->DrawThreadProc();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
UINT CLatteArtMachineDlg::DrawThreadProc(void)
{
	DWORD	eventCode	= 0;
	BOOL	bContinue	= TRUE;

	while( bContinue )
	{
		eventCode = ::WaitForMultipleObjects( 2, m_hDrawEvent, FALSE, INFINITE );

		if( eventCode < WAIT_OBJECT_0 )
		{
			continue;
		}
		DWORD eventNo = eventCode - WAIT_OBJECT_0;

		switch( eventNo )
		{
		case	DRAW_EVENT_EXEC:
			{
				::ResetEvent( m_hDrawEvent[ DRAW_EVENT_EXEC ] );

				//��ʂ̍X�V
				m_hParentWnd->PostMessageW( m_DrawMessage, 0 );
				
			}
			break;
			
		case	DRAW_EVENT_STOP:
			{
				bContinue = FALSE;
			}
			break;

		default:
			break;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedDisconnect()
{
	UpdateData();
	CString		strLog;

	if( bConnect )
	{
		TerminateThread();
		if( ComThread.hCom != INVALID_HANDLE_VALUE )
		{
			::CloseHandle( ComThread.hCom );
			ComThread.hCom = INVALID_HANDLE_VALUE;
		}
		bConnect = FALSE;

		m_Edit_ScriptLog.GetWindowText( strLog );
		strLog =  L" > �ؒf���܂���\r\n" + strLog;
		m_Edit_ScriptLog.SetWindowText( strLog );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
afx_msg LRESULT CLatteArtMachineDlg::UpdateImage(WPARAM /*wDummy*/, LPARAM /*Ldummy*/)
{
	Invalidate( FALSE );
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �g���NON
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedTrqueOn()
{
	BYTE byteSendBuffer[9];
	DWORD	dwWrite;
	BOOL	bRet;

	//�g���NON(�T�[�{1)
	if( bServo1 )
	{
		byteSendBuffer[0] = 0xFA;
		byteSendBuffer[1] = 0xAF;
		byteSendBuffer[2] = 0x01;
		byteSendBuffer[3] = 0x00;
		byteSendBuffer[4] = 0x24;
		byteSendBuffer[5] = 0x01;
		byteSendBuffer[6] = 0x01;
		byteSendBuffer[7] = 0x01;
		byteSendBuffer[8] = 0x24;
		bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	}

	if( bServo2 )
	{
		//�g���NON(�T�[�{2)
		byteSendBuffer[0] = 0xFA;
		byteSendBuffer[1] = 0xAF;
		byteSendBuffer[2] = 0x02;
		byteSendBuffer[3] = 0x00;
		byteSendBuffer[4] = 0x24;
		byteSendBuffer[5] = 0x01;
		byteSendBuffer[6] = 0x01;
		byteSendBuffer[7] = 0x01;
		byteSendBuffer[8] = 0x27;
		bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	}

	//�g���NON(�T�[�{3)
	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x03;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x24;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = 0x01;
	byteSendBuffer[8] = byteSendBuffer[2] ^ byteSendBuffer[3] ^ byteSendBuffer[4] ^ byteSendBuffer[5] ^ byteSendBuffer[6] ^ byteSendBuffer[7];
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	bServo3	 = TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �g���NOFF
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedTrqueOff()
{
	BYTE byteSendBuffer[9];
	DWORD	dwWrite;
	BOOL	bRet;

	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x01;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x24;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = 0x00;
	byteSendBuffer[8] = 0x25;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );

	//�g���NOFF(�T�[�{2)
	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x02;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x24;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = 0x00;
	byteSendBuffer[8] = 0x26;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );

	//�g���NOFF(�T�[�{3)
	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x03;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x24;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = 0x00;
	byteSendBuffer[8] = byteSendBuffer[2] ^ byteSendBuffer[3] ^ byteSendBuffer[4] ^ byteSendBuffer[5] ^ byteSendBuffer[6] ^ byteSendBuffer[7];
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	bServo3	 = FALSE;

	CommandValue.bWrite = FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Write ON
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedButtonWriteOn()
{
	int		iData1	 = ServoValue.iWriteONdeg;
	BYTE	byteSendBuffer[64];
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	if( bServo3 )
	{
		//�~���NON�ʒu�Ɉړ�
		byteDeg1	= ( iData1*10 ) & 0xFF;
		byteDeg2	= ( iData1*10 ) >> 8;
		byteSum		= 0x03 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;
		byteSendBuffer[0]	= 0xFA;
		byteSendBuffer[1]	= 0xAF;
		byteSendBuffer[2]	= 0x03;
		byteSendBuffer[3]	= 0x00;
		byteSendBuffer[4]	= 0x1E;
		byteSendBuffer[5]	= 0x02;
		byteSendBuffer[6]	= 0x01;
		byteSendBuffer[7]	= byteDeg1;
		byteSendBuffer[8]	= byteDeg2;
		byteSendBuffer[9]	= byteSum;
		bRet				= WriteFile( ComThread.hCom, byteSendBuffer, 64, &dwWrite, NULL );
		if( !bRet )
		{
			return;
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Write OFF
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedButtonWriteOff()
{
	int		iData1	 = ServoValue.iWriteOFFdeg;
	BYTE	byteSendBuffer[64];
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	if( bServo3 )
	{
		byteDeg1	= ( iData1*10 ) & 0xFF;
		byteDeg2	= ( iData1*10 ) >> 8;
		byteSum		= 0x03 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;
		byteSendBuffer[0]	= 0xFA;
		byteSendBuffer[1]	= 0xAF;
		byteSendBuffer[2]	= 0x03;
		byteSendBuffer[3]	= 0x00;
		byteSendBuffer[4]	= 0x1E;
		byteSendBuffer[5]	= 0x02;
		byteSendBuffer[6]	= 0x01;
		byteSendBuffer[7]	= byteDeg1;
		byteSendBuffer[8]	= byteDeg2;
		byteSendBuffer[9]	= byteSum;
		bRet				= WriteFile( ComThread.hCom, byteSendBuffer, 64, &dwWrite, NULL );
		if( !bRet )
		{
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 90�x��]
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedTurn90deg()
{
	BYTE	byteSendBuffer[10];
	DWORD	dwWrite;
	BOOL	bRet;

	if( bServo1 )
	{
		//90�x��](�T�[�{1)
		byteSendBuffer[0]	= 0xFA;
		byteSendBuffer[1]	= 0xAF;
		byteSendBuffer[2]	= 0x01;
		byteSendBuffer[3]	= 0x00;
		byteSendBuffer[4]	= 0x1E;
		byteSendBuffer[5]	= 0x02;
		byteSendBuffer[6]	= 0x01;
		byteSendBuffer[7]	= 0x84;
		byteSendBuffer[8]	= 0x03;
		byteSendBuffer[9]	= 0x9B;
		bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	}

	if( bServo2 )
	{
		//90�x��](�T�[�{2)
		byteSendBuffer[0]	= 0xFA;
		byteSendBuffer[1]	= 0xAF;
		byteSendBuffer[2]	= 0x02;
		byteSendBuffer[3]	= 0x00;
		byteSendBuffer[4]	= 0x1E;
		byteSendBuffer[5]	= 0x02;
		byteSendBuffer[6]	= 0x01;
		byteSendBuffer[7]	= 0x84;
		byteSendBuffer[8]	= 0x03;
		byteSendBuffer[9]	= 0x98;
		bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// -90�x��]
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedReturn90deg()
{
	BYTE	byteSendBuffer[10];
	DWORD	dwWrite;
	BOOL	bRet;

	if( bServo1 )
	{
		//90�x��](�T�[�{1)
		byteSendBuffer[0]	= 0xFA;
		byteSendBuffer[1]	= 0xAF;
		byteSendBuffer[2]	= 0x01;
		byteSendBuffer[3]	= 0x00;
		byteSendBuffer[4]	= 0x1E;
		byteSendBuffer[5]	= 0x02;
		byteSendBuffer[6]	= 0x01;
		byteSendBuffer[7]	= 0x7C;
		byteSendBuffer[8]	= 0xFC;
		byteSendBuffer[9]	= 0x9C;
		bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	}

	if( bServo2 )
	{
		//90�x��](�T�[�{2)
		byteSendBuffer[0]	= 0xFA;
		byteSendBuffer[1]	= 0xAF;
		byteSendBuffer[2]	= 0x02;
		byteSendBuffer[3]	= 0x00;
		byteSendBuffer[4]	= 0x1E;
		byteSendBuffer[5]	= 0x02;
		byteSendBuffer[6]	= 0x01;
		byteSendBuffer[7]	= 0x7C;
		byteSendBuffer[8]	= 0xFC;
		byteSendBuffer[9]	= 0x9F;
		bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedCheckServo1()
{
	UpdateData();

	if( m_chkServo1.GetCheck() )
	{
		bServo1 = TRUE;
	}
	else
	{
		bServo1 = FALSE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedCheckServo2()
{
	UpdateData();

	if( m_chkServo2.GetCheck() )
	{
		bServo2 = TRUE;
	}
	else
	{
		bServo2 = FALSE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedCheckServo3()
{
	UpdateData();

	if( m_chkServo3.GetCheck() )
	{
		bServo3 = TRUE;
	}
	else
	{
		bServo3 = FALSE;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �w��p�x��]
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedDirectionDeg()
{
	BYTE	byteSendBuffer[10];
	DWORD	dwWrite;
	BOOL	bRet;

	CString strTmp;
	int iServo1deg = 0;
	int iServo2deg = 0;
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteSum;
	
	if( bServo1 )
	{
		//�G�f�B�b�g�{�b�N�X����l���擾
		m_EditServo1.GetWindowText( strTmp );
		iServo1deg	= _tstoi( strTmp );
		byteDeg1	= ( iServo1deg * 10 ) & 0xFF;
		byteDeg2	= ( iServo1deg * 10 ) >> 8;
		byteSum		= 0x01 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;

		byteSendBuffer[0]	= 0xFA;
		byteSendBuffer[1]	= 0xAF;
		byteSendBuffer[2]	= 0x01;
		byteSendBuffer[3]	= 0x00;
		byteSendBuffer[4]	= 0x1E;
		byteSendBuffer[5]	= 0x02;
		byteSendBuffer[6]	= 0x01;
		byteSendBuffer[7]	= byteDeg1;
		byteSendBuffer[8]	= byteDeg2;
		byteSendBuffer[9]	= byteSum;
		bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	}

	if( bServo2 )
	{
		//�G�f�B�b�g�{�b�N�X����l���擾
		m_EditServo2.GetWindowText( strTmp );
		iServo2deg	= _tstoi( strTmp );
		byteDeg1	= ( iServo2deg * 10 ) & 0xFF;
		byteDeg2	= ( iServo2deg * 10 ) >> 8;
		byteSum		= 0x02 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;

		byteSendBuffer[0]	= 0xFA;
		byteSendBuffer[1]	= 0xAF;
		byteSendBuffer[2]	= 0x02;
		byteSendBuffer[3]	= 0x00;
		byteSendBuffer[4]	= 0x1E;
		byteSendBuffer[5]	= 0x02;
		byteSendBuffer[6]	= 0x01;
		byteSendBuffer[7]	= byteDeg1;
		byteSendBuffer[8]	= byteDeg2;
		byteSendBuffer[9]	= byteSum;
		bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ʓ������N���b�N�������Ƃ�
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);

	if( ( point.x > ( DRAW_OFFSET_X - DRAW_AREA_WIDTH / 2 ) ) && ( point.x < ( DRAW_OFFSET_X + DRAW_AREA_WIDTH / 2 ) )
		&& ( point.y > ( DRAW_OFFSET_Y - DRAW_AREA_HEIGHT / 2 ) ) && ( point.y < ( DRAW_OFFSET_Y + DRAW_AREA_HEIGHT / 2 ) ) )
	{
		double dAlpha, dBeta;						//�p�x��,��
		double dTh11, dTh12, dTh21, dTh22;			//��1,��2
		double dDeg11, dDeg12, dDeg21, dDeg22;		//��1��deg,��2��deg
		double dDegree1, dDegree2;

		double dCup_x = ( point.x - DRAW_OFFSET_X ) * static_cast<double>( ( CommandValue.iCupDiameter / 60.0 ) * ( 60.0 / DRAW_AREA_WIDTH ) );
		double dCup_y = ( point.y - DRAW_OFFSET_Y ) * static_cast<double>( ( CommandValue.iCupDiameter / 60.0 ) * ( 60.0 / DRAW_AREA_HEIGHT ) );
		double dScale = 1.0;
		double x = dCup_x;
		double y = dCup_y +90.0;
		
		//���ƃ����Z�o(������ƕ��G)
		dAlpha	= acos( ( -( x*x + y*y ) + CommandValue.dARM_L2*CommandValue.dARM_L2 + CommandValue.dARM_L1*CommandValue.dARM_L1 )
							/ ( 2 * CommandValue.dARM_L1 * CommandValue.dARM_L2 ) );
		dBeta	= acos( ( -( CommandValue.dARM_L2*CommandValue.dARM_L2 ) + CommandValue.dARM_L1*CommandValue.dARM_L1 + ( x*x + y*y ) )
							/ ( 2 * CommandValue.dARM_L1 * sqrt( x*x + y*y ) ) );

		//��1�ƃ�2���Z�o
		dTh11	= atan( y / x ) - dBeta;
		dTh12	= atan( y / x ) + dBeta;
		dTh21	=  M_PI - dAlpha;
		dTh22	= ( -1 ) * ( M_PI - dAlpha );

		dDeg11	= dTh11 * 180.0 / M_PI;
		dDeg12	= dTh12 * 180.0 / M_PI;
		dDeg21	= dTh21 * 180.0 / M_PI;
		dDeg22	= dTh22 * 180.0 / M_PI;

		if( x >= 0 )
		{
			dDegree1 = ServoValue.iServo1Direction * ( dDeg11 - 90 );
			dDegree2 = ServoValue.iServo2Direction * dDeg22;
		}
		else
		{
			dDegree1 = ServoValue.iServo1Direction * ( dDeg11 + 90 );
			dDegree2 = ServoValue.iServo2Direction * dDeg22;
		}

		CString		strTmp;
		strTmp.Format( L"%2.2f", dDegree1 );
		m_EditServo1.SetWindowText( strTmp );
		strTmp.Format( L"%2.2f", ( dDegree2 ) );
		m_EditServo2.SetWindowText( strTmp );

		OnBnClickedDirectionDeg();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// csv�t�@�C���̓ǂݍ���
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedLoadcsv()
{
	BOOL		bRet;
	CStringA	aStr;
	CString		strFilePath;
	CString		strLog;
	CFileDialog	*FileDlg;
	double		dMultiple = 1.0;		//�g��k����
	BOOL		bMultiple = FALSE;		//�g��k���t���O
	BOOL		bInterpolation = TRUE;	//��ԃt���O
 	dX_old	= 0.0;
	dY_old	= 0.0;

	bRet	= InitializationScript();
	if( !bRet )
	{
		AfxMessageBox( L"�L�����u���[�V�����G���[" );
		return;
	}

	//�t�@�C���I�[�v��
	FileDlg = new CFileDialog( TRUE,
								_T("Load csv"),
								_T(""),
								OFN_READONLY | OFN_NOCHANGEDIR,
								_T("csv�t�@�C��(*.csv)|*.csv|�S��(*.*)|*.*||")
								);
	if( FileDlg->DoModal() == IDOK )
	{
		strFilePath = FileDlg->GetPathName();
		aStr = strFilePath;
		m_bCsvPath.SetWindowText( strFilePath );
	}
	else
	{
		delete	FileDlg;
		return;
	}
	delete FileDlg;


	//csv�t�@�C���̓ǂݍ��݊J�n
	char			charTmp[255];				//�ǂݍ���1�s�f�[�^���ꎞ�I�Ɋi�[���镶����
	CString			strTmp;
	std::fstream	file;
	//int				iScriptLineCount = 0;

	//��U�t�@�C�����J���čs�����J�E���g����
	iScriptLineCount = GetLineCount( strFilePath );
	if( iScriptLineCount == 0 )
	{
		AfxMessageBox( L"csv�t�@�C�����J�����Ƃ��ł��܂���" );
		return;
	}

	//�X�N���v�g����͂���
	file.open( strFilePath, std::ios::in );
	if( !file.is_open() )
	{
		AfxMessageBox( L"csv�t�@�C�����J�����Ƃ��ł��܂���" );
		return;
	}
	

	//�v���r���[�p�̃J�E���^���N���A
	iDrawCount = 0;

	//�X�N���v�g���擾���Ċi�[����
	for( iCount = 0 ; iCount < iScriptLineCount ; iCount++ )
	{
		CString		strComment;
		int iCode;
		file.getline( charTmp, 255 );
		strTmp = charTmp;

		//�R�����g�����ǂ������f����
		strComment			= strTmp.Mid( 0, 1 );
		if( strComment == ';' )		//�R�����g���̂Ƃ�
		{
			iScriptLineCount--;
			iCount--;
		}
		else						//�R�[�h�̂Ƃ�
		{
			//�R�[�h�ԍ���ǂݎ��
			iCode			= strTmp.Find( ',', 0 );
			strTmpScript[iCount][0] = strTmp.Mid( 1, iCode - 1 );
			strTmp			= strTmp.Mid( iCode + 1 );

			//��������ǂݎ��
			iCode		= strTmp.Find( ',', 0 );
			strTmpScript[iCount][1] = strTmp.Mid( 0, iCode );
			strTmp			= strTmp.Mid( iCode + 1 );

			//��������ǂݎ��
			iCode		= strTmp.Find( ',', 0 );
			strTmpScript[iCount][2] = strTmp.Mid( 0 );

			//��ԃt���O�𗧂Ă�
			if( strTmpScript[iCount][0] == "06" )
			{
				if( strTmpScript[iCount][1] == "0" )
				{
					bInterpolation = FALSE;
				}
				else if( strTmpScript[iCount][1] == "1" )
				{
					bInterpolation = TRUE;
				}
			}

			//Write ON�t���O�𗧂Ă�
			if( strTmpScript[iCount][0] == "11" )
			{
				bWrite	= TRUE;
			}

			//Write ON�t���O���~�낷
			if( strTmpScript[iCount][0] == "12" )
			{
				bWrite = FALSE;
			}

			//G10�R�}���h�̂Ƃ�
			if( strTmpScript[iCount][0] == "10" )
			{
				if( !bWrite )	//Write OFF���͐��`��Ԃ����A���ݍ��W��ێ�����
				{
					dX_old		= _wtof( strTmpScript[iCount][1] );
					dY_old		= _wtof( strTmpScript[iCount][2] );
				}
				else			//Write ON���͐��`��Ԃ��s��
				{			
					//���`��Ԃ��s��
					double dX1, dY1, dX2, dY2;
					dX1	= dX_old;
					dY1	= dY_old;
					dX2 = _wtof( strTmpScript[iCount][1] );
					dY2	= _wtof( strTmpScript[iCount][2] );

					if( bInterpolation )
					{
						//LinearInterpolation( dX1, dY1, dX2, dY2 );	//���`���
						LinearInterpolation2( dX1, dY1, dX2, dY2 );	//���`���
					}

					//��Ԃ��I������猻�ݍ��W��ێ�����
					dX_old	= dX2;
					dY_old	= dY2;
				}
			}
		}
	}
	file.close();

	//��Ԃ��I������R�}���h��CommandValue�Ɋi�[
	for( int i = 0 ; i < iScriptLineCount ; i++ )
	{
		strScript[i][0]	= strTmpScript[i][0];
		strScript[i][1]	= strTmpScript[i][1];
		strScript[i][2]	= strTmpScript[i][2];

		//�g��k������ꍇ
		if( strScript[i][0] == "05" )
		{
			bMultiple = TRUE;
			dMultiple = _wtof( strScript[i][1] );
		}

		//Write ON�t���O�𗧂Ă�
		if( strScript[i][0] == "11" )
		{
			bWrite	= TRUE;
		}

		//Write ON�t���O���~�낷
		if( strScript[i][0] == "12" )
		{
			bWrite = FALSE;
		}

		if( strScript[i][0] == "10" )
		{
			if( bWrite )
			{
				//�R�}���h�ł́}30���}180pix�ɑ�������̂ŁA6�{����
				if( bMultiple )	//�g��k������Ƃ�
				{
					pntDraw[iDrawCount].x	= static_cast<int>( _wtoi( strScript[i][1] ) * 6.0 * dMultiple + DRAW_OFFSET_X );
					pntDraw[iDrawCount].y	= static_cast<int>( _wtoi( strScript[i][2] ) * 6.0 * dMultiple + DRAW_OFFSET_Y );
				}
				else
				{
					pntDraw[iDrawCount].x	= static_cast<int>( _wtoi( strScript[i][1] ) * 6.0 * CommandValue.iCupDiameter / 60.0 + DRAW_OFFSET_X );
					pntDraw[iDrawCount].y	= static_cast<int>( _wtoi( strScript[i][2] ) * 6.0 * CommandValue.iCupDiameter / 60.0 + DRAW_OFFSET_Y );
				}
			}
			else
			{
				pntDraw[iDrawCount].x	= -1;
				pntDraw[iDrawCount].y	= -1;
				iDrawCount++;

				if( bMultiple )	//�g��k������Ƃ�
				{
					pntDraw[iDrawCount].x	= static_cast<int>( _wtoi( strScript[i][1] ) * 6.0 * dMultiple + DRAW_OFFSET_X );
					pntDraw[iDrawCount].y	= static_cast<int>( _wtoi( strScript[i][2] ) * 6.0 * dMultiple + DRAW_OFFSET_Y );
				}
				else
				{
					pntDraw[iDrawCount].x	= static_cast<int>( _wtoi( strScript[i][1] ) * 6.0 * CommandValue.iCupDiameter / 60.0 + DRAW_OFFSET_X );
					pntDraw[iDrawCount].y	= static_cast<int>( _wtoi( strScript[i][2] ) * 6.0 * CommandValue.iCupDiameter / 60.0 + DRAW_OFFSET_Y );
				}
			}
			iDrawCount++;
		}
	}

	bReadCsv	= TRUE;
	bMultiple	= FALSE;
	m_Edit_ScriptLog.GetWindowText( strLog );
	strLog =  L" > �X�N���v�g�ǂݍ���\r\n" + strLog;
	m_Edit_ScriptLog.SetWindowText( strLog );
	::SetEvent( m_hDrawEvent[ DRAW_EVENT_EXEC ] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int CLatteArtMachineDlg::GetLineCount(CString strFilePath)
{
	std::fstream	file;
	int iScriptLineCount = 0;

	file.open( strFilePath, std::ios::in );
	if( !file.is_open() )
	{
		return 0;
	}

	//�X�N���v�g���Ō�܂œǂݍ���
	while( !file.eof() )
	{
		std::string strBuffer;
		getline( file, strBuffer );
		iScriptLineCount++;
	}
	file.close();		//�ŏI�s�܂œǂ񂾂̂ň�U����

	//��s�]���ɃJ�E���g����̂Ō��Z
	iScriptLineCount--;

	return iScriptLineCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ���e�A�[�g�J�n
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedStartLatteart()
{
	BOOL	bRet;
	CString	strLog;

	int	iMsg = MessageBox( L"���e�A�[�g���J�n���܂��B��낵���ł����H", 0, MB_YESNO );
	if( iMsg == IDNO )
	{
		return;
	}

	m_Edit_ScriptLog.GetWindowText( strLog );
	if( !bConnect )
	{
		AfxMessageBox( L"�ʐM�G���[" );
		strLog = L" > �ʐM�G���[\r\n" + strLog;
		m_Edit_ScriptLog.SetWindowText( strLog );
		return;
	}
	if( !bReadCsv )
	{
		AfxMessageBox( L"�R�}���h���ǂݍ��܂�Ă��܂���" );
		strLog = L" > �R�}���h�ǂݍ��݃G���[\r\n" + strLog;
		m_Edit_ScriptLog.SetWindowText( strLog );
		return;
	}

	strLog =  L" > ���e�A�[�g�J�n\r\n" + strLog;
	m_Edit_ScriptLog.SetWindowText( strLog );
	UpdateWindow();

	//�X�N���v�g���s
	bRet	= StartLatteArt();

	if( !bRet )
	{
		AfxMessageBox( L"�R�}���h�G���[" );
		return;
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �R�}���h���s
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLatteArtMachineDlg::StartLatteArt(void)
{
	BOOL	bRet = FALSE;
	CString	strLog;

	//�R�}���h�X���b�h�J�n
	if( !bCommandThread )
	{
		::SetEvent( m_hCommandEvent[ COMMAND_EVENT_EXEC ] );
		bCommandThread = TRUE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::CommandThreadStart(CWnd* hWnd, UINT message)
{
	if( m_pCommandThread != NULL )
	{
		ASSERT( FALSE );
		return;
	}

	ASSERT( hWnd != NULL );
	ASSERT( message >= WM_USER );
	m_hParentWnd	= hWnd;
	m_CommandMessage	= message;

	m_pCommandThread				= ::AfxBeginThread( CommandThreadEntry, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL );
	m_pCommandThread->m_bAutoDelete	= FALSE;
	m_pCommandThread->ResumeThread();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::CommandThreadTerminate(void)
{
	if( m_pCommandThread == NULL )
	{
		ASSERT( FALSE );
		return;
	}

	::SetEvent( m_hCommandEvent[ COMMAND_EVENT_STOP ] );

	if( ::WaitForSingleObject( m_pCommandThread->m_hThread, 10000 ) == WAIT_TIMEOUT )
	{
		::TerminateThread( m_pCommandThread->m_hThread, 0xffffffff );
		::AfxMessageBox( L"Command Thread Force Terminate" );
	}

	delete m_pCommandThread;
	m_pCommandThread = NULL;

	::ResetEvent( m_hCommandEvent[ COMMAND_EVENT_STOP ] );
	bCommandThread = FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
UINT CLatteArtMachineDlg::CommandThreadEntry(LPVOID pParam)
{
	CLatteArtMachineDlg*	pDlg	= (CLatteArtMachineDlg*)pParam;

	return pDlg->CommandThreadProc();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �R�}���h���s�X���b�h
////////////////////////////////////////////////////////////////////////////////////////////////////
UINT CLatteArtMachineDlg::CommandThreadProc(void)
{
	DWORD	eventCode	= 0;
	BOOL	bContinue	= TRUE;
	int		iCount		= 0;
	int		iData1		= 0;
	int		iData2		= 0;
	int		iCode, iTensPlace;
	double	dData1		= 0.0;
	double	dData2		= 0.0;
	BOOL	bRet;

	while( bContinue )
	{
		eventCode = ::WaitForMultipleObjects( 3, m_hCommandEvent, FALSE, INFINITE );

		if( eventCode < WAIT_OBJECT_0 )
		{
			continue;
		}
		DWORD eventNo = eventCode - WAIT_OBJECT_0;

		switch( eventNo )
		{
		case	COMMAND_EVENT_EXEC:
			{
				//::ResetEvent( m_hCommandEvent[ COMMAND_EVENT_EXEC ] );

				iCode	=	_tstoi( strScript[iCount][0] );
				dData1	=	_tstof( strScript[iCount][1] );
				dData2	=	_tstof( strScript[iCount][2] );
				iData1	= 0;
				iData2	= 0;
				iTensPlace = iCode / 10;	//10�Ŋ����Ă킴��1�̈ʂ�������������

				//�R�}���h���ɃX�N���v�g���s�֐��ɓ�����
				switch( iTensPlace )
				{
					//�R�}���hG0x
					case 0:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G0x( iCode, dData1, dData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG1x
					case 1:
					{
						bRet	= LatteArtCommand_G1x( iCode, dData1, dData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG2x
					case 2:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G2x( iCode, iData1, iData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG3x
					case 3:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G3x( iCode, iData1, iData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG4x
					case 4:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G4x( iCode, iData1, iData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG5x
					case 5:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G5x( iCode, iData1, iData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG6x
					case 6:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G6x( iCode, iData1, iData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG7x
					case 7:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G7x( iCode, iData1, iData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG8x
					case 8:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G8x( iCode, iData1, iData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					//�R�}���hG9x
					case 9:
					{
						iData1	= static_cast<int>( dData1 );
						iData2	= static_cast<int>( dData2 );
						bRet	= LatteArtCommand_G9x( iCode, iData1, iData2 );
						if( bRet )
						{
							::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
							AfxMessageBox( L"�R�}���h�G���[" );
						}
						break;
					}

					default:
					{
						break;
					}
				}
				
				iCount++;
			}
			break;
		
		case	COMMAND_EVENT_BREAK:
			{
				CString strLog;
				::ResetEvent( m_hCommandEvent[ COMMAND_EVENT_EXEC ] );
				::ResetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );

				m_Edit_ScriptLog.GetWindowText( strLog );
				if( bEscape )
				{
					AfxMessageBox( L"���f���܂���" );
					strLog = L" > ���f���܂���\r\n" + strLog;
					bEscape = FALSE;
				}
				else if( !bConnect )
				{
					strLog = L" > �ʐM�G���[\r\n" + strLog;
					bEscape = FALSE;
				}
				else
				{
					strLog = L" > ����\r\n" + strLog;
				}

				OnBnClickedTrqueOff();	//�g���NOFF
				m_Edit_ScriptLog.SetWindowText( strLog );
				iCount = 0;
				bCommandThread			= FALSE;
				CommandValue.bMultiple	= FALSE;
			}
			break;
			
		case	COMMAND_EVENT_STOP:
			{
				bContinue = FALSE;
			}
			break;

		default:
			break;
		}

		if( iCount == iScriptLineCount )
		{
				::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::LoadConnectInitial(void)
{
	int			iSetting;
	CString		strSetting;
	CWinApp*	pApp = AfxGetApp();

	//�|�[�gNo��ǂݍ���
	iSetting					= pApp->GetProfileInt( L"ComPort", L"PortNo", NULL );	//ini�t�@�C������ǂݍ���
	ConnectValue.iComNumber		= iSetting;			//���L�ϐ��ɑ��

	//�{�[���[�g��ǂݍ���
	iSetting					= pApp->GetProfileInt( L"ComPort", L"BaudRate", NULL );	//ini�t�@�C������ǂݍ���
	ConnectValue.iBaudNumber	= iSetting;			//���L�ϐ��ɑ��

	//�f�[�^�r�b�g�������o��
	iSetting					= pApp->GetProfileInt( L"ComPort", L"DataBit", NULL );	//ini�t�@�C������ǂݍ���
	ConnectValue.iDataBit		= iSetting;			//���L�ϐ��ɑ��

	//�X�g�b�v�r�b�g�������o��
	iSetting					= pApp->GetProfileInt( L"ComPort", L"StopBit", NULL );	//ini�t�@�C������ǂݍ���
	ConnectValue.iStopBit		= iSetting;			//���L�ϐ��ɑ��


	//�R�b�v���a��ǂݍ���
	iSetting					= pApp->GetProfileInt( L"Calibration", L"Diameter", NULL );	//ini�t�@�C������ǂݍ���
	CommandValue.iCupDiameter	= iSetting;			//���L�ϐ��ɑ��
	
	//�L�����u���[�V�����񐔂�ǂݍ���
	iSetting					= pApp->GetProfileInt( L"Calibration", L"Repeat", NULL );
	CommandValue.iCalibRepeat	= iSetting;


	//�T�[�{���t�]�ݒ��ǂݍ���
	//�T�[�{1
	iSetting					= pApp->GetProfileInt( L"ServoDirection", L"Servo1", NULL );
	if( iSetting == 0 )
	{
		iSetting				= 1;
	}
	ServoValue.iServo1Direction	= iSetting;

	//�T�[�{2
	iSetting					= pApp->GetProfileInt( L"ServoDirection", L"Servo2", NULL );
	if( iSetting == 0 )
	{
		iSetting				= 1;
	}
	ServoValue.iServo2Direction	= iSetting;


	//Write�T�[�{�̊p�x�ǂݍ���
	iSetting					= pApp->GetProfileInt( L"WriteServo", L"WriteON", NULL );
	ServoValue.iWriteONdeg		= iSetting;

	iSetting					= pApp->GetProfileInt( L"WriteServo", L"WriteOFF", NULL );
	ServoValue.iWriteOFFdeg		= iSetting;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// G10�R�}���h�ō��W�Ԃ���`���
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::LinearInterpolation(double dX1, double dY1, double dX2, double dY2)
{
	int		iDiv	= 0;		//������
	double	dStepX	= 0.0;		//��ԍ��WX
	double	dStepY	= 0.0;		//��ԍ��WY
	double	dSubX	= 0.0;		//�O���X���W�ƍ����X���W�̍���
	double	dSubY	= 0.0;		//�O���Y���W�ƍ����Y���W�̍���
	double	dLength	= 0.0;		//�O����W���獡����W�܂ł̋���
	CString strStepX, strStepY;

	//�O����W�ƍ�����W�̍��������߂�
	if( dX2 >= dX1 )	{	dSubX	= fabs( dX2 - dX1 );	}
	else				{	dSubX	= fabs( dX1 - dX2 );	}
	if( dY2 >= dY1 )	{	dSubY	= fabs( dY2 - dY1 );	}
	else				{	dSubY	= fabs( dY1 - dY2 );	}

	//dSubX	= ( dX2 - dX1 ) *( dX2 - dX1 );
	//dSubY	= ( dY2 - dY1 ) *( dY2 - dY1 );
	//dLength	= sqrt( abs( dSubX + dSubY ) );
	dLength		= sqrt( ( dSubX ) * ( dSubX ) + ( dSubY ) * ( dSubY ) );



	//�����������߂�
	if( dSubX >= dSubY )
	{
		iDiv = static_cast<int>( dLength - 1 );

		for( int i = 1 ; i <= iDiv ; i++ )
		{
			if( dX2 > dX1 )
			{
				dStepX = dX1 + i;
				dStepY = dY1 + ( dX1 + i  - dX1 ) * ( dY2 - dY1 ) / ( dX2 - dX1 );
			}
			else
			{
				dStepX = dX1 - i;
				dStepY = dY1 + ( dX1 - i  - dX1 ) * ( dY2 - dY1 ) / ( dX2 - dX1 );
			}

			strTmpScript[iCount][0]	= L"10";					//G10�R�}���h�ł����
			strTmpScript[iCount][1].Format( L"%3.3f", dStepX );	//��Ԃ���X���W
			strTmpScript[iCount][2].Format( L"%3.3f", dStepY );	//��Ԃ���Y���W
			iCount++;
			iScriptLineCount++;
		}
	}
	else
	{
		iDiv = static_cast<int>( abs( dSubY ) - 1 );

		for( int i = 1 ; i <= iDiv ; i++ )
		{
			if( dY2 > dY1 )
			{
				dStepY = dY1 + i;
				dStepX = dX1 + ( dY1 + i  - dY1 ) * ( dX2 - dX1 ) / ( dY2 - dY1 );
			}
			else
			{
				dStepY = dY1 - i;
				dStepX = dX1 + ( dY1 - i  - dY1 ) * ( dX2 - dX1 ) / ( dY2 - dY1 );
			}
			
			strTmpScript[iCount][0]	= L"10";					//G10�R�}���h�ł����
			strTmpScript[iCount][1].Format( L"%3.3f", dStepX );	//��Ԃ���X���W
			strTmpScript[iCount][2].Format( L"%3.3f", dStepY );	//��Ԃ���Y���W
			iCount++;
			iScriptLineCount++;
		}
	}

	strTmpScript[iCount][0]	= L"10";					//G10�R�}���h�ł����
	strTmpScript[iCount][1].Format( L"%3.3f", dX2 );	//��ԏI�_X���W
	strTmpScript[iCount][2].Format( L"%3.3f", dY2 );	//��ԏI�_Y���W

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// G10�R�}���h�ō��W�Ԃ���`���2(���x�����)
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::LinearInterpolation2(double dX1, double dY1, double dX2, double dY2)
{
	int		iDiv	= 0;		//������
	double	dStepX	= 0.0;		//��ԍ��WX
	double	dStepY	= 0.0;		//��ԍ��WY
	double	dSubX	= 0.0;		//�O���X���W�ƍ����X���W�̍���
	double	dSubY	= 0.0;		//�O���Y���W�ƍ����Y���W�̍���
	double	dRatio	= 0.0;		//�O����W���獡����W�܂ł̋���
	double	dDiv	= 0.0;
	CString strStepX, strStepY;

	//�O����W�ƍ�����W�̍��������߂�
	if( dX2 >= dX1 )	{	dSubX	= fabs( dX2 - dX1 );	}
	else				{	dSubX	= fabs( dX1 - dX2 );	}
	if( dY2 >= dY1 )	{	dSubY	= fabs( dY2 - dY1 );	}
	else				{	dSubY	= fabs( dY1 - dY2 );	}

	dDiv = sqrt( ( dSubX * dSubX ) + ( dSubY * dSubY ) );// / CommandValue.dAccuracy ;
	iDiv = static_cast<int>( dDiv / CommandValue.dAccuracy );

	//�������x�����Ԍ������߂�
	//if( dSubX >= dSubY )
	//{
	//	iDiv = static_cast<int>( dSubX / CommandValue.dAccuracy );
	//}
	//else
	//{
	//	iDiv = static_cast<int>( dSubY / CommandValue.dAccuracy );
	//}

	//���`��Ԃ��s��
	for( int i = 1 ; i < iDiv ; i++ )
	{
		//���`��Ԃ̔䗦���v�Z
		dRatio = static_cast<double>( i ) / static_cast<double>( iDiv );
		dStepX = ( 1 - dRatio ) * dX1 + dRatio * dX2;
		dStepY = ( 1 - dRatio ) * dY1 + dRatio * dY2;


		//if( dX2 >= dX1 )		//X�����̐V���W�̕��������W�����v���X�����̏ꍇ
		//{
		//	dStepX = ( 1 - dRatio ) * dX1 + dRatio * dX2;
		//}
		//else
		//{
		//	dStepX = ( 1 - dRatio ) * dX2 + dRatio * dX1;
		//}

		//if( dY2 >= dY1 )		//Y�����̐V���W�̕��������W�����v���X�����̏ꍇ
		//{
		//	dStepY = ( 1 - dRatio ) * dY1 + dRatio * dY2;
		//}
		//else
		//{
		//	dStepY = ( 1 - dRatio ) * dY2 + dRatio * dY1;
		//}

		strTmpScript[iCount][0]	= L"10";					//G10�R�}���h�ł����
		strTmpScript[iCount][1].Format( L"%3.3f", dStepX );	//��Ԃ���X���W
		strTmpScript[iCount][2].Format( L"%3.3f", dStepY );	//��Ԃ���Y���W
		TRACE( "%3.3f	%3.3f\n", dStepX, dStepY );

		iCount++;
		iScriptLineCount++;
	}

	strTmpScript[iCount][0]	= L"10";					//G10�R�}���h�ł����
	strTmpScript[iCount][1].Format( L"%3.3f", dX2 );	//��ԏI�_X���W
	strTmpScript[iCount][2].Format( L"%3.3f", dY2 );	//��ԏI�_Y���W
	TRACE( "%3.3f	%3.3f\n", dX2, dY2 );

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �L�����u���[�V�������s��
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedStartCalibration()
{
	BOOL	bRet;
	CString	strTmpX, strTmpY;
	double	dTmpX, dTmpY;
	double	dMultiple = 0.5;
	int		iCount = 0;
	iScriptLineCount = 0;

	bRet	= InitializationScript();
	if( !bRet )
	{
		AfxMessageBox( L"�L�����u���[�V�����G���[" );
		return;
	}


	//�g���NON
	strScript[iScriptLineCount][0]	= L"00";
	iScriptLineCount++;

	//���x�w��
	strScript[iScriptLineCount][0]	= L"02";
	strScript[iScriptLineCount][1] = L"1";
	iScriptLineCount++;

	//Write OFF
	strScript[iScriptLineCount][0] = L"12";
	iScriptLineCount++;

	//�L�����u���[�V�������암
	for( int i = 1 ; i < CommandValue.iCalibRepeat+1 ; i++ )	//�������邩
	{
		for( int j = 0 ; j < 360 ; j++ ) //1deg����360����
		{
			//X���W�AY���W���v�Z
			dTmpX	= CommandValue.iCupDiameter * sin( DEG2RAD( static_cast<double>( j ) ) ) * dMultiple;
			dTmpY	= ( -1 ) * CommandValue.iCupDiameter * cos( DEG2RAD( static_cast<double>( j ) ) ) * dMultiple;
			strTmpX.Format( L"%3.3f", dTmpX );
			strTmpY.Format( L"%3.3f", dTmpY );
			//TRACE( L"%3.3f	%3.3f\n", dTmpX, dTmpY );

			strScript[iScriptLineCount + iCount][0] = L"10";
			strScript[iScriptLineCount + iCount][1] = strTmpX;
			strScript[iScriptLineCount + iCount][2] = strTmpY;
			iCount++;
		}
	}
	iScriptLineCount = iScriptLineCount + iCount;


	bRet = StartLatteArt();
	if( !bRet )
	{
		AfxMessageBox( L"�L�����u���[�V�����G���[" );
		return;
	}

	//�L�����u���[�V���������ŁA�R�}���h�ǂݍ��݃t���O�N���A
	bReadCsv = FALSE;
	m_bCsvPath.SetWindowText( L"" );
	Invalidate( FALSE );

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �X�N���v�g�z���������
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLatteArtMachineDlg::InitializationScript(void)
{
	for( int i = 0 ; i < SCRIPT_COLUMN2 ; i++ )
	{
		for( int j = 0 ; j < SCRIPT_ROW ; j++ )
		{
			strScript[i][j] = L"";
			strTmpScript[i][j] = L"";
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ���샍�O���N���A
////////////////////////////////////////////////////////////////////////////////////////////////////
void CLatteArtMachineDlg::OnBnClickedClearLog()
{
	CString strLog;

	strLog = L"";
	m_Edit_ScriptLog.SetWindowText( strLog );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �L�[���͂��t�b�N
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLatteArtMachineDlg::PreTranslateMessage(MSG* pMsg)
{
	if( WM_KEYDOWN == pMsg->message )
	{
		switch( pMsg->wParam )
		{
			case VK_ESCAPE:
				{
					if( bCommandThread )
					{
						bEscape = TRUE;
						::SetEvent( m_hCommandEvent[ COMMAND_EVENT_BREAK ] );
					}
					return FALSE;
				}
			case VK_RETURN:
				{
					return FALSE;
				}
			default:
				{
					break;
				}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////