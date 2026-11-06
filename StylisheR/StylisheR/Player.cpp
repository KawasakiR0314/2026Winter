#include "Player.h"
#include <DxLib.h>

Player::Player()
{
	// メンバ変数にプレイヤー画像を代入
	Character::m_handle = LoadGraph("asset/image/player.png");
}

Player::~Player()
{
	// 画像をメモリから解放
	DeleteGraph(m_handle);
}

void Player::Init()
{
}

void Player::End()
{
}

void Player::Draw()
{
	DrawGraph(150, 150, m_handle, true);
}

void Player::Update()
{
}
