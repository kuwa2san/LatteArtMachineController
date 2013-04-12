////////////////////////////////////////////////////////////////////////////////////////////////////
// ConnectDlg.cpp : �����t�@�C��
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LatteArtMachine.h"
#include "ConnectDlg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CConnectDlg �_�C�A���O
////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CConnectDlg, CDialog)

////////////////////////////////////////////////////////////////////////////////////////////////////
CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
CConnectDlg::~CConnectDlg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_COM, m_bSetComPort);
	//DDX_Control(pDX, IDC_COMBO_BAUD, m_bSpeed);
	//DDX_Control(pDX, IDC_COMBO_DATABIT, m_bDataBit);
	//DDX_Control(pDX, IDC_COMBO_STOPBIT, m_bStopBit);
	//DDX_Control(pDX, IDC_COMBO_COM, m_bSetComPort);
	//DDX_Control(pDX, IDC_COMBO_SPEED, m_bSpeed);
	//DDX_Control(pDX, IDC_COMBO_DATABIT, m_bDataBit);
	//DDX_Control(pDX, IDC_COMBO_STOPBIT, m_bStopBit);
	//DDX_Control(pDX, IDC_COMBO_COMPORT, m_bSetComPort);
	//DDX_Control(pDX, IDC_COMBO_BAUD, m_bSpeed);
	//DDX_Control(pDX, IDC_COMBO_BITDATA, m_bDataBit);
	//DDX_Control(pDX, IDC_COMBO_BITSTOP, m_bStopBit);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_bComPortSet);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_bBaudRateSet);
	DDX_Control(pDX, IDC_COMBO_BITDATA, m_bDataBitSet);
	DDX_Control(pDX, IDC_COMBO_BITSTOP, m_bStopBitSet);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConnectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConnectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SAVECHANGES, &CConnectDlg::OnBnClickedSavechanges)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////
// CConnectDlg ���b�Z�[�W �n���h��
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CConnectDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	AddStringComPort();
	LoadConnectInitial();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CConnectDlg::OnBnClickedOk()
{
	//OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CConnectDlg::OnBnClickedCancel()
{
	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CConnectDlg::OnBnClickedSavechanges()
{
	ConnectValue.iComNumber		= m_bComPortSet.GetCurSel();
	ConnectValue.iBaudNumber	= m_bBaudRateSet.GetCurSel();
	ConnectValue.iDataBit		= m_bDataBitSet.GetCurSel();
	ConnectValue.iStopBit		= m_bStopBitSet.GetCurSel();


	CString		strSetting;
	int			iSetting;
	CWinApp*	pApp = AfxGetApp();

	//�|�[�gNo�������o��
	iSetting	= m_bComPortSet.GetCurSel();
	pApp->WriteProfileInt( L"ComPort", L"PortNo", iSetting );

	//�{�[���[�g�������o��
	iSetting	= m_bBaudRateSet.GetCurSel();
	pApp->WriteProfileInt( L"ComPort", L"BaudRate", iSetting );

	//�f�[�^�r�b�g�������o��
	iSetting	= m_bDataBitSet.GetCurSel();
	pApp->WriteProfileInt( L"ComPort", L"DataBit", iSetting );

	//�X�g�b�v�r�b�g�������o��
	iSetting	= m_bStopBitSet.GetCurSel();
	pApp->WriteProfileInt( L"ComPort", L"StopBit", iSetting );

	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CConnectDlg::AddStringComPort(void)
{
	m_bComPortSet.AddString( L"COM1" );
	m_bComPortSet.AddString( L"COM2" );
	m_bComPortSet.AddString( L"COM3" );
	m_bComPortSet.AddString( L"COM4" );
	m_bComPortSet.AddString( L"COM5" );
	m_bComPortSet.AddString( L"COM6" );
	m_bComPortSet.AddString( L"COM7" );
	m_bComPortSet.AddString( L"COM8" );
	m_bComPortSet.AddString( L"COM9" );
	m_bComPortSet.AddString( L"COM10" );

	m_bBaudRateSet.AddString( L"2400" );
	m_bBaudRateSet.AddString( L"4800" );
	m_bBaudRateSet.AddString( L"9600" );
	m_bBaudRateSet.AddString( L"19200" );
	m_bBaudRateSet.AddString( L"38400" );
	m_bBaudRateSet.AddString( L"57600" );
	m_bBaudRateSet.AddString( L"115200" );

	m_bDataBitSet.AddString( L"4" );
	m_bDataBitSet.AddString( L"5" );
	m_bDataBitSet.AddString( L"6" );
	m_bDataBitSet.AddString( L"7" );
	m_bDataBitSet.AddString( L"8" );

	m_bStopBitSet.AddString( L"1" );
	m_bStopBitSet.AddString( L"1.5" );
	m_bStopBitSet.AddString( L"2" );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CConnectDlg::LoadConnectInitial(void)
{
	int			iSetting;
	CWinApp*	pApp = AfxGetApp();

	//�|�[�gNo��ǂݍ���
	iSetting	= pApp->GetProfileInt( L"ComPort", L"PortNo", NULL );	//ini�t�@�C������ǂݍ���
	ConnectValue.iComNumber		= iSetting;			//���L�ϐ��ɑ��
	m_bComPortSet.SetCurSel( iSetting );

	//�{�[���[�g��ǂݍ���
	iSetting	= pApp->GetProfileInt( L"ComPort", L"BaudRate", NULL );	//ini�t�@�C������ǂݍ���
	ConnectValue.iBaudNumber		= iSetting;			//���L�ϐ��ɑ��
	m_bBaudRateSet.SetCurSel( iSetting );

	//�f�[�^�r�b�g��ǂݍ���
	iSetting	= pApp->GetProfileInt( L"ComPort", L"DataBit", NULL );	//ini�t�@�C������ǂݍ���
	ConnectValue.iDataBit		= iSetting;			//���L�ϐ��ɑ��
	m_bDataBitSet.SetCurSel( iSetting );

	//�X�g�b�v�r�b�g��ǂݍ���
	iSetting	= pApp->GetProfileInt( L"ComPort", L"StopBit", NULL );	//ini�t�@�C������ǂݍ���
	ConnectValue.iStopBit		= iSetting;			//���L�ϐ��ɑ��
	m_bStopBitSet.SetCurSel( iSetting );
}
