///////////////////////////////////
//キャラクターの攻撃管理クラスの基底
//作成者：GS3_05_稲垣
//作成日：2020年10月23日
///////////////////////////////////

#pragma once

#include "AttackType.h"
#include <SimpleMath.h>
#include "../../DeviceResources.h"
#include "ICharacterAttackManager.h"

class CharacterBase;
class CharacterAttackBase;

class CharacterAttackManagerBase : public ICharacterAttackManager
{
public:
	//デストラクタ
	~CharacterAttackManagerBase() {}

	//初期化
	void Initialize(CharacterBase* pCharacter) override {}
	//更新
	void Update() override {}
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override {}
	//終了処理
	void Finalize()override {}
	//リセット
	void Reset()override {}

};
