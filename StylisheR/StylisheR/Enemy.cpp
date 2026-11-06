#include "Enemy.h"
#include <DxLib.h>

Enemy::Enemy()
{
	// メンバ変数にプレイヤー画像を代入
	Character::m_handle = LoadGraph("asset/image/enemy.png");
}

Enemy::~Enemy()
{
	// 画像をメモリから解放
	DeleteGraph(m_handle);
}

void Enemy::Init()
{
}

void Enemy::End()
{
}

void Enemy::Draw()
{
}

void Enemy::Update()
{
}
