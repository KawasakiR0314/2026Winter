#pragma once
#include "Character.h"

/// <summary>
/// エネミーの実装を行うクラス
/// </summary>
class Enemy:public Character
{
public:
	/// <summary>
	/// エネミーコンストラクタ
	/// </summary>
	Enemy();
	
	/// <summary>
	/// エネミーデストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// エネミー初期化処理
	/// </summary>
	virtual void Init()override;

	/// <summary>
	/// エネミー終了処理
	/// </summary>
	virtual void End()override;

	/// <summary>
	/// エネミー描画処理
	/// </summary>
	virtual void Draw()override;

	/// <summary>
	/// エネミー更新処理
	/// </summary>
	virtual void Update()override;
};

