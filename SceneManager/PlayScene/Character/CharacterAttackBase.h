///////////////////////////////////
//キャラクターの攻撃の基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月13日
///////////////////////////////////

#pragma once

#include <SimpleMath.h>
#include "GeometricPrimitive.h"
#include "../../DeviceResources.h"

class CharacterBase;
struct AttackStruct;

class CharacterAttackBase
{
public:
	~CharacterAttackBase() {};
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