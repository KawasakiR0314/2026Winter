#pragma once
#include <cmath>
#include <cassert>

class Vec2
{
public:
	float x;
	float y;

public:
	//　引数なしコンストラクタ 各成分を0で初期化
	Vec2() :Vec2(0.0f, 0.0f) {}
	// 実体生成時に同時に初期化も可能
	Vec2(float posX, float posY) :
		x(posX),
		y(posY)
	{
	}

	// ベクトルの長さの2乗を取得する
	float SqLength() const
	{
		return (x * x) + (y * y);
	}

	// ベクトルの長さを取得する
	float Length()const
	{
		return sqrtf(SqLength());
	}

	// 自身の正規化を行う
	void Normalize()
	{
		float len = Length();
		// 0除算回避
		if (len <= 0.0f) return;
		x /= len;
		y /= len;
	}

	// 自身の正規化したベクトルを所得する
	Vec2 GetNormalize()const
	{
		float len = Length();
		// 0除算回避
		if (len <= 0.0f) 
		{
			return Vec2(0.0f, 0.0f);
		}
		// 長さがの場合
		return Vec2(x/len,y/len);
	}

	// 演算子オーバーロード
	// -A
	Vec2 operator-()const { return{ -x,-y }; }
	// A+B
	Vec2 operator+(const Vec2& r)const { return{ x + r.x,y + r.y }; }
	// A-B
	Vec2 operator-(const Vec2& r)const { return{ x - r.x,y - r.y }; }
	// A*k
	Vec2 operator*(float k)const { return{ x * k,y * k }; }
	// A/k
	Vec2 operator/(float k)const
	{
		assert(k != 0.0f && "Vec2::operator/:0 division");
		return{ x / k,y / k };
	}
	// A+=B
	Vec2& operator+=(const Vec2& r)
	{
		x += r.x; y += r.y;
		return *this;
	}
	// A-=B
	Vec2& operator-=(const Vec2& r)
	{
		x -= r.x; y -= r.y;
		return *this;
	}
	// A*=k
	Vec2& operator*=(float k)
	{
		x *= k; y *= k;
		return *this;
	}
	// k*A
	friend Vec2 operator*(float k, const Vec2& v)
	{
		return{ v.x * k,v.y * k };
	}

};