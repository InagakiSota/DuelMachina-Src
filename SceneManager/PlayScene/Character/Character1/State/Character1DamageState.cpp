///////////////////////////////////
//キャラクター１のやられ状態のクラス
//作成者：なし
//作成日：なし
///////////////////////////////////
#include "pch.h"
#include "Character1DamageState.h"
#include "../../CharacterBase.h"
#include "../../CharacterState.h"
#include "../Character1Params.h"


//ステートが戻るまでの時間
const float Character1DamageState::STATE_BACK_TIME = 3.0f;
//ノックバックの移動量の初期値
const float Character1DamageState::KNOCK_BACK_VEL_START = 0.1f;
//ノックバックの移動量
const float Character1DamageState::KNOCK_BACK_VEL = 0.005f;


//コンストラクタ
Character1DamageState::Character1DamageState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character1DamageState::~Character1DamageState()
{
	m_pCharacter = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクタークラスのポインタ
//戻り値:なし
//////////////////////////
void Character1DamageState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1DamageState::Update()
{
	DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();

	m_pCharacter->SetDamageTimer(m_pCharacter->GetDamageTimer() + KNOCK_BACK_VEL_START);

	//ノックバックの移動量を設定
	static float knockBackVel = KNOCK_BACK_VEL_START;
	knockBackVel -= Character1DamageState::KNOCK_BACK_VEL;
	if (knockBackVel <= 0.0f)knockBackVel = 0.0f;

	//ノックバック
	if (m_pCharacter->GetFrontVector().x > 0 && m_pCharacter->GetPos().x > -Character1Params::MOVE_LIMIT_X)
	{
		//ノックバックの移動量をプレイヤーの移動量に代入
		vel.x = -knockBackVel;
	}
	else if (m_pCharacter->GetFrontVector().x < 0 && m_pCharacter->GetPos().x <  Character1Params::MOVE_LIMIT_X)
	{
		//ノックバックの移動量をプレイヤーの移動量に代入
		vel.x = knockBackVel;
	}
	m_pCharacter->SetVel(vel);

	//時間経過で待ち状態に戻る
	if (m_pCharacter->GetDamageTimer() >= Character1DamageState::STATE_BACK_TIME)
	{
		m_pCharacter->SetCharaState(eCHARACTER_STATE::WAIT);

		m_pCharacter->SetDamageTimer(0.0f);

		knockBackVel = KNOCK_BACK_VEL_START;
	}

}
