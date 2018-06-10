#include "PVCRushHour.h"

int main()
{
	PHwnd hwnd = pvcCreateWindow("RushHour", 0, 100, 100,
		WNDWIDTH + WND_EDGE_SIZE * 2, WNDHEIGHT + WND_EDGE_SIZE + WND_TITLE_HEIGHT);
  pvcWndExec(hwnd, wndProc);
  exit();
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
bool wndProc(PHwnd hwnd, PMessage msg)
{
	switch (msg.type)
	{
	case MSG_CREATE:
		//��ʼ��
		init(hwnd, msg);
		break;
	case MSG_PAINT:
		//����
		Render(hwnd);
		if (msg.type == WPT_DC)
			msg.type = WPT_DIRECT;
		break;
	case MSG_KEY_DOWN:
		//���̰����¼�
		KeyDown(hwnd, msg);
		break;
	case MSG_KEY_UP:
		//�����ɿ��¼�
		KeyUp(hwnd, msg);
		break;
	case MSG_LBUTTON_DOWN:
		//������¼�
		LButtonDown(hwnd, msg);
		break;
	case MSG_TIMEOUT:
		//��ʱ���¼�
		TimerUpdate(hwnd, msg);
		break;
	default:
		break;
	}
	return pvcWndProc(hwnd, msg);
}

void init(PHwnd hwnd, PMessage msg)
{
	//���ر���λͼ
	m_hBackgroundBmp = pvcLoadBitmap("RHBackground.bmp");
	//����Buildingλͼ
	m_hBuildingBmp = pvcLoadBitmap("RHBuilding.bmp");
	//����Buildingλͼ
	m_hHeroBmp = pvcLoadBitmap("RHHero.bmp");
	//������Ϸ״̬λͼ
	m_hGameStatusBmp =  pvcLoadBitmap("RHGameStatus.bmp");
	//����Blockλͼ

	int k;
	for (k = 0; k < BLOCK_COLOR_NUM; ++k)
	{
		m_hBlockBmp[k] = pvcLoadBitmap(m_blockBmpNames[k]);
	}
	//�����ݶ�λͼ
	for (k = 0; k < ROOF_COLOR_NUM; ++k)
	{
		m_hRoofkBmp[k] = pvcLoadBitmap(m_roofBmpNames[k]);
	}

	//����Ӣ�ۡ�����
	m_hero = CreateHero(100, 50, 30, 50, m_hHeroBmp, 0, 3);
	m_building = CreateBuilding(0, 70, 500, 250, m_hBuildingBmp);
	//��������
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		if (k % 4 == 0)
		{
			continue;
		}
		m_terrian[k] = CreateTerrian(k * 65, 50 + 50 * (k % 2), 65, 300, m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], 33, 65);
	}

	//������Ϸ״̬
	m_gameStatus = CreateGameStatus(0, 0, 40, 30, m_hGameStatusBmp);

	jump = 0;
	jumpFlag = 0;

	//������ʱ��
	setTimer(hwnd, TIMER, TIMER_INTERVAL);
}

void Render(PHwnd hwnd)
{
	PHdc hdc = pvcGetDC(hwnd);

	PHdc hdcBmp = pvcCreateCompatibleDCFromBitmap(m_hBackgroundBmp);

	//���Ʊ���������
	pvcBltbit(hdc, 0, 0, hdcBmp, 0, 0, WNDWIDTH, WNDHEIGHT);

	//���ƽ���������
	hdcBmp = pvcCreateCompatibleDCFromBitmap(m_building.hBmp);
	pvcTransparentBit(
		hdc, m_building.pos.x, m_building.pos.y,
		hdcBmp, 0, 0, m_building.size.cx, m_building.size.cy,
		RGB(255, 255, 255)
		);

	//����Hero������
	hdcBmp = pvcCreateCompatibleDCFromBitmap(m_hero.hBmp);
	pvcTransparentBit(
		hdc, m_hero.pos.x, m_hero.pos.y,
		hdcBmp, 0, m_hero.size.cy * m_hero.curFrameIndex, m_hero.size.cx, m_hero.size.cy,
		RGB(255, 255, 255)
		);

	//���Ƶ���
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		Terrian terrian = m_terrian[k];
		hdcBmp = pvcCreateCompatibleDCFromBitmap(terrian.hRoofBmp);
		pvcTransparentBit(
			hdc, terrian.pos.x, terrian.pos.y,
			hdcBmp, 0, 0, terrian.roofWidth, terrian.roofHeight,
			RGB(255, 255, 255)
			);
		hdcBmp = pvcCreateCompatibleDCFromBitmap(terrian.hBlockBmp);
		int t;
		for (t = 0; t < terrian.blockNum; ++t)
		{
			pvcTransparentBit(
				hdc, terrian.pos.x, terrian.pos.y + terrian.roofHeight + terrian.blockHeight * t,
				hdcBmp, 0, 0, terrian.blockWidth, terrian.blockHeight,
				RGB(255, 255, 255)
				);
		}
	}

	//������Ϸ״̬
	//��ͣ�����λͼ
	hdcBmp = pvcCreateCompatibleDCFromBitmap(m_gameStatus.hBmp);
	pvcTransparentBit(hdc, m_gameStatus.pos.x, m_gameStatus.pos.y,
		hdcBmp, 0, m_gameStatus.size.cy * m_gameStatus.isPaused,
		m_gameStatus.size.cx, m_gameStatus.size.cy, RGB(255, 255, 255));

	//���Ʒ���
	char szDist[13];
	hdc->font.color = RGB(0, 0, 0);
	sprintf(szDist, "Dist:%4d", m_gameStatus.totalDist);
	pvcDrawText(hdc, szDist, WNDWIDTH - 80, 5);
}

Hero CreateHero(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp, int curFrameIndex, int maxFrameSize)
{
	Hero hero;
	hero.hBmp = hBmp;
	hero.pos.x = posX;
	hero.pos.y = posY;
	hero.size.cx = sizeX;
	hero.size.cy = sizeY;
	hero.curFrameIndex = curFrameIndex;
	hero.maxFrameSize = maxFrameSize;
	return hero;
}

Building CreateBuilding(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	Building building;
	building.hBmp = hBmp;
	building.pos.x = posX;
	building.pos.y = posY;
	building.size.cx = sizeX;
	building.size.cy = sizeY;
	return building;
}

GameStatus CreateGameStatus(int posX, int posY, int sizeX, int sizeY, PBitmap hBmp)
{
	GameStatus gameStatus;
	gameStatus.pos.x = posX;
	gameStatus.pos.y = posY;
	gameStatus.size.cx = sizeX;
	gameStatus.size.cy = sizeY;
	gameStatus.hBmp = hBmp;
	gameStatus.totalDist = 0;
	gameStatus.isPaused = false;
	return gameStatus;
}

Terrian CreateTerrian(int posX, int posY, int sizeX, int sizeY,
					  PBitmap hBlockBmp, PBitmap hRoofBmp, int roofHeight, int blockHeight)
{
	Terrian terrian;
	terrian.pos.x = posX;
	terrian.pos.y = posY;
	terrian.size.cx = sizeX;
	terrian.size.cy = sizeY;
	terrian.hBlockBmp = hBlockBmp;
	terrian.hRoofBmp = hRoofBmp;
	terrian.roofWidth = sizeX;
	terrian.roofHeight = roofHeight;
	terrian.blockWidth = sizeX;
	terrian.blockHeight = blockHeight;
	terrian.blockNum = (int)((sizeY - roofHeight) * 1.0 / blockHeight + 0.9);
	return terrian;
}

void TimerUpdate(PHwnd hwnd, PMessage msg)
{
	HeroUpdate();
	TerrianUpdate();
	GameStatusUpdate();
	pvcInvalidate(hwnd);
}

void HeroUpdate()
{
	//TODO
	//����λ��
	if (jumpFlag == 1)
	{
		if (jump <= 0)
		{
			jumpFlag = -1;
			jump += 2;
		}
		else
		{
			m_hero.pos.y -= jump;
			jump -= 2;
		}
	}
	else if (jumpFlag == -1)
	{
		if (jump > 16)
			jumpFlag = 0;
		else
		{
			m_hero.pos.y += jump;
			jump += 2;
		}
	}
	m_hero.pos.x = m_hero.pos.x >= WNDWIDTH ? 0 : m_hero.pos.x;
	//���¶���
	++m_hero.curFrameIndex;
	m_hero.curFrameIndex = m_hero.curFrameIndex >= m_hero.maxFrameSize ? 0 : m_hero.curFrameIndex;
}

void TerrianUpdate()
{
	//TODO
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		m_terrian[k].pos.x -= 3;
		if (m_terrian[k].pos.x + m_terrian[k].size.cx < 0)
		{
			m_terrian[k].pos.x += MAX_TERRIAN_NUM * m_terrian[k].size.cx;
		}
	}
}

void GameStatusUpdate()
{
	//TODO
	++m_gameStatus.totalDist;
}

bool Paused(PPoint ptMouse)
{
	if (ptMouse.x > m_gameStatus.pos.x && ptMouse.x < m_gameStatus.pos.x + m_gameStatus.size.cx)
		if (ptMouse.y > m_gameStatus.pos.y && ptMouse.y < m_gameStatus.pos.y + m_gameStatus.size.cy)
			return true;
	return false;
}

void KeyDown(PHwnd hwnd, PMessage msg)
{
	//TODO
	switch (msg.param)
	{
	case VK_UP:
		if (jumpFlag == 0)
		{
			jump = 15;
			jumpFlag = 1;
		}
		//m_hero.pos.y -= 50;
		pvcInvalidate(hwnd);
		break;
	case VK_DOWN:
		pvcInvalidate(hwnd);
		break;
	default:
		break;
	}
}

void KeyUp(PHwnd hwnd, PMessage msg)
{
	//TODO
	switch (msg.param)
	{
	case VK_UP:
		//m_hero.pos.y += 50;
		pvcInvalidate(hwnd);
		break;
	case VK_DOWN:
		pvcInvalidate(hwnd);
		break;
	default:
		break;
	}
}

void LButtonDown(PHwnd hwnd, PMessage msg)
{
	PPoint ptMouse;
	ptMouse.x = ((msg.param >> 16) & 0xffff) - hwnd->clientPos.x;
	ptMouse.y = (msg.param & 0xffff) - hwnd->clientPos.y;

	//�������˼�������ͣͼ��
	if (Paused(ptMouse))
	{
		if (!m_gameStatus.isPaused)
		{
			killTimer(hwnd, TIMER);
			m_gameStatus.isPaused = true;
		}
		else
		{
			setTimer(hwnd, TIMER, TIMER_INTERVAL);
			m_gameStatus.isPaused = false;
		}
		pvcInvalidate(hwnd);
	}
}
