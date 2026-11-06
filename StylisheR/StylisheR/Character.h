#pragma once

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
	virtual void Draw()abstract;
	
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update()abstract;

protected:

	/// <summary>
	/// 重力計算
	/// </summary>
	void Gravity();

	/// <summary>
	/// 画像ハンドルを持つメンバ変数
	/// </summary>
	int m_handle;
};

