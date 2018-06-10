#include "PVCPaintBoard.h"

bool wndProc(PHwnd hwnd, PMessage msg)
{
  switch (msg.type)
  {
  case MSG_PAINT:
    if (msg.param == WPT_WHOLEDC)
    {
      memset(hwnd->dc.content, 0x0c,
             sizeof(PColor) * hwnd->dc.size.cx * hwnd->dc.size.cy);
    }
    else
      draw(hwnd);
    break;
  case MSG_DESTROY:
    if (MB_ACCEPT == pvcMessageBox(hwnd, "tips", "Do you want to exit?"))
      break;
    else
      return FINISH;
    break;
  case MSG_CREATE:
    init(hwnd);
    case MSG_SLIDER_POS_CHANGE:
    {
      int id = (msg.param >> 16) & 0xffff;
      int pos = msg.param & 0xffff;
      if (id == 0)
      {
        sliSt.color.r = pos;
      }
      if (id == 1)
      {
        sliSt.color.g = pos;
      }
      if (id == 2)
      {
        sliSt.color.b = pos;
      }
      if (id == 3)
      {
        sliSt.size = pos;
      }
      if (painter.category == 5)
      {
        painter.eraserColor.r = sliSt.color.r;
        painter.eraserColor.g = sliSt.color.g;
        painter.eraserColor.b = sliSt.color.b;
        painter.size = sliSt.size;
      }
      else
      {
        painter.penColor.r = sliSt.color.r;
        painter.penColor.g = sliSt.color.g;
        painter.penColor.b = sliSt.color.b;
        painter.size = sliSt.size;
      }
      pvcCtrlInvalidate(control1);
    }
    break;
  default:
    break;
  }
  return pvcWndProc(hwnd, msg);
}

int main()
{
  PHwnd hwnd = pvcCreateWindow("PaintBoard", 0, 50, 50, WND_WIDTH, WND_HEIGHT);
  pvcWndExec(hwnd, wndProc);
  exit();
}

void pvcPaintGroupButton(GroupButtonData * data, PHdc hdc)
{
  hdc->pen.size = 1;
  hdc->pen.color = COLOR_NULL;
  hdc->brush.color = RGB(245, 235, 245);
  pvcDrawRect(hdc, 0, 0, hdc->size.cx, hdc->size.cy);
  hdc->pen.size = 1;
  hdc->pen.color = RGB(10, 10, 10);
  hdc->brush.color = COLOR_NULL;
  pvcDrawRect(hdc, 5, 5, 20, 10);
  pvcDrawEllipse(hdc, 5, 25, 20, 10);
  pvcDrawLine(hdc, 5, 45, 25, 58);
  pvcDrawLine(hdc, 9, 63, 9, 77);
  pvcDrawLine(hdc, 9, 77, 18, 77);
  pvcDrawLine(hdc, 18, 77, 18, 68);
  pvcDrawLine(hdc, 18, 68, 25, 72);
  pvcDrawLine(hdc, 25, 72, 18, 63);
  pvcDrawLine(hdc, 18, 63, 9, 63);
  hdc->brush.color = RGB(10, 10, 10);
  pvcDrawRect(hdc, 5, 84, 15, 10);
  pvcDrawLine(hdc, 20, 84, 23, 89);
  pvcDrawLine(hdc, 20, 94, 23, 89);
  hdc->brush.color = RGB(235, 235, 235);
  pvcDrawRect(hdc, 8, 102, 8, 15);
  hdc->brush.color = COLOR_NULL;
  if (data->now >= 0)
  {
    for (int j = data->now * 20; j < data->now * 20 + 20; j++)
    {
      for (int i = 3; i < 27; i++)
      {
        PColor color = pvcPixel(hdc, i, j);
        color.r -= 50;
        color.g -= 50;
        color.b -= 50;
        pvcSetPixel(hdc, i, j, color);
      }
    }
  }
}
bool pvcGroupButtonProc(PHControl btn, PMessage msg)
{
  GroupButtonData * data = (GroupButtonData *)btn->data;

  switch (msg.type)
  {
  case MSG_PAINT:
    pvcPaintGroupButton(data, &(btn->dc));
    break;
  case MSG_LBUTTON_DOWN:
  {
    //int x = (msg.param >> 16) & 0xffff;
    int y = msg.param & 0xffff;
    data->now = y / 20;
    painter.category = data->now;
    pvcCtrlInvalidate(btn);
    return FINISH;
  }
  case MSG_LBUTTON_UP:
    return FINISH;
  default:
    break;
  }
  return pvcCtrlProc(btn, msg);
}

void pvcPaintHdc(PHControl ctrl)
{
  PHdc hdc = &ctrl->dc;
  hdc->pen.size = painter.size;
  hdc->pen.color = painter.penColor;
  hdc->brush.color = COLOR_NULL;
  if (painter.category == 0)
  {
    copyDC(hdc, painter.originalHdc, painter.first_pos.x, painter.first_pos.y,
      painter.final_pos.x, painter.final_pos.y);
    pvcDrawRect(hdc, MIN(painter.first_pos.x, painter.pos.x), MIN(painter.first_pos.y, painter.pos.y),
                       ABS(painter.pos.x - painter.first_pos.x),
                       ABS(painter.pos.y - painter.first_pos.y));
    int x1 = MIN(MIN(painter.first_pos.x, painter.pos.x), painter.final_pos.x);
    int x2 = MAX(MAX(painter.first_pos.x, painter.pos.x), painter.final_pos.x);
    int y1 = MIN(MIN(painter.first_pos.y, painter.pos.y), painter.final_pos.y);
    int y2 = MAX(MAX(painter.first_pos.y, painter.pos.y), painter.final_pos.y);
    paintCtrl(ctrl, x1, y1, x2, y2);
    return;
  }
  if (painter.category == 1)
  {
    copyDC(hdc, painter.originalHdc, painter.first_pos.x, painter.first_pos.y,
      painter.final_pos.x, painter.final_pos.y);
    pvcDrawEllipse(hdc, MIN(painter.first_pos.x, painter.pos.x), MIN(painter.first_pos.y, painter.pos.y),
                       ABS(painter.pos.x - painter.first_pos.x),
                       ABS(painter.pos.y - painter.first_pos.y));

    int x1 = MIN(MIN(painter.first_pos.x, painter.pos.x), painter.final_pos.x);
    int x2 = MAX(MAX(painter.first_pos.x, painter.pos.x), painter.final_pos.x);
    int y1 = MIN(MIN(painter.first_pos.y, painter.pos.y), painter.final_pos.y);
    int y2 = MAX(MAX(painter.first_pos.y, painter.pos.y), painter.final_pos.y);
    paintCtrl(ctrl, x1, y1, x2, y2);
    return;
  }
  if (painter.category == 2)
  {
    copyDC(hdc, painter.originalHdc, painter.first_pos.x, painter.first_pos.y,
      painter.final_pos.x, painter.final_pos.y);
    pvcDrawLine(hdc, painter.first_pos.x, painter.first_pos.y,
                       painter.pos.x, painter.pos.y);

    int x1 = MIN(MIN(painter.first_pos.x, painter.pos.x), painter.final_pos.x);
    int x2 = MAX(MAX(painter.first_pos.x, painter.pos.x), painter.final_pos.x);
    int y1 = MIN(MIN(painter.first_pos.y, painter.pos.y), painter.final_pos.y);
    int y2 = MAX(MAX(painter.first_pos.y, painter.pos.y), painter.final_pos.y);
    paintCtrl(ctrl, x1, y1, x2, y2);
    return;
  }
  if (painter.category == 3)
  {
    PColor targetColor = painter.penColor;
    PColor color = pvcPixel(hdc, painter.first_pos.x, painter.first_pos.y);
    if (targetColor.r == color.r &&
        targetColor.g == color.g &&
        targetColor.b == color.b) return;
    PPoint point;
    point.x = painter.first_pos.x;
    point.y = painter.first_pos.y;
    pvcSetPixel(hdc, point.x, point.y, targetColor);
    PPoint * queue = malloc(sizeof(PPoint) * PAINT_HDC_SIZE_X * PAINT_HDC_SIZE_Y);
    int head, tail;
    head = tail = -1;
    tail++;
    queue[tail] = point;
    while (head < tail)
    {
      head++;
      if (head == tail) printf(1, "4");
      int pos_x = queue[head].x;
      int pos_y = queue[head].y;
      if (pos_x < hdc->size.cx && pos_y < hdc->size.cy && pos_x > 0 && pos_y > 0)
      {
        if (pvcPixel(hdc, pos_x + 1, pos_y).r == color.r &&
            pvcPixel(hdc, pos_x + 1, pos_y).g == color.g &&
            pvcPixel(hdc, pos_x + 1, pos_y).b == color.b &&
            (pos_x < hdc->size.cx))
        {
          PPoint newPoint;
          newPoint.x = pos_x + 1;
          newPoint.y = pos_y;
          tail++;
          queue[tail] = newPoint;
          pvcSetPixel(hdc, newPoint.x, newPoint.y, targetColor);
        }
        if (pvcPixel(hdc, pos_x - 1, pos_y).r == color.r &&
            pvcPixel(hdc, pos_x - 1, pos_y).g == color.g &&
            pvcPixel(hdc, pos_x - 1, pos_y).b == color.b &&
            (pos_x > 0))
        {
          PPoint newPoint;
          newPoint.x = pos_x - 1;
          newPoint.y = pos_y;
          tail++;
          queue[tail] = newPoint;
          pvcSetPixel(hdc, newPoint.x, newPoint.y, targetColor);
        }
        if (pvcPixel(hdc, pos_x, pos_y + 1).r == color.r &&
            pvcPixel(hdc, pos_x, pos_y + 1).g == color.g &&
            pvcPixel(hdc, pos_x, pos_y + 1).b == color.b &&
            (pos_y < hdc->size.cy))
        {
          PPoint newPoint;
          newPoint.x = pos_x;
          newPoint.y = pos_y + 1;
          tail++;
          queue[tail] = newPoint;
          pvcSetPixel(hdc, newPoint.x, newPoint.y, targetColor);
        }
        if (pvcPixel(hdc, pos_x, pos_y - 1).r == color.r &&
            pvcPixel(hdc, pos_x, pos_y - 1).g == color.g &&
            pvcPixel(hdc, pos_x, pos_y - 1).b == color.b &&
            (pos_y > 0))
        {
          PPoint newPoint;
          newPoint.x = pos_x;
          newPoint.y = pos_y - 1;
          tail++;
          queue[tail] = newPoint;
          pvcSetPixel(hdc, newPoint.x, newPoint.y, targetColor);
        }
      }
    }
    free(queue);
  }
  if (painter.category == 4)
  {
    pvcDrawLine(hdc, painter.first_pos.x, painter.first_pos.y,
                       painter.pos.x, painter.pos.y);
    paintCtrl(ctrl, painter.first_pos.x, painter.first_pos.y, painter.pos.x, painter.pos.y);
    painter.first_pos.x = painter.pos.x;
    painter.first_pos.y = painter.pos.y;
    return;
  }
  if (painter.category == 5)
  {
    hdc->pen.size = painter.size;
    hdc->pen.color = painter.eraserColor;
    pvcDrawLine(hdc, painter.first_pos.x, painter.first_pos.y,
                       painter.pos.x, painter.pos.y);
    paintCtrl(ctrl, painter.first_pos.x, painter.first_pos.y, painter.pos.x, painter.pos.y);
    painter.first_pos.x = painter.pos.x;
    painter.first_pos.y = painter.pos.y;
    return;
  }
  pvcPaintControl(ctrl);
}

void undo(void *data)
{
  PHControl btn = (PHControl)data;
  if (painter.head != painter.tail && (painter.current) % 10 != painter.head % 10)
  {
    painter.current = (painter.current + 9 ) % 10;
    pvcBltbit(&(btn->dc), 0, 0, painter.hdcQueue[painter.current], 0,
              0, btn->dc.size.cx, btn->dc.size.cy);
    printf(1,"%d",painter.tail);
    printf(1,"%d",painter.head);
    printf(1,"%d",painter.current);
    pvcPaintControl(btn);
  }
}

void redo(void *data)
{
  PHControl btn = (PHControl)data;
  if (painter.head != painter.tail && (painter.current + 1) % 10 != painter.tail % 10)
  {
    painter.current = (painter.current + 1 ) % 10;
    pvcBltbit(&(btn->dc), 0, 0, painter.hdcQueue[(painter.current) % 10], 0,
              0, btn->dc.size.cx, btn->dc.size.cy);
    printf(1,"%d",painter.tail);
    printf(1,"%d",painter.head);
    printf(1,"%d",painter.current);
    pvcPaintControl(btn);
  }
}

bool pvcPaintHdcProc(PHControl btn, PMessage msg)
{
  switch (msg.type)
  {
  case MSG_PAINT:
    //pvcPaintHdc(&(btn->dc));
    break;
  case MSG_CREATE:
  {
    btn->dc.brush.color = RGB(245, 245, 245);
    pvcDrawRect(&btn->dc, 0, 0, btn->dc.size.cx, btn->dc.size.cy);
    break;
  }
  case MSG_LBUTTON_DOWN:
  {
    IfMousePressed = 1;
    int x = (msg.param >> 16) & 0xffff;
    int y = msg.param & 0xffff;
    painter.first_pos.x = x;
    painter.first_pos.y = y;

    pvcBltbit(painter.originalHdc, 0, 0, &(btn->dc), 0, 0, btn->dc.size.cx,
              btn->dc.size.cy);
    pvcPaintHdc(btn);
    btn->hwnd->ctrlLst.mouseCatch = btn;
    return FINISH;
  }
  case MSG_MOUSE_MOVE:
  {
    int x = (msg.param >> 16) & 0xffff;
    int y = msg.param & 0xffff;
    painter.final_pos.x = painter.pos.x;
    painter.final_pos.y = painter.pos.y;
    painter.pos.x = x;
    painter.pos.y = y;
    if (IfMousePressed > 0 && painter.category != 3) pvcPaintHdc(btn);
    return FINISH;
  }
  case MSG_LBUTTON_UP:
  {
    IfMousePressed = 0;
    int x = (msg.param >> 16) & 0xffff;
    int y = msg.param & 0xffff;
    painter.final_pos.x = x;
    painter.final_pos.y = y;
    painter.tail = (painter.current + 1) % 10;
    if (((painter.tail + 1) % 10) == (painter.head % 10))
          painter.head = (painter.head + 1) % 10;
    pvcBltbit(painter.hdcQueue[painter.tail], 0, 0, &(btn->dc), 0, 0,
              btn->dc.size.cx, btn->dc.size.cy);
    painter.current = painter.tail;
    painter.tail = (painter.tail + 1) % 10;
    if (painter.category != 3) pvcPaintHdc(btn);
    return FINISH;
  }
  default:
    break;
  }
  return pvcCtrlProc(btn, msg);
}
void pvcPaintSetColor(PHdc hdc)
{
  hdc->brush.color = RGB(245, 235, 245);
  pvcDrawRect(hdc, 0, 0, hdc->size.cx, hdc->size.cy);
  hdc->pen.size = 1;
  hdc->pen.color = RGB(10, 10, 10);
  hdc->brush.color = COLOR_NULL;
  pvcDrawRect(hdc, 10, 10, 6, 10);
  pvcDrawLine(hdc, 10, 10, 13, 5);
  pvcDrawLine(hdc, 16, 10, 13, 5);
  pvcDrawRect(hdc, 20, 10, 8, 10);
  PFont font;
  font.color = RGB(10, 10, 10);
  pvcSetFont(hdc, font);
  pvcDrawText(hdc, "R", 35, 10);
  pvcDrawText(hdc, "G", 85, 10);
  pvcDrawText(hdc, "B", 135, 10);
  pvcDrawText(hdc, "SIZE", 185, 10);
  if (painter.isPen >= 0)
  {
    for (int j = (1 - painter.isPen) * 10 + 8; j < (1 - painter.isPen) * 10 + 18; j++)
    {
      for (int i = 3; i < 23; i++)
      {
        PColor color = pvcPixel(hdc, j, i);
        color.r -= 50;
        color.g -= 50;
        color.b -= 50;
        pvcSetPixel(hdc, j, i, color);
      }
    }
  }
}
void setColor(void * setColorPa)
{
  char buf_r[16];
  char buf_g[16];
  char buf_b[16];
  char buf_size[16];
  setColorParent* set = (setColorParent *)setColorPa;
  PHdc hdc = &set->ctrl->dc;
  pvcGetLineEditString(set->lineEdit[0], buf_r);
  pvcGetLineEditString(set->lineEdit[1], buf_g);
  pvcGetLineEditString(set->lineEdit[2], buf_b);
  pvcGetLineEditString(set->lineEdit[3], buf_size);
  painter.size = atoi(buf_size);
  if (painter.isPen == 1)
    painter.penColor = RGB(atoi(buf_r),atoi(buf_g),atoi(buf_b));
  else painter.eraserColor = RGB(atoi(buf_r),atoi(buf_g),atoi(buf_b));
  if (painter.isPen == 1) hdc->brush.color = painter.penColor;
  else hdc->brush.color = painter.eraserColor;
  pvcDrawRect(hdc, 305, 10, 30, 15);
  pvcPaintControl(set->ctrl);
}


bool pvcSetColorProc(PHControl btn, PMessage msg)
{
  switch (msg.type)
  {
  case MSG_PAINT:
    pvcPaintSetColor(&(btn->dc));
    break;
  case MSG_LBUTTON_DOWN:
  {
    int x = (msg.param >> 16) & 0xffff;
    if (x > 8 && x < 28)
    {
      if (x < 18) painter.isPen = 1;
      else painter.isPen = 0;
      pvcCtrlInvalidate(btn);
    }
    return SUBMIT;
  }
  default:
    break;
  }
  return pvcCtrlProc(btn, msg);
}
void openFile(void *data)
{
  PHControl btn = (PHControl)data;
  char name[256];
  pvcGetInputDialog(btn->hwnd, "Input name", name, 256);
  PBitmap bmp = pvcLoadBitmap(name);
  PHdc hdc = pvcCreateCompatibleDCFromBitmap(bmp);
  pvcBltbit(&(btn->dc), 0, 0, hdc, 0, 0 ,hdc->size.cx, hdc->size.cy);
  pvcPaintControl(btn);
}

void savebmp(uchar * pdata, char * bmp_file, int width, int height )
{      //分别为rgb数据，要保存的bmp文件名，图片长宽
	  int lineWidth = (((width * 3 * 8 + 31) >> 5) << 2) / 3;
    int size = lineWidth*height*3*sizeof(char); // 每个像素��?3个字��?
     // 位图第一部分，文件信��?
     PBitmapFileHeader bfh;
     bfh.bfType = (ushort)0x4d42;  //bm
     bfh.bfSize = width*height*3*sizeof(char)  // data size
            + sizeof( PBitmapFileHeader ) // first section size
            + sizeof( PBitmapInfoHeader ) // second section size
            ;
     bfh.bfReserved1 = 0; // reserved
     bfh.bfReserved2 = 0; // reserved
     bfh.bfOffBits = sizeof( PBitmapFileHeader )+ sizeof( PBitmapInfoHeader );//真正的数据的位置
     // 位图第二部分，数据信��?
     PBitmapInfoHeader bih;
     bih.biSize = sizeof(PBitmapInfoHeader);
     bih.biWidth = width;
     bih.biHeight = -height;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了
     bih.biPlanes = 1;//��?1，不用改
     bih.biBitCount = 24;
     bih.biCompression = 0;//不压��?
     bih.biSizeImage = size;
     bih.biXPelsPerMeter = 2835 ;//像素每米
     bih.biYPelsPerMeter = 2835 ;
     bih.biClrUsed = 0;//已用过的颜色��?24位的��?0*
     bih.biClrImportant = 0;//每个像素都重��?
     int fp = open( bmp_file,O_CREATE | O_WRONLY );
     if( fp < 0 ) return;
     write( fp, &bfh, sizeof(PBitmapFileHeader));//由于linux��?4字节对齐，而信息头大小��?54字节，第一部分14字节，第二部��?40字节
								 //，所以会将第一部分补齐��?16字节，直接用sizeof，打开图片时就会遇到premature end-of-file encountered错误
     write( fp, &bih, sizeof(PBitmapInfoHeader));
     write( fp, pdata, size);
     close( fp );
     printf(1,"3");
 }

void saveFile(void *data)
{
  PHControl btn = (PHControl)data;
  char buf[512];
  if (MB_ACCEPT != pvcGetInputDialog(btn->hwnd, "Input Name", buf, 512))
    return;
  int height = btn->dc.size.cx;
  int width = btn->dc.size.cy;
  int lineWidth = (((width * 3 * 8 + 31) >> 5) << 2) / 3;
	PColor *pdata = (PColor *)malloc(sizeof(PColor) * lineWidth * height);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			pdata[i * lineWidth + j] = pvcPixel(&btn->dc, j, i);
	savebmp((uchar *)pdata, buf, width, height );
  free(pdata);
}
void pvcPaintSlider(PHdc hdc)
{
  hdc->brush.color = RGB(245, 235, 245);
  pvcDrawRect(hdc, 0, 0, hdc->size.cx, hdc->size.cy);
  hdc->pen.color = RGB(10, 10, 10);
  hdc->brush.color = COLOR_NULL;
  hdc->pen.size = 1;
  PFont font;
  font.color = RGB(10, 10, 10);
  pvcSetFont(hdc, font);
  int step = 65;
  int index = 0;
  pvcDrawText(hdc, "R", 15, 5 + step * index);
  ++index;
  pvcDrawText(hdc, "G", 15, 5 + step * index);
  ++index;
  pvcDrawText(hdc, "B", 15, 5 + step * index);
  ++index;
  pvcDrawText(hdc, "SIZE", 15, 5 + step * index);
  ++index;
  int sy = 10 + step * index;
  int sx = 5;
  int w = hdc->size.cx - 2 * sx;
  int h = hdc->size.cy - sy - sx;
  hdc->brush.color = RGB(sliSt.color.r, sliSt.color.g, sliSt.color.b);
  pvcDrawRect(hdc, sx, sy, w, h);
}
bool pvcSliderProc(PHControl btn, PMessage msg)
{
  switch (msg.type)
  {
  case MSG_PAINT:
    pvcPaintSlider(&(btn->dc));
    break;

  default:
    break;
  }
  return pvcCtrlProc(btn, msg);
}
void init(PHwnd hwnd)
{
  GroupButtonData * data = (GroupButtonData*)malloc(sizeof(GroupButtonData));
  painter.penColor = RGB(10, 10, 10);
  painter.eraserColor = RGB(245, 245, 245);
  painter.pos.x = 0;
  painter.pos.y = 0;
  painter.category = -1;
  painter.originalHdc = pvcCreateCompatibleDC(&(hwnd->dc));
  for (int i = 0; i < 10; i++)
    painter.hdcQueue[i] = pvcCreateCompatibleDC(&(hwnd->dc));
  data->now = -1;
  data->length = 6;
  IfMousePressed = 0;
  painter.isPen = 1;
  painter.size = 1;
  setColorParent *setColorPa = malloc (sizeof(setColorParent));
  setColorParent1 *setColorPa1 = malloc (sizeof(setColorParent1));
  painter.head = 0;
  painter.tail = 0;
  pvcCreateControl(hwnd, 10, 55, GROUP_BUTTON_WIDTH, WND_HEIGHT - 55 - WND_EDGE_SIZE,
                  (void*)data, pvcGroupButtonProc);
  PHControl hdc_control = pvcCreateControl(hwnd, 45, 55, PAINT_HDC_SIZE_X,
                          PAINT_HDC_SIZE_Y, 0, pvcPaintHdcProc);
  PHControl control = pvcCreateControl(hwnd, 45, PAINT_HDC_SIZE_Y + 60, WND_WIDTH - 45 - WND_EDGE_SIZE,
                      SET_COLOR_SIZE_Y, 0, pvcSetColorProc);
  setColorPa->ctrl = control;
  setColorPa->lineEdit[0] = pvcCreateLineEdit(hwnd, 90, PAINT_HDC_SIZE_Y + 70, LINE_EDIT_SIZE_X,
                            LINE_EDIT_SIZE_Y, LINE_EDIT_LENGTH, 0, pvcPaintLineEdit);
  setColorPa->lineEdit[1] = pvcCreateLineEdit(hwnd, 140, PAINT_HDC_SIZE_Y + 70, LINE_EDIT_SIZE_X,
                            LINE_EDIT_SIZE_Y, LINE_EDIT_LENGTH, 0, pvcPaintLineEdit);
  setColorPa->lineEdit[2] = pvcCreateLineEdit(hwnd, 190, PAINT_HDC_SIZE_Y + 70, LINE_EDIT_SIZE_X,
                            LINE_EDIT_SIZE_Y, LINE_EDIT_LENGTH, 0, pvcPaintLineEdit);
  setColorPa->lineEdit[3] = pvcCreateLineEdit(hwnd, 265, PAINT_HDC_SIZE_Y + 70, LINE_EDIT_SIZE_X,
                            LINE_EDIT_SIZE_Y, LINE_EDIT_LENGTH, 0, pvcPaintLineEdit);
  set_color = pvcCreateButton(hwnd, "Set", 305, PAINT_HDC_SIZE_Y + 70, LINE_EDIT_SIZE_X,
              LINE_EDIT_SIZE_Y, setColorPa, setColor, pvcPaintButton);
  control1 = pvcCreateControl(hwnd, 45 + PAINT_HDC_SIZE_X + 5,
             55, PAINT_SLIDERPROC_SIZE_X, PAINT_SLIDERPROC_SIZE_Y, 0, pvcSliderProc);
  setColorPa1->ctrl = control1;
  for (int i = 0; i < 3; i++)
  {
    setColorPa1->slide[i] = pvcCreateSliderBar(hwnd, i, 45 + PAINT_HDC_SIZE_X + 10,
                            75 + 65 * i, PAINT_SLIDERPROC_SIZE_X - 5, 40, 0, 255, 0.1, SBT_HORIZONAL,
                            pvcPaintSliderBar);
  }
  setColorPa1->slide[3] = pvcCreateSliderBar(hwnd, 3, 45 + PAINT_HDC_SIZE_X + 10,
                          75 + 65 * 3, PAINT_SLIDERPROC_SIZE_X - 5, 40, 0, 30, 0.1, SBT_HORIZONAL,
                          pvcPaintSliderBar);
  PHMenuBar menuBar = pvcCreateMenuBar(hwnd);

  PHMenu file = pvcCreateMenu(hwnd, false);
  pvcMenuAddItem(file, "open", (void *)hdc_control, openFile);
  pvcMenuAddItem(file, "save", (void *)hdc_control, saveFile);
  pvcMenuAddItem(file, "close", hwnd, pvcDestroyWindow);
  pvcMenuBarAddMenu(menuBar, "file", file);

  PHMenu edit = pvcCreateMenu(hwnd, false);
  pvcMenuAddItem(edit, "undo", (void *)hdc_control, undo);
  pvcMenuAddItem(edit, "redo", (void *)hdc_control, redo);
  pvcMenuBarAddMenu(menuBar, "edit", edit);

  pvcBltbit(painter.hdcQueue[painter.tail], 0, 0, &(hdc_control->dc),
            0, 0, hdc_control->dc.size.cx, hdc_control->dc.size.cy);
  painter.current = painter.tail;
  painter.tail = (painter.tail + 1) % 10;
  pvcInvalidate(hwnd);
}

void draw(PHwnd hwnd)
{
  PHdc hdc = pvcGetDC(hwnd);
  hdc->brush.color = RGB(190, 190, 195);
  pvcDrawRect(hdc, 0, 0, hdc->size.cx, hdc->size.cy);
}

void paintCtrl(PHControl ctrl, int x1, int y1, int x2, int y2)
{
  if (x1 > x2)
  {
    int t = x1;
    x1 = x2;
    x2 = t;
  }
  if (y1 > y2)
  {
    int t = y1;
    y1 = y2;
    y2 = t;
  }
  x1 -= painter.size;
  if (x1 < 0)
    x1 = 0;
  y1 -= painter.size;
  if (y1 < 0)
    y1 = 0;
  x2 += painter.size;
  int w = x2 - x1 + 1;
  if (w > ctrl->dc.size.cx)
    w = ctrl->dc.size.cx;
  y2 += painter.size;
  int h = y2 - y1 + 1;
  if (h > ctrl->dc.size.cy)
    h = ctrl->dc.size.cy;
  //printf(1, "%d %d %d %d\n", x1, y1, w, h);
  paintWindow(ctrl->hwnd, ctrl->pos.x + x1, ctrl->pos.y + y1, &ctrl->dc,
    x1, y1, w, h);
}

void copyDC(PHdc dest, PHdc src, int x1, int y1, int x2, int y2)
{
  int x = MIN(x1, x2);
  int y = MIN(y1, y2);
  int w = ABS(x2 - x1) + 1;
  int h = ABS(y2 - y1) + 1;
  x -= painter.size;
  y -= painter.size;
  w += painter.size * 2;
  h += painter.size * 2;
  x = BOUND(0, x, src->size.cx);
  y = BOUND(0, y, src->size.cy);
  w = BOUND(0, w, src->size.cx);
  h = BOUND(0, h, src->size.cy);
  pvcBltbit(dest, x, y, src, x, y, w, h);
}
