///////////////////////////////////
//キャラクタ－2のしゃがみ状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#include "pch.h"
#include "Character2SquatState.h"
#include "../../../../../FrameWork/Collision/Collision.h"
#include "../../CharacterBase.h"
#include "../Character2Params.h"

//体の当たり判定のY座標
const float Character2SquatState::BODY_COLL_POS_Y = 1.2f;
//頭の当たり判定のY座標
const float Character2SquatState::HEAD_COLL_POS_Y = 0.5f;

//コンストラクタ
Character2SquatState::Character2SquatState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character2SquatState::~Character2SquatState()
{
	m_pCharacter = nullptr;
}

void Character2SquatState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

void Character2SquatState::Update()
{
	//体の当たり判定
	Collision::BoxCollision bodyCollBox = m_pCharacter->GetBodyCollBox();
	//頭の当たり判定
	Collision::BoxCollision headCollBox = m_pCharacter->GetHeadCollBox();

	//体の当たり判定のサイズ、座標の設定
	bodyCollBox.size_h = Character2Params::BODY_COLL_SIZE_SQUAT;
	bodyCollBox.pos = DirectX::SimpleMath::Vector3(
		m_pCharacter->GetPos().x,
		m_pCharacter->GetPos().y - Character2SquatState::BODY_COLL_POS_Y,
		m_pCharacter->GetPos().z);
	//頭の当たり判定の座標の設定
	headCollBox.pos = DirectX::SimpleMath::Vector3(
		m_pCharacter->GetPos().x,
		m_pCharacter->GetPos().y - Character2SquatState::HEAD_COLL_POS_Y,
		m_pCharacter->GetPos().z);

	m_pCharacter->SetBodyCollBox(bodyCollBox);
	m_pCharacter->SetHeadCollBox(headCollBox);
}
