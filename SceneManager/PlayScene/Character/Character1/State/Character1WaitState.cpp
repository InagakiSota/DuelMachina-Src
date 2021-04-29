///////////////////////////////////
//キャラクター１の待ち状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月22日
///////////////////////////////////
#include "pch.h"
#include "Character1WaitState.h"
#include "../../CharacterBase.h"

//コンストラクタ
Character1WaitState::Character1WaitState()
{
	m_pCharacter = nullptr;
}

//デストラクタ
Character1WaitState::~Character1WaitState()
{
	m_pCharacter = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクタークラスのポインタ
//戻り値:なし
//////////////////////////
void Character1WaitState::Initialize(CharacterBase * pCharacterBase)
{
	m_pCharacter = pCharacterBase;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1WaitState::Update()
{
	DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();
	//横の移動量を0にする
	vel.x = 0;
	m_pCharacter->SetVel(vel);

}
