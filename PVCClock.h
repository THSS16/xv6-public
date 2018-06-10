#ifndef PVCCLOCK_H
#define PVCCLOCK_H

#include "PVC.h"

#define HOUR_L 20
#define MINUTE_L 30
#define SECOND_L 40

//PBitmap bmp1;
PBitmap bmp2;
PBitmap bmp3;
PBitmap bmp4;
PBitmap bmp5;
PHdc bmpDc;

PHControl clock;
PHwnd hwnd;

bool clockProc(PHControl, PMessage);

#endif
