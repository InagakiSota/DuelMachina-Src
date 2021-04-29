///////////////////////////////////
//エフェクト管理クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月23日
///////////////////////////////////
#pragma once

#include "../../DeviceResources.h"
#include "../../StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include "../MyEffect/MyEffect.h"

class EffectManager
{
public:
	//コンストラクタ
	EffectManager()
	{

	}
	//デストラクタ
	~EffectManager()
	{
		m_batchEffect.reset();
		m_effectList.clear();
		m_batch.reset();
	}

	void Create(DX::DeviceResources* deviceResources);
	//Create()で全部処理は長いので分割
	void Initialize(float life,DirectX::SimpleMath::Vector3 pos);

	void Update(DX::StepTimer timer);
	void Render();

	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

private:
	//バッチエフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;


	//エフェクトのポインタ
	//MyEffect*	m_effect;
	std::list<std::unique_ptr<MyEffect>> m_effectList;

	//テクスチャデータ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;

};