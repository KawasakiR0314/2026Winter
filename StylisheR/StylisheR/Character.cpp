#include "Character.h"
#include "Map.h"
#include "Rect.h"

namespace
{
	constexpr float kGravity = 1.0f; // 重力
}

Character::Character():
	m_handle(-1),
	m_width(0),
	m_height(0),
	m_pos(0.0f,0.0f),
	m_move(0.0f,0.0f),
	m_isRight(true),
	m_isGround(true)
{
}

Character::~Character()
{
}

void Character::Gravity()
{
	m_move.y += kGravity;
}

void Character::CheckHitMap()
{
	Rect chipRect; // 当たったマスの矩形
	float w = static_cast<float>(m_width);
	float h = static_cast<float>(m_height);

	// マップとの当たり判定
	// 横:動かして当たったら押し戻す
	m_pos.x += m_move.x;
	m_colRect.SetLT(m_pos.x, m_pos.y, w, h);
	if (m_pMap->IsCollision(m_colRect, chipRect))
	{
		// 左右の押し戻し
		if (m_move.x > 0.0f)
		{
			m_pos.x = chipRect.GetLeft() - w; // 壁の左側に止める
		}
		else if (m_move.x < 0.0f)
		{
			m_pos.x = chipRect.GetRight(); // 壁の右側に止める
		}
		m_move.x;
	}

	// 縦:動かして当たったら押し戻す
	m_pos.y += m_move.y;
	m_colRect.SetLT(m_pos.x, m_pos.y, w, h);
	m_isGround = false;
	if (m_pMap->IsCollision(m_colRect, chipRect))
	{
		// 上下の押し戻し
		if (m_move.y > 0.0f)
		{
			m_pos.y = chipRect.GetTop() - h; // 床の上に立たせる
			m_isGround = true;
		}
		else if (m_move.y < 0.0f)
		{
			m_pos.y = chipRect.GetBottom(); // 天井の下で止める
		}
		m_move.y = 0.0f;
	}
}

