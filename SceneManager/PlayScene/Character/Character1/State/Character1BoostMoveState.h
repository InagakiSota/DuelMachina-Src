///////////////////////////////////
//キャラクター１のブースト移動状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月16日
///////////////////////////////////

#pragma once
#include <SimpleMath.h>
#include "../../CharacterStateBase.h"

class CharacterBase;

class Character1BoostMoveState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character1BoostMoveState();
	//デストラクタ
	~Character1BoostMoveState();

	//初期化
	void Initialize(CharacterBase* pCharacter) override;
	//更新
	void Update();

};
