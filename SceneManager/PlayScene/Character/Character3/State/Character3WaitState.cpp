///////////////////////////////////
//キャラクター2の待ち状態のクラス
//作成者：GS305_稲垣
//作成日：2020年10月22日
///////////////////////////////////
#include "pch.h"
#include "Character3WaitState.h"
#include "../../CharacterBase.h"

//コンストラクタ
Character3WaitState::Character3WaitState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character3WaitState::~Character3WaitState()
{
	m_pCharacter = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクタークラスのポインタ
//戻り値:なし
//////////////////////////
void Character3WaitState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character3WaitState::Update()
{
	DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();
	//横の移動量を0にする
	vel.x = 0;
	m_pCharacter->SetVel(vel);

}