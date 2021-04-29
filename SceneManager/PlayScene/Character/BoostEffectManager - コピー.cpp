///////////////////////////////////
//ブーストのエフェクトクラス
//作成者：GT2_05_稲垣颯太
//作成日：2021年4月5日
///////////////////////////////////

#include "pch.h"
#include "BoostEffectManager.h"

#include "DeviceResources.h" 
#include "StepTimer.h" 
#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h> 

#include "../../BinaryFile.h"

//コンストラクタ
BoostEffectManager::BoostEffectManager()
	:m_isPlay(false)
{
}

//デストラクタ
BoostEffectManager::~BoostEffectManager()
{
}

///////////////////////////
//作成
//引数:デバイスリソース、エフェクトの個数
//戻り値:なし
//////////////////////////
void BoostEffectManager::Create(DX::DeviceResources * deviceResources, const int & count)
{
	//D3DDeviceを取得する
	auto device = deviceResources->GetD3DDevice();

	//バッチエフェクトの作成
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	//プリミティブバッチの作成
	m_batch =
		std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(deviceResources->GetD3DDeviceContext());

	//テクスチャのロード
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/boostEffect_2.png", nullptr,
		m_texture.GetAddressOf());


	//エフェクトを実体化し、リストに登録する。
	for (int i = 0; i < count; i++) {
		std::unique_ptr<BoostEffect> effect = std::make_unique<BoostEffect>();
		effect->Create(deviceResources, m_texture.Get(), m_inputLayout.Get());
		m_boostEffectList.push_back(std::move(effect));
	}

}

///////////////////////////
//初期化
//引数:寿命、座標、エフェクトの発射タイプ
//戻り値:なし
//////////////////////////
void BoostEffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type)
{
	switch (type)
	{
		//ランダム
		case BoostEffectManager::eEFFECT_TYPE::RANDOM:
		{
			InitializeRandom(life, pos);
			break;

		}

		//均等
		case BoostEffectManager::eEFFECT_TYPE::AVERAGE:
		{
			InitializeAverage(life, pos);

			break;

		}

		//シェーダ
		case BoostEffectManager::eEFFECT_TYPE::CONE:
		{
			InitializeCone(life, pos);

			break;
		}
		default:
			break;
	}

}

///////////////////////////
//初期化(平均)
//引数:寿命、座標
//戻り値:なし
//////////////////////////
void BoostEffectManager::InitializeAverage(float life, DirectX::SimpleMath::Vector3 pos)
{
	const float RAD = DirectX::XM_PI * 2.0f;
	int num = 0;
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		float size = (float)m_boostEffectList.size();
		DirectX::SimpleMath::Vector3 vel =
			DirectX::SimpleMath::Vector3(cos(RAD * num / size + DirectX::XM_PI / 2.0f), sin(RAD * num / size + DirectX::XM_PI / 2.0f), 0);
		vel *= 0.1f;
		(*ite)->Initialize(life, pos, vel);
		num++;
	}

}

///////////////////////////
//初期化(ランダム)
//引数:寿命、座標
//戻り値:なし
//////////////////////////
void BoostEffectManager::InitializeRandom(float life, DirectX::SimpleMath::Vector3 pos)
{
	int range = 100;

	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		DirectX::SimpleMath::Vector3 vel =
			DirectX::SimpleMath::Vector3(
			((rand() % (range * 2)) - range) / (float)range * 0.2f,
				((rand() % (range * 2)) - range) / (float)range * 0.2f,
				((rand() % (range * 2)) - range) / (float)range * 0.2f);

		while (vel.Length() < 0.03f)
		{
			vel =
				DirectX::SimpleMath::Vector3(
				((rand() % (range * 2)) - range) / (float)range * 0.1f,
					((rand() % (range * 2)) - range) / (float)range * 0.1f,
					((rand() % (range * 2)) - range) / (float)range * 0.1f);
		}

		(*ite)->Initialize(life, pos, vel);
		//m_world = DirectX::SimpleMath::Matrix::CreateRotationZ(atan2f(vel.y,vel.x));
	}

}

///////////////////////////
//初期化(コーン)
//引数:寿命、座標
//戻り値:なし
//////////////////////////
void BoostEffectManager::InitializeCone(float life, DirectX::SimpleMath::Vector3 pos)
{
	//エフェクトの中心軸
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3(-3, 1, 0);

	//軸の値を正規化する
	dir.Normalize();

	//外積に使うためのカメラ位置（本当は外から受け渡せると良い）
	DirectX::SimpleMath::Vector3 camera = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);


	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		//軸とカメラから外積を求める
		DirectX::SimpleMath::Vector3 d = dir.Cross(camera);
		//外積を正規化する
		d.Normalize();
		//外積の値と中心軸を加算する
		d = dir + d * (float)sin(rand())*0.3f;
		//値が大きいので0.1倍
		d *= 0.1f;
		(*ite)->Initialize(life, pos, d);
	}

}

///////////////////////////
//更新
//引数:タイマー
//戻り値:なし
//////////////////////////
void BoostEffectManager::Update(DX::StepTimer timer)
{
	if (m_isPlay == true)
	{
		for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
		{
			(*ite)->Update(timer);

			//エフェクトの寿命が0になったら再生をやめる
			if ((*ite)->GetLife() < 0)m_isPlay = false;
		}
	}

}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void BoostEffectManager::Render()
{
	if (m_isPlay == true)
	{
		for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
		{
			(*ite)->Render();
		}
	}

}

///////////////////////////
//カメラ、ビュー行列、射影行列の設定
//引数:カメラ座標、ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void BoostEffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera, view, proj);
	}
	m_cameraPos = camera;
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void BoostEffectManager::Finalize()
{
}

///////////////////////////
//重力の設定
//引数:重力の有無
//戻り値:なし
//////////////////////////
void BoostEffectManager::SetGravity(bool gravity)
{
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		(*ite)->SetGravity(gravity);
	}

}

///////////////////////////
//再生
//引数:寿命、座標、タイプ
//戻り値:なし
//////////////////////////
void BoostEffectManager::Play(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type)
{
	switch (type)
	{
		//ランダム
		case BoostEffectManager::eEFFECT_TYPE::RANDOM:
		{
			InitializeRandom(life, pos);
			break;

		}

		//均等
		case BoostEffectManager::eEFFECT_TYPE::AVERAGE:
		{
			InitializeAverage(life, pos);

			break;

		}

		//シェーダ
		case BoostEffectManager::eEFFECT_TYPE::CONE:
		{
			InitializeCone(life, pos);

			break;
		}
		default:
			break;
	}
	//再生フラグを立てる
	m_isPlay = true;

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void BoostEffectManager::Reset()
{
	m_isPlay = false;

}
