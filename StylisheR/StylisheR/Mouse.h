#pragma once

namespace Mouse
{
	// 毎フレームの更新処理
	// 1フレームに1回だけ呼び出す
	void Update();

	// 左クリックが押された瞬間
	bool IsTriggerLeft();
	// 上記の右ver
	bool IsTriggerRight();
	
	// 今左クリックが押されているかどうか
	bool IsPressLeft();
	// 上記の右ver
	bool IsPressRight();

	// 座標の取得
	int GetX();
	int GetY();
}

