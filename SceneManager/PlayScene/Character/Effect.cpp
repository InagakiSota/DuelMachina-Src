///////////////////////////////////
//エフェクトのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年12月7日
///////////////////////////////////
#include "pch.h"
#include "Effect.h"
#include "../../BinaryFile.h"

//コンストラクタ
Effect::Effect()
{
}

//デストラクタ
Effect::~Effect()
{
}

///////////////////////////
//作成
//引数:なし
//戻り値:なし
//////////////////////////
void Effect::Create()
{
}

///////////////////////////
//初期化
//引数:寿命、座標、移動量
//戻り値:なし
//////////////////////////
void Effect::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity)
{
	m_startPos = m_pos = pos;
	m_startVel = m_vel = velocity;
	m_startLife = m_life = life;
	m_gravity = DirectX::SimpleMath::Vector3(0, 0.001f, 0);
}

///////////////////////////
//更新
//引数:タイマー
//戻り値:なし
//////////////////////////
void Effect::Update(DX::StepTimer timer)
{
	float time = float(m_timer.GetTotalSeconds());
	m_timer = timer;

	//m_pos = DirectX::SimpleMath::Vector3(cosf(time+m_vel.x), sinf(time + m_vel.x),0);

	//m_pos.x += 0.01f;
	m_life --;

	m_pos += m_vel;

	if (m_life < 0)
	{
		Initialize(m_startLife, m_startPos, m_startVel);
	}

}
