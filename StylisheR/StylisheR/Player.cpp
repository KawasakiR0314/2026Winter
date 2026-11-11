#include "Player.h"
#include "Pad.h"
#include <DxLib.h>

namespace
{
	constexpr int kGroundY = 700; // 地面のY座標
	constexpr float kMoveSpeed = 2.5f; // 通常移動速度
	constexpr float kDashSpeed = 5.0f; // ダッシュ速度
	constexpr float kAccel = 0.2f; // 入力中の1フレームの加速度
	constexpr float kFriction = 0.25f; // 日入力の原則量
	constexpr float kJumpPower = 25.0f; // ジャンプ力
	constexpr float kJumpCutSpeed = 6.0f; // 離した時の上昇速度の上限
	constexpr int kJumpBufferFrame = 6; // 先行入力の受付フレーム
}

Player::Player():
	m_jumpBufferCount(0)
{
	// メンバ変数にプレイヤー画像を代入
	Character::m_handle = LoadGraph("asset/image/player.png");

	// キャラクターの横幅と縦幅を取得
	GetGraphSize(m_handle, &m_width, &m_height);

	m_pos = Vec2(150.0f, 636.0f); // 初期位置
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
	// float型の座標をint型にやって描画する
	if (m_isRight)
	{
		DrawGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_handle, true);
	}
	else
	{
		DrawTurnGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_handle, true);
	}

	DrawFormatString(0, 48, GetColor(255, 255, 255), "JUMP BUFFER:%d", m_jumpBufferCount);
}

void Player::Update()
{
	Gravity();
	Move();
	Jump();
	m_pos += m_move;

	// 地面との当たり判定
	m_isGround = false;
	if (m_pos.y+m_height >= kGroundY)
	{
		m_pos.y = kGroundY - m_height;
		m_move.y = 0.0f;
		m_isGround = true;
	}
}

void Player::Move()
{
	if (Pad::IsPress(PAD_INPUT_LEFT) && Pad::IsPress(PAD_INPUT_1))
	{
		m_move.x += -kDashSpeed;
		m_isRight = false;
	}
	else if (Pad::IsPress(PAD_INPUT_RIGHT) && Pad::IsPress(PAD_INPUT_1))
	{
		m_move.x += kDashSpeed;
		m_isRight = true;
	}
	else if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_move.x += -kMoveSpeed;
		m_isRight = false;
	}
	else if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_move.x += kMoveSpeed;
		m_isRight = true;
	}
}

void Player::Jump()
{
	// ボタンを押した瞬間にジャンプ入力を保存する
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		m_jumpBufferCount = kJumpBufferFrame;
	}

	// プレイヤーが地面についている際
	if (m_isGround)
	{
		// ジャンプ入力が残っていたら実行する
		if (m_jumpBufferCount>0)
		{
			m_move.y = -kJumpPower;
			m_isGround = false;
			m_jumpBufferCount = 0; // ジャンプ入力を消費する
		}
	}
	else
	{
		// ジャンプボタンを離したら、上への力をなくす
		if (Pad::IsRelease(PAD_INPUT_2) && m_move.y < -kJumpCutSpeed)
		{
			m_move.y = -kJumpCutSpeed;
		}

		// ジャンプ入力の残りフレーム数を減らす
		if (m_jumpBufferCount > 0)
		{
			m_jumpBufferCount--;
		}
	}
}

int Player::GetX()
{
	return m_pos.x;
}

int Player::GetY()
{
	return m_pos.y;
}
