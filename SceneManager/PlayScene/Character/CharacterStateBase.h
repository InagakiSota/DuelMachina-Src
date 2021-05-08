///////////////////////////////////
//キャラクターの攻撃の基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月13日
///////////////////////////////////

#pragma once

#include "ICharacterState.h"

class CharacterBase;

class CharacterStateBase : public ICharacterState
{
public:
	~CharacterStateBase() {};
	//初期化
	void Initialize(CharacterBase* pCharacter) override {}
	//更新
	void Update() override {}

private:
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;


};