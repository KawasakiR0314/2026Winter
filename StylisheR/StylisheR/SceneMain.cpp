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
	m_pMap->Load("assets/map/stage1.csv");
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
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
	DrawFormatString(0, 32, GetColor(255, 255, 255), "PLAYER:(%d,%d)", m_pPlayer->GetX(), m_pPlayer->GetY());
	DrawLine(0, 700, 1280, 700, GetColor(255, 255, 255));
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pMap->Draw();
}
