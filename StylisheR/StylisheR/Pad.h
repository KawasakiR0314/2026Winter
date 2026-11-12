#pragma once

/// <summary>
///  ゲームパッドの入力管理クラス
/// </summary>
namespace Pad
{
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 押されているかの判定
	/// </summary>
	/// <param name="key">入力キー</param>
	/// <returns>押されているかどうか</returns>
	bool IsPress(int key);

	/// <summary>
	/// 押された瞬間を取得
	/// </summary>
	/// <param name="key">入力キー</param>
	/// <returns>押された瞬間</returns>
	bool IsTrigger(int key);

	/// <summary>
	/// 離した瞬間を取得
	/// </summary>
	/// <param name="key">入力キー</param>
	/// <returns>離した瞬間</returns>
	bool IsRelease(int key);

};

