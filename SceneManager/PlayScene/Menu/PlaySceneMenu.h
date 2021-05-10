///////////////////////////////
//プレイシーンのメニューのクラス
//作成日：2021年5月9日
//作成者：GT2_04_稲垣颯太
///////////////////////////////

#pragma once

#include <SimpleMath.h>
#include "Src/FrameWork/Sprite2D/Sprite2D.h"

class PlayScene;
class PlaySceneManual;

//メニューのカーソルの場所
enum class eMENU_CURSOR
{
	NONE = -1,

	MANUAL,			//操作説明
	CHARA_SELECT,	//キャラクターセレクト
	EXIT,			//ゲーム終了

	OVER_ID
};

class PlaySceneMenu
{
public:
	//コンストラクタ
	PlaySceneMenu();
	//デストラクタ
	~PlaySceneMenu();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Render();
	//終了処理
	void Finalize();
	//プレイシーンのポインタ設定
	void SetPlayScene(PlayScene* pPlayScene)
	{
		m_pPlayScene = pPlayScene;
	}


private:


	//プレイシーンのポインタ
	PlayScene* m_pPlayScene;
	//メニューの画像
	std::unique_ptr<Sprite2D> m_pMenuSprite;
	//メニューのカーソル画像
	std::unique_ptr<Sprite2D> m_pMenuCursorSprite;
	//メニュー表示フラグ
	bool m_isMenuDisplay;

	int m_menuCursor;
	//カーソルの座標
	static const DirectX::SimpleMath::Vector2 MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::OVER_ID)];
	//操作説明クラスのポインタ
	std::unique_ptr<PlaySceneManual> m_pPlaySceneManual;
};

