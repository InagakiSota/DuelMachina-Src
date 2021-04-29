///////////////////////////////////
//エフェクトのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年12月7日
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

class Effect
{
public:
	//コンストラクタ
	Effect();
	//デストラクタ
	~Effect();

	//作成
	void Create();

	//初期化
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity);
	//更新
	void Update(DX::StepTimer timer);

	//座標の取得
	DirectX::SimpleMath::Vector3 GetPos() 
	{
		return m_pos;
	}

	//移動量の取得
	DirectX::SimpleMath::Vector3 GetVel()
	{ 
		return m_vel; 
	}

	//重力のフラグの設定
	void SetGravity(bool isGravity)
	{
		m_isGravity = isGravity;
	}

private:

	//タイマー
	DX::StepTimer m_timer;

	//重力
	DirectX::SimpleMath::Vector3 m_gravity;
	//座標
	DirectX::SimpleMath::Vector3 m_pos;
	//移動量
	DirectX::SimpleMath::Vector3 m_vel;
	//寿命
	float m_life;

	//初期描画
	DirectX::SimpleMath::Vector3 m_startPos;
	//初期移動量
	DirectX::SimpleMath::Vector3 m_startVel;
	//初期寿命
	float m_startLife;

	//重力のフラグ
	bool m_isGravity;

};