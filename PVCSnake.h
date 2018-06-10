#ifndef PVCSNAKE_H
#define PVCSNAKE_H

#include "PVC.h"

#define BLOCK_NUM_X 10
#define BLOCK_NUM_Y 10
#define BLOCK_SIZE_CX 30
#define BLOCK_SIZE_CY 30
#define MAX_LENGTH  BLOCK_NUM_X * BLOCK_NUM_Y
#define STATUS_BAR_HEIGHT 20
#define WND_WIDTH BLOCK_NUM_X * BLOCK_SIZE_CX + WND_EDGE_SIZE * 2
#define WND_HEIGHT BLOCK_NUM_Y * BLOCK_SIZE_CY + WND_EDGE_SIZE + WND_TITLE_HEIGHT + STATUS_BAR_HEIGHT

#define HEAD_COLOR RGB(255, 0, 0)
#define BODY_COLOR RGB(255, 255, 255)
#define BGND_COLOR RGB(0, 0, 0)
#define STAR_COLOR RGB(0, 255, 0)

#define TIMER_INTERVAL 500

PPoint blocks[MAX_LENGTH];

PPoint star;

int length = 9;

enum Direction { Down, Up, Left, Right };

bool pause = true;

int status = true;

int dir = Right;

void init(PHwnd hwnd);

void timerUpdate(PHwnd hwnd);

void keydown(PHwnd hwnd, PMessage msg);

void draw(PHwnd hwnd);

bool updateStart();

#endif
