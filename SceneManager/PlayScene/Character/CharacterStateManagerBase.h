///////////////////////////////////
//キャラクターのステート管理クラスの基底
//作成者：GS3_05_稲垣
//作成日：2020年10月17日
///////////////////////////////////

#pragma once
#include "CharacterState.h"
#include "CharacterStateBase.h"

class CharacterBase;

class CharacterStateManagerBase
{
public:
	~CharacterStateManagerBase() {};

	//初期化
	virtual void Initialize(CharacterBase* pCharacterBase) = 0;
	//更新
	virtual void Update(const eCHARACTER_STATE& charaState) = 0;
	//終了処理
	virtual void Finalize() = 0;

protected:
	//キャラクターの状態のポインタの配列
	std::unique_ptr<CharacterStateBase> m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MAX_NUM)];

};
