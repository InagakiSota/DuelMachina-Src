///////////////////////////////////
//キャラクタ－2のジャンプ状態のクラス
//作成者：GS305_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#pragma once

#include "../../CharacterStateBase.h"

class Character3JumpState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character3JumpState();
	//デストラクタ
	~Character3JumpState();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;

	//更新
	void Update() override;

private:
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;

};