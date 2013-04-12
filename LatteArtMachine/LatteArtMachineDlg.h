////////////////////////////////////////////////////////////////////////////////////////////////////
// LatteArtMachineDlg.h : ヘッダー ファイル
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"
#include "ConnectDlg.h"
#include "ConnectInitial.h"
#include "ServoDlg.h"
#include "ServoInitial.h"
#include "LatteArtCommand.h"
#include "CalibrationDlg.h"
#include <fstream>

#define SCRIPT_COLUMN	65535	//行(縦)
#define SCRIPT_COLUMN2	16383	//行2(縦)	strTmpScriptも65535行確保させたらstack over flowした・・
#define SCRIPT_ROW		3		//列(縦)

////////////////////////////////////////////////////////////////////////////////////////////////////
// CLatteArtMachineDlg ダイアログ
////////////////////////////////////////////////////////////////////////////////////////////////////
class CLatteArtMachineDlg : public CDialog
{
// コンストラクション
public:
	CLatteArtMachineDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_LATTEARTMACHINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedTrqueOn();
	afx_msg void OnBnClickedTurn90deg();
	afx_msg void OnBnClickedReturn90deg();
	afx_msg void OnBnClickedCheckServo1();
	afx_msg void OnBnClickedCheckServo2();
	afx_msg void OnBnClickedCheckServo3();
	afx_msg void OnBnClickedTrqueOff();
	afx_msg void OnBnClickedDirectionDeg();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExit();
	afx_msg void OnSettingConnect();
	afx_msg void OnBnClickedLoadcsv();
	afx_msg void OnBnClickedStartLatteart();
	afx_msg void OnBnClickedButtonWriteOn();
	afx_msg void OnBnClickedButtonWriteOff();
	afx_msg void OnSettingServo();
	afx_msg void OnSettingCalibrate();
	afx_msg void OnBnClickedStartCalibration();
	afx_msg void OnBnClickedClearLog();
	afx_msg LRESULT UpdateImage(WPARAM wDummy, LPARAM Ldummy);
private:
	int iScriptLineCount;
	int iCount;
	int iDrawCount;
	double dX_old;
	double dY_old;

	BOOL bConnect;
	BOOL bServo1;
	BOOL bServo2;
	BOOL bServo3;
	BOOL bCreateSettingDlg;
	BOOL bWrite;
	BOOL bReadCsv;
	BOOL bCommandThread;
	BOOL bEscape;
	BOOL DestroyWindow(void);
	BOOL StartLatteArt(void);
	BOOL InitializationScript(void);

	CEdit m_EditServo1;
	CEdit m_EditServo2;
	CEdit m_Edit_ScriptLog;

	CButton m_chkServo1;
	CButton m_chkServo2;
	CButton m_chkServo3;

	CString strScript[SCRIPT_COLUMN2][SCRIPT_ROW];
	CString strTmpScript[SCRIPT_COLUMN2][SCRIPT_ROW];
	CPoint pntDraw[SCRIPT_COLUMN2];

	CConnectDlg* mDlg_Connect;
	CServoDlg* mDlg_Servo;
	CCalibrationDlg* mDlg_Calibrate;
	CStatic m_bCsvPath;
	CStatic mxc_PreViewArea;
	CDC* m_pDC_PreViewArea;

	int GetLineCount(CString strFilePath);
	void AddStringComPort(void);
	void LinearInterpolation(double dX1, double dY1, double dX2, double dY2);
	void LoadConnectInitial(void);	
	void LinearInterpolation2(double dX1, double dY1, double dX2, double dY2);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//----------------------------------------------------------------------------------------------------
	enum { WM_UPDATE_IMAGE = WM_USER + 2000 };
	enum { WM_LATTE_COMMAND = WM_USER + 2100 };
	enum EVENTCODE		{ DRAW_EVENT_STOP = 0, DRAW_EVENT_EXEC, DRAW_EVENT_MAX };
	enum EVENTCOMMAND	{ COMMAND_EVENT_STOP = 0, COMMAND_EVENT_BREAK, COMMAND_EVENT_EXEC, COMMAND_EVENT_MAX };
	HANDLE m_hDrawEvent[DRAW_EVENT_MAX];
	HANDLE m_hCommandEvent[COMMAND_EVENT_MAX];
	CWinThread* m_pDrawThread;
	CWinThread* m_pCommandThread;
	CWnd* m_hParentWnd;
	UINT m_DrawMessage;
	UINT m_CommandMessage;
	void DrawThreadStart(CWnd* hWnd, UINT message);
	void DrawThreadTerminate(void);
	static UINT DrawThreadEntry(LPVOID pParam);
	UINT DrawThreadProc(void);
	void CommandThreadStart(CWnd* hWnd, UINT message);
	void CommandThreadTerminate(void);
	static UINT CommandThreadEntry(LPVOID pParam);
	UINT CommandThreadProc(void);
};
