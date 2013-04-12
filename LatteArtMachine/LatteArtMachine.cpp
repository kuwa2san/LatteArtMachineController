////////////////////////////////////////////////////////////////////////////////////////////////////
// LatteArtMachine.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LatteArtMachine.h"
#include "LatteArtMachineDlg.h"
#include "shlwapi.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
// CLatteArtMachineApp
////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLatteArtMachineApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////
// CLatteArtMachineApp �R���X�g���N�V����
////////////////////////////////////////////////////////////////////////////////////////////////////
CLatteArtMachineApp::CLatteArtMachineApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// �B��� CLatteArtMachineApp �I�u�W�F�N�g�ł��B
////////////////////////////////////////////////////////////////////////////////////////////////////
CLatteArtMachineApp theApp;

////////////////////////////////////////////////////////////////////////////////////////////////////
// CLatteArtMachineApp ������
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLatteArtMachineApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	//SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));		//������R�����g�A�E�g���Ȃ��ƃ��W�X�g���ɏ������܂��


	//�ȉ��ǉ�
	//ini�t�@�C���̃p�X��ύX����
	if( m_pszProfileName )
	{
		delete ( ( void* )m_pszProfileName );
	}
	TCHAR	szModulePathName[MAX_PATH];
	::GetModuleFileName( AfxGetInstanceHandle(), szModulePathName, MAX_PATH );
	TCHAR	szIniPathName[_MAX_PATH];
	TCHAR	drive[_MAX_PATH];
	TCHAR	dir[_MAX_DIR];
	TCHAR	fname[_MAX_FNAME];
	TCHAR	ext[_MAX_EXT];
	
	_tsplitpath_s( szModulePathName, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );
	_tmakepath_s( szIniPathName, _MAX_PATH, drive, dir, fname, L"ini" );
	m_pszProfileName = _tcsdup( szIniPathName );

	if( !m_pszProfileName )
	{
		AfxMessageBox( L"�������s���G���[�ł�" );
		return FALSE;
	}

	//�ǉ��ȏ�


	CLatteArtMachineDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <�L�����Z��> �ŏ����ꂽ���̃R�[�h��
		//  �L�q���Ă��������B
	}


	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	//  �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
