///////////////////////////////////
//キャラクターのステート管理クラスの基底
//作成者：GS3_05_稲垣
//作成日：2020年10月17日
///////////////////////////////////

#pragma once
#include "CharacterState.h"
#include "CharacterStateBase.h"
#include "ICharacterStateManager.h"

class CharacterBase;

class CharacterStateManagerBase : public ICharacterStateManager
{
public:
	~CharacterStateManagerBase() {};

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override {}
	//更新
	void Update(const eCHARACTER_STATE& charaState) override {}
	//終了処理
	void Finalize() override {}

protected:
	//キャラクターの状態のポインタの配列
	std::unique_ptr<CharacterStateBase> m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MAX_NUM)];

};
