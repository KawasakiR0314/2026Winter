#include "Camera.h"
#include "Game.h"

Camera::Camera():
	m_pos(0.0f,0.0f)
{
}

void Camera::Update(const Vec2& targetCenter, float mapWidth, float mapHeight)
{
	// プレイヤーが画面の中央に来る位置
	float x = targetCenter.x - Game::kScreenWidth * 0.5f;
	float y = targetCenter.y - Game::kScreenHeight * 0.5f;

	// カメラが動ける(映る)範囲:0~
	float maxX = mapWidth - Game::kScreenWidth;
	float maxY = mapHeight - Game::kScreenHeight;

	// マップが画面より小さかったら固定する
	if (maxX < 0.0f)maxX = 0.0f;
	if (maxY < 0.0f)maxY = 0.0f;

	// カメラ範囲の中に収める
	if (x < 0.0f)x = 0.0f;
	if (x > maxX)x = maxX;
	if (y < 0.0f)y = 0.0f;
	if (y > maxY)y = maxY;

	// ブレ防止のため整数にする
	m_pos.x = static_cast<float>(static_cast<int>(x));
	m_pos.y = static_cast<float>(static_cast<int>(y));

}
