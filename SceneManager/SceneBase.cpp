///////////////////////////////////
//シーンの基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////
#include "pch.h"
#include "SceneBase.h"
#include "Src/Cgdi.h"

//コンストラクタ
SceneBase::SceneBase()
	:m_pDeviceResources(nullptr),
	m_pStates(nullptr),
	m_pKeyTracker(nullptr),
	m_fadeTimer(1.0f),
	m_sceneState(eSCENE_STATE::FADE_IN),
	m_view{},
	m_proj{},
	m_cameraPos{}
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
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

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
