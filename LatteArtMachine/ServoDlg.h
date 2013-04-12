////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"
#include "ServoInitial.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CServoDlg ダイアログ
////////////////////////////////////////////////////////////////////////////////////////////////////
class CServoDlg : public CDialog
{
	DECLARE_DYNAMIC(CServoDlg)

public:
	CServoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CServoDlg();

// ダイアログ データ
	enum { IDD = IDD_SERVODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnBnClickedWriteServoid();
	afx_msg void OnBnClickedRadioSelectId();
	afx_msg void OnBnClickedRadioSelectDirection();
private:
	CComboBox m_bOldServoID;
	CComboBox m_bNewServoID;
	int m_Radio_Servo1;
	int m_Radio_Servo2;
	int m_RadioSelect;
	CEdit m_Edit_WriteON;
	CEdit m_Edit_WriteOFF;

	void EnableRewriteServoID(void);
	void DisableRewriteServoID(void);
	void EnableDirection(void);
	void DisableDirection(void);
	BOOL OnInitDialog(void);
};
