///////////////////////////////////
//キャラクターの攻撃の基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月13日
///////////////////////////////////

#pragma once

class CharacterBase;

class CharacterStateBase
{
public:
	~CharacterStateBase() {};
	//初期化
	virtual void Initialize(CharacterBase* pCharacter) = 0;
	//更新
	virtual void Update() = 0;

protected:
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;
};