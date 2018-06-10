#include "PVCClock.h"

void setType(void * param)
{
  int i = (int)param;
  switch (i)
  {
    case 1:
      //bmpDc = pvcCreateCompatibleDCFromBitmap(bmp1);
    break;
    case 2:
      bmpDc = pvcCreateCompatibleDCFromBitmap(bmp2);
    break;
    case 3:
      bmpDc = pvcCreateCompatibleDCFromBitmap(bmp3);
    break;
    case 4:
      bmpDc = pvcCreateCompatibleDCFromBitmap(bmp4);
    break;
    case 5:
      bmpDc = pvcCreateCompatibleDCFromBitmap(bmp5);
    break;
    default:
    break;
  }
	resetWindow(hwnd, WSS_BACKGROUND);
	pvcInvalidate(hwnd);
}

bool clockProc(PHControl clock, PMessage msg)
{
	switch (msg.type)
	{
	case MSG_PAINT:
	{
		uint t = getTime();
	  PHdc timeDc = &clock->dc;
		memset(timeDc->content, 0x0c, sizeof(PColor) * timeDc->size.cx * timeDc->size.cy);
	  timeDc->brush.color = RGB(255, 0, 0);
	  timeDc->pen.size = 1;
	  timeDc->pen.color = RGB(0, 0, 0);
	  timeDc->font.color = RGB(0, 0, 255);

	  pvcTransparentBit(timeDc, 0, 0, bmpDc, 0, 0, bmpDc->size.cx, bmpDc->size.cy, RGB(0, 255, 0));

	  int O_X = bmpDc->size.cx / 2;
	  int O_Y = bmpDc->size.cy / 2;
	  int hour = (t >> 16) & 0xffff;
	  int minute = (t >> 8) & 0xff;
	  int second = t & 0xff;
	  pvcDrawLine(timeDc, O_X, O_Y, O_X + SECOND_L * cos((double)second * 6 - 90),
	    O_Y + SECOND_L * sin((double)second * 6 - 90));
	  timeDc->pen.size = 3;
	  pvcDrawLine(timeDc, O_X, O_Y, O_X + MINUTE_L * cos((double)minute * 6 + second /10 - 90),
	    O_Y + MINUTE_L * sin((double)minute * 6 + second /10 - 90));
	  pvcDrawLine(timeDc, O_X, O_Y, O_X + HOUR_L * cos((double)(hour % 12) * 30 + minute / 2 - 90),
	    O_Y + HOUR_L * sin((double)(hour % 12) * 30 + minute / 2 - 90));
	}
		break;
	case MSG_TIMEOUT:
		pvcCtrlInvalidate(clock);
		break;
	}
	return pvcCtrlProc(clock, msg);
}

bool wndProc(PHwnd hwnd, PMessage msg)
{
  switch (msg.type)
  {
  case MSG_PAINT:
		memset(hwnd->dc.content, 0x0c, sizeof(PColor) * hwnd->dc.size.cx * hwnd->dc.size.cy);
		memset(hwnd->wholeDc.content, 0x0c, sizeof(PColor) * hwnd->wholeDc.size.cx * hwnd->wholeDc.size.cy);
    break;
  case MSG_CREATE:
    //bmp1 = pvcLoadBitmap("FX_CLOCK_TYPE1.bmp");
    bmp2 = pvcLoadBitmap("FX_CLOCK_TYPE2.bmp");
    bmp3 = pvcLoadBitmap("FX_CLOCK_TYPE3.bmp");
    bmp4 = pvcLoadBitmap("FX_CLOCK_TYPE4.bmp");
    bmp5 = pvcLoadBitmap("FX_CLOCK_TYPE5.bmp");
		setType((void *)5);
		clock = pvcCreateControl(hwnd, 0, 0, 150, 150, 0, clockProc);
		pvcInvalidate(hwnd);
    setTimer(hwnd, 1, 500);
    return FINISH;
  case MSG_RBUTTON_DOWN:
  {
    int x = (msg.param >> 16) & 0xffff;
    int y = msg.param & 0xffff;
    PHMenu menu = pvcCreateMenu(hwnd, true);
    pvcMenuAddItem(menu, "type1", (void *)2, setType);
    pvcMenuAddItem(menu, "type2", (void *)3, setType);
    pvcMenuAddItem(menu, "type3", (void *)4, setType);
    pvcMenuAddItem(menu, "type4", (void *)5, setType);
    pvcShowMenuAt(menu, x, y);
  }
		break;
	case MSG_LBUTTON_DOWN:
		msg.type = MSG_NC_LBUTTON_DOWN;
		break;
	case MSG_LBUTTON_UP:
		msg.type = MSG_NC_LBUTTON_UP;
		resetWindow(hwnd, WSS_BACKGROUND);
		break;
	case MSG_HAS_FOCUS:
	case MSG_LOSE_FOCUS:
		return FINISH;
  default:
    break;
  }
  return pvcWndProc(hwnd, msg);
}

int main()
{
  hwnd = pvcCreateWindow("clock", 0, 100, 100, 150, 150);
  pvcWndExec(hwnd, wndProc);
  exit();
}

void timerUpdate(PHwnd hwnd)
{
  pvcInvalidate(hwnd);
}
