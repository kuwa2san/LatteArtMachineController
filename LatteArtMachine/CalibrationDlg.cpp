////////////////////////////////////////////////////////////////////////////////////////////////////
// CalibrationDlg.cpp : 実装ファイル
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LatteArtMachine.h"
#include "CalibrationDlg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CCalibrationDlg ダイアログ
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
// CCalibrationDlg メッセージ ハンドラ
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

	//コップ直径を書き出し
	m_EditCupDiameter.GetWindowText( strSetting );
	iSetting	= _wtoi( strSetting );
	pApp->WriteProfileInt( L"Calibration", L"Diameter", iSetting );
	CommandValue.iCupDiameter = iSetting;

	//キャリブレーション回数を書き出し
	m_EditCalibRepeat.GetWindowText( strSetting );
	iSetting	= _wtoi( strSetting );
	pApp->WriteProfileInt( L"Calibration", L"Repeat", iSetting );
	CommandValue.iCalibRepeat = iSetting;

	//補間精度を書き出し
	m_EditAccuracy.GetWindowText( strSetting );
	dSetting	= _wtof( strSetting );
	pApp->WriteProfileString( L"Interpolation", L"Accuracy", strSetting );
	CommandValue.dAccuracy	= dSetting;

	
	//コマンドリロード促し
	AfxMessageBox( L"設定が変更されました。\nコマンドを読み込んでいた場合はリロードしてください。" );
	

	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCalibrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadCalibParam();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CCalibrationDlg::LoadCalibParam(void)
{
	int			iSetting;
	CString		strSetting;
	CWinApp*	pApp = AfxGetApp();

	//コップ直径を読み込み
	iSetting					= pApp->GetProfileInt( L"Calibration", L"Diameter", NULL );	//iniファイルから読み込み
	CommandValue.iCupDiameter	= iSetting;			//共有変数に代入
	strSetting.Format( L"%d\n", iSetting );
	m_EditCupDiameter.SetWindowText( strSetting );
	CommandValue.iCupDiameter = iSetting;
	
	//キャリブレーション回数を読み込み
	iSetting					= pApp->GetProfileInt( L"Calibration", L"Repeat", NULL );
	CommandValue.iCalibRepeat	= iSetting;
	strSetting.Format( L"%d\n", iSetting );
	m_EditCalibRepeat.SetWindowText( strSetting );
	CommandValue.iCalibRepeat = iSetting;

	//補間精度を読み込み
	strSetting					= pApp->GetProfileString( L"Interpolation", L"Accuracy", NULL );
	m_EditAccuracy.SetWindowText( strSetting );
	CommandValue.dAccuracy		= _wtof( strSetting );
}
