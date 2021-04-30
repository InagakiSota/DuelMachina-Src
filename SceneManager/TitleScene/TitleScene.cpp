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
	m_pDeviceResources = nullptr;
	m_pKeyTracker = nullptr;

	m_sceneState = eSCENE_STATE::FADE_IN;
	m_fadeTimer = 1.0f;

	m_pLogoSprite = nullptr;
	m_pSpaceSprite = nullptr;

	m_pushSpaceBlindFlag = true;
	m_pushSpaceBlindTimer = 0.0f;

	m_skyBoxAngleY = 0.0f;
}


TitleScene::~TitleScene()
{
	m_pKeyTracker.reset();
	m_pDeviceResources = nullptr;

	m_pLogoSprite.reset();
	m_pSpaceSprite.reset();

	m_pStates = nullptr;
}


///////////////////////////
//初期化
//引数:デバイスリソースのポインタ
//戻り値:なし
//////////////////////////
void TitleScene::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();

	m_pStates = gdi->GetStates();

	auto device = m_pDeviceResources->GetD3DDevice();
	auto context = m_pDeviceResources->GetD3DDeviceContext();


	DebugFont::GetInstance()->Create(device, context);

	//キートラッカーの作成
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	m_sceneState = eSCENE_STATE::FADE_IN;

	//ビュー行列を作成
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

	//画面サイズを取得する
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//射影行列を作成
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);

	//ロゴ画像の読み込み
	m_pLogoSprite = std::make_unique<Sprite2D>();
	m_pLogoSprite->Create(L"Resources/Textures/logo.png");
	//pushSpaceの画像の読み込み
	m_pSpaceSprite = std::make_unique<Sprite2D>();
	m_pSpaceSprite->Create(L"Resources/Textures/pushSpace.png");

	//天球のモデル読み込み
	m_pSkyBox = std::make_unique<ModelObject>();
	m_pSkyBox->Create(m_pDeviceResources, L"Resources/Models/Space.cmo");

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
	//キーボードの状態の取得
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	m_pSpaceSprite->Update(PUSH_SPACE_POS);

	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(SKYBOX_POS);
	m_skyBoxAngleY += 0.001f;
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_skyBoxAngleY);

//	m_skyBoxWorld *= rotY;// *pos;
	//m_skyBoxWorld *= rotY;// *pos;
	//m_skyBoxWorld += pos;
	//m_skyBoxWorld *= rotY; 
	//m_skyBoxWorld += pos;
	m_skyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero) * rotY * pos;

	//m_skyBoxWorld *= rotY;

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
	switch (m_sceneState)
	{
		//フェードイン
		case SceneBase::eSCENE_STATE::FADE_IN:
		{
			m_fadeTimer -= static_cast<float>(timer.GetElapsedSeconds())* 2.0f;
			if (m_fadeTimer <= 0.0f)m_sceneState = eSCENE_STATE::MAIN;

			break;
		}
		//メイン
		case SceneBase::eSCENE_STATE::MAIN:
		{
			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
			{
				m_sceneState = eSCENE_STATE::FADE_OUT;

				soundID = ADX2::GetInstance().Play(CRI_CUESHEET_0_SUBMIT);
			}

			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape))
			{
				PostQuitMessage(0);
			}


			break;
		}
		//フェードアウト
		case SceneBase::eSCENE_STATE::FADE_OUT:
		{
			m_fadeTimer += static_cast<float>(timer.GetElapsedSeconds())* 2.0f;
			if(m_fadeTimer >= 1.0f)SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);

			break;
		}
		default:
			break;
	}
	
	//フェードマネージャーの更新
	FadeManager::GetInstance()->Update(timer, m_fadeTimer);
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void TitleScene::Render()
{
	//天球の描画
	m_pSkyBox->Render(m_view, m_proj);

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
	DebugFont::Dispose();

	m_pSkyBox->Lost();

	m_pLogoSprite->Reset();
	m_pSpaceSprite->Reset();
}






