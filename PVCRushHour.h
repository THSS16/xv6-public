#ifndef PVCRUSHHOUR_H
#define PVCRUSHHOUR_H

#include "PVC.h"

/*定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高*/
#define TIMER                1
#define TIMER_INTERVAL       30
#define WNDWIDTH             250
#define WNDHEIGHT            200
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      10

/*英雄结构体，英雄位图、位置、大小*/
typedef struct
{
	PBitmap	hBmp;
	PPoint	pos;
	PSize	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Hero;

/*地形结构体，地形方块位图、屋顶位图、位置、大小、屋顶以及方块大小、方块个数*/
typedef struct
{
	PBitmap hBlockBmp;
	PBitmap hRoofBmp;
	PPoint	pos;
	PSize    size;
	int     roofWidth;
	int     roofHeight;
	int     blockWidth;
	int     blockHeight;
	int     blockNum;
}Terrian;

/*背景建筑结构体，建筑位图、位置、大小、类型*/
typedef struct
{
	PBitmap	hBmp;
	PPoint	pos;
	PSize	size;
}Building;

/*游戏状态结构体*/
typedef struct
{
	PBitmap	hBmp;
	PPoint   pos;
	PSize	size;
	bool	isPaused;
	int     totalDist;
}GameStatus;


/*声明位图句柄*/
PBitmap m_hBackgroundBmp;
PBitmap m_hBuildingBmp;
PBitmap m_hHeroBmp;
PBitmap m_hGameStatusBmp;
PBitmap	m_hBlockBmp[BLOCK_COLOR_NUM];
PBitmap	m_hRoofkBmp[ROOF_COLOR_NUM];

/*定义方块颜色数组，与m_hBlockBmp[BLOCK_COLOR_NUM]个数对应，0表示蓝色方块，1表示绿色方块，2表示橙色方块，3表示粉色方块*/
char *	m_blockBmpNames[] = { "RHBlueBlock.bmp", "RHGreenBlock.bmp", "RHOrangeBlock.bmp", "RHPinkBlock.bmp" };
/*定义屋顶颜色数组，与m_hRoofkBmp[ROOF_COLOR_NUM]个数对应，0表示黑色屋顶，1表示灰色屋顶*/
char *	m_roofBmpNames[] = { "RHBlackRoof.bmp", "RHGreyRoof.bmp" };

/*声明英雄、建筑、地形、游戏状态*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus     m_gameStatus;

int jump;
bool jumpFlag = 0;

/*全局函数*/
//窗体过程函数
bool wndProc(PHwnd, PMessage);

//初始化
void init(PHwnd hwnd, PMessage);

/*************************************************
Fucntion : 创建英雄飞机
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
	curFrameIndex当前帧
	maxFrameSize最大帧率
*************************************************/
Hero CreateHero(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : 创建背景建筑
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/
Building CreateBuilding(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

/*************************************************
Fucntion : 创建游戏状态
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/
GameStatus CreateGameStatus(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

/*************************************************
Fucntion : 创建英雄飞机
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBlockBmp表示方块位图句柄
	hRoofBmp表示屋顶位图句柄
	roofWidth、roofHeight屋顶宽度和高度
	blockWidth、blockHeight方块宽度和高度
*************************************************/
Terrian CreateTerrian(int posX, int posY, int sizeX, int sizeY,
					  PBitmap hBlockBmp, PBitmap hRoofBmp, int roofHeight, int blockHeight);

//双缓冲绘制
void Render(PHwnd hwnd);
//定时器事件
void TimerUpdate(PHwnd hwnd, PMessage msg);
//hero更新
void HeroUpdate();
//地形更新
void TerrianUpdate();
//游戏状态更新
void GameStatusUpdate();
//判断是否点击暂停
bool Paused(PPoint);
//键盘按下事件处理
void KeyDown(PHwnd hwnd, PMessage msg);
//键盘松开事件处理
void KeyUp(PHwnd hwnd, PMessage msg);
//左鼠标点击事件
void LButtonDown(PHwnd hwnd, PMessage msg);


#endif
