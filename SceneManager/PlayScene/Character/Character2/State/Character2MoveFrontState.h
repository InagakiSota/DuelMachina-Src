///////////////////////////////////
//キャラクター2の前移動状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月16日
///////////////////////////////////
#pragma once

#include "../../CharacterStateBase.h"

class CharacterBase;

class Character2MoveFrontState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character2MoveFrontState();
	//デストラクタ
	~Character2MoveFrontState();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;

	//更新
	void Update() override;

private:
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;

};