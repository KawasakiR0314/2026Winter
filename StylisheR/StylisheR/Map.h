#pragma once
#include <vector>

class Rect;

/// <summary>
/// マップチップを管理するクラス
/// </summary>
class Map
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Map();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Map();

	/// <summary>
	/// マップの描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 指定のタイル番号を取得
	/// </summary>
	int GetTile(int col, int row)const;

	/// <summary>
	/// Platinumで出力したcsvファイルからマップを読み込む
	/// </summary>
	/// <param name="path">ファイルのパス</param>
	/// <returns>読み込めたかどうか</returns>
	bool Load(const char* path);

	/// <summary>
	/// 指定した矩形とブロックが当たっているか判定する
	/// </summary>
	/// <param name="rect">調べる矩形</param>
	/// <param name="chipRect">当たったマスの矩形</param>
	/// <returns>当たっているか</returns>
	bool IsCollision(const Rect& rect, Rect& chipRect)const;

	/// <summary>
	/// マスが当たり判定のあるマスかどうか
	/// </summary>
	/// <param name="col">列(横のマス番号)</param>
	/// <param name="row">行(縦のマス番号)</param>
	/// <returns>当たり判定があるならtrue</returns>
	bool IsSolid(int col, int row)const;

	/// <summary>
	/// マップ用のチップ画像の読み込み
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool LoadChipImage(const char* path);

	// マップチップ1マスの大きさ(ピクセル)
	static constexpr int kTileSize = 32;

	// csvファイルのブロック番号
	static constexpr int kTileBlock = 0;
	static constexpr int kTileEmpty = 1;

private:
	// タイル番号 m_tiles[row][col]([行][列])
	std::vector<std::vector<int>>m_tiles;
	int m_chipHandle;   // チップシートの画像
	int m_chipNumX;      // シートの横に並ぶチップの数
};

