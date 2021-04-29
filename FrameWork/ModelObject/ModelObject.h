///////////////////////////////////
//モデル描画クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年5月21日
///////////////////////////////////
#pragma once

#include "../../DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <CommonStates.h>
#include <Model.h>

class ModelObject
{
public:
	ModelObject();		//コンストラクタ
	~ModelObject();		//デストラクタ

	void Update(DirectX::SimpleMath::Matrix world);		//更新
	void Lost();
	void Create(DX::DeviceResources* deviceResources , const wchar_t* name);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

private:
	DX::DeviceResources* m_deviceResources;

	DirectX::SimpleMath::Matrix m_world;
	//DirectX::SimpleMath::Matrix m_view;
	//DirectX::SimpleMath::Matrix m_proj;

	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::EffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::Model> m_model;

};

