#include "DxLib.h"
#include "Game.h"
#include <memory>

#include "SceneMain.h"
#include "Mouse.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	
	ChangeWindowMode(1); // windowモードで起動

	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kScreenDepth); // ウィンドウサイズを16：9に

	SetMainWindowText("DxLibTemplate"); // ウィンドウのタイトル表示を変更


	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK); // 裏画面を描画対象にする

//	SceneMain* pScene = new SceneMain(); // ヒープ領域にメモリを確保
//	pScene->Init(); // アロー演算子を使用してメンバ関数にアクセスする
//	delete pScene; // newを使用して明示的に確保したメモリは、deleteを使用して削除する

	// ゲームシーンの作成
	std::shared_ptr<SceneMain> pScene=std::make_shared<SceneMain>();
	pScene->Init();


	while (ProcessMessage() ==0) 
	{
		// 現在のフレーム開始時刻を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 画面をクリア
		ClearDrawScreen();

		// ゲームの処理
		pScene->Update();

		pScene->Draw();
		
		// 画面の書き換えを待つ
		ScreenFlip();

		// Escキーを押したらゲームを強制終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// 画面のリフレッシュレートにかかわらず1/60経過するまで待つ
		while (GetNowHiPerformanceCount() - start < 16667)
		{

			// 時間経過まで何もしない
		}

	}

	pScene->End();
	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}