///////////////////////////////////
//キャラクターの攻撃の基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月13日
///////////////////////////////////

#pragma once

#include <SimpleMath.h>
#include "GeometricPrimitive.h"
#include "ICharacterAttack.h"
#include "DeviceResources.h"

class CharacterBase;
struct AttackStruct;

class CharacterAttackBase : public ICharacterAttack
{
public:
	~CharacterAttackBase() {};
	//初期化
	void Initialize(CharacterBase* pCharacter) override {}
	//更新
	void Update()override {}
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override {}
	//終了処理
	void Finalize() override {}
	//リセット
	void Reset() override {}

protected:
	//キャラクターのポインタ
	CharacterBase* m_pCharacter;
	//攻撃のポインタ
	AttackStruct* m_pAttack;
	//弾のジオメトリプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBulletGP;
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//デバイスリソースのポインタ
	DX::DeviceResources* m_pDeviceResources;
};