///////////////////////////////////
//キャラクター１のブースト移動状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月16日
///////////////////////////////////
#include "pch.h"
#include "Character1BoostMoveState.h"
#include "Keyboard.h"
#include "../../CharacterBase.h"
#include "../../CharacterState.h"
#include "../Character1Params.h"

//コンストラクタ
Character1BoostMoveState::Character1BoostMoveState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character1BoostMoveState::~Character1BoostMoveState()
{
	m_pCharacter = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクタークラスのポインタ
//戻り値:なし
//////////////////////////
void Character1BoostMoveState::Initialize(CharacterBase * pCharacter)
{
	m_pCharacter = pCharacter;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1BoostMoveState::Update()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();

	//体の当たり判定
	Collision::BoxCollision bodyColl = m_pCharacter->GetBodyCollBox();
	bodyColl.size_h = Character1Params::MOVE_BOOST_BODY_SIDE;
	bodyColl.pos = DirectX::SimpleMath::Vector3(m_pCharacter->GetPos().x, m_pCharacter->GetPos().y, m_pCharacter->GetPos().z);
	m_pCharacter->SetBodyCollBox(bodyColl);

	//ブースト容量を減らす
	int boostCap = m_pCharacter->GetBoostCap();
	boostCap--;
	m_pCharacter->SetBoostCap(boostCap);

	//ブースト容量が0以下になったら0代入
	if (m_pCharacter->GetBoostCap() <= 0)
	{
		m_pCharacter->SetBoostCap(0);
	}

	//右入力
	if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) && m_pCharacter->GetPos().x <  Character1Params::MOVE_LIMIT_X)
	{
		vel.x = Character1Params::GetInstance()->MOVE_BOOST_FORCE;
	}
	//左入力
	else if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) && m_pCharacter->GetPos().x > -Character1Params::MOVE_LIMIT_X)
	{
		vel.x = -Character1Params::GetInstance()->MOVE_BOOST_FORCE;
	}
	else vel.x = 0.0f;
	
	//上入力
	if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Up) && m_pCharacter->GetPos().y <  Character1Params::MOVE_LIMIT_Y)
	{
		vel.y = Character1Params::GetInstance()->MOVE_BOOST_FORCE;
	}
	//下入力
	else if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) && m_pCharacter->GetLandingFlag() == false)
	{
		vel.y = -Character1Params::GetInstance()->MOVE_BOOST_FORCE;
	}
	else vel.y = 0.0f;

	m_pCharacter->SetVel(vel);
	
	//スペースキーを放すかブースト容量が0になったら元のステートに戻る
	if (keyState.IsKeyUp(DirectX::Keyboard::Keys::Space) || m_pCharacter->GetBoostCap() <= 0)
	{
		m_pCharacter->SetCharaState(eCHARACTER_STATE::WAIT);
	}
}

