////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ComThread.h"
#include "LatteArtMachine.h"
#include "LatteArtMachineDlg.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
STRCT_THREAD ComThread;

////////////////////////////////////////////////////////////////////////////////////////////////////
void StartThread(LPVOID pParam)
{
	if( ComThread.bRun == false )
	{
		AfxBeginThread( RecieveThread, pParam );
		Sleep( 20 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TerminateThread(void)
{
	if( ComThread.bRun == true )
	{
		::SetEvent( ComThread.hStop );
		if( ::WaitForSingleObject( ComThread.hEnd, 30000 ) == WAIT_TIMEOUT )
			AfxMessageBox( L"Terminate Thread Timeout" );
		::ResetEvent( ComThread.hEnd );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
UINT RecieveThread(LPVOID pParam)
{
	CLatteArtMachineDlg *pInst = (CLatteArtMachineDlg *) pParam;

	DWORD		dwErrors;		//エラー情報
	COMSTAT		ComStat;		//デバイスの状態
	char		pszBuf[256];	//読み出しデータバッファ
	DWORD		dwRead;			//ポートから読み出したバイト数
	CString SampleData;
	ComThread.oldSeek = 0;
	int i = 0;

	ComThread.bRun = true;
	while( ComThread.hCom != INVALID_HANDLE_VALUE )
	{
		if( ::WaitForSingleObject( ComThread.hStop, 0 ) == WAIT_OBJECT_0 )		break;
		if( ClearCommError( ComThread.hCom, &dwErrors, &ComStat ) )
		{
			if( ComStat.cbInQue )
			{
				dwRead = 0;
				if( ReadFile( ComThread.hCom, pszBuf, ComStat.cbInQue, &dwRead, NULL ) )
				{
					CString str( pszBuf, dwRead );
					ComThread.RecieveData.Format( str );
					
					ComThread.NowSeek = ComThread.RecieveData.Find( ',' );
					SampleData = ComThread.RecieveData.Mid( ComThread.oldSeek, ComThread.NowSeek - 1 );
					ComThread.oldSeek = ComThread.NowSeek + 1;
					
					TRACE( "Recieved: %s\r\n", SampleData );
					//pInst->UpdateRecieve();
				}
			}
		}
	}

	::ResetEvent( ComThread.hStop );
	ComThread.bRun = false;
	::SetEvent( ComThread.hEnd );

	return 0;
}
