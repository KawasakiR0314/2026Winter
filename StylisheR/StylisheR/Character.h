#pragma once
#include "Vec2.h"
#include "Rect.h"

class Map;

/// <summary>
/// プレイヤー、敵の基底クラス
/// </summary>
class Character
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Character();

	// 継承元のデストラクタにはvirtualをつける
	// →正しく動作しない可能性があるため
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Character();

	// 継承先のクラスで上書きする関数は全て仮想関数(virtual)にしておく
	// 基底クラスのメンバー関数は呼ばれない
	// 継承先で実装を行う前提の関数は純粋仮想関数(abstract;)にしておく
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() abstract; 
	
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End()abstract;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const Vec2& offset)abstract;
	
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update()abstract;

	bool IsGround() const { return m_isGround; }

	void SetMap(Map* map) { m_pMap = map; }

	Vec2 GetCenter()const;

protected:

	/// <summary>
	/// 重力計算
	/// </summary>
	void Gravity();

	/// <summary>
	/// 画像ハンドルを持つメンバ変数
	/// </summary>
	int m_handle;

	/// <summary>
	/// 画像ハンドルの横幅
	/// </summary>
	int m_width;

	/// <summary>
	/// 画像ハンドルの縦幅
	/// </summary>
	int m_height;

	/// <summary>
	/// しゃがみ時の縦幅
	/// </summary>
	float m_colHeight;

	/// <summary>
	/// 座標情報
	/// </summary>
	Vec2 m_pos;

	/// <summary>
	/// 移動量
	/// </summary>
	Vec2 m_move;

	/// <summary>
	/// 右を向いているか
	/// </summary>
	bool m_isRight;

	/// <summary>
	/// 地面についているかどうか
	/// </summary>
	bool m_isGround;

	/// <summary>
	/// 指定した壁の向きに触れているか
	/// </summary>
	/// <param name="dir">-1:左,1:右</param>
	/// <returns></returns>
	bool IsTouchingWall(int dir)const;

	/// <summary>
	/// 位置の更新とマップとの当たり判定
	/// </summary>
	void CheckHitMap();

	Rect m_colRect; // 当たり判定用の矩形
	Map* m_pMap; // マップのポインタ
};

