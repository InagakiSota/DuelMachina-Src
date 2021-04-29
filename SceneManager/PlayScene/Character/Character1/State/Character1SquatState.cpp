///////////////////////////////////
//キャラクタ－１のしゃがみ状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#include "pch.h"
#include "Character1SquatState.h"
#include "../../../../../FrameWork/Collision/Collision.h"
#include "../../CharacterBase.h"
#include "../Character1Params.h"

//Y軸の角度(マイナス)
const float Character1SquatState::ANGLE_Y_MINUS = 320.0f;
//Y軸の角度(プラス)
const float Character1SquatState::ANGLE_Y_PLUS= 130.0f;
//体の当たり判定のY座標
const float Character1SquatState::BODY_COLL_BOX_POS_Y = -0.37f;
//頭の当たり判定のY座標
const float Character1SquatState::HEAD_COLL_BOX_POS_Y = 0.25f;


//コンストラクタ
Character1SquatState::Character1SquatState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character1SquatState::~Character1SquatState()
{
	m_pCharacter = nullptr;
}

void Character1SquatState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

void Character1SquatState::Update()
{
	//Y軸の角度の設定
	if (m_pCharacter->GetFrontVector().x > 0)
	{
		m_pCharacter->SetAngleY(Character1SquatState::ANGLE_Y_PLUS);
	}
	else
	{
		m_pCharacter->SetAngleY(Character1SquatState::ANGLE_Y_MINUS);
	}

	//座標
	DirectX::SimpleMath::Vector3 pos = m_pCharacter->GetPos();

	//体の当たり判定
	Collision::BoxCollision bodyCollBox = m_pCharacter->GetBodyCollBox();
	//頭の当たり判定
	Collision::BoxCollision headCollBox = m_pCharacter->GetHeadCollBox();
	//足の当たり判定
	//Collision::BoxCollision legCollBox = m_pCharacter->GetLegCollBox();

	//座標の設定
	//pos.y = -1.0f;

	//体の当たり判定のサイズ、座標の設定
	bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_SQUAT;
	bodyCollBox.pos = DirectX::SimpleMath::Vector3(
		m_pCharacter->GetPos().x,
		m_pCharacter->GetPos().y + Character1SquatState::BODY_COLL_BOX_POS_Y,
		m_pCharacter->GetPos().z);

	//頭の当たり判定の座標の設定
	headCollBox.pos = DirectX::SimpleMath::Vector3(
		m_pCharacter->GetPos().x,
		m_pCharacter->GetPos().y + Character1SquatState::HEAD_COLL_BOX_POS_Y,
		m_pCharacter->GetPos().z);

	////脚の当たり判定の座標の設定
	//legCollBox.pos = DirectX::SimpleMath::Vector3(
	//	m_pCharacter->GetPos().x,
	//	m_pCharacter->GetPos().y - 0.1f,
	//	m_pCharacter->GetPos().z);


	m_pCharacter->SetPos(pos);

	m_pCharacter->SetBodyCollBox(bodyCollBox);
	m_pCharacter->SetHeadCollBox(headCollBox);
	//m_pCharacter->SetLegCollBox(legCollBox);
}
