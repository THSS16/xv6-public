#ifndef PVCNOTEPAD_H
#define PVCNOTEPAD_H

#include "PVC.h"

#define NOTEPAD_WIDTH 400
#define NOTEPAD_HEIGHT 200

PHMultiLineEdit multiLineEdit = 0;

void startFile(PHwnd hwnd);
void openFile(void * param);
void showFile(PHwnd hwnd, char * buf);
void saveFile(void * param);

#endif
