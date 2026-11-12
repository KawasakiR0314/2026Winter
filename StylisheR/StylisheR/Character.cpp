#include "Character.h"

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