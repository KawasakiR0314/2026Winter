#include "Map.h"
#include "Rect.h"
#include <DxLib.h>
#include <fstream> // ファイル読み込み
#include <sstream> // 文字列を区切る
#include <string>
#include <cmath>

Map::Map()
{
	// テスト用に手書き（0:空き 1:ブロック）。あとでPlatinumのデータに置き換える
	m_tiles = {
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	};
}

Map::~Map()
{
}

void Map::Draw()
{
	for (int row = 0; row < static_cast<int>(m_tiles.size()); row++)
	{
		for (int col = 0; col < static_cast<int>(m_tiles[row].size()); col++)
		{
			if (m_tiles[row][col] == 0)continue; // 空きマスは描画しない
			
			int x = col * kTileSize;
			int y = row * kTileSize;
			if (m_tiles[row][col] == 1)
			{
				DrawBox(x, y, x + kTileSize, y + kTileSize, GetColor(180, 100, 40), true);
				DrawBox(x, y, x + kTileSize, y + kTileSize, GetColor(90, 50, 20), false);
			}
		}
	}
}

int Map::GetTile(int col, int row) const
{
	// 範囲外は空きマスとして扱う(バグ防止)
	if (row < 0 || row >= static_cast<int>(m_tiles.size()))
	{
		return 0;
	}
	if (col < 0 || col >= static_cast<int>(m_tiles[0].size()))
	{
		return 0;
	}
	return m_tiles[row][col];
}

bool Map::Load(const char* path)
{
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
		while (std::getline(ss, cell, ',')) // カンマ区切り
		{
			row.push_back(cell.empty() ? 0 : std::stoi(cell)); // 文字列を整数に変換して入れる
		}
		tiles.push_back(row);
	}
	m_tiles = tiles;
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
			if (GetTile(col, row) == 0) continue; // 空きマスは当たり判定がない

			Rect tileRect;
			tileRect.SetLT(static_cast<float>(col * kTileSize),
				static_cast<float>(row * kTileSize),
				static_cast<float>(kTileSize),
				static_cast<float>(kTileSize));
			if (rect.IsCollision(tileRect))
			{
				chipRect = tileRect; // 当たったマスの矩形を返す
				return true;
			}
		}
	}
	return false;
}
