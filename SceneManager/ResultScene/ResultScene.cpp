///////////////////////////////////
//リザルトシーンのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月28日
///////////////////////////////////

#include "pch.h"
#include "ResultScene.h"

#include "Keyboard.h"
#include "../SceneManager.h"
#include "Src\FrameWork\Sprite2D\Sprite2D.h"
#include "Src\FrameWork\FadeManager\FadeManager.h"
#include "Src/Cgdi.h"

ePLAYER_ID ResultScene::m_winPlayerID = ePLAYER_ID::PLAYER_1;

//コンストラクタ
ResultScene::ResultScene()
{
	//基底クラスのコンストラクタ
	SceneBase::SceneBase();
}

//デストラクタ
ResultScene::~ResultScene()
{
	//基底クラスのデストラクタ
	SceneBase::~SceneBase();
}

///////////////////////////
//初期化
//引数:ウィンドウ、デバイスのポインタ
//戻り値:なし
//////////////////////////
void ResultScene::Initialize()
{
	//基底クラスの初期化関数
	SceneBase::Initialize();


	SetFadeTimer(1.0f);
	SetSceneState(eSCENE_STATE::FADE_IN);

	//画像の読み込み
	m_winPlayer1Sprite = std::make_unique<Sprite2D>();
	m_winPlayer1Sprite->Create(L"Resources/Textures/p1win_result.png");
	m_winPlayer2Sprite = std::make_unique<Sprite2D>();
	m_winPlayer2Sprite->Create(L"Resources/Textures/p2win_result.png");
	m_winPlayer1Sprite->Update(640.0f, 300.0f);
	m_winPlayer2Sprite->Update(640.0f, 300.0f);

}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void ResultScene::Update(DX::StepTimer const & timer)
{
	//基底クラスの更新関数
	SceneBase::Update(timer);

	//ビュー行列の更新
	SetView( DirectX::SimpleMath::Matrix::CreateLookAt(GetCameraPos(), GetTargetPos(), DirectX::SimpleMath::Vector3::UnitY));
	//m_view = m_pDebugCamera->getViewMatrix();

	float time = float(timer.GetTotalSeconds());

	//シーンのステート
	switch (GetSceneState())
	{
		//フェードイン
		case eSCENE_STATE::FADE_IN:
		{
			//フェードのタイマーを減算する
			SetFadeTimer(GetFadeTimer() - static_cast<float>(timer.GetElapsedSeconds()) * 2.0f);
			//タイマーが0以下になったらメインステートに遷移
			if (GetFadeTimer() <= 0.0f)SetSceneState(eSCENE_STATE::MAIN);
			break;
		}
		//メイン
		case eSCENE_STATE::MAIN:
		{
			//スペースキー入力でフェードアウトに遷移
			if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Space))
			{
				SetSceneState(eSCENE_STATE::FADE_OUT);
			}

			break;
		}


		//フェードアウト
		case eSCENE_STATE::FADE_OUT:
		{
			//フェードのタイマーを加算する
			SetFadeTimer(GetFadeTimer() + static_cast<float>(timer.GetElapsedSeconds()) * 2.0f);
			//タイマーが規定値を越えたらキャラクターセレクトシーンに遷移
			if (GetFadeTimer() >= 1.0f)SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);

			break;
		}
		default:
			break;
	}

	//フェードマネージャーの更新
	FadeManager::GetInstance()->Update(timer, GetFadeTimer());
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void ResultScene::Render()
{
	if (m_winPlayerID == ePLAYER_ID::PLAYER_1)
	{
		m_winPlayer1Sprite->Draw(true, false);
	}
	else if (m_winPlayerID == ePLAYER_ID::PLAYER_2)
	{
		m_winPlayer2Sprite->Draw(true, false);
	}

}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void ResultScene::Finalize()
{
	m_winPlayer1Sprite->Reset();
	m_winPlayer2Sprite->Reset();

}
