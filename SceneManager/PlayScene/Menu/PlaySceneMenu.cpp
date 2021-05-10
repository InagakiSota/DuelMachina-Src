///////////////////////////////
//プレイシーンのメニューのクラス
//作成日：2021年5月9日
//作成者：GT2_04_稲垣颯太
///////////////////////////////

#include "pch.h"
#include "PlaySceneMenu.h"

//コンストラクタ
PlaySceneMenu::PlaySceneMenu()
{
	m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);

}

//デストラクタ
PlaySceneMenu::~PlaySceneMenu()
{
}

////////////////////////
//初期化
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneMenu::Initialize()
{
	//メニューの画像読み込み
	m_pMenuSprite = std::make_unique<Sprite2D>();
	m_pMenuSprite->Create(L"Resources/Textures/Menu.png");
	//カーソルの画像の読み込み
	m_pMenuCursorSprite = std::make_unique<Sprite2D>();
	m_pMenuCursorSprite->Create(L"Resources/Textures/menuCursol.png");


}

////////////////////////
//更新
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneMenu::Update()
{
}

////////////////////////
//描画
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneMenu::Render()
{
	m_pMenuSprite->Draw(true);
	m_pMenuCursorSprite->Draw(true);

}

////////////////////////
//終了処理
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneMenu::Finalize()
{
}
