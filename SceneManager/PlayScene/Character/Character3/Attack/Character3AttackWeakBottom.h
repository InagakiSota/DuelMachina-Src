///////////////////////////////////
//キャラクター３の弱下攻撃クラス
//作成者：GS305_稲垣颯太2020年11月23日
//作成日：
///////////////////////////////////

#pragma once

#include "../../CharacterAttackBase.h"

class Character3AttackWeakBottom : public CharacterAttackBase
{
public:
	//コンストラクタ
	Character3AttackWeakBottom();
	//デストラクタ
	~Character3AttackWeakBottom();

	//初期化
	void Initialize(CharacterBase* pCharacter, DX::DeviceResources* pDeviceResources) override;
	//更新
	void Update() override;
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	//終了処理
	void Finalize() override;
	//リセット
	void Reset() override;

private:
	//攻撃力
	static const int POWER;
	//X座標
	static const float POS_X;
	//Y座標
	static const float POS_Y;
	//当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 COLL_SIZE;
	//消えるまでの時間
	static const float VANISH_TIME;
};