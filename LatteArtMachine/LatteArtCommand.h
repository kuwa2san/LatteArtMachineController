////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef	struct	_LatteArtValue
{
	int		iDegPerSec;
	int		iZeroPoint_X;
	int		iZeroPoint_Y;
	int		iCupDiameter;
	int		iCalibRepeat;

	double	dARM_L1;
	double	dARM_L2;
	double	dNowPoint_X;
	double	dNowPoint_Y;
	double	dMultiple;		//ägëÂèkè¨ó¶
	double	dAccuracy;		//ï‚ä‘ê∏ìx

	BOOL	bWrite;
	BOOL	bMultiple;
	BOOL	bInterpolation;

}	STRCT_LATTEARTBALUE;

extern	STRCT_LATTEARTBALUE	CommandValue;
////////////////////////////////////////////////////////////////////////////////////////////////////
int LatteArtCommand_G0x( int iCommandNo, double dData1, double dData2 );
int LatteArtCommand_G1x( int iCommandNo, double dData1, double dData2 );
int LatteArtCommand_G2x( int iCommandNo, int iData1, int iData2 );
int LatteArtCommand_G3x( int iCommandNo, int iData1, int iData2 );
int LatteArtCommand_G4x( int iCommandNo, int iData1, int iData2 );
int LatteArtCommand_G5x( int iCommandNo, int iData1, int iData2 );
int LatteArtCommand_G6x( int iCommandNo, int iData1, int iData2 );
int LatteArtCommand_G7x( int iCommandNo, int iData1, int iData2 );
int LatteArtCommand_G8x( int iCommandNo, int iData1, int iData2 );
int LatteArtCommand_G9x( int iCommandNo, int iData1, int iData2 );
int Command_G00( int iCommandNo, int iData1, int iData2 );
int Command_G01( int iCommandNo, int iData1, int iData2 );
int Command_G02( int iCommandNo, int iData1, int iData2 );
int Command_G04( int iCommandNo, int iData1, int iData2 );
int Command_G05( int iCommandNo, double dData1, double dData2 );
int Command_G06( int iCommandNo, int iData1, int iData2 );
int Command_G10( int iCommandNo, double dData1, double dData2 );
int Command_G11( int iCommandNo, int iData1, int iData2 );
int Command_G12( int iCommandNo, int iData1, int iData2 );
int Command_G13( int iCommandNo, int iData1, int iData2 );
int Command_G50( int iCommandNo, int iData1, int iData2 );
int Command_G60( int iCommandNo, int iData1, int iData2 );
int Command_G70( int iCommandNo, int iData1, int iData2 );
int Command_G90( int iCommandNo, int iData1, int iData2 );
int Command_G91( int iCommandNo, int iData1, int iData2 );
int Command_G92( int iCommandNo, int iData1, int iData2 );