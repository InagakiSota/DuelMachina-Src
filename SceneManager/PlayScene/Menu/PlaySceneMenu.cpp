///////////////////////////////
//プレイシーンのメニューのクラス
//作成日：2021年5月9日
//作成者：GT2_04_稲垣颯太
///////////////////////////////

#include "pch.h"
#include "PlaySceneMenu.h"
#include "../PlayScene.h"

#include "Adx2.h"
#include "CueSheet_0.h"
#include "../../SceneManager.h"

 //カーソルの座標
const DirectX::SimpleMath::Vector2 PlaySceneMenu::MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::OVER_ID)] =
{
   DirectX::SimpleMath::Vector2(630.0f,300.0f),
   DirectX::SimpleMath::Vector2(630.0f,500.0f),
   DirectX::SimpleMath::Vector2(630.0f,700.0f)
};
//コンストラクタ
PlaySceneMenu::PlaySceneMenu()
{
	m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);
	m_isMenuDisplay = false;
	m_pPlayScene = nullptr;
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

	//操作説明画面クラスのポインタ
	m_pPlaySceneManual = std::make_unique<PlaySceneManual>();
	//プレイシーンのポインタを渡す
	m_pPlaySceneManual->SetPlayScene(m_pPlayScene);
	//操作説明画面クラスの初期化
	m_pPlaySceneManual->Initialize();

}

////////////////////////
//更新
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneMenu::Update()
{
	if (m_pPlayScene->GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_pPlayScene->GetIsManualDisplay() == false)
	{
		//SE再生
		if (m_pPlayScene->GetIsStop() == false)
			ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);
		else if (m_pPlayScene->GetIsStop() == true)
			ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);

		bool isStop = m_pPlayScene->GetIsStop();

		m_pPlayScene->SetIsStop(!isStop);
	}

	//カーソルの座標設定
	switch (static_cast<eMENU_CURSOR>(m_menuCursor))
	{
		//マニュアル
	case eMENU_CURSOR::MANUAL:
	{
		m_pMenuCursorSprite->
			Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::MANUAL)].x - cosf(m_pPlayScene->GetTotalTime() * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::MANUAL)].y);
		break;
	}
	//キャラクターセレクト
	case eMENU_CURSOR::CHARA_SELECT:
	{
		m_pMenuCursorSprite->
			Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::CHARA_SELECT)].x - cosf(m_pPlayScene->GetTotalTime() * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::CHARA_SELECT)].y);
		break;
	}
	//ゲーム終了
	case eMENU_CURSOR::EXIT:
	{
		m_pMenuCursorSprite->
			Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::EXIT)].x - cosf(m_pPlayScene->GetTotalTime() * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::EXIT)].y);
		break;
	}
	default:
		break;
	}

	//キー入力でカーソル移動
	//上入力
	if (m_pPlayScene->GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Down)
		&& m_pPlayScene->GetIsStop() == true &&
		m_pPlayScene->GetIsManualDisplay() == false)
	{
		m_menuCursor++;
		if (m_menuCursor >= static_cast<int>(eMENU_CURSOR::OVER_ID))
		{
			m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);
		}

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	//下入力
	else if (m_pPlayScene->GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Up) && m_pPlayScene->GetIsStop() == true && m_pPlayScene->GetIsManualDisplay() == false)
	{
		m_menuCursor--;
		if (m_menuCursor <= static_cast<int>(eMENU_CURSOR::NONE))
		{
			m_menuCursor = static_cast<int>(eMENU_CURSOR::EXIT);
		}

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);
	}

	//スペースキー入力で決定
	if (m_pPlayScene->GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Space) && m_pPlayScene->GetIsStop() == true && m_pPlayScene->GetIsManualDisplay() == false)
	{
		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_SUBMIT);

		switch (static_cast<eMENU_CURSOR>(m_menuCursor))
		{
			//操作説明表示
		case eMENU_CURSOR::MANUAL:
		{
			if (m_pPlayScene->GetIsManualDisplay() == false)
			{
				m_pPlayScene->SetIsManualDisplay(true);
				m_pPlaySceneManual->SetIsManualDown(true);
			}
			break;
		}

		//キャラクターセレクトに戻る
		case eMENU_CURSOR::CHARA_SELECT:
		{
			SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
			//SE再生
			//ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
			//ADX2::GetInstance().Stop(GetSoundID());
			break;
		}

		//ゲーム終了
		case eMENU_CURSOR::EXIT:
		{
			PostQuitMessage(0);
			break;
		}

		default:
			break;
		}
	}

	//操作説明の表示フラグが立ったら操作説明の更新
	if (m_pPlayScene->GetIsManualDisplay() == true && m_pPlaySceneManual != nullptr)m_pPlaySceneManual->Update();

}

////////////////////////
//描画
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneMenu::Render()
{
	if (m_pPlayScene->GetIsStop() == true)
	{	
		//メニュー画像の描画
		m_pMenuSprite->Draw(true);
		//メニューのカーソル画像の描画
		m_pMenuCursorSprite->Draw(true);
	}
	//操作説明の画像の描画
	if (m_pPlayScene->GetIsManualDisplay() == true)
	{
		m_pPlaySceneManual->Render();
	}
}

////////////////////////
//終了処理
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneMenu::Finalize()
{
	//操作説明クラスの終了処理
	m_pPlaySceneManual->Finalize();

	//リソースの解放
	m_pMenuCursorSprite.reset();
	m_pMenuCursorSprite = nullptr;
	m_pMenuSprite.reset();
	m_pMenuSprite = nullptr;

}
