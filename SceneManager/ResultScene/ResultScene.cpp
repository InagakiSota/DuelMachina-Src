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
}

//デストラクタ
ResultScene::~ResultScene()
{
	m_pStates = nullptr;
}

///////////////////////////
//初期化
//引数:ウィンドウ、デバイスのポインタ
//戻り値:なし
//////////////////////////
void ResultScene::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();

	m_pStates = gdi->GetStates();

	auto device = m_pDeviceResources->GetD3DDevice();

	// TODO: Initialize device dependent objects here (independent of window size).
	device;

	auto context = m_pDeviceResources->GetD3DDeviceContext();

	//ビュー行列を作成する
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.f, 5.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

	//画面サイズを取得する
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//射影行列を作る
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);

	//m_pDebugCamera = std::make_unique<DebugCamera>();

	m_fadeTimer = 1.0f;
	m_sceneState = eSCENE_STATE::FADE_IN;

	//キートラッカーの作成
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

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
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//ビュー行列の更新
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	//m_view = m_pDebugCamera->getViewMatrix();

	float time = float(timer.GetTotalSeconds());

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
			////床のワールド行列の更新
			//Matrix boxScale = Matrix::CreateScale(Vector3(1, 1, 1));
			//Matrix boxTrans = Matrix::CreateTranslation(Vector3(1.5f, 0, 0));
			//Matrix boxRotY = Matrix::CreateRotationY(time);

			//cupWorld = boxScale  * boxRotY* boxTrans;

			//static float rot = 0;
			//rot += 0.01f;
			//Matrix effectRotY = Matrix::CreateRotationY(rot);
			//Matrix effectTrans = Matrix::CreateTranslation(0.0f, 1.0f, -1.0f);
			//Matrix effectScale = Matrix::CreateScale(2.0f, 2.0f, 0.0f);

			//Matrix effectWorld = effectScale * effectRotY * effectTrans;

	


			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
			{
				m_sceneState = eSCENE_STATE::FADE_OUT;
			}

			break;
		}


		//フェードアウト
		case SceneBase::eSCENE_STATE::FADE_OUT:
		{
			m_fadeTimer += static_cast<float>(timer.GetElapsedSeconds())* 2.0f;

			if (m_fadeTimer >= 1.0f)SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);

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
