///////////////////////////////////
//キャラクタ－１の後移動状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#pragma once

#include "../../CharacterStateBase.h"

class CharacterBase;
enum class eCHARACTER_STATE;

class Character1MoveBackState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character1MoveBackState();
	//デストラクタ
	~Character1MoveBackState();

	//初期化
	void Initialize(CharacterBase* pCharacter) override;
	//更新
	void Update() override;
private:
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;

};