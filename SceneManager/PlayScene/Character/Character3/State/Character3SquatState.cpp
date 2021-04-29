///////////////////////////////////
//キャラクタ－2のしゃがみ状態のクラス
//作成者：GS305_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#include "pch.h"
#include "Character3SquatState.h"
#include "../../../../../FrameWork/Collision/Collision.h"
#include "../../CharacterBase.h"
#include "../Character3Params.h"

//コンストラクタ
Character3SquatState::Character3SquatState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character3SquatState::~Character3SquatState()
{
	m_pCharacter = nullptr;
}

void Character3SquatState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

void Character3SquatState::Update()
{
	//体の当たり判定
	Collision::BoxCollision bodyCollBox = m_pCharacter->GetBodyCollBox();
	//頭の当たり判定
	Collision::BoxCollision headCollBox = m_pCharacter->GetHeadCollBox();

	//体の当たり判定のサイズ、座標の設定
	bodyCollBox.size_h = Character3Params::BODY_COLL_SIZE_SQUAT;
	bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pCharacter->GetPos().x, m_pCharacter->GetPos().y - 1.7f, m_pCharacter->GetPos().z);
	//頭の当たり判定の座標の設定
	headCollBox.pos = DirectX::SimpleMath::Vector3(m_pCharacter->GetPos().x, m_pCharacter->GetPos().y - 1.1f, m_pCharacter->GetPos().z);

	m_pCharacter->SetBodyCollBox(bodyCollBox);
	m_pCharacter->SetHeadCollBox(headCollBox);
}
