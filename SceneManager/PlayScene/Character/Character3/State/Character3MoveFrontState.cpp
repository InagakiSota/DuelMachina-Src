///////////////////////////////////
//キャラクター2の前移動状態のクラス
//作成者：GS305_稲垣
//作成日：2020年10月16日
///////////////////////////////////
#include "pch.h"

#include "Character3MoveFrontState.h"
#include "../../CharacterBase.h"
#include "../Character3Params.h"



//コンストラクタ
Character3MoveFrontState::Character3MoveFrontState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character3MoveFrontState::~Character3MoveFrontState()
{
	m_pCharacter = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクタークラスのポインタ
//戻り値:なし
//////////////////////////
void Character3MoveFrontState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character3MoveFrontState::Update()
{
	DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();

	//移動制限内にいたら移動
	if (m_pCharacter->GetPos().x >= -Character3Params::MOVE_LIMIT_X || m_pCharacter->GetPos().x <= Character3Params::MOVE_LIMIT_X)
	{
		//敵が右側にいたら右方向に移動
		if (m_pCharacter->GetFrontVector().x > 0)
		{
			vel.x = Character3Params::GetInstance()->MOVE_FRONT_FORCE;
		}
		//敵が左側にいたら左方向に移動
		else if (m_pCharacter->GetFrontVector().x < 0)
		{
			vel.x = -(Character3Params::GetInstance()->MOVE_FRONT_FORCE);
		}
		m_pCharacter->SetVel(vel);
	}

}
