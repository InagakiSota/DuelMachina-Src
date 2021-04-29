///////////////////////////////////
//キャラクター2の状態管理クラス
//作成者：GS305_稲垣
//作成日：2020年10月17日
///////////////////////////////////

#pragma once

#include "../../CharacterStateManagerBase.h"
#include "../../CharacterState.h"

class CharacterBase;
class CharacterStateBase;

class Character3StateManager : public CharacterStateManagerBase
{
public:
	//コンストラクタ
	Character3StateManager();
	//デストラクタ
	~Character3StateManager();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;

	//更新
	void Update(const eCHARACTER_STATE& charaState) override;

	//終了処理
	void Finalize() override;

};
