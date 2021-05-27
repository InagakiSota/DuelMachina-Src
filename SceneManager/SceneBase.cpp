///////////////////////////////////
//シーンの基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////
#include "pch.h"
#include "SceneBase.h"
#include "Src/Cgdi.h"
#include <random>

//コンストラクタ
SceneBase::SceneBase()
	:m_pDeviceResources(nullptr),
	m_pStates(nullptr),
	m_pKeyTracker(nullptr),
	m_fadeTimer(1.0f),
	m_sceneState(eSCENE_STATE::FADE_IN),
	m_view{},
	m_proj{},
	m_cameraPos{},
	m_targetPos{},
	m_isShake(false),
	m_soundID(0),
	m_shakeDuration(0.0f),
	m_shakeMagnitude(0.0f)
{
}

//デストラクタ
SceneBase::~SceneBase()
{
	m_pKeyTracker.reset();
	m_pDeviceResources = nullptr;
	m_pStates = nullptr;

}

///////////////////////////
//初期化
//引数:なし
//戻り値:なし
//////////////////////////
void SceneBase::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();
	m_pStates = gdi->GetStates();

	auto device = m_pDeviceResources->GetD3DDevice();
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	//キートラッカーの作成
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	//シーンのステート設定
	m_sceneState = eSCENE_STATE::FADE_IN;

	//ビュー行列を作成
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, m_targetPos, DirectX::SimpleMath::Vector3::UnitY);

	//画面サイズを取得する
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//射影行列を作成
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);

}

///////////////////////////
//更新
//引数:ステップタイマー
//戻り値:なし
//////////////////////////
void SceneBase::Update(DX::StepTimer const& timer)
{
	//キーボードの状態の取得
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);


	//if(m_isShake == false)
	//	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, m_targetPos, DirectX::SimpleMath::Vector3::UnitY);
	DoShake(timer);
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void SceneBase::Render()
{

}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void SceneBase::Finalize()
{

}

///////////////////////////
//カメラの振動
//引数:ステップタイマー、振動する時間、振動する強さ
//戻り値:なし
//////////////////////////
void SceneBase::CameraShake(float duration, float magnitude)
{
	if (m_isShake == false)
	{
		//カメラの振動を実行
		//DoShake(timer, duration, magnitude);
		m_isShake = true;

		//振動する時間の設定
		m_shakeDuration = duration;
		//振動する強さの設定
		m_shakeMagnitude = magnitude;
	}
}

///////////////////////////
//カメラの振動を実行
//引数:ス
//戻り値:
//////////////////////////
void SceneBase::DoShake(DX::StepTimer const& timer)
{
	////初期座標を取得
	//DirectX::SimpleMath::Vector3 pos = m_cameraPos;
	//DirectX::SimpleMath::Vector3 targetPos = m_targetPos;
	
	if (m_isShake == true)
	{
		//経過時間
		static float elapsed = 0.0f;
		//終了時間になるまで繰り返す

		//XとY方向にランダムに移動させる
		std::random_device rnd;
		
		float x = ((rnd() % 3) - 1.0f) * m_shakeMagnitude;
		float y = ((rnd() % 3) - 1.0f) * m_shakeMagnitude;
		
		//カメラ座標をずらす
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(x, y, 0.0f);
		////カメラの注視点をずらす
		DirectX::SimpleMath::Vector3 targetPos = DirectX::SimpleMath::Vector3(x, y, 0.0f);
		//m_targetPos = DirectX::SimpleMath::Vector3(targetPos.x + 0.1, targetPos.y, targetPos.z);
		//ビュー行列を作成
		m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
			m_cameraPos + pos, 
			m_targetPos + targetPos, 
			DirectX::SimpleMath::Vector3::UnitY);

		
		//経過時間を加算する
		elapsed += static_cast<float>(timer.GetElapsedSeconds());

		if (elapsed > m_shakeDuration)
		{
			//初期座標に戻す
			m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
				m_cameraPos,
				m_targetPos,
				DirectX::SimpleMath::Vector3::UnitY);
		 
			//カメラの振動フラグを消す
			m_isShake = false;
			elapsed = 0.0f;

			m_shakeDuration = 0.0f;
			m_shakeMagnitude = 0.0f;
		}
	}
}


