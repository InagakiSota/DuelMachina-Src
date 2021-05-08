///////////////////////////////////
//キャラクタ－2のしゃがみ状態のクラス
//作成者：GS305_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#pragma once

#include "../../CharacterStateBase.h"
#include <SimpleMath.h>

class CharacterBase;

class Character3SquatState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character3SquatState();
	//デストラクタ
	~Character3SquatState();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;
	//更新
	void Update() override;

private:
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;
};