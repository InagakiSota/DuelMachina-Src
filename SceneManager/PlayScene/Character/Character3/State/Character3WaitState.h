///////////////////////////////////
//キャラクター2の待ち状態のクラス
//作成者：GS305_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#pragma once

#include "../../CharacterStateBase.h"

class Character3WaitState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character3WaitState();
	//デストラクタ
	~Character3WaitState();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;
	//更新
	void Update() override;
};