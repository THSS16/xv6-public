#ifndef PVCRUSHHOUR_H
#define PVCRUSHHOUR_H

#include "PVC.h"

/*����������WNDWIDTH��WNDHEIGHTΪ���ڵĿ�͸�*/
#define TIMER                1
#define TIMER_INTERVAL       30
#define WNDWIDTH             250
#define WNDHEIGHT            200
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      10

/*Ӣ�۽ṹ�壬Ӣ��λͼ��λ�á���С*/
typedef struct
{
	PBitmap	hBmp;
	PPoint	pos;
	PSize	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Hero;

/*���νṹ�壬���η���λͼ���ݶ�λͼ��λ�á���С���ݶ��Լ������С���������*/
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

/*���������ṹ�壬����λͼ��λ�á���С������*/
typedef struct
{
	PBitmap	hBmp;
	PPoint	pos;
	PSize	size;
}Building;

/*��Ϸ״̬�ṹ��*/
typedef struct
{
	PBitmap	hBmp;
	PPoint   pos;
	PSize	size;
	bool	isPaused;
	int     totalDist;
}GameStatus;


/*����λͼ���*/
PBitmap m_hBackgroundBmp;
PBitmap m_hBuildingBmp;
PBitmap m_hHeroBmp;
PBitmap m_hGameStatusBmp;
PBitmap	m_hBlockBmp[BLOCK_COLOR_NUM];
PBitmap	m_hRoofkBmp[ROOF_COLOR_NUM];

/*���巽����ɫ���飬��m_hBlockBmp[BLOCK_COLOR_NUM]������Ӧ��0��ʾ��ɫ���飬1��ʾ��ɫ���飬2��ʾ��ɫ���飬3��ʾ��ɫ����*/
char *	m_blockBmpNames[] = { "RHBlueBlock.bmp", "RHGreenBlock.bmp", "RHOrangeBlock.bmp", "RHPinkBlock.bmp" };
/*�����ݶ���ɫ���飬��m_hRoofkBmp[ROOF_COLOR_NUM]������Ӧ��0��ʾ��ɫ�ݶ���1��ʾ��ɫ�ݶ�*/
char *	m_roofBmpNames[] = { "RHBlackRoof.bmp", "RHGreyRoof.bmp" };

/*����Ӣ�ۡ����������Ρ���Ϸ״̬*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus     m_gameStatus;

int jump;
bool jumpFlag = 0;

/*ȫ�ֺ���*/
//������̺���
bool wndProc(PHwnd, PMessage);

//��ʼ��
void init(PHwnd hwnd, PMessage);

/*************************************************
Fucntion : ����Ӣ�۷ɻ�
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
	curFrameIndex��ǰ֡
	maxFrameSize���֡��
*************************************************/
Hero CreateHero(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : ������������
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
*************************************************/
Building CreateBuilding(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

/*************************************************
Fucntion : ������Ϸ״̬
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
*************************************************/
GameStatus CreateGameStatus(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp);

/*************************************************
Fucntion : ����Ӣ�۷ɻ�
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBlockBmp��ʾ����λͼ���
	hRoofBmp��ʾ�ݶ�λͼ���
	roofWidth��roofHeight�ݶ���Ⱥ͸߶�
	blockWidth��blockHeight�����Ⱥ͸߶�
*************************************************/
Terrian CreateTerrian(int posX, int posY, int sizeX, int sizeY,
					  PBitmap hBlockBmp, PBitmap hRoofBmp, int roofHeight, int blockHeight);

//˫�������
void Render(PHwnd hwnd);
//��ʱ���¼�
void TimerUpdate(PHwnd hwnd, PMessage msg);
//hero����
void HeroUpdate();
//���θ���
void TerrianUpdate();
//��Ϸ״̬����
void GameStatusUpdate();
//�ж��Ƿ�����ͣ
bool Paused(PPoint);
//���̰����¼�����
void KeyDown(PHwnd hwnd, PMessage msg);
//�����ɿ��¼�����
void KeyUp(PHwnd hwnd, PMessage msg);
//��������¼�
void LButtonDown(PHwnd hwnd, PMessage msg);


#endif
