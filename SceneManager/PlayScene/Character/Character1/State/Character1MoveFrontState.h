///////////////////////////////////
//キャラクター１の前移動状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月16日
///////////////////////////////////
#pragma once

#include "../../CharacterStateBase.h"

class CharacterBase;


class Character1MoveFrontState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character1MoveFrontState();
	//デストラクタ
	~Character1MoveFrontState();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;

	//更新
	void Update() override;

private:

};