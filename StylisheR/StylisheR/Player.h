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
};

