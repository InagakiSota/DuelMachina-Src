///////////////////////////////////
//キャラクター３の弱ニュートラル
//作成者：GS305_稲垣颯太
//作成日：2020年11月23日
///////////////////////////////////

#pragma once

#include "../../CharacterAttackBase.h"

class Character3AttackWeakNeutral : public CharacterAttackBase
{
public:
	//コンストラクタ
	Character3AttackWeakNeutral();
	//デストラクタ
	~Character3AttackWeakNeutral();

	//初期化
	void Initialize(CharacterBase* pCharacter) override;
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
	//消えるまでの時間
	static const float VANISH_TIME;
	//X座標
	static const float POS_X;
	//Y座標
	static const float POS_Y;
	//X方向の移動量
	static const float VEL_X;
	//当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 COLL_SIZE;
	//ジオメトリプリミティブのサイズ
	static const float GP_SIZE;
};