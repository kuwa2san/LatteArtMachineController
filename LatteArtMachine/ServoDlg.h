////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"
#include "ServoInitial.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CServoDlg �_�C�A���O
////////////////////////////////////////////////////////////////////////////////////////////////////
class CServoDlg : public CDialog
{
	DECLARE_DYNAMIC(CServoDlg)

public:
	CServoDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CServoDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SERVODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
