#include "Player.h"
#include "Pad.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	constexpr float kPlayerWidth = 32.0f; // プレイヤーの横幅
	constexpr float kPlayerHeight = 64.0f; // プレイヤーの縦幅
	constexpr float kMoveSpeed = 2.5f; // 通常移動速度
	constexpr float kDashSpeed = 5.0f; // ダッシュ速度
	constexpr float kAccel = 0.2f; // 入力中の1フレームの加速度
	constexpr float kFriction = 0.25f; // 非入力時の減速量
	constexpr float kBrake = 0.5f; // 逆向きに入力したときのブレーキ量
	constexpr float kJumpCutSpeed = 6.0f; // 離した時の上昇速度の上限
	constexpr float kJumpSpeedBonus = 0.6f; // ダッシュ中のジャンプ力上乗せ
	constexpr int kJumpMaxStage = 3; // ジャンプの最大連続回数
	constexpr float kJumpPower[kJumpMaxStage] =
	{ 20.0f, 22.5f, 500.0f }; // 三段ジャンプの威力
	constexpr int kComboFrame = 10; // 三段ジャンプの受付フレーム
	constexpr float kAirFriction = 0.0f; // 空中での非入力時の減速量
	constexpr float kAirAccel = 0.1f; // 空中での入力時の加速度
	constexpr float kAirBrake = 0.25f; // 空中での逆方向入力時のブレーキ量
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
	m_jumpBufferCount(0),
	m_jumpStage(0),
	m_jumpComboRemainCount(0)
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

#ifdef _DEBUG
	
	if (m_isRight)
	{
		DrawBox(m_pos.x, m_pos.y, 
			m_pos.x + kPlayerWidth,
			m_pos.y + kPlayerHeight,
			GetColor(255, 0, 0), false);
	}
	else
	{
		DrawBox(m_pos.x, m_pos.y, 
			m_pos.x + kPlayerWidth,
			m_pos.y + kPlayerHeight,
			GetColor(0, 0, 255), false);
	}

	DrawFormatString(0, 48, GetColor(255, 255, 255), "JUMP BUFFER:%d", m_jumpBufferCount);
	DrawFormatString(0, 64, GetColor(255, 255, 255), "NEXT JUMP:%d  INPUT FRAME:%d", m_jumpStage + 1, m_jumpComboRemainCount);
#endif		
}

void Player::Update()
{
	Gravity();
	Move();
	Jump();
	CheckHitMap(); // 位置の更新+マップとの当たり判定
	m_pos += m_move;

	m_colRect.SetCenter(m_pos.x, m_pos.y, kPlayerWidth, kPlayerHeight); // 当たり判定用の矩形を更新

	// 地面との当たり判定
	bool wasGround = m_isGround; // 1フレーム前の地面接触状態を保存
	m_isGround = false;

	// n段ジャンプの受付処理
	if (m_isGround && !wasGround)
	{
		// n段ジャンプの受付フレームをリセット
		m_jumpComboRemainCount = kComboFrame; 
	}

	// 地面に接触している間は、n段ジャンプの受付フレームを減らし
	// 連続ジャンプの段数をリセットする
	if (m_isGround && m_jumpComboRemainCount > 0)
	{
		m_jumpComboRemainCount--;
		if (m_jumpComboRemainCount == 0)
		{
			m_jumpStage = 0;
		}
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

	// 入力の種類に応じて、地面と空中で値を切り替える
	float step = 0.0f;
	if (targetSpeed == 0.0f)
	{
		// 非入力時の減速量を切り替える(地面接触時/空中滞在時)
		step = m_isGround ? kFriction : kAirFriction;
	}
	else if (m_move.x * targetSpeed < 0.0f)
	{
		// 逆方向に入力したときのブレーキ量を切り替える(地面接触時/空中滞在時)
		step = m_isGround ? kBrake : kAirBrake;
	}
	else
	{
		// 進行方向に入力したときの加速度を切り替える(地面接触時/空中滞在時)
		step = m_isGround ? kAccel : kAirAccel;
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
			// n段ごとのジャンプ力+ダッシュ中のジャンプ力上乗せ
			float jumpPower = kJumpPower[m_jumpStage] + std::abs(m_move.x) * kJumpSpeedBonus;
			m_move.y = -jumpPower;
			m_isGround = false;
			m_jumpBufferCount = 0; // ジャンプ入力を消費する

			// ジャンプ段数を増やす
			m_jumpStage++;
			// ジャンプ段数が3を超えたらリセット
			if (m_jumpStage >= kJumpMaxStage)
			{
				m_jumpStage = 0;
			}
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
