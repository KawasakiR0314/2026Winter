#include "Pad.h"
#include <DxLib.h>

namespace
{
	// 前のフレームのパッドの押し下げ状態
	int lastPad;
	// このフレームのパッドの押し下げ状態
	int nowPad;
}

namespace Pad
{
	void Pad::Update()
	{
		// 前のフレームに取得したパッド情報を１つ古い情報にする
		lastPad = nowPad;

		// 現在のパッドの情報を取得する
		nowPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool Pad::IsPress(int key)
	{
		// 現在のフレームで押されている
		return (nowPad&key);
	}

	bool Pad::IsTrigger(int key)
	{
		bool isNow = (nowPad & key); // 現在のフレーム
		bool isLast = (lastPad & key); // 前のフレーム

		// 現在のフレームに押されていて、
		// 前回のフレームに押されていない
		if (isNow && !isLast)
		{
			return true;
		}
		return false;
	}

	bool Pad::IsRelease(int key)
	{
		bool isNow = (nowPad & key); // 現在のフレーム
		bool isLast = (lastPad & key); // 前のフレーム

		// 現在のフレームに押されておらず、
		// 前回のフレームは押されていた
		if (!isNow && isLast)
		{
			return true;
		}
		return false;
	}

}
