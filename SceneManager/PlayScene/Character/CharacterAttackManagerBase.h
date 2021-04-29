///////////////////////////////////
//キャラクターの攻撃管理クラスの基底
//作成者：GS3_05_稲垣
//作成日：2020年10月23日
///////////////////////////////////

#pragma once

#include "AttackType.h"
#include <SimpleMath.h>
#include "../../DeviceResources.h"

class CharacterBase;
class CharacterAttackBase;

class CharacterAttackManagerBase
{
public:
	//デストラクタ
	~CharacterAttackManagerBase() {}

	//初期化
	virtual void Initialize(CharacterBase* pCharacter, DX::DeviceResources* pDeviceResources) = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	//終了処理
	virtual void Finalize() = 0;
	//リセット
	virtual void Reset() = 0;

protected:
	CharacterAttackBase* m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM)];
};
