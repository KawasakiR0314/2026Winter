#include "SceneMain.h"
#include "Dxlib.h"
#include "Mouse.h"
#include "Pad.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"

SceneMain::SceneMain():
	m_frameCount(0) // 初期化子
{
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pMap = std::make_shared<Map>();
	m_pMap->Load("asset/map/TestMap.csv");
	m_pPlayer->SetMap(m_pMap.get());

	if (!m_pMap->LoadChipImage("asset/image/ground.png"))   // Platinumのチップ画像
	{
		printfDx("CHIP LOAD FAILED\n");
	}
	if (!m_pMap->Load("asset/map/Test.csv"))
	{
		printfDx("MAP LOAD FAILED\n");
	}
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_frameCount = 0;
}

void SceneMain::End()
{
}

void SceneMain::Update()
{
	m_frameCount++;
	Mouse::Update();
	Pad::Update();
	m_pPlayer->Update();
	m_pEnemy->Update();
}

void SceneMain::Draw()
{
	m_pMap->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
#ifdef _DEBUG
	//DrawString(32, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(32, 32, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
	DrawFormatString(32, 48, GetColor(255, 255, 255), "PLAYER:(%d,%d)", m_pPlayer->GetX(), m_pPlayer->GetY());
#endif
}
