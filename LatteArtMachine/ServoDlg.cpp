////////////////////////////////////////////////////////////////////////////////////////////////////
// ServoDlg.cpp : �����t�@�C��
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LatteArtMachine.h"
#include "ServoDlg.h"
#include "ComThread.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// CServoDlg �_�C�A���O
////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CServoDlg, CDialog)

CServoDlg::CServoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServoDlg::IDD, pParent)
	, m_Radio_Servo1(0)
	, m_Radio_Servo2(0)
	, m_RadioSelect(0)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
CServoDlg::~CServoDlg()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OLDSERVOID, m_bOldServoID);
	DDX_Control(pDX, IDC_COMBO_NEWSERVOID, m_bNewServoID);
	DDX_Radio(pDX, IDC_RADIO_SERVO1_TURN, m_Radio_Servo1);
	DDX_Radio(pDX, IDC_RADIO_SERVO2_TURN, m_Radio_Servo2);
	DDX_Radio(pDX, IDC_RADIO_SELECT_ID, m_RadioSelect);
	DDX_Control(pDX, IDC_EDIT_WRITEON, m_Edit_WriteON);
	DDX_Control(pDX, IDC_EDIT_WRITEOFF, m_Edit_WriteOFF);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CServoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CServoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CServoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_WRITE_SERVOID, &CServoDlg::OnBnClickedWriteServoid)
	ON_BN_CLICKED(IDC_RADIO_SELECT_ID, &CServoDlg::OnBnClickedRadioSelectId)
	ON_BN_CLICKED(IDC_RADIO_SELECT_DIRECTION, &CServoDlg::OnBnClickedRadioSelectDirection)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////
// CServoDlg ���b�Z�[�W �n���h��
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CServoDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	m_bOldServoID.AddString( L"1" );
	m_bOldServoID.AddString( L"2" );
	m_bOldServoID.AddString( L"3" );
	m_bOldServoID.SetCurSel( 0 );

	m_bNewServoID.AddString( L"1" );
	m_bNewServoID.AddString( L"2" );
	m_bNewServoID.AddString( L"3" );
	m_bNewServoID.SetCurSel( 0 );

	//�T�[�{ID���������𖳌���
	DisableRewriteServoID();

	//�T�[�{���t�]��L����
	EnableDirection();

	//���W�I�{�^�����T�[�{���t�]�̕��ɑI��
	CheckRadioButton( IDC_RADIO_SELECT_ID, IDC_RADIO_SELECT_DIRECTION, IDC_RADIO_SELECT_DIRECTION );


	//�T�[�{���t�]�̃��W�I�{�^����I��
	//�T�[�{1
	if( ServoValue.iServo1Direction >= 1 )
	{
		CheckRadioButton( IDC_RADIO_SERVO1_TURN, IDC_RADIO_SERVO1_RETURN, IDC_RADIO_SERVO1_TURN );
	}
	else
	{
		CheckRadioButton( IDC_RADIO_SERVO1_TURN, IDC_RADIO_SERVO1_RETURN, IDC_RADIO_SERVO1_RETURN );
	}

	//�T�[�{2
	if( ServoValue.iServo2Direction >= 1 )
	{
		CheckRadioButton( IDC_RADIO_SERVO2_TURN, IDC_RADIO_SERVO2_RETURN, IDC_RADIO_SERVO2_TURN );
	}
	else
	{
		CheckRadioButton( IDC_RADIO_SERVO2_TURN, IDC_RADIO_SERVO2_RETURN, IDC_RADIO_SERVO2_RETURN );
	}


	//Write�T�[�{�̃G�f�B�b�g�{�b�N�X�ɒl��ݒ�
	CString		strSetting;
	strSetting.Format( L"%d", ServoValue.iWriteONdeg );
	m_Edit_WriteON.SetWindowText( strSetting );
	strSetting.Format( L"%d", ServoValue.iWriteOFFdeg );
	m_Edit_WriteOFF.SetWindowText( strSetting );

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::OnBnClickedOk()
{
	//OnOK();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::OnBnClickedCancel()
{
	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::OnBnClickedWriteServoid()
{
	CWinApp*	pApp = AfxGetApp();
	CButton*	RadioSelect	= (CButton*)GetDlgItem(IDC_RADIO_SELECT_ID);
	CString		strSetting;
	int			iSetting	= 0;
	int			iSelect		= 0;

	if( RadioSelect->GetCheck() )
	{
		iSelect	= 1;
	}
	else
	{
		iSelect	= 2;
	}


	switch( iSelect )
	{
	case 1:		//�T�[�{ID��������
		{
			BYTE	byteSendBuffer[9];
			BYTE	byteOldServoID;
			BYTE	byteNewServoID;
			DWORD	dwWrite;
			BOOL	bRet;

			ServoValue.iOldServoID	= m_bOldServoID.GetCurSel() + 1;
			ServoValue.iNewServoID	= m_bNewServoID.GetCurSel() + 1;
			byteOldServoID			= ServoValue.iOldServoID;
			byteNewServoID			= ServoValue.iNewServoID;

			//�T�[�{ID��������
			byteSendBuffer[0]	= 0xFA;
			byteSendBuffer[1]	= 0xAF;
			byteSendBuffer[2]	= byteOldServoID;
			byteSendBuffer[3]	= 0x00;
			byteSendBuffer[4]	= 0x04;
			byteSendBuffer[5]	= 0x01;
			byteSendBuffer[6]	= 0x01;
			byteSendBuffer[7]	= byteNewServoID;
			byteSendBuffer[8]	= byteSendBuffer[2] ^ byteSendBuffer[3] ^ byteSendBuffer[4] ^ byteSendBuffer[5] ^ byteSendBuffer[6] ^ byteSendBuffer[7];
			bRet				= WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
			if( !bRet )
			{
				AfxMessageBox( L"�T�[�{ID�̏��������Ɏ��s���܂���" );
				return;
			}

			//�t���b�V��ROM�֏�������
			byteSendBuffer[0]	= 0xFA;
			byteSendBuffer[1]	= 0xAF;
			byteSendBuffer[2]	= byteNewServoID;
			byteSendBuffer[3]	= 0x40;
			byteSendBuffer[4]	= 0xFF;
			byteSendBuffer[5]	= 0x00;
			byteSendBuffer[6]	= 0x00;
			byteSendBuffer[7]	= byteSendBuffer[2] ^ byteSendBuffer[3] ^ byteSendBuffer[4] ^ byteSendBuffer[5] ^ byteSendBuffer[6];
			bRet				= WriteFile( ComThread.hCom, byteSendBuffer, 8, &dwWrite, NULL );
			if( !bRet )
			{
				AfxMessageBox( L"�T�[�{ID�̏��������Ɏ��s���܂���" );
				return;
			}

			//�T�[�{�ċN��
			byteSendBuffer[0]	= 0xFA;
			byteSendBuffer[1]	= 0xAF;
			byteSendBuffer[2]	= byteNewServoID;
			byteSendBuffer[3]	= 0x20;
			byteSendBuffer[4]	= 0xFF;
			byteSendBuffer[5]	= 0x00;
			byteSendBuffer[6]	= 0x00;
			byteSendBuffer[7]	= byteSendBuffer[2] ^ byteSendBuffer[3] ^ byteSendBuffer[4] ^ byteSendBuffer[5] ^ byteSendBuffer[6];
			bRet				= WriteFile( ComThread.hCom, byteSendBuffer, 8, &dwWrite, NULL );
			if( !bRet )
			{
				AfxMessageBox( L"�T�[�{ID�̏��������Ɏ��s���܂���" );
				return;
			}

			break;
		}
	case 2:		//�T�[�{���t�]
		{
			CButton*	RadioDirection1	= (CButton*)GetDlgItem(IDC_RADIO_SERVO1_TURN);
			CButton*	RadioDirection2	= (CButton*)GetDlgItem(IDC_RADIO_SERVO2_TURN);
			int			iDirection		= 0;

			//�T�[�{1
			iDirection = RadioDirection1->GetCheck();
			if( iDirection )
			{
				iSetting = 1;
			}
			else
			{
				iSetting = -1;
			}
			pApp->WriteProfileInt( L"ServoDirection", L"Servo1", iSetting );
			ServoValue.iServo1Direction = iSetting;

			//�T�[�{2
			iDirection = RadioDirection2->GetCheck();
			if( iDirection )
			{
				iSetting = 1;
			}
			else
			{
				iSetting = -1;
			}
			pApp->WriteProfileInt( L"ServoDirection", L"Servo2", iSetting );
			ServoValue.iServo2Direction = iSetting;

			break;
		}
	}

	//Write�T�[�{�ݒ��ۑ�
	m_Edit_WriteON.GetWindowText( strSetting );
	iSetting	= _wtoi( strSetting );
	pApp->WriteProfileInt( L"WriteServo", L"WriteON", iSetting );
	ServoValue.iWriteONdeg = iSetting;

	m_Edit_WriteOFF.GetWindowText( strSetting );
	iSetting	= _wtoi( strSetting );
	pApp->WriteProfileInt( L"WriteServo", L"WriteOFF", iSetting );
	ServoValue.iWriteOFFdeg = iSetting;


	OnCancel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::OnBnClickedRadioSelectId()
{
	UpdateData();

	//�T�[�{ID����������L����
	EnableRewriteServoID();

	//�T�[�{���t�]�𖳌���
	DisableDirection();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::OnBnClickedRadioSelectDirection()
{
	UpdateData();

	//�T�[�{ID���������𖳌���
	DisableRewriteServoID();

	//�T�[�{���t�]��L����
	EnableDirection();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::EnableRewriteServoID(void)
{
	//�T�[�{ID����������L����
	CComboBox*		ComboOldID	= (CComboBox*)GetDlgItem( IDC_COMBO_OLDSERVOID );
	CComboBox*		ComboNewID	= (CComboBox*)GetDlgItem( IDC_COMBO_NEWSERVOID );

	ComboOldID->EnableWindow( TRUE );
	ComboNewID->EnableWindow( TRUE );

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::DisableRewriteServoID(void)
{
	//�T�[�{ID���������𖳌���
	CComboBox*		ComboOldID	= (CComboBox*)GetDlgItem( IDC_COMBO_OLDSERVOID );
	CComboBox*		ComboNewID	= (CComboBox*)GetDlgItem( IDC_COMBO_NEWSERVOID );

	ComboOldID->EnableWindow( FALSE );
	ComboNewID->EnableWindow( FALSE );

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::EnableDirection(void)
{
	//�T�[�{���t�]��L����
	CButton*		RadioServo1_Turn	= (CButton*)GetDlgItem( IDC_RADIO_SERVO1_TURN );
	CButton*		RadioServo1_Return	= (CButton*)GetDlgItem( IDC_RADIO_SERVO1_RETURN );
	CButton*		RadioServo2_Turn	= (CButton*)GetDlgItem( IDC_RADIO_SERVO2_TURN );
	CButton*		RadioServo2_Return	= (CButton*)GetDlgItem( IDC_RADIO_SERVO2_RETURN );

	RadioServo1_Turn->EnableWindow( TRUE );
	RadioServo1_Return->EnableWindow( TRUE );
	RadioServo2_Turn->EnableWindow( TRUE );
	RadioServo2_Return->EnableWindow( TRUE );

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CServoDlg::DisableDirection(void)
{
	//�T�[�{���t�]�𖳌���
	CButton*		RadioServo1_Turn	= (CButton*)GetDlgItem( IDC_RADIO_SERVO1_TURN );
	CButton*		RadioServo1_Return	= (CButton*)GetDlgItem( IDC_RADIO_SERVO1_RETURN );
	CButton*		RadioServo2_Turn	= (CButton*)GetDlgItem( IDC_RADIO_SERVO2_TURN );
	CButton*		RadioServo2_Return	= (CButton*)GetDlgItem( IDC_RADIO_SERVO2_RETURN );

	RadioServo1_Turn->EnableWindow( FALSE );
	RadioServo1_Return->EnableWindow( FALSE );
	RadioServo2_Turn->EnableWindow( FALSE );
	RadioServo2_Return->EnableWindow( FALSE );

	return;
}
