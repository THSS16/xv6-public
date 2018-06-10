#ifndef PVCPAINTBOARD_H
#define PVCPAINTBOARD_H

#include "PVC.h"
#define GROUP_BUTTON_NUM 6
#define GROUP_BUTTON_HEIGHT GROUP_BUTTON_NUM * 20
#define GROUP_BUTTON_WIDTH 30
#define PAINT_HDC_SIZE_X 500
#define PAINT_HDC_SIZE_Y 400
#define PAINT_SLIDERPROC_SIZE_X 100
#define PAINT_SLIDERPROC_SIZE_Y PAINT_HDC_SIZE_Y
#define SET_COLOR_SIZE_X 455
#define SET_COLOR_SIZE_Y 30
#define LINE_EDIT_SIZE_X 35
#define LINE_EDIT_SIZE_Y 15
#define LINE_EDIT_LENGTH 16
#define WND_WIDTH 50 + PAINT_HDC_SIZE_X + PAINT_SLIDERPROC_SIZE_X + WND_EDGE_SIZE
#define WND_HEIGHT 60 + PAINT_HDC_SIZE_Y + SET_COLOR_SIZE_Y + WND_EDGE_SIZE

PHButton set_color;
PHButton set_color1;
typedef struct setColorParent1
{
  PHControl ctrl;
  PHSliderBar slide[4];
}setColorParent1;
PHControl control1;
typedef struct SliderStyle
{
  PColor color;
  int size;
}SliderStyle;
SliderStyle sliSt;
bool IfMousePressed;
typedef struct GroupButtonData
{
  int length;
  int now;
}GroupButtonData;

typedef struct HdcPainter
{
  PPoint first_pos;
  PPoint pos;
  PPoint final_pos;
  int category;
  PColor penColor;
  PColor eraserColor;
  PHdc originalHdc;
  int size;
  bool isPen;
  PHdc hdcQueue[10];
  int head,tail,current;
}HdcPainter;

typedef struct setColorParent
{
  PHControl ctrl;
  PHLineEdit lineEdit[4];
}setColorParent;
HdcPainter painter;

void openFile(void *data);
void saveFile(void *data);
void init(PHwnd hwnd);
void undo(void *data);
void redo(void *data);
void draw(PHwnd hwnd);

void paintCtrl(PHControl ctrl, int x1, int y1, int x2, int y2);

void copyDC(PHdc dest, PHdc src, int x1, int y1, int x2, int y2);

#endif
