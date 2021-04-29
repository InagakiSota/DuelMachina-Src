///////////////////////////////////
//フェードイン、アウトの管理クラス
//作成者：GS3_05_稲垣颯太
//作成日：2021年1月8日
///////////////////////////////////

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include "../MyEffect/MyEffect.h"
#include "Model.h"
#include <Mouse.h>
#include <PrimitiveBatch.h>
#include <list>
#include <Effects.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include "../../SceneManager/BinaryFile.h"


class FadeManager
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

	//作成
	void Create(DX::DeviceResources* deviceResources);
	//初期化
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos);
	//更新
	void Update(DX::StepTimer timer,float time);
	//描画
	void Render();
	//終了処理
	void Finalize();
	//描画
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	//インスタンスの取得
	static FadeManager* GetInstance();
	//インスタンスの破棄
	static void ReleaseInstance();

private:
	//コンストラクタ
	FadeManager();
	//デストラクタ
	~FadeManager();

	//エフェクトのリスト
	std::list<MyEffect*>		m_effectList;
	//テクスチャ1
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	//テクスチャ2(使うかはわからない)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;

	//タイマー
	DX::StepTimer                           m_timer;
	//時間
	float m_time;


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

	DirectX::SimpleMath::Matrix m_billboardTranslation;	//ビルボード
	DirectX::SimpleMath::Matrix m_view;					//ビュー行列
	DirectX::SimpleMath::Matrix m_proj;					//射影行列

	//自身のインスタンス
	static FadeManager* m_pMyInstance;

};
