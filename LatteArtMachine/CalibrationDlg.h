////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"
#include "LatteArtCommand.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CCalibrationDlg ダイアログ
////////////////////////////////////////////////////////////////////////////////////////////////////
class CCalibrationDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CCalibrationDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCalibrationDlg();

// ダイアログ データ
	enum { IDD = IDD_CALIBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSavechanges();
private:
	CEdit m_EditCupDiameter;
	CEdit m_EditCalibRepeat;
	CEdit m_EditAccuracy;
	void LoadCalibParam(void);
	virtual BOOL OnInitDialog();
};
