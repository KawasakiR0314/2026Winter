#pragma once
#include "Character.h"

/// <summary>
/// プレイヤーの実装を行うクラス
/// </summary>
class Player:public Character
{
public:
	/// <summary>
	/// プレイヤーコンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// プレイヤーデストラクタ
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// プレイヤー初期化処理
	/// </summary>
	virtual void Init()override;

	/// <summary>
	/// プレイヤー終了処理
	/// </summary>
	virtual void End()override;

	/// <summary>
	/// プレイヤー描画処理
	/// </summary>
	virtual void Draw()override;

	/// <summary>
	/// プレイヤー更新処理
	/// </summary>
	virtual void Update()override;

private:

	/// <summary>
	/// 左右の通常移動
	/// </summary>
	void Move();
	
	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// ジャンプ先行入力の残りフレーム数
	/// </summary>
	int m_jumpBufferCount;

public:
	/// <summary>
	/// プレイヤーのX座標の取得
	/// </summary>
	/// <returns>現在のプレイヤーのX座標</returns>
	int GetX();

	/// <summary>
	/// プレイヤーのY座標の取得
	/// </summary>
	/// <returns>現在のプレイヤーのY座標</returns>
	int GetY();
};

