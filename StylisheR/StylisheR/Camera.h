#pragma once
#include "Vec2.h"
#include "Game.h"

/// <summary>
/// 画面スクロールの管理クラス
/// </summary>
class Camera
{
public:
	Camera();

	/// <summary>
	/// プレイヤーに対してカメラの位置を更新する
	/// </summary>
	/// <param name="targetCenter">プレイヤーの位置(中心)</param>
	/// <param name="mapWidth">マップの横幅</param>
	/// <param name="mapHeight">マップの縦幅</param>
	void Update(const Vec2& targetCenter, float mapWidth, float mapHeight);

	/// <summary>
	/// カメラの位置
	/// </summary>
	const Vec2& GetPos()const { return m_pos; }

private:
	Vec2 m_pos;

};

