///////////////////////////////////
//キャラクター１の前移動状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月16日
///////////////////////////////////
#include "pch.h"

#include "Character1MoveFrontState.h"
#include "../../CharacterBase.h"
#include "../Character1Params.h"

//コンストラクタ
Character1MoveFrontState::Character1MoveFrontState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character1MoveFrontState::~Character1MoveFrontState()
{
	m_pCharacter = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクタークラスのポインタ
//戻り値:なし
//////////////////////////
void Character1MoveFrontState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1MoveFrontState::Update()
{	
	DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();

	//移動制限内にいたら移動
	if (m_pCharacter->GetPos().x >= -Character1Params::MOVE_LIMIT_X || m_pCharacter->GetPos().x <= Character1Params::MOVE_LIMIT_X && m_pCharacter->GetLandingFlag() ==true)
	{
		//敵が右側にいたら右方向に移動
		if (m_pCharacter->GetFrontVector().x > 0)
		{
			vel.x = Character1Params::GetInstance()->MOVE_FRONT_FORCE;
		}
		//敵が左側にいたら左方向に移動
		else if (m_pCharacter->GetFrontVector().x < 0)
		{
			vel.x = -Character1Params::GetInstance()->MOVE_FRONT_FORCE;
		}
		m_pCharacter->SetVel(vel);
	}

}
