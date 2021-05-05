///////////////////////////////////
//キャラクター１のクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月13日
///////////////////////////////////
#pragma once

#include "../CharacterBase.h"
#include "Keyboard.h"
#include "Character1Params.h"


class Character1 : public CharacterBase
{
public:
	//コンストラクタ
	Character1(ePLAYER_ID playerID);
	//デストラクタ
	~Character1();

	//初期化
	void Initialize() override;
	//更新
	void Update(DX::StepTimer const& timer) override;
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	//終了
	void Finalize() override;
	//床との当たり判定
	void HitFloor(const Collision::BoxCollision& floorColl) override;
	//敵の体との当たり判定
	void HitEnemyBody(const Collision::BoxCollision& enemyBodyColl, const Collision::BoxCollision& enemyHeadColl) override;
	//攻撃との当たり判定
	void HitAttack() override;
	//準備
	void Ready(DX::StepTimer const& timer) override;
	//敗北
	void Lose(DX::StepTimer const& timer) override;
	//リセット
	void Reset() override;
	//アニメーションの切り替え
	void ChangeAnimation(const int& animationStack) override;

private:

	//攻撃
	void Attack()override;
	//キャラのステート管理
	void StateManager()override;
	//AI
	void AI();
	//当たり判定の更新
	void CollisionUpdate()override;


	int thinkTimer;

	float toEnemyRange;

};