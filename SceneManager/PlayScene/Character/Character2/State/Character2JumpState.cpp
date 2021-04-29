///////////////////////////////////
//キャラクタ－2のジャンプ状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月22日
///////////////////////////////////
#include "pch.h"
#include "Character2JumpState.h"
#include "../../CharacterBase.h"
#include "../Character2Params.h"

//コンストラクタ
Character2JumpState::Character2JumpState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character2JumpState::~Character2JumpState()
{
	m_pCharacter = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクターのポインタ
//戻り値:なし
//////////////////////////
void Character2JumpState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character2JumpState::Update()
{
	if (m_pCharacter->GetJumpFlag() == false && m_pCharacter->GetLandingFlag() == true)
	{
		//キャラクターの移動量を保存
		DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();

		//ジャンプフラグをtrueにする
		m_pCharacter->SetJumpFlag(true);
		//接地フラグをfalseにする
		m_pCharacter->SetLandingFlag(false);

		//上方向に移動量を与える
		vel.y = Character2Params::GetInstance()->JUMP_FORCE;
		m_pCharacter->SetVel(vel);
	}

}
