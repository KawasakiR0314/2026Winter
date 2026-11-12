#include "Player.h"
#include "Pad.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	constexpr int kGroundY = 700; // 地面のY座標
	constexpr float kMoveSpeed = 2.5f; // 通常移動速度
	constexpr float kDashSpeed = 5.0f; // ダッシュ速度
	constexpr float kAccel = 0.2f; // 入力中の1フレームの加速度
	constexpr float kFriction = 0.25f; // 非入力時の減速量
	constexpr float kBrake = 0.5f; // 逆向きに入力したときのブレーキ量
	constexpr float kJumpPower = 25.0f; // ジャンプ力
	constexpr float kJumpCutSpeed = 6.0f; // 離した時の上昇速度の上限
	constexpr float kJumpSpeedBonus = 6.0f; // ダッシュ中のジャンプ力上乗せ
	constexpr float kAirFriction = 6.0f; // 空中での非入力時の減速量
	constexpr int kJumpBufferFrame = 6; // 先行入力の受付フレーム

	/// <summary>
	/// 次向かう場所に向かって現在値を少しずつ近づける
	/// </summary>
	/// <param name="current">現在地</param>
	/// <param name="target">プレイヤーの移動先</param>
	/// <param name="step">近づける値</param>
	/// <returns></returns>
	float Approach(float current, float target, float step)
	{
		if (current < target)
		{
			current += step;
			if (current > target)
			{
				current = target;
			}
		}
		else if (current > target)
		{
			current -= step;
			if (current < target)
			{
				current = target;
			}
		}
		return current;
	}
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
//	m_move.x = 0.0f;
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
	// ダッシュで最高速度を切り替える
	float maxSpeed = kMoveSpeed;
	if (Pad::IsPress(PAD_INPUT_1))
	{
		maxSpeed = kDashSpeed;
	}

	// 入力から目標速度を決める
	float targetSpeed = 0.0f;
	
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		targetSpeed -= maxSpeed;
		m_isRight = false;
	}
	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		targetSpeed += maxSpeed;
		m_isRight = true;
	}

	// 入力時の加減速 ここで減速かブレーキかを判断
	float step = kAccel;
	if (targetSpeed == 0.0f)
	{
		// 非入力時の減速量を切り替える(地面接触時/空中滞在時)
		step = m_isGround ? kFriction : kAirFriction;
	}
	else if (m_move.x * targetSpeed < 0.0f)
	{
		step = kBrake;
	}
	m_move.x = Approach(m_move.x, targetSpeed, step);
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
			float jumpPower = kJumpPower+std::abs(m_move.x) * kJumpSpeedBonus;
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
