///////////////////////////////////
//キャラクター２の弱ニュートラル攻撃
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月20日
///////////////////////////////////

#pragma once

#include "../../CharacterAttackBase.h"

class Character2AttackWeakNeutral : public CharacterAttackBase
{
public:
	//コンストラクタ
	Character2AttackWeakNeutral();
	//デストラクタ
	~Character2AttackWeakNeutral();

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
	//X座標
	static const float POS_X;
	//Y座標
	static const float POS_Y;
	//当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 COLL_SIZE;
	//消えるまでの時間
	static const float VANISH_TIME;

};
