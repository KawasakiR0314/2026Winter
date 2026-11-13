#pragma once
#include <vector>

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

	// マップチップ1マスの大きさ(ピクセル)
	static constexpr int kTileSize = 32;



private:
	// タイル番号 m_tiles[row][col]
	std::vector<std::vector<int>>m_tiles;
};

