///////////////////////////////////
//ヒットエフェクトのマネージャー
//作成者：GS3_05_稲垣颯太
//作成日：2020年12月22日
///////////////////////////////////

#include "pch.h"
#include "HitEffectManager.h"

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
HitEffectManager::HitEffectManager()
	:m_isPlay(false)
{
}

//デストラクタ
HitEffectManager::~HitEffectManager()
{
}

///////////////////////////
//作成
//引数:デバイスリソースのポインタ,エフェクトの個数
//戻り値:なし
//////////////////////////
void HitEffectManager::Create(DX::DeviceResources * deviceResources,const int& count)
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
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/hitEffect.png", nullptr,
		m_texture.GetAddressOf());


	//エフェクトを実体化し、リストに登録する。
	for (int i = 0; i < count; i++) {
		std::unique_ptr<HitEffect> effect = std::make_unique<HitEffect>();
		effect->Create(deviceResources, m_texture.Get(), m_inputLayout.Get());
		m_hitEffectList.push_back(std::move(effect));
	}
}

///////////////////////////
//初期化
//引数:寿命、座標
//引数:寿命、座標
//引数:寿命、座標
//戻り値:なし
//////////////////////////
void HitEffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type)
{
	switch (type)
	{
		//ランダム
		case HitEffectManager::eEFFECT_TYPE::RANDOM:
		{
			InitializeRandom(life, pos);
			break;

		}

		//均等
		case HitEffectManager::eEFFECT_TYPE::AVERAGE:
		{
			InitializeAverage(life, pos);

			break;

		}

		//コーン
		case HitEffectManager::eEFFECT_TYPE::CONE:
		{
			InitializeCone(life, pos);

			break;
		}

		//ステイ
		case HitEffectManager::eEFFECT_TYPE::STAY:
		{
			InitializeStay(life, pos);
			break;
		}


		default:
			break;
	}
}

///////////////////////////
//初期化(等間隔)
//引数:寿命、座標
//戻り値:なし
//////////////////////////
void HitEffectManager::InitializeAverage(float life, DirectX::SimpleMath::Vector3 pos)
{
	const float RAD = DirectX::XM_PI * 2.0f;
	int num = 0;
	for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end(); ite++)
	{
		float size = (float)m_hitEffectList.size();
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
void HitEffectManager::InitializeRandom(float life, DirectX::SimpleMath::Vector3 pos)
{
	int range = 100;

	//life,pos,vel の値でm_effectを初期化する
	for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end(); ite++)
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
	}

}

///////////////////////////
//初期化(コーン)
//引数:寿命、座標
//戻り値:なし
//////////////////////////
void HitEffectManager::InitializeCone(float life, DirectX::SimpleMath::Vector3 pos)
{
	//エフェクトの中心軸
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3(-3, 1, -5);

	//軸の値を正規化する
	dir.Normalize();

	//外積に使うためのカメラ位置（本当は外から受け渡せると良い）
	DirectX::SimpleMath::Vector3 camera = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);

	//life,pos,vel の値でm_effectを初期化する
	for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end(); ite++)
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
//初期化(ステイ)
//引数:寿命、初期座標
//戻り値:なし
//////////////////////////
void HitEffectManager::InitializeStay(float life, DirectX::SimpleMath::Vector3 pos)
{
	for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end();ite++)
	{
		(*ite)->Initialize(life, pos, DirectX::SimpleMath::Vector3::Zero);
	}
}

///////////////////////////
//初期化(ステイ)
//引数:寿命、初期座標
//戻り値:なし
//////////////////////////
void HitEffectManager::InitializeStay(float life, DirectX::SimpleMath::Vector3 pos, int index)
{
	m_hitEffectList[index]->Initialize(life,pos,DirectX::SimpleMath::Vector3::Zero);
}

///////////////////////////
//更新
//引数:タイマー
//戻り値:なし
//////////////////////////
void HitEffectManager::Update(DX::StepTimer timer)
{
	if(m_isPlay == true)
	{
		for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end(); ite++)
		{
			(*ite)->Update(timer);

			//エフェクトの寿命が0になったら再生をやめる
			//if ((*ite)->GetLife() < 0)m_isPlay = false;
		}
	}
}


///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void HitEffectManager::Render()
{
	if (m_isPlay == true)
	{
		for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end(); ite++)
		{
			(*ite)->Render();
		}
	}
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void HitEffectManager::Render(float angleZ, float scale)
{
	if (m_isPlay == true)
	{
		for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end(); ite++)
		{
			(*ite)->Render(angleZ, scale);
		}
	}

}

///////////////////////////
//ビュー行列、射影行列の設定
//引数:
//戻り値:
//////////////////////////
void HitEffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera,view,proj);
	}
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void HitEffectManager::Finalize()
{
	//m_hitEffectList.clear();
	//m_inputLayout.Reset();
	//m_texture.Reset();
}

///////////////////////////
//重力の設定
//引数:重力のフラグ
//戻り値:なし
//////////////////////////
void HitEffectManager::SetGravity(bool gravity)
{
	for (std::vector<std::unique_ptr<HitEffect>>::iterator ite = m_hitEffectList.begin(); ite != m_hitEffectList.end(); ite++)
	{
		(*ite)->SetGravity(gravity);
	}
}

///////////////////////////
//再生
//引数:寿命、初期座標
//戻り値:なし
//////////////////////////
void HitEffectManager::Play(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type)
{
	switch (type)
	{
		//ランダム
		case HitEffectManager::eEFFECT_TYPE::RANDOM:
		{
			InitializeRandom(life, pos);
			break;

		}

		//均等
		case HitEffectManager::eEFFECT_TYPE::AVERAGE:
		{
			InitializeAverage(life, pos);

			break;

		}

		//コーン
		case HitEffectManager::eEFFECT_TYPE::CONE:
		{
			InitializeCone(life, pos);

			break;
		}

		//ステイ
		case HitEffectManager::eEFFECT_TYPE::STAY:
		{
			InitializeStay(life, pos);
			break;
		}

		default:
			break;
	}
	//再生フラグを立てる
	m_isPlay = true;
}

///////////////////////////
//再生
//引数:寿命、初期座標
//戻り値:なし
//////////////////////////
void HitEffectManager::Play(float life, DirectX::SimpleMath::Vector3 pos, int index, eEFFECT_TYPE type)
{
	//再生フラグを立てる
	m_isPlay = true;

	switch (type)
	{
		//ランダム
		case HitEffectManager::eEFFECT_TYPE::RANDOM:
		{
			InitializeRandom(life, pos);
			break;

		}

		//均等
		case HitEffectManager::eEFFECT_TYPE::AVERAGE:
		{
			InitializeAverage(life, pos);

			break;

		}

		//コーン
		case HitEffectManager::eEFFECT_TYPE::CONE:
		{
			InitializeCone(life, pos);

			break;
		}

		//ステイ
		case HitEffectManager::eEFFECT_TYPE::STAY:
		{
			InitializeStay(life, pos, index);
			break;
		}

		default:
			break;
	}

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void HitEffectManager::Reset()
{
	m_isPlay = false;
}

