///////////////////////////////////
//キャラクター１のやられ状態のクラス
//作成者：なし
//作成日：なし
///////////////////////////////////
#pragma once

#include "../../CharacterStateBase.h"

class Character1DamageState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character1DamageState();
	//デストラクタ
	~Character1DamageState();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;
	//更新
	void Update() override;

private:
	//ステートが戻るまでの時間
	static const float STATE_BACK_TIME;
	//ノックバックの移動量の初期値
	static const float KNOCK_BACK_VEL_START;
	//ノックバックの移動量
	static const float KNOCK_BACK_VEL;
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;

};
