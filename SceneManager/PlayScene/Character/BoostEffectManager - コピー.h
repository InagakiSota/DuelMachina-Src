///////////////////////////////////
//ブーストのエフェクトクラス
//作成者：GT2_05_稲垣颯太
//作成日：2021年4月5日
///////////////////////////////////

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include "Effect.h"
#include <list>
#include "BoostEffect.h"


class BoostEffectManager
{
public:
	enum class eEFFECT_TYPE
	{
		NONE = -1,
		RANDOM,			//ランダム
		AVERAGE,		//均等
		CONE,			//コーン

		OVER_ID,
	};

	//コンストラクタ
	BoostEffectManager();
	//デストラクタ
	~BoostEffectManager();

	//Create()で全部処理は長いので分割
	void Create(DX::DeviceResources* deviceResources, const int& count);

	//初期化
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type = eEFFECT_TYPE::RANDOM);
	//等間隔でエフェクトを飛ばす処理の初期化
	void InitializeAverage(float life, DirectX::SimpleMath::Vector3 pos);
	//ランダムにエフェクトを飛ばす処理の初期化
	void InitializeRandom(float life, DirectX::SimpleMath::Vector3 pos);
	//コーン状にエフェクトを飛ばす処理の初期化
	void InitializeCone(float life, DirectX::SimpleMath::Vector3 pos);

	//更新
	void Update(DX::StepTimer timer);
	//描画
	void Render();
	//カメラ、射影、ビューの反映
	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//終了処理
	void Finalize();
	//重力を設定する
	void SetGravity(bool gravity);

	//再生
	void Play(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type = eEFFECT_TYPE::CONE);
	//リセット
	void Reset();

private:
	//バッチエフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//エフェクトのポインタ
	std::list<std::unique_ptr<BoostEffect>>	m_boostEffectList;
	//テクスチャデータ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;

	//再生フラグ
	bool m_isPlay;
	//カメラ座標
	DirectX::SimpleMath::Vector3 m_cameraPos;
};