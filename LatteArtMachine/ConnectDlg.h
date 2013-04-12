////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"
#include "ConnectInitial.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CConnectDlg ダイアログ
////////////////////////////////////////////////////////////////////////////////////////////////////
class CConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CConnectDlg();

// ダイアログ データ
	enum { IDD = IDD_CONNECTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSavechanges();
private:
	CComboBox m_bComPortSet;
	CComboBox m_bBaudRateSet;
	CComboBox m_bDataBitSet;
	CComboBox m_bStopBitSet;
	void AddStringComPort(void);
	void LoadConnectInitial(void);
};
