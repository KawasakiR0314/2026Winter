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
	constexpr float kCrouchJumpSpeed = 1.5f; // しゃがみジャンプの横方向初速
	constexpr int kComboFrame = 10; // 三段ジャンプの受付フレーム
	constexpr float kAirFriction = 0.0f; // 空中での非入力時の減速量
	constexpr float kAirAccel = 0.3f; // 空中での入力時の加速度
	constexpr float kAirBrake = 0.25f; // 空中での逆方向入力時のブレーキ量
	constexpr int kJumpBufferFrame = 8; // 先行入力の受付フレーム
	constexpr float kWallKickSpeedX = 5.0f; // 壁キック時の横速度
	constexpr float kWallKickPowerY = 15.0f; // 壁キック時の上向きの威力
	constexpr int kWallKickLockFrame = 12; // 壁キック後に入力を無視するフレーム
	constexpr float kWallSlideSpeed = 4.0f; // ずり落ち中の、最大の落下速度
	constexpr float kWallSlideBrake = 4.0f; // 落下速度を上限まで落とす量(1f)
	constexpr int kHipDropWindupFrame = 12; // ヒップドロップの溜めのフレーム数
	constexpr float kHipDropSpeed = 22.0f; // ヒップドロップの落下速度
	constexpr int kHipDropLandFrame = 15; // 着地後に動けないフレーム数
	
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
	m_inputDir(0),
	m_crouchHandle(-1),
	m_stateCount(0),
	m_state(PlayerState::Normal)
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
	DeleteGraph(m_crouchHandle);
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

	int handle = m_state==PlayerState::Crouch ? m_crouchHandle : m_handle;
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
	DrawFormatString(32, 112, GetColor(255, 255, 255), "m_inputDir:%d", m_inputDir);
	DrawFormatString(32, 144, GetColor(255, 255, 255), "STATE:%d", static_cast<int>(m_state));
#endif		
}

void Player::Update()
{
	Gravity();
	// 今の状態に合わせ、処理の振り分けをする
	switch (m_state)
	{
	case PlayerState::Normal: UpdateNormal();
		break;
	case PlayerState::Crouch: UpdateCrouch();
		break;
	case PlayerState::WallKick: UpdateWallKick();
		break;
	case PlayerState::HipDrop: UpdateHipDrop();
		break;
	case PlayerState::HipDropLand: UpdateHipDropLand();
		break;
	}

	// 地面との当たり判定
	bool wasGround = m_isGround; // 1フレーム前の地面接触状態を保存
	CheckHitMap(); // 位置の更新+マップとの当たり判定

	// ヒップドロップで着地したら、一定f硬直状態にする
	if (m_state == PlayerState::HipDrop && m_isGround)
	{
		ChangeState(PlayerState::HipDropLand);
	}

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

void Player::ChangeState(PlayerState next)
{
	if (m_state == next)return;
	
	// 今の状態を出るときの処理
	if (m_state==PlayerState::Crouch)
	{
		// 立ち上がる(足元を固定し、頭の位置を上げる)
		m_pos.y -= m_height - kCrouchHeight;
		m_colHeight = static_cast<float>(m_height);
	}

	m_state = next;

	// 新しい状態に入るときの処理
	switch (m_state)
	{
	case PlayerState::Crouch:
		//しゃがむ(足元を固定し、頭の位置を下げる)
		m_pos.y += m_height - kCrouchHeight;
		m_colHeight = kCrouchHeight;
		break;
	case PlayerState::WallKick:
		m_stateCount = kWallKickLockFrame;
		break;
	case PlayerState::HipDrop:
		m_stateCount = kHipDropWindupFrame;
		m_jumpBufferCount = 0; // 古いジャンプ入力を捨てる
		m_jumpStage = 0; // n段ジャンプを途切れさせる
		break;
	case PlayerState::HipDropLand:
		m_stateCount = kHipDropLandFrame;
		break;
	default:
		break;
	}
}

void Player::UpdateNormal()
{
	// 空中で下を押した瞬間ヒップドロップ
	// しゃがみジャンプ時などの下をおしっぱのタイミングで
	// 勝手にヒップが実行されないためにIsTreggerにしておく
	if (!m_isGround && Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		ChangeState(PlayerState::HipDrop);
		UpdateHipDrop(); // 今のフレームからすぐに止める
		return;
	}

	// 地面にいる状態で下を押したらしゃがむ
	if (m_isGround && Pad::IsPress(PAD_INPUT_DOWN))
	{
		ChangeState(PlayerState::Crouch);
	}
	Move();
	WallSlide();
	Jump();
}

void Player::UpdateCrouch()
{
	// 下を離して、立ち上がれる状態ならば通常状態に戻る
	if (!Pad::IsPress(PAD_INPUT_DOWN) && CanStandUp())
	{
		ChangeState(PlayerState::Normal);
	}
	Move();
	Jump();
}

void Player::UpdateWallKick()
{
	// 入力を受け付けない
	m_inputDir = 0;
	m_isDash = false;

	m_stateCount--;
	if (m_stateCount <= 0)
	{
		ChangeState(PlayerState::Normal);
	}
}

void Player::UpdateHipDrop()
{
	m_inputDir = 0;
	m_isDash = false;

	if (m_stateCount > 0)
	{
		// 空中で止まる(溜め)
		m_stateCount--;
		m_move = Vec2(0.0f, 0.0f);
	}
	else
	{
		m_move.x = 0.0f;
		m_move.y = kHipDropSpeed;
	}
}

void Player::UpdateHipDropLand()
{
	m_inputDir = 0;
	m_isDash = false;
	m_move.x = 0.0f; // 滑らない

	m_stateCount--;
	if (m_stateCount <= 0)
	{
		ChangeState(PlayerState::Normal);
	}
}

void Player::Move()
{
	// ダッシュボタンが押されているか
	bool dashPress = Pad::IsPress(PAD_INPUT_1);
	
	// ダッシュで最高速度を切り替える
	float maxSpeed = dashPress ? kDashSpeed : kMoveSpeed;
	if (m_state==PlayerState::Crouch)
	{
		maxSpeed = m_isGround ? kCrouchSpeed : kMoveSpeed;
	}

	// 入力から目標速度を決める
	float targetSpeed = 0.0f;
	
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		targetSpeed -= maxSpeed;
		m_isRight = false;
		m_inputDir = -1;
	}
	else if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		targetSpeed += maxSpeed;
		m_isRight = true;
		m_inputDir = 1;
	}
	else
	{
		m_inputDir = 0;
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
			bool canCombo = m_isDash && m_state == PlayerState::Normal;

			// ダッシュ中でなければ必ず一段目
			int stage = m_isDash ? m_jumpStage : 0;

			// n段ごとのジャンプ力+ダッシュ中のジャンプ力上乗せ
			float jumpPower = kJumpPower[stage] + std::abs(m_move.x) * kJumpSpeedBonus;
			m_move.y = -jumpPower;
			if (m_state == PlayerState::Crouch && m_inputDir != 0)
			{
				m_move.x = m_inputDir * kCrouchJumpSpeed;
			}
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
		// 壁キック
		int wallDir = GetWallDir();
		if (wallDir != 0 && m_jumpBufferCount > 0 && m_state==PlayerState::Normal)
		{
			m_move.x = -wallDir * kWallKickSpeedX; // 壁と反対向きに飛ぶ
			m_move.y = -kWallKickPowerY;
			m_isRight = (wallDir < 0); // 壁が左にあるなら右向き
			ChangeState(PlayerState::WallKick);
			m_jumpBufferCount = 0; // ジャンプ入力の消費
			m_jumpStage = 0; // n段ジャンプを途切れさせる
			return;
		}

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

int Player::GetWallDir() const
{
	// 壁に向かって入力している時だけ、その側の画面を調べる
	if (m_inputDir != 0 && IsTouchingWall(m_inputDir))
	{
		return m_inputDir;
	}
	return 0;
}

void Player::WallSlide()
{
	// 空中にいる状態かつ非しゃがみ状態
	// 加えて壁に向かって入力している時
	if (m_isGround)return;
	if (GetWallDir() == 0)return;

	// 落下速度が上限を超えていたら少しずつ上限まで落とす
	if (m_move.y > kWallSlideSpeed)
	{
		m_move.y = Approach(m_move.y, kWallSlideSpeed, kWallSlideBrake);
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
