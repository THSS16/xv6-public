#include "PVCSnake.h"

bool wndProc(PHwnd hwnd, PMessage msg)
{
  switch (msg.type)
  {
  case MSG_TIMEOUT:
    timerUpdate(hwnd);
    break;
  case MSG_KEY_DOWN:
    keydown(hwnd, msg);
    break;
  case MSG_PAINT:
    draw(hwnd);
    break;
  case MSG_CREATE:
    init(hwnd);
    break;
  default:
    break;
  }
  return pvcWndProc(hwnd, msg);
}

int main()
{
  PHwnd hwnd = pvcCreateWindow("Snake", 0, 100, 100, WND_WIDTH, WND_HEIGHT);
  pvcWndExec(hwnd, wndProc);
  exit();
}

void init(PHwnd hwnd)
{
  random(getTime());
  length = 5;
  dir = Right;
  status = true;
  pause = true;
  int i;
  for (i = 0; i < length; ++i)
  {
    blocks[i].x = length - 1 - i;
    blocks[i].y = 0;
  }
  updateStart();
  pvcInvalidate(hwnd);
}

void timerUpdate(PHwnd hwnd)
{
  PPoint temp = blocks[0];
  if (dir == Left)
  {
    if (temp.x - 1 < 0)
      status = false;
    else
      temp.x -= 1;
  }
  else if (dir == Right)
  {
    if (temp.x + 1 >= BLOCK_NUM_X)
      status = false;
    else
      temp.x += 1;
  }
  else if (dir == Up)
  {
    if (temp.y - 1 < 0)
      status = false;
    else
      temp.y -= 1;
  }
  else if (dir == Down)
  {
    if (temp.y + 1 >= BLOCK_NUM_Y)
      status = false;
    else
      temp.y += 1;
  }
  for (int i = 0; i < length - 1; ++i)
  {
    if (temp.x == blocks[i].x && temp.y == blocks[i].y)
      status = false;
  }
  /*if (blocks[0].x < 0 || blocks[0].y < 0 || blocks[0].x >= BLOCK_NUM_X || blocks[0].y >= BLOCK_NUM_Y)
    status = false;*/
  if (status == false)
  {
    //printf(1, "kill Timer\n");
    killTimer(hwnd, 2);
  }
  else
  {
    if (temp.x == star.x && temp.y == star.y)
    {
      status = updateStart();
      if (status)
        ++length;
    }
    for (int i = length - 1; i > 0; --i)
      blocks[i] = blocks[i - 1];
    blocks[0] = temp;
  }
  pvcInvalidate(hwnd);
}

void keydown(PHwnd hwnd, PMessage msg)
{
  switch (msg.param)
  {
  case VK_UP:
    if (dir == Left || dir == Right)
      dir = Up;
    break;
  case VK_DOWN:
    if (dir == Left || dir == Right)
      dir = Down;
    break;
  case VK_RIGHT:
    if (dir == Up || dir == Down)
      dir = Right;
    break;
  case VK_LEFT:
    if (dir == Up || dir == Down)
      dir = Left;
    break;
  case VK_SPACE:
    if (!status)
    {
      status = true;
      setTimer(hwnd, 2, TIMER_INTERVAL);
      break;
    }
    if (pause)
      setTimer(hwnd, 2, TIMER_INTERVAL);
    else
      killTimer(hwnd, 2);
    pause ^= true;
    pvcInvalidate(hwnd);
    break;
  case VK_LSHIFT:
    init(hwnd);
    pause = false;
    setTimer(hwnd, 2, TIMER_INTERVAL);
    break;
  }
}

void draw(PHwnd hwnd)
{
  PHdc hdc = pvcGetDC(hwnd);
  PPen pen;
  pen.size = 0;
  pen.color = COLOR_NULL;
  pvcSetPen(hdc, pen);
  PBrush brush;
  //memset(hdc->content, 0x0c, sizeof(PColor) * hdc->size.cx * hdc->size.cy);
  brush.color = HEAD_COLOR;
  pvcSetBrush(hdc, brush);
  pvcDrawRect(hdc, blocks[0].x * BLOCK_SIZE_CX, blocks[0].y * BLOCK_SIZE_CY, BLOCK_SIZE_CX, BLOCK_SIZE_CY);
  brush.color = BODY_COLOR;
  pvcSetBrush(hdc, brush);
  int i;
  for (i = 1; i < length; ++i)
    pvcDrawRect(hdc, blocks[i].x * BLOCK_SIZE_CX, blocks[i].y * BLOCK_SIZE_CY, BLOCK_SIZE_CX, BLOCK_SIZE_CY);

  brush.color = STAR_COLOR;
  pvcSetBrush(hdc, brush);
  pvcDrawRect(hdc, star.x * BLOCK_SIZE_CX, star.y * BLOCK_SIZE_CY, BLOCK_SIZE_CX, BLOCK_SIZE_CY);

  pen.size = 1;
  pen.color = RGB(255, 255, 255);
  pvcSetPen(hdc, pen);
  pvcDrawLine(hdc, 0, BLOCK_SIZE_CY * BLOCK_NUM_Y, BLOCK_SIZE_CX * BLOCK_NUM_X, BLOCK_SIZE_CY * BLOCK_NUM_Y);
  PFont font;
  font.color = RGB(255, 255, 255);
  pvcSetFont(hdc, font);
  char * buf;
  if (!status)
    buf = "Game over! Press LShift to restart";
  else if (pause)
    buf = "press space to start";
  else
    buf = "press space to stop";
  pvcDrawText(hdc, buf, 2, BLOCK_SIZE_CY * BLOCK_NUM_Y + 2);
}

bool updateStart()
{
  if (length == MAX_LENGTH)
    return false;
  while (1)
  {
    int x = random(0) % BLOCK_NUM_X;
    int y = random(0) % BLOCK_NUM_Y;
    int i;
    for (i = 0; i < length; ++i)
      if (blocks[i].x == x && blocks[i].y == y)
        break;
    if (i == length)
    {
      star.x = x;
      star.y = y;
      return true;
    }
  }
  return false;
}
