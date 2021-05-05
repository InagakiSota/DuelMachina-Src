///////////////////////////////////
//キャラクター３の中横攻撃クラス
//作成者：GS305_稲垣颯太
//作成日：2020年11月23日
///////////////////////////////////

#pragma once

#include "../../CharacterAttackBase.h"

class Character3AttackMiddleSide : public CharacterAttackBase
{
public:
	//コンストラクタ
	Character3AttackMiddleSide();
	//デストラクタ
	~Character3AttackMiddleSide();

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
	//当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 COLL_SIZE;
	//消えるまでの時間
	static const float VANISH_TIME;
	//ジオメトリプリミティブのサイズ
	static const float GP_SIZE;
	//弾道の数
	static const int BALLISTIC_NUM = 10;
	//弾道のジオメトリプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBallisticGPArray[BALLISTIC_NUM];
	//弾道の座標
	DirectX::SimpleMath::Vector3 m_ballisticPos[BALLISTIC_NUM];
	//弾道のワールド行列
	DirectX::SimpleMath::Matrix m_ballisticWorld[BALLISTIC_NUM];

};