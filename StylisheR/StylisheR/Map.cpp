#include "Map.h"
#include "Rect.h"
#include <DxLib.h>
#include <fstream> // ファイル読み込み
#include <sstream> // 文字列を区切る
#include <string>
#include <cmath>

Map::Map():
	m_chipHandle(-1),
	m_chipNumX(1)
{
	
}

Map::~Map()
{
	if (m_chipHandle != -1)
	{
		DeleteGraph(m_chipHandle); // マップ素材をメモリから解放
	}
}

void Map::Draw(const Vec2& offset)
{
	// 全てのマスを上から順に読んでいく
	for (int row = 0; row < static_cast<int>(m_tiles.size()); row++)
	{
		// 行ごとに列の数を調べる
		for (int col = 0; col < static_cast<int>(m_tiles[row].size()); col++)
		{
			int tile = m_tiles[row][col];
			if (tile < 0 || tile == kTileEmpty) continue;   // 空きマスは描かない

			int x = col * kTileSize - static_cast<int>(offset.x); // 画面に描画する位置(x)
			int y = row * kTileSize - static_cast<int>(offset.y); // 画面に描画する位置(y)

			// マップ用のチップ画像のどこを切り出すか
			int srcX = (tile % m_chipNumX) * kTileSize;
			int srcY = (tile / m_chipNumX) * kTileSize;
			DrawRectGraph(x, y, srcX, srcY, 
				kTileSize, kTileSize, m_chipHandle, true);
		}
	}
}

int Map::GetTile(int col, int row) const
{
	// 範囲外は空きマスとして扱う(バグ防止)
	if (row < 0 || row >= static_cast<int>(m_tiles.size()))
	{
		return kTileEmpty;
	}
	if (col < 0 || col >= static_cast<int>(m_tiles[row].size()))
	{
		return kTileEmpty;
	}
	return m_tiles[row][col];
}

bool Map::Load(const char* path)
{
	// ファイル(path)を開く
	std::ifstream file(path);
	if (!file.is_open())
	{
		return false; // ファイルが開けなかった場合
	}

	std::vector<std::vector<int>> tiles; // 読み込んだマップデータを一時的に入れる変数
	std::string line;
	while (std::getline(file, line)) // 1行ずつ読み込む
	{
		if (line.empty()) continue; // 空行は飛ばす
		std::vector<int> row;
		std::istringstream ss(line);
		std::string cell;
		// 一行の文字列をカンマで区切る
		while (std::getline(ss, cell, ','))
		{
			// 文字列"1"を整数1に変換して、1行分に追加する
			row.push_back(cell.empty() ? kTileEmpty : std::stoi(cell)); // 文字列を整数に変換して入れる
		}
		tiles.push_back(row);
	}
	m_tiles = tiles; // 読み込んだデータを置き換える
	return true;
}

bool Map::IsCollision(const Rect& rect, Rect& chipRect) const
{
	// 矩形が重なり合うマスの範囲を計算する
	int colMin = static_cast<int>(std::floor(rect.GetLeft() / kTileSize));
	int colMax = static_cast<int>(std::floor(rect.GetRight() / kTileSize));
	int rowMin = static_cast<int>(std::floor(rect.GetTop() / kTileSize));
	int rowMax = static_cast<int>(std::floor(rect.GetBottom() / kTileSize));

	// 範囲内にあるマスを確認する
	for (int row = rowMin; row <= rowMax; row++)
	{
		for (int col = colMin; col <= colMax; col++)
		{
			if (!IsSolid(col, row)) continue; // 空きマスは当たり判定がない

			// 調べるマスを矩形にして、矩形同士であったているかを調べる
			Rect tileRect;
			tileRect.SetLT(static_cast<float>(col * kTileSize),
				static_cast<float>(row * kTileSize),
				static_cast<float>(kTileSize),
				static_cast<float>(kTileSize));
			if (rect.IsCollision(tileRect))
			{
				// 当たったマスの矩形を、呼び出し元に渡す(押し戻しなどに使用)
				chipRect = tileRect; 
				return true;
			}
		}
	}
	return false;
}

bool Map::IsSolid(int col, int row) const
{
	// ブロックのチップ番号と同じなら壁
	return GetTile(col, row) == kTileBlock;
}

bool Map::LoadChipImage(const char* path)
{
	m_chipHandle = LoadGraph(path);
	if (m_chipHandle == -1)return false;

	int w = 0, h = 0;
	GetGraphSize(m_chipHandle, &w, &h);
	m_chipNumX = w / kTileSize; // 横に何個並んでいるか
	return true;
}

float Map::GetWidthPx() const
{
	if (m_tiles.empty())return 0.0f;
	return static_cast<float>(m_tiles[0].size() * kTileSize);
}

float Map::GetHeightPx() const
{
	return static_cast<float>(m_tiles.size() * kTileSize);
}
