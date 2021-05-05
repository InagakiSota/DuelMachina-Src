///////////////////////////////
//キャラクターの攻撃のインターフェースクラス
//作成日：2021年5月4日
//作成者：GT2_04_稲垣颯太
//////////////////////////////
#pragma once

#include "pch.h"
#include <SimpleMath.h>

class CharacterBase;

class ICharacterAttack
{
public:
	virtual ~ICharacterAttack() {}
	//初期化
	virtual void Initialize(CharacterBase* pCharacter) = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	//終了処理
	virtual void Finalize() = 0;
	//リセット
	virtual void Reset() = 0;

};