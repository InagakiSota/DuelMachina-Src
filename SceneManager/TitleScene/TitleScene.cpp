///////////////////////////////////
//タイトルシーンのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////

#include "pch.h"
#include "TitleScene.h"

#include "../SceneManager.h"
#include "Keyboard.h"
#include "Src\FrameWork\DebugFont\DebugFont.h"

#include "Src\FrameWork\Sprite2D\Sprite2D.h"
#include "Src\FrameWork\FadeManager\FadeManager.h"
#include "Game.h"

#include "Adx2.h"
#include "CueSheet_0.h"
#include "Src/Cgdi.h"


//pushSpaceの画像の座標
const DirectX::SimpleMath::Vector2 TitleScene::PUSH_SPACE_POS = DirectX::SimpleMath::Vector2(700.0f,700.0f);
//pushSpaceの画像の点滅インターバル
const float TitleScene::PUSH_SPACE_BLIND_INTERBVAL = 1.0f;
//天球の座標
const DirectX::SimpleMath::Vector3 TitleScene::SKYBOX_POS = DirectX::SimpleMath::Vector3(0.0f, 20.0f, 0.0f);

TitleScene::TitleScene()
{
	//基底クラスのコンストラクタ
	SceneBase::SceneBase();

	m_pLogoSprite = nullptr;
	m_pSpaceSprite = nullptr;

	m_pushSpaceBlindFlag = true;
	m_pushSpaceBlindTimer = 0.0f;

	m_skyBoxAngleY = 0.0f;
}


TitleScene::~TitleScene()
{
	//基底クラスのデストラクタ
	SceneBase::~SceneBase();

	m_pLogoSprite.reset();
	m_pSpaceSprite.reset();

}


///////////////////////////
//初期化
//引数:デバイスリソースのポインタ
//戻り値:なし
//////////////////////////
void TitleScene::Initialize()
{
	//基底クラスの初期化関数
	SceneBase::Initialize();

	//ロゴ画像の読み込み
	m_pLogoSprite = std::make_unique<Sprite2D>();
	m_pLogoSprite->Create(L"Resources/Textures/logo.png");
	//pushSpaceの画像の読み込み
	m_pSpaceSprite = std::make_unique<Sprite2D>();
	m_pSpaceSprite->Create(L"Resources/Textures/pushSpace.png");

	//天球のモデル読み込み
	m_pSkyBox = std::make_unique<ModelObject>();
	m_pSkyBox->Create(GetDeviceResources(), L"Resources/Models/Space.cmo");
	//天球のワールド行列設定
	m_skyBoxWorld = DirectX::SimpleMath::Matrix::Identity;

	//pushSpaceの画像の点滅フラグ初期化
	m_pushSpaceBlindFlag = true;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void TitleScene::Update(DX::StepTimer const& timer)
{
	//基底クラスの更新
	SceneBase::Update(timer);

	//スペース入力の画像更新
	m_pSpaceSprite->Update(PUSH_SPACE_POS);

	//天球を回転させる
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(SKYBOX_POS);
	m_skyBoxAngleY += 0.001f;
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_skyBoxAngleY);
	//天球のワールド行列の設定
	m_skyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero) * rotY * pos;

	//天球のモデルの更新
	m_pSkyBox->Update(m_skyBoxWorld);

	////pushSpaceの画像の点滅タイマー。フラグの更新
	m_pushSpaceBlindTimer += static_cast<float>(timer.GetElapsedSeconds());
	if (m_pushSpaceBlindTimer >= PUSH_SPACE_BLIND_INTERBVAL)
	{
		m_pushSpaceBlindFlag = !m_pushSpaceBlindFlag;
		m_pushSpaceBlindTimer = 0.0f;
	}
	
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
				//SE再生
				SetSoundID(ADX2::GetInstance().Play(CRI_CUESHEET_0_SUBMIT));
			}
			//ESC入力でゲームを終了する
			if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Escape))
			{
				PostQuitMessage(0);
			}

			break;
		}
		//フェードアウト
		case eSCENE_STATE::FADE_OUT:
		{
			//フェードのタイマーを加算する
			SetFadeTimer(GetFadeTimer() + static_cast<float>(timer.GetElapsedSeconds()) * 2.0f);
			//タイマーが規定値を越えたらキャラクターセレクトシーンに遷移
			if(GetFadeTimer() >= 1.0f)SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);

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
void TitleScene::Render()
{
	//天球の描画
	m_pSkyBox->Render(GetView(), GetProj());

	//ロゴ画像の描画
	m_pLogoSprite->Draw(true);
	//pushSpaceの画像の描画
	if(m_pushSpaceBlindFlag)m_pSpaceSprite->Draw(true);
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void TitleScene::Finalize()
{
	m_pSkyBox->Lost();

	m_pLogoSprite->Reset();
	m_pSpaceSprite->Reset();
}






