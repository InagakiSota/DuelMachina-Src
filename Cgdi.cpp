///////////////////////////////////
//DirectXTKで描画の際に必要なインスタンスを管理するクラス
//作成者：2021年4月30日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#include "pch.h"
#include "Cgdi.h"

// コンストラクタ
Cgdi::Cgdi()
	: m_deviceResources(nullptr), m_screenW(0), m_screenH(0)
{
}

//デストラクタ
Cgdi::~Cgdi()
{
	m_deviceResources = nullptr;
	m_states = nullptr;

	m_spriteBatch.reset();
}

///////////////////////////
//自身のインスタンスの取得
//引数:なし
//戻り値:自身のインスタンスのポインタ
//////////////////////////
Cgdi* Cgdi::GetInstance()
{
	static Cgdi instance;
	return &instance;
}

///////////////////////////
//初期化
//引数:デバイスリソース、画面の幅、高さ
//戻り値:なし
//////////////////////////
void Cgdi::Initialize(DX::DeviceResources* deviceResources, DirectX::CommonStates* commonStates, float width, float height)
{
	//画面サイズの設定
	SetScreenSize(width, height);

	assert(m_deviceResources == nullptr);

	m_deviceResources = deviceResources;

	m_states = commonStates;

	auto device = m_deviceResources->GetD3DDevice();
	auto deviceContext = m_deviceResources->GetD3DDeviceContext();

	// コモンステートの生成
	//m_states = std::make_unique<DirectX::CommonStates>(device);

	//スプライトバッチの生成
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext);


}

///////////////////////////
//スクリーンサイズの設定
//引数:幅、高さ
//戻り値:なし
//////////////////////////
void Cgdi::SetScreenSize(float width, float height)
{
	m_screenW = width;
	m_screenH = height;
}

///////////////////////////
//スクリーンサイズの取得
//引数:幅、高さ
//戻り値:なし
//////////////////////////
void Cgdi::GetScreenSize(float& width, float& height)
{
	width = m_screenW;
	height = m_screenH;
}
