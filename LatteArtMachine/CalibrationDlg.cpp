////////////////////////////////////////////////////////////////////////////////////////////////////
// CalibrationDlg.cpp : �����t�@�C��
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LatteArtMachine.h"
#include "CalibrationDlg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CCalibrationDlg �_�C�A���O
////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CCalibrationDlg, CDialog)

CCalibrationDlg::CCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibrationDlg::IDD, pParent)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
CCalibrationDlg::~CCalibrationDlg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CUPDIAMETER, m_EditCupDiameter);
	DDX_Control(pDX, IDC_EDIT_CALIBREPEAT, m_EditCalibRepeat);
	DDX_Control(pDX, IDC_EDIT_ACCURACY, m_EditAccuracy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCalibrationDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCalibrationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCalibrationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SAVECHANGES, &CCalibrationDlg::OnBnClickedSavechanges)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////
// CCalibrationDlg ���b�Z�[�W �n���h��
////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalibrationDlg::OnBnClickedOk()
{
	//OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalibrationDlg::OnBnClickedCancel()
{
	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalibrationDlg::OnBnClickedSavechanges()
{
	int			iSetting;
	double		dSetting;
	CString		strSetting;
	CWinApp*	pApp = AfxGetApp();

	//�R�b�v���a�������o��
	m_EditCupDiameter.GetWindowText( strSetting );
	iSetting	= _wtoi( strSetting );
	pApp->WriteProfileInt( L"Calibration", L"Diameter", iSetting );
	CommandValue.iCupDiameter = iSetting;

	//�L�����u���[�V�����񐔂������o��
	m_EditCalibRepeat.GetWindowText( strSetting );
	iSetting	= _wtoi( strSetting );
	pApp->WriteProfileInt( L"Calibration", L"Repeat", iSetting );
	CommandValue.iCalibRepeat = iSetting;

	//��Ԑ��x�������o��
	m_EditAccuracy.GetWindowText( strSetting );
	dSetting	= _wtof( strSetting );
	pApp->WriteProfileString( L"Interpolation", L"Accuracy", strSetting );
	CommandValue.dAccuracy	= dSetting;

	
	//�R�}���h�����[�h����
	AfxMessageBox( L"�ݒ肪�ύX����܂����B\n�R�}���h��ǂݍ���ł����ꍇ�̓����[�h���Ă��������B" );
	

	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCalibrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadCalibParam();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalibrationDlg::LoadCalibParam(void)
{
	int			iSetting;
	CString		strSetting;
	CWinApp*	pApp = AfxGetApp();

	//�R�b�v���a��ǂݍ���
	iSetting					= pApp->GetProfileInt( L"Calibration", L"Diameter", NULL );	//ini�t�@�C������ǂݍ���
	CommandValue.iCupDiameter	= iSetting;			//���L�ϐ��ɑ��
	strSetting.Format( L"%d\n", iSetting );
	m_EditCupDiameter.SetWindowText( strSetting );
	CommandValue.iCupDiameter = iSetting;
	
	//�L�����u���[�V�����񐔂�ǂݍ���
	iSetting					= pApp->GetProfileInt( L"Calibration", L"Repeat", NULL );
	CommandValue.iCalibRepeat	= iSetting;
	strSetting.Format( L"%d\n", iSetting );
	m_EditCalibRepeat.SetWindowText( strSetting );
	CommandValue.iCalibRepeat = iSetting;

	//��Ԑ��x��ǂݍ���
	strSetting					= pApp->GetProfileString( L"Interpolation", L"Accuracy", NULL );
	m_EditAccuracy.SetWindowText( strSetting );
	CommandValue.dAccuracy		= _wtof( strSetting );
}
