#pragma once
#include "Character.h"

enum class PlayerState
{
	Normal,		// 通常(歩き、ダッシュ、ジャンプ、空中)
	Crouch,		// しゃがみ
	WallKick,	// 壁キック直後
	HipDrop,	// ヒップドロップ(溜め+落下) 
	HipDropLand,// ヒップドロップ(着地時硬直)
};

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
	virtual void Draw(const Vec2& offset)override;

	/// <summary>
	/// プレイヤー更新処理
	/// </summary>
	virtual void Update()override;

private:

	/// <summary>
	/// プレイヤーのアクション状態の切り替え
	/// </summary>
	/// <param name="next"></param>
	void ChangeState(PlayerState next); 

	/// <summary>
	/// 通常状態の更新処理
	/// </summary>
	void UpdateNormal();

	/// <summary>
	/// しゃがみ状態の更新処理
	/// </summary>
	void UpdateCrouch();

	/// <summary>
	/// 壁キック直後の更新処理
	/// </summary>
	void UpdateWallKick();

	/// <summary>
	/// ヒップドロップ状態の更新処理
	/// </summary>
	void UpdateHipDrop();

	/// <summary>
	/// ヒップ直後の硬直状態の更新処理
	/// </summary>
	void UpdateHipDropLand();

	/// <summary>
	/// 左右の通常移動
	/// </summary>
	void Move();
	
	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// しゃがみ時、今の位置で立ち上がれるかどうか
	/// </summary>
	/// <returns></returns>
	bool CanStandUp();

	/// <summary>
	/// 蹴れる壁の向き
	/// </summary>
	int GetWallDir()const;

	/// <summary>
	/// 壁に向かって入力している間のずり落ち
	/// </summary>
	void WallSlide();

private:
	PlayerState m_state;
	int m_crouchHandle;		// しゃがみ時のプレイヤーの画像ハンドル
	int m_jumpBufferCount;	// ジャンプ先行入力の残りフレーム数
	int m_jumpStage;		// 次に跳ぶジャンプの段数(0~2)
	int m_jumpComboRemainCount;// 連続ジャンプの受付残りフレーム数
	bool m_isDash;			// ダッシュ中かどうか
	int m_inputDir;			// 左右の入力方向(-1:左, 0:なし, 1:右)
	int m_stateCount;		// 状態の残りフレーム数

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

