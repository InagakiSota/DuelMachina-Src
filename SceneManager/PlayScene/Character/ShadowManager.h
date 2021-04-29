///////////////////////////////////
//足元の影のマネージャークラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年12月7日
///////////////////////////////////

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include "Effect.h"
#include "Model.h"

#include <list>

class ShadowManager
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	Time;
	};
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	//Y座標
	static const float POS_Y;
	//Z座標
	static const float POS_Z;
	//X軸の角度
	static const float ROT_X;
	//スケール
	static const float SCALE;

	//コンストラクタ
	ShadowManager();
	//デストラクタ
	~ShadowManager();

	//初期化
	void Initialize(DX::DeviceResources* deviceResources, int count, float life, DirectX::SimpleMath::Vector3 pos);
	//更新
	void Update(DX::StepTimer timer,DirectX::SimpleMath::Vector3 pos);
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//終了処理
	void Finalize();
	
	//描画
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

private:
	//MyEffect*					m_myEffect[10];
	std::list<Effect*>		m_effectList;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;


	DX::StepTimer                           m_timer;


	DX::DeviceResources*			m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	std::unique_ptr<DirectX::CommonStates> m_states;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::vector<DirectX::VertexPositionColorTexture>  m_vertex;

	DirectX::SimpleMath::Vector3 m_centerPosition;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	std::unique_ptr<DirectX::EffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::Model> m_model;


	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_capture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_rtv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

	//座標
	DirectX::SimpleMath::Vector3 m_pos;
	//スケール
	float m_scale;

};

