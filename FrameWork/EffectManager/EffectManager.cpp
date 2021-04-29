#include  "pch.h"
#include "EffectManager.h"
#include <PrimitiveBatch.h> 
#include <Effects.h>

using namespace DirectX::SimpleMath;

void EffectManager::Create(DX::DeviceResources * deviceResources)
{
	auto device = deviceResources->GetD3DDevice();

	//バッチエフェクトの作成
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);

	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_batchEffect->SetReferenceAlpha(0);
	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	//プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(deviceResources->GetD3DDeviceContext());
	
	//テクスチャのロード
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Black.png", nullptr,
		m_texture.GetAddressOf());
	

	int count = 1;
	for (int i = 0; i < count; i++) {
		std::unique_ptr<MyEffect> effect = std::make_unique<MyEffect>();
		//effect->Create(deviceResources, m_texture.Get(), m_batchEffect.get(), m_batch.get(), m_inputLayout.Get());
		m_effectList.push_back(std::move(effect));
	}

}

void EffectManager::Initialize(float life,Vector3 pos)
{
	int range = 100;

	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f);
		while (vel.Length() < 0.03f)
		{
			vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f);
		}

		//(*ite)->Initialize(life,pos,vel);
	}
}


void EffectManager::Update(DX::StepTimer timer)
{
	//timerを渡してm_effectの更新処理を行う
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Update(timer);
	}


}

void EffectManager::Render()
{
	//m_effectを描画する
	//timerを渡してm_effectの更新処理を行う
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		//(*ite)->Render();
	}
}

void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//camera,view,proj,の値をm_effectに渡す
	Matrix  mat = Matrix::Identity;
	for (std::list<std::unique_ptr<MyEffect>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		//(*ite)->SetRenderState(DirectX::SimpleMath::Vector3::Zero, mat, mat);
	}

}
