///////////////////////////////////
//ブーストエフェクトのクラス
//作成者：
//作成日：
///////////////////////////////////
#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

class BoostEffect
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	//コンストラクタ
	BoostEffect();
	//デストラクタ
	~BoostEffect();

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_SHADER_LAYOUT;

	//作成
	void Create(DX::DeviceResources* deviceresources, ID3D11ShaderResourceView* texture, ID3D11InputLayout* inputLayout);
	//初期化
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity);
	//更新
	void Update(DX::StepTimer timer);
	//描画
	void Render();
	//リセット
	void Reset();
	//終了処理
	void Finalize();
	//ビュー行列、射影行列の設定
	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//重力の設定
	void SetGravity(bool isGravity);
	//寿命の取得
	float GetLife()
	{
		return m_life;
	}

private:
	//シェーダーの描画
	void ShaderDraw();
	//描画
	void Draw();

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DX::StepTimer                           m_timer;
	DX::DeviceResources*	m_deviceResources;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputShaderLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	//ジオメトリシェーダー
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	//カメラの座標
	DirectX::SimpleMath::Vector3		m_camera;
	//重力
	DirectX::SimpleMath::Vector3		m_gravity;
	//座標
	DirectX::SimpleMath::Vector3		m_pos;
	//移動量
	DirectX::SimpleMath::Vector3		m_vel;
	//生存時間
	float								m_life;

	//初期座標
	DirectX::SimpleMath::Vector3		m_startPos;
	//初期移動量
	DirectX::SimpleMath::Vector3		m_startVel;
	//初期ライフ
	float								m_startLife;
	//待機時間
	float								m_wait;
	//重力が作用するかどうか
	bool								m_isGravity;
};