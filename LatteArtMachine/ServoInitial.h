////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _Servo
{
	int		iOldServoID;
	int		iNewServoID;
	int		iServo1Direction;
	int		iServo2Direction;
	int		iWriteONdeg;
	int		iWriteOFFdeg;

} STRCT_SERVOVALUE;

////////////////////////////////////////////////////////////////////////////////////////////////////
extern STRCT_SERVOVALUE	ServoValue;