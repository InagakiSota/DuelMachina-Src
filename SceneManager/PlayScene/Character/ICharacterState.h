///////////////////////////////
//キャラクターの状態マネージャーのインターフェースクラス
//作成日：2021年5月4日
//作成者：GT2_04_稲垣颯太
//////////////////////////////

#pragma once

class CharacterBase;

class ICharacterState
{
public:
	//デストラクタ
	virtual ~ICharacterState() {}

	//初期化
	virtual void Initialize(CharacterBase* pCharacter) = 0;
	//更新
	virtual void Update() = 0;

};