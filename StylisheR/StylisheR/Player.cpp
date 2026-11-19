#include "Player.h"
#include "Pad.h"
#include "Map.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	constexpr float kPlayerWidth = 32.0f; // プレイヤーの横幅
	constexpr float kPlayerHeight = 64.0f; // プレイヤーの縦幅
	constexpr float kCrouchHeight = 32.0f; // しゃがみ時縦幅
	constexpr float kMoveSpeed = 2.5f; // 通常移動速度
	constexpr float kDashSpeed = 7.0f; // ダッシュ速度
	constexpr float kCrouchSpeed = 0.0f; // ダッシュ速度
	constexpr float kAccel = 0.2f; // 入力中の1フレームの加速度
	constexpr float kFriction = 0.5f; // 非入力時の減速量
	constexpr float kBrake = 0.5f; // 逆向きに入力したときのブレーキ量
	constexpr float kJumpCutSpeed = 6.0f; // 離した時の上昇速度の上限
	constexpr float kJumpSpeedBonus = 0.6f; // ダッシュ中のジャンプ力上乗せ
	constexpr int kJumpMaxStage = 3; // ジャンプの最大連続回数
	constexpr float kJumpPower[kJumpMaxStage] =
	{ 15.0f, 17.5f, 20.0f }; // 三段ジャンプの威力
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
	m_jumpComboRemainCount(0),
	m_isDash(false),
	m_isCrouch(false),
	m_crouchHandle(-1)
{
	// メンバ変数にプレイヤー画像を代入
	Character::m_handle = LoadGraph("asset/image/player.png");
	m_crouchHandle = LoadGraph("asset/image/player_syagami.png");
	// キャラクターの横幅と縦幅を取得
	GetGraphSize(m_handle, &m_width, &m_height);
	m_colHeight = static_cast<float>(m_height); // 立っている時の判定の高さ
	m_pos = Vec2(240.0f, 900.0f); // 初期位置
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

void Player::Draw(const Vec2& offset)
{
	int x = static_cast<int>(m_pos.x - offset.x);
	int y = static_cast<int>(m_pos.y - offset.y);

	int handle = m_isCrouch ? m_crouchHandle : m_handle;
	// float型の座標をint型にsite描画する
	if (m_isRight)
	{
		DrawGraph(x, y, handle, true);
	}
	else
	{
		DrawTurnGraph(x, y, handle, true);
	}

#ifdef _DEBUG
	
	m_colRect.Draw(m_isRight ? GetColor(255, 0, 0) : GetColor(0, 255, 0), false, offset);
	DrawFormatString(32, 64, GetColor(255, 255, 255), "JUMP BUFFER:%d", m_jumpBufferCount);
	DrawFormatString(32, 80, GetColor(255, 255, 255), "NEXT JUMP:%d  INPUT FRAME:%d", m_jumpStage + 1, m_jumpComboRemainCount);
#endif		
}

void Player::Update()
{
	Gravity();
	Crouch();
	Move();
	Jump();

	// 地面との当たり判定
	bool wasGround = m_isGround; // 1フレーム前の地面接触状態を保存
	CheckHitMap(); // 位置の更新+マップとの当たり判定

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
	// ダッシュボタンが押されているか
	bool dashPress = Pad::IsPress(PAD_INPUT_1);
	
	// ダッシュで最高速度を切り替える
	float maxSpeed = dashPress ? kDashSpeed : kMoveSpeed;

	// しゃがみ中は移動不可
	if (m_isCrouch)
	{
		maxSpeed = kCrouchSpeed;
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

	// ダッシュボタンを押している、
	// かつ左右移動している時だけダッシュする
	m_isDash = dashPress && targetSpeed != 0.0f;

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
			// n段ジャンプの受付条件
			bool canCombo = m_isDash && !m_isCrouch;

			// ダッシュ中でなければ必ず一段目
			int stage = m_isDash ? m_jumpStage : 0;

			// n段ごとのジャンプ力+ダッシュ中のジャンプ力上乗せ
			float jumpPower = kJumpPower[stage] + std::abs(m_move.x) * kJumpSpeedBonus;
			m_move.y = -jumpPower;
			m_isGround = false;
			m_jumpBufferCount = 0; // ジャンプ入力を消費する

			if (canCombo)
			{
				// もしダッシュ中なら、ジャンプ段数を増やす
				m_jumpStage++;
				// ジャンプ段数が3を超えたらリセット
				if (m_jumpStage >= kJumpMaxStage)
				{
					m_jumpStage = 0;
				}
			}
			else
			{
				// ダッシュしていなければ、nを途切れさせる
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

void Player::Crouch()
{
	bool downPress = Pad::IsPress(PAD_INPUT_DOWN);

	if (!m_isCrouch)
	{
		// 地面についている、かつ下を押したらしゃがむ
		if (m_isGround && downPress)
		{
			m_isCrouch = true;
			// 足元の固定
			m_pos.y += m_height - kCrouchHeight;
			m_colHeight = kCrouchHeight;
		}
	}
	else
	{
		// 下を離す、かつ立ち上がれる状態だったら立ち上がる
		if (!downPress && CanStandUp())
		{
			m_isCrouch = false;
			m_pos.y -= m_height - kCrouchHeight;
			m_colHeight = static_cast<float>(m_height);
		}
	}
}

bool Player::CanStandUp()
{
	// 今の足元から、立った高さ分だけ上に伸ばした矩形の作成をする
	float feetY = m_pos.y + m_colHeight;
	Rect standRect;
	standRect.SetLT(m_pos.x, feetY - m_height,
		static_cast<float>(m_width), static_cast<float>(m_height));

	Rect dummy; // 当たったマスの情報は使わない
	return !m_pMap->IsCollision(standRect,dummy);
}

int Player::GetX()
{
	return m_pos.x;
}

int Player::GetY()
{
	return m_pos.y;
}
