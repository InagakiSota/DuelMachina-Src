///////////////////////////////////
//キャラクタ－１のしゃがみ状態のクラス
//作成者：GS3_05_稲垣
//作成日：2020年10月22日
///////////////////////////////////

#pragma once

#include "../../CharacterStateBase.h"
#include <SimpleMath.h>

class CharacterBase;

class Character1SquatState : public CharacterStateBase
{
public:
	//コンストラクタ
	Character1SquatState();
	//デストラクタ
	~Character1SquatState();

	//初期化
	void Initialize(CharacterBase* pCharacterBase) override;
	//更新
	void Update() override;

private:
	//Y軸の角度(マイナス)
	static const float ANGLE_Y_MINUS;
	//Y軸の角度(プラス)
	static const float ANGLE_Y_PLUS;
	//体の当たり判定のY座標
	static const float BODY_COLL_BOX_POS_Y;
	//頭の当たり判定のY座標
	static const float HEAD_COLL_BOX_POS_Y;
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;


};