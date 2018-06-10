#include "PVCNotePad.h"

char file_tobe_open[MAX_ADDRESS_LEN];

void startFile(PHwnd hwnd)
{
	if (strlen(file_tobe_open) != 0)
	{
		showFile(hwnd, file_tobe_open);
		file_tobe_open[0] = 0;
	}

	return;
}

void openFile(void * param)
{
  PHwnd hwnd = (PHwnd)param;
  char buf[512];
  if (MB_ACCEPT != pvcGetInputDialog(hwnd, "Input name", buf, 512))
    return;

  showFile(hwnd, buf);

  return;
}

void showFile(PHwnd hwnd, char * buf)
{
	int file = open(buf, O_RDONLY);
	if (file < 0)
	{
		pvcMessageBox(hwnd, "Error", "Open File failed!");
		return;
	}

	PMultiLineEditData * data = (PMultiLineEditData *)multiLineEdit->data;

	for (int i = 0; i <= data->rows_size; ++i)
		data->buf[i].length = 0;

	data->rows = 0;
	data->cursor_x = data->cursor_y = 0;
	data->position_x = data->position_y = 0;
	data->state = LES_INACTIVE;
	data->sy = 0;
	data->rows_all = 0;

	while (1)
	{
		int l = read(file, buf, sizeof(char) * 512);
		if (l <= 0)
			break;
		for (int i = 0; i < 512; ++i)
		{
			if (buf[i] == '\n')
			{
				++data->rows;
				if (data->rows + 1 >= data->rows_size)
				{
					printf(1, "file too long\n");
					close(file);
					return;
				}
				continue;
			}
			if (data->buf[data->rows].length >= data->buf[data->rows].size)
			{
				data->buf[data->rows].size *= 2;
				char * temp = (char *)malloc(sizeof(char) * data->buf[data->rows].size);
				for (int j = 0; j < data->buf[data->rows].length; ++j)
					temp[j] = data->buf[data->rows].buf[j];
				free(data->buf[data->rows].buf);
				data->buf[data->rows].buf = temp;
			}
			data->buf[data->rows].buf[data->buf[data->rows].length++] = buf[i];
		}
		//printf(1, "%s", buf);
	}
	close(file);
}

void saveFile(void * param)
{
  PHwnd hwnd = (PHwnd)param;
  PMultiLineEditData * data = (PMultiLineEditData *)multiLineEdit->data;
  char buf[512];
  if (MB_ACCEPT != pvcGetInputDialog(hwnd, "Input name", buf, 512))
    return;
  int file = open(buf, O_WRONLY | O_CREATE);
  if (file < 0)
  {
    pvcMessageBox(hwnd, "Error", "Save File failed!");
    return;
  }
  //printf(1, "%d\n", data->rows);
  for (int i = 0; i <= data->rows; ++i)
  {
    write(file, data->buf[i].buf, sizeof(char) * data->buf[i].length);
    write(file, "\n", sizeof(char));
  }
  write(file, "\0", sizeof(char));
  close(file);
}

bool wndProc(PHwnd hwnd, PMessage msg)
{
  switch (msg.type)
  {
	case MSG_CREATE:
  {
    PHMenu menu = pvcCreateMenu(hwnd, false);
    pvcMenuAddItem(menu, "open", (void *)hwnd, openFile);
    pvcMenuAddItem(menu, "save", (void *)hwnd, saveFile);
    pvcMenuAddItem(menu, "close", (void *)hwnd, pvcCloseWindow);

    PHMenuBar menuBar = pvcCreateMenuBar(hwnd);
    pvcMenuBarAddMenu(menuBar, "file", menu);

    multiLineEdit = pvcCreateMultiLineEdit(hwnd, hwnd->clientPos.x, hwnd->clientPos.y + WND_MENUBAR_HEIGHT,
      hwnd->dc.size.cx, hwnd->dc.size.cy - WND_MENUBAR_HEIGHT, pvcPaintMultiLineEdit);

	startFile(hwnd);
  }
		break;
  case MSG_DESTROY:
  {
    if (MB_ACCEPT == pvcMessageBox (hwnd, "NotePad", "Do you want to quit?"))
      break;
    return FINISH;
  }
  default:
    break;
  }
  return pvcWndProc(hwnd, msg);
}

int main(int argc, char* argv[])
{
  PHwnd hwnd = pvcCreateWindow("NotePad", 0, 200, 100, NOTEPAD_WIDTH, NOTEPAD_HEIGHT);

  // Passing the second argument as file name
  if (argc >= 2)
  {
	  int i = 0;
	  for (; argv[1][i] != 0; i++)
	  {
		  file_tobe_open[i] = argv[1][i];
	  }
	  file_tobe_open[i] = 0;
  }

  pvcWndExec(hwnd, wndProc);
  exit();
}
