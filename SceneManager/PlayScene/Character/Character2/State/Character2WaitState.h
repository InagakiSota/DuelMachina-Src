///////////////////////////////////
//キャラクター2の待ち状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#pragma once

#include "../../CharacterStateBase.h"

class Character2WaitState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character2WaitState();
	//デストラクタ
	~Character2WaitState();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;
	//更新
	void Update() override;
};