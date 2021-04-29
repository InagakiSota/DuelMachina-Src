///////////////////////////////////
//ロード画面の管理クラス
//作成者：GS3_05_稲垣
//作成日：2021年3月25日
///////////////////////////////////

#include "pch.h"
#include "LoadDisplay.h"


LoadDisplay* LoadDisplay::m_pMyInstance = nullptr;

///////////////////////////
//初期化
//引数:DeviceResourcesのポインタ、CommonStatesのポインタ
//戻り値:なし
//////////////////////////
void LoadDisplay::Initialize(DX::DeviceResources * pDeviceResources, DirectX::CommonStates * pStates)
{
	m_pLoadSprite = std::make_unique<Sprite2D>();
	m_pLoadSprite->Create(pDeviceResources->GetD3DDevice(), pDeviceResources->GetD3DDeviceContext(),
		L"Resources/Textures/Load.png", pStates);

	
	m_color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

///////////////////////////
//更新
//引数:ロード画面の表示フラグ
//戻り値:なし
//////////////////////////
void LoadDisplay::Update(const bool & isLoad, float alpha)
{
	m_isLoadDisplay = isLoad;

	
	m_color = DirectX::SimpleMath::Vector4(1, 1, 1, alpha);
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void LoadDisplay::Render()
{
	if (m_isLoadDisplay == true)
	{
		m_pLoadSprite->Update(m_loadSpritePos);
		m_pLoadSprite->Draw(false,false, m_color);
	}
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void LoadDisplay::Finalize()
{
	m_pLoadSprite->Reset();
}

///////////////////////////
//インスタンスの取得
//引数:なし
//戻り値:LoadDisplayのポインタ
//////////////////////////
LoadDisplay * LoadDisplay::GetInstance()
{
	if (m_pMyInstance == nullptr)m_pMyInstance = new LoadDisplay();

	return m_pMyInstance;
}

///////////////////////////
//インスタンスの解放
//引数:なし
//戻り値:なし
//////////////////////////
void LoadDisplay::ReleaseInstance()
{
	if (m_pMyInstance != nullptr)delete m_pMyInstance;
	m_pMyInstance = nullptr;
}

//コンストラクタ
LoadDisplay::LoadDisplay()
	:m_loadSpritePos{},
	m_pLoadSprite(nullptr)
{
	
}

//デストラクタ
LoadDisplay::~LoadDisplay()
{
	m_pLoadSprite.reset();
}
