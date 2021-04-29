///////////////////////////////////
//キャラクター３の中ニュートラル攻撃クラス
//作成者：GS305_稲垣颯太
//作成日：2020年11月23日
///////////////////////////////////

#pragma once

#include "../../CharacterAttackBase.h"
#include "../../HitEffectManager.h"



class Character3AttackMiddleNeutral : public CharacterAttackBase
{
public:
	//コンストラクタ
	Character3AttackMiddleNeutral();
	//デストラクタ
	~Character3AttackMiddleNeutral();

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
	//X方向の移動量
	static const float VEL_X;
	//Y方向の移動量
	static const float VEL_Y;
	//消えるまでの時間
	static const float VANISH_TIME;
	//ジオメトリプリミティブのサイズ
	static const float GP_SIZE;
	//弾道の数
	static const int BALLISTIC_NUM = 5;
	//当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 COLL_SIZE;
	//弾道のジオメトリプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBallisticGPArray[BALLISTIC_NUM];
	//弾道の座標
	DirectX::SimpleMath::Vector3 m_ballisticPos[BALLISTIC_NUM];

	//弾道の添字
	int m_ballisticIndex;
};