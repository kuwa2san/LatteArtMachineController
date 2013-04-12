////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LatteArtCommand.h"
#include "ComThread.h"
#include "ServoInitial.h"

#include <errno.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include "math.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
STRCT_LATTEARTBALUE	CommandValue;

////////////////////////////////////////////////////////////////////////////////////////////////////
// �R�}���h����
// ���s����:0	���s���s:1
////////////////////////////////////////////////////////////////////////////////////////////////////
int LatteArtCommand_G0x( int iCommandNo, double dData1, double dData2 )
{
	BOOL	bRet;
	int		iData1, iData2;
	CString	strLog;

	iData1	= static_cast<int>( dData1 );
	iData2	= static_cast<int>( dData2 );

	switch( iCommandNo )
	{
	case 0:		//�ړ��p�T�[�{�@�g���NON
		{
			bRet = Command_G00( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 1:		//�ړ��p�T�[�{�@�g���NOFF
		{
			bRet = Command_G01( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 2:
		{
			bRet = Command_G02( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 4:
		{
			bRet = Command_G04( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 5:
		{
			bRet = Command_G05( iCommandNo, dData1, dData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 6:
		{
			bRet = Command_G06( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	default:
		{
			break;
		}
	}


	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G1x( int iCommandNo, double dData1, double dData2 )
{
	BOOL	bRet;
	int		iData1, iData2;
	CString	strLog;

	iData1	= static_cast<int>( dData1 );
	iData2	= static_cast<int>( dData2 );

	switch( iCommandNo )
	{
	case 10:		//�ړ��p�T�[�{���w����W�ֈړ�����
		{
			bRet = Command_G10( iCommandNo, dData1, dData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 11:		//Write�p�T�[�{��ON����
		{
			bRet = Command_G11( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 12:		//Write�p�T�[�{��OFF����
		{
			bRet = Command_G12( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 13:
		{
			bRet = Command_G13( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	default:
		{
			break;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G2x( int iCommandNo, int iData1, int iData2 )
{
	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G3x( int iCommandNo, int iData1, int iData2 )
{
	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G4x( int iCommandNo, int iData1, int iData2 )
{
	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G5x( int iCommandNo, int iData1, int iData2 )
{
	BOOL	bRet;

	switch( iCommandNo )
	{
	case 50:		//�ړ��p�T�[�{���w��p�x�ֈړ�����
		{
			bRet = Command_G50( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	default:
		{
			break;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G6x( int iCommandNo, int iData1, int iData2 )
{
	BOOL	bRet;

	switch( iCommandNo )
	{
	case 60:		//�ړ��p�T�[�{���w��p�x�ֈړ�����
		{
			bRet = Command_G60( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	default:
		{
			break;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G7x( int iCommandNo, int iData1, int iData2 )
{
	BOOL	bRet;

	switch( iCommandNo )
	{
	case 70:		//Write�p�T�[�{���w��p�x�ֈړ�����
		{
			bRet = Command_G70( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	default:
		{
			break;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G8x( int iCommandNo, int iData1, int iData2 )
{
	return 0;
}

//----------------------------------------------------------------------------------------------------
int LatteArtCommand_G9x( int iCommandNo, int iData1, int iData2 )
{
	BOOL	bRet;

	switch( iCommandNo )
	{
	case 90:		//�ړ��p�T�[�{�̃g���N��ύX����
		{
			bRet = Command_G90( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 91:		//Write�p�T�[�{�̃g���N��ύX����
		{
			bRet = Command_G91( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	case 92:		//Write�p�T�[�{�̃f�B���C��ύX����
		{
			bRet = Command_G92( iCommandNo, iData1, iData2 );
			if( bRet )
			{
				return 1;
			}
			break;
		}
	default:
		{
			break;
		}
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ���R�}���h�W
////////////////////////////////////////////////////////////////////////////////////////////////////
// �ړ��p�T�[�{�̃g���NON
//----------------------------------------------------------------------------------------------------
int Command_G00( int iCommandNo, int iData1, int iData2 )
{
	BYTE byteSendBuffer[9];
	DWORD	dwWrite;
	BOOL	bRet;

	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x01;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x24;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = 0x01;
	byteSendBuffer[8] = 0x24;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x02;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x24;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = 0x01;
	byteSendBuffer[8] = 0x27;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	Sleep( 20 );

	return 0;
}

// �ړ��p�T�[�{�̃g���NOFF
//----------------------------------------------------------------------------------------------------
int Command_G01( int iCommandNo, int iData1, int iData2 )
{
	BYTE byteSendBuffer[9];
	DWORD	dwWrite;
	BOOL	bRet;

	//�g���NOFF(�T�[�{1)
	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x01;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x24;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = 0x00;
	byteSendBuffer[8] = 0x25;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	//�g���NOFF(�T�[�{2)
	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x02;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x24;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = 0x00;
	byteSendBuffer[8] = 0x26;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	return 0;
}

// Write�p�T�[�{ON���Ɉړ��p�T�[�{�̑��x�ύX
//----------------------------------------------------------------------------------------------------
int Command_G02( int iCommandNo, int iData1, int iData2 )
{
	if( iData1 <= 0 )
	{
		iData1 = 60;
	}
	CommandValue.iDegPerSec = iData1;

	return 0;
}

// �ړ��p�T�[�{�̌��_�w��
//----------------------------------------------------------------------------------------------------
int Command_G04( int iCommandNo, int iData1, int iData2 )
{
	CommandValue.iZeroPoint_X = iData1;
	CommandValue.iZeroPoint_Y = iData2;

	return 0;
}

// �g��k�����̐ݒ�
//----------------------------------------------------------------------------------------------------
int Command_G05( int iCommandNo, double dData1, double dData2 )
{
	if( dData1 > 0 )
	{
		CommandValue.bMultiple = TRUE;
		CommandValue.dMultiple = dData1;
	}
	else
	{
		return 1;
	}

	return 0;
}

// ���`��ԃt���O
//----------------------------------------------------------------------------------------------------
int Command_G06( int iCommandNo, int iData1, int iData2 )
{
	if( iData1 == 0 )
	{
		CommandValue.bInterpolation = FALSE;
	}
	else if( iData1 == 1 )
	{
		CommandValue.bInterpolation = TRUE;
	}
	else
	{
		return 1;
	}

	return 0;
}


// �ړ��p�T�[�{���w����W�ֈړ�����
//----------------------------------------------------------------------------------------------------
int Command_G10( int iCommandNo, double dData1, double dData2 )
{
	BYTE	byteSendBuffer[12];
	DWORD	dwWrite;
	BOOL	bRet;

	int iServo1deg	= 0;
	int iServo2deg	= 0;
	int	iSec1		= 0;
	int	iSec2		= 0;
	int	iDelay		= 0;
	int	iDegPerSecBak = 0;
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteMoveTime1;
	BYTE	byteMoveTime2;
	BYTE	byteSum;

	double dAlpha, dBeta;						//�p�x��,��
	double dTh11, dTh12, dTh21, dTh22;			//��1,��2
	double dDeg11, dDeg12, dDeg21, dDeg22;		//��1��deg,��2��deg
	double dDegree1, dDegree2;
	double dDiffX;
	double dDiffY;
	double dDelay	= 0.0;	//�ړ��ɂ����鎞�Ԃ̃f�B���C

	//�ړ����x���W�Ay���W���Z�o
	double dCupDiameter	= static_cast<double>( CommandValue.iCupDiameter );
	double dCup_x;
	double dCup_y;
	if( CommandValue.bMultiple )
	{
		dCup_x	= ( dData1 ) * CommandValue.dMultiple;
		dCup_y	= ( dData2 ) * CommandValue.dMultiple;
	}
	else
	{
		dCup_x	= ( dData1 ) * ( dCupDiameter / 60.0 );
		dCup_y	= ( dData2 ) * ( dCupDiameter / 60.0 );
	}

	//�ړ��ɂ�����f�B���C���Ԃ��Z�o
	if( !CommandValue.bWrite )
	{
		iDegPerSecBak = CommandValue.iDegPerSec;
		CommandValue.iDegPerSec = 300;				//�ō����x�A���ȁH
	}
	dDiffX						= abs( dCup_x - CommandValue.dNowPoint_X );
	dDiffY						= abs( dCup_y - CommandValue.dNowPoint_Y );
	dDelay						= sqrt( ( dDiffX ) * ( dDiffX ) + ( dDiffY ) * ( dDiffY ) ) / CommandValue.iDegPerSec;
	CommandValue.dNowPoint_X	= dCup_x;
	CommandValue.dNowPoint_Y	= dCup_y;

	double x = dCup_x;
	double y = dCup_y + 90.0;

	//�ړ����x���W�Ay���W����p�x���Z�o
	//���ƃ����Z�o(������ƕ��G)
	dAlpha	= acos( ( -( x*x + y*y ) + CommandValue.dARM_L2*CommandValue.dARM_L2 + CommandValue.dARM_L1*CommandValue.dARM_L1 )
						/ ( 2 * CommandValue.dARM_L1 * CommandValue.dARM_L2 ) );
	dBeta	= acos( ( -( CommandValue.dARM_L2*CommandValue.dARM_L2 ) + CommandValue.dARM_L1*CommandValue.dARM_L1 + ( x*x + y*y ) )
						/ ( 2 * CommandValue.dARM_L1 * sqrt( x*x + y*y ) ) );

	//��1�ƃ�2���Z�o
	dTh11	= atan( y / x ) - dBeta;
	dTh12	= atan( y / x ) + dBeta;
	dTh21	=  M_PI - dAlpha;
	dTh22	= ( -1 ) * ( M_PI - dAlpha );

	dDeg11	= dTh11 * 180.0 / M_PI;
	dDeg12	= dTh12 * 180.0 / M_PI;
	dDeg21	= dTh21 * 180.0 / M_PI;
	dDeg22	= dTh22 * 180.0 / M_PI;

	//����p�x�����肷��
	if( dData1 >= 0 )
	{
		dDegree1 = ServoValue.iServo1Direction * ( dDeg11 - 90.0 );
		dDegree2 = ServoValue.iServo2Direction * dDeg22;
	}
	else
	{
		dDegree1 = ServoValue.iServo1Direction * ( dDeg11 + 90.0 );
		dDegree2 = ServoValue.iServo2Direction * dDeg22;
	}


	//�T�[�{1�̈ړ����Ԃ��Z�o
	iSec1			= static_cast<int>( dDelay * 100.0 );
	if( iSec1 <= 0 )
	{
		iSec1 = 2;	//�T�[�{�ő呬�x�ړ�
		iDelay = 2;
	}
	else
	{
		iDelay = iSec1 * 10;
	}

	//�ړ��p�T�[�{1����J�n
	byteDeg1		= ( static_cast<int>( dDegree1 ) * 10 ) & 0xFF;
	byteDeg2		= ( static_cast<int>( dDegree1 ) * 10 ) >> 8;
	byteMoveTime1	= ( iSec1 ) & 0xFF;
	byteMoveTime2	= ( iSec1 ) >> 8;
	byteSum			= 0x01 ^ 0x00 ^ 0x1E ^ 0x04 ^ 0x01 ^ byteDeg1 ^ byteDeg2 ^ byteMoveTime1 ^ byteMoveTime2;
	byteSendBuffer[0]	= 0xFA;
	byteSendBuffer[1]	= 0xAF;
	byteSendBuffer[2]	= 0x01;
	byteSendBuffer[3]	= 0x00;
	byteSendBuffer[4]	= 0x1E;
	byteSendBuffer[5]	= 0x04;
	byteSendBuffer[6]	= 0x01;
	byteSendBuffer[7]	= byteDeg1;
	byteSendBuffer[8]	= byteDeg2;
	byteSendBuffer[9]	= byteMoveTime1;
	byteSendBuffer[10]	= byteMoveTime2;
	byteSendBuffer[11]	= byteSum;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 12, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	//�ړ��p�T�[�{2����J�n
	byteDeg1		= ( static_cast<int>( dDegree2 ) * 10 ) & 0xFF;
	byteDeg2		= ( static_cast<int>( dDegree2 ) * 10 ) >> 8;
	byteMoveTime1	= ( iSec1 ) & 0xFF;
	byteMoveTime2	= ( iSec1 ) >> 8;
	byteSum			= 0x02 ^ 0x00 ^ 0x1E ^ 0x04 ^ 0x01 ^ byteDeg1 ^ byteDeg2 ^ byteMoveTime1 ^ byteMoveTime2;
	byteSendBuffer[0]	= 0xFA;
	byteSendBuffer[1]	= 0xAF;
	byteSendBuffer[2]	= 0x02;
	byteSendBuffer[3]	= 0x00;
	byteSendBuffer[4]	= 0x1E;
	byteSendBuffer[5]	= 0x04;
	byteSendBuffer[6]	= 0x01;
	byteSendBuffer[7]	= byteDeg1;
	byteSendBuffer[8]	= byteDeg2;
	byteSendBuffer[9]	= byteMoveTime1;
	byteSendBuffer[10]	= byteMoveTime2;
	byteSendBuffer[11]	= byteSum;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 12, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}


	//�ړ��p�T�[�{�����슮������܂ő҂�
	Sleep( iDelay );

	if( !CommandValue.bWrite )
	{
		CommandValue.iDegPerSec = iDegPerSecBak;
	}

	return 0;
}

// Write�T�[�{��WriteON�ʒu�Ɉړ�
//----------------------------------------------------------------------------------------------------
int Command_G11( int iCommandNo, int iData1, int iData2 )
{
	iData1	 = ServoValue.iWriteONdeg;
	BYTE	byteSendBuffer[10];
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	//Write�p�T�[�{ �g���NON
	byteSendBuffer[0]	= 0xFA;
	byteSendBuffer[1]	= 0xAF;
	byteSendBuffer[2]	= 0x03;
	byteSendBuffer[3]	= 0x00;
	byteSendBuffer[4]	= 0x24;
	byteSendBuffer[5]	= 0x01;
	byteSendBuffer[6]	= 0x01;
	byteSendBuffer[7]	= 0x01;
	byteSendBuffer[8]	= byteSendBuffer[2] ^ byteSendBuffer[3] ^ byteSendBuffer[4] ^ byteSendBuffer[5] ^ byteSendBuffer[6] ^ byteSendBuffer[7];
	bRet				= WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	//�g���NON�f�B���C
	Sleep( 5 );

	//WriteON�ʒu�Ɉړ�
	byteDeg1	= ( iData1*10 ) & 0xFF;
	byteDeg2	= ( iData1*10 ) >> 8;
	byteSum		= 0x03 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;
	byteSendBuffer[0]	= 0xFA;
	byteSendBuffer[1]	= 0xAF;
	byteSendBuffer[2]	= 0x03;
	byteSendBuffer[3]	= 0x00;
	byteSendBuffer[4]	= 0x1E;
	byteSendBuffer[5]	= 0x02;
	byteSendBuffer[6]	= 0x01;
	byteSendBuffer[7]	= byteDeg1;
	byteSendBuffer[8]	= byteDeg2;
	byteSendBuffer[9]	= byteSum;
	bRet				= WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	CommandValue.bWrite = TRUE;

	return 0;
}

// Write�T�[�{��WriteOFF�ʒu�Ɉړ�
//----------------------------------------------------------------------------------------------------
int Command_G12( int iCommandNo, int iData1, int iData2 )
{
	iData1	 = ServoValue.iWriteOFFdeg;
	BYTE	byteSendBuffer[10];
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	byteDeg1	= ( iData1*10 ) & 0xFF;
	byteDeg2	= ( iData1*10 ) >> 8;
	byteSum		= 0x03 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;
	byteSendBuffer[0]	= 0xFA;
	byteSendBuffer[1]	= 0xAF;
	byteSendBuffer[2]	= 0x03;
	byteSendBuffer[3]	= 0x00;
	byteSendBuffer[4]	= 0x1E;
	byteSendBuffer[5]	= 0x02;
	byteSendBuffer[6]	= 0x01;
	byteSendBuffer[7]	= byteDeg1;
	byteSendBuffer[8]	= byteDeg2;
	byteSendBuffer[9]	= byteSum;
	bRet				= WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	//Write OFF�̌�͋����I�Ƀf�B���C������
	CommandValue.bWrite = FALSE;
	Sleep( 50 );

	return 0;
}

// �w��b���ҋ@(delay�R�}���h)
//----------------------------------------------------------------------------------------------------
int Command_G13( int iCommandNo, int iData1, int iData2 )
{
	Sleep( iData1 );
	return 0;
}

// �ړ��p�T�[�{1���w��p�x�Ɉړ�
//----------------------------------------------------------------------------------------------------
int Command_G50( int iCommandNo, int iData1, int iData2 )
{
	BYTE	byteSendBuffer[12];
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	byteDeg1	= ( iData1 * 10 ) & 0xFF;
	byteDeg2	= ( iData1 * 10 ) >> 8;
	byteSum		= 0x01 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;
	
	byteSendBuffer[0]	= 0xFA;
	byteSendBuffer[1]	= 0xAF;
	byteSendBuffer[2]	= 0x01;
	byteSendBuffer[3]	= 0x00;
	byteSendBuffer[4]	= 0x1E;
	byteSendBuffer[5]	= 0x02;
	byteSendBuffer[6]	= 0x01;
	byteSendBuffer[7]	= byteDeg1;
	byteSendBuffer[8]	= byteDeg2;
	byteSendBuffer[9]	= byteDeg2;
	byteSendBuffer[10]	= byteDeg2;
	byteSendBuffer[11]	= byteSum;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 12, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	return 0;
}

// �ړ��p�T�[�{2���w��p�x�Ɉړ�
//----------------------------------------------------------------------------------------------------
int Command_G60( int iCommandNo, int iData1, int iData2 )
{
	BYTE	byteSendBuffer[10];
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	byteDeg1	= ( iData1 * 10 ) & 0xFF;
	byteDeg2	= ( iData1 * 10 ) >> 8;
	byteSum		= 0x02 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;

	byteSendBuffer[0]	= 0xFA;
	byteSendBuffer[1]	= 0xAF;
	byteSendBuffer[2]	= 0x02;
	byteSendBuffer[3]	= 0x00;
	byteSendBuffer[4]	= 0x1E;
	byteSendBuffer[5]	= 0x02;
	byteSendBuffer[6]	= 0x01;
	byteSendBuffer[7]	= byteDeg1;
	byteSendBuffer[8]	= byteDeg2;
	byteSendBuffer[9]	= byteSum;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}
	
	return 0;
}

// Write�p�T�[�{���w��p�x�Ɉړ�
//----------------------------------------------------------------------------------------------------
int Command_G70( int iCommandNo, int iData1, int iData2 )
{
	BYTE	byteSendBuffer[10];
	BYTE	byteDeg1;
	BYTE	byteDeg2;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	byteDeg1	= ( iData1 * 10 ) & 0xFF;
	byteDeg2	= ( iData1 * 10 ) >> 8;
	byteSum		= 0x03 ^ 0x00 ^ 0x1E ^ 0x02 ^ 0x01 ^ byteDeg1 ^ byteDeg2;

	byteSendBuffer[0]	= 0xFA;
	byteSendBuffer[1]	= 0xAF;
	byteSendBuffer[2]	= 0x03;
	byteSendBuffer[3]	= 0x00;
	byteSendBuffer[4]	= 0x1E;
	byteSendBuffer[5]	= 0x02;
	byteSendBuffer[6]	= 0x01;
	byteSendBuffer[7]	= byteDeg1;
	byteSendBuffer[8]	= byteDeg2;
	byteSendBuffer[9]	= byteSum;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 10, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}
	
	return 0;
}

// �ړ��p�T�[�{�̃g���N�ύX
//----------------------------------------------------------------------------------------------------
int Command_G90( int iCommandNo, int iData1, int iData2 )
{
	BYTE byteSendBuffer[9];
	BYTE	byteData;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	//�T�[�{1
	byteData	= iData1 & 0xFF;
	byteSum		= 0x01 ^ 0x00 ^ 0x23 ^ 0x01 ^ 0x01 ^ byteData;
	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x01;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x23;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = byteData;
	byteSendBuffer[8] = byteSum;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	//�T�[�{2
	byteData	= iData1 & 0xFF;
	byteSum		= 0x02 ^ 0x00 ^ 0x23 ^ 0x01 ^ 0x01 ^ byteData;
	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x02;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x23;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = byteData;
	byteSendBuffer[8] = byteSum;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	return 0;
}

// Write�p�T�[�{�̃g���N�ύX
//----------------------------------------------------------------------------------------------------
int Command_G91( int iCommandNo, int iData1, int iData2 )
{
	BYTE byteSendBuffer[9];
	BYTE	byteData;
	BYTE	byteSum;
	DWORD	dwWrite;
	BOOL	bRet;

	byteData	= iData1 & 0xFF;
	byteSum		= 0x03 ^ 0x00 ^ 0x23 ^ 0x01 ^ 0x01 ^ byteData;
	byteSendBuffer[0] = 0xFA;
	byteSendBuffer[1] = 0xAF;
	byteSendBuffer[2] = 0x03;
	byteSendBuffer[3] = 0x00;
	byteSendBuffer[4] = 0x23;
	byteSendBuffer[5] = 0x01;
	byteSendBuffer[6] = 0x01;
	byteSendBuffer[7] = byteData;
	byteSendBuffer[8] = byteSum;
	bRet	 = WriteFile( ComThread.hCom, byteSendBuffer, 9, &dwWrite, NULL );
	if( !bRet )
	{
		return 1;
	}

	return 0;
}

// �C���N�ύX����delay�R�}���h
//----------------------------------------------------------------------------------------------------
int Command_G92( int iCommandNo, int iData1, int iData2 )
{
	Sleep( iData1 );

	return 0;
}