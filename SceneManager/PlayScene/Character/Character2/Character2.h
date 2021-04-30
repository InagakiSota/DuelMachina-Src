///////////////////////////////////
//キャラクター2のクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月13日
///////////////////////////////////
#pragma once

#include "../CharacterBase.h"
#include "Keyboard.h"
#include "Character2Params.h"

class CharacterStateManagerBase;

class Character2 : public CharacterBase
{
public:
	//コンストラクタ
	Character2(ePLAYER_ID playerID);
	//デストラクタ
	~Character2();

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
	//初期座標の設定
	void SetStartPos(const DirectX::SimpleMath::Vector3& startPos) override
	{
		m_startPos = startPos;
		m_pos = m_startPos;
	}

	//足元の当たり判定の箱の取得
	Collision::BoxCollision GetLegCollBox() override
	{
		return m_legCollBox;
	}

	//足元の当たり判定の箱の設定
	void SetLegCollBox(const Collision::BoxCollision& legCollBox) override
	{
		m_legCollBox = legCollBox;
	}

	//体の当たり判定の箱の取得
	Collision::BoxCollision GetBodyCollBox() override
	{
		return m_bodyCollBox;
	}

	//体の当たり判定の箱の設定
	void SetBodyCollBox(const Collision::BoxCollision& bodyCollBox) override
	{
		m_bodyCollBox = bodyCollBox;
	}

	//頭の当たり判定の箱の取得
	Collision::BoxCollision GetHeadCollBox() override
	{
		return m_headCollBox;
	}

	//頭の当たり判定の箱の設定
	void SetHeadCollBox(const Collision::BoxCollision& headCollBox) override
	{
		m_headCollBox = headCollBox;
	}

	//敵の体との当たり判定
	void HitEnemyBody(const Collision::BoxCollision& enemyBodyColl, const Collision::BoxCollision& enemyHeadColl) override;
	//攻撃マネージャーの取得
	AttackManager* GetAttackManager()
	{
		return m_pAttackManager;
	}
	//攻撃マネージャーの設定
	void SetAttackManager(AttackManager* pAttackManager) override
	{
		m_pAttackManager = pAttackManager;
	}

	//攻撃との当たり判定
	void HitAttack()override;
	//敵の座標の設定
	void SetEnemyPos(const DirectX::SimpleMath::Vector3& enemyPos) override
	{
		m_enemyPos = enemyPos;
	}
	//敵の座標の取得
	DirectX::SimpleMath::Vector3 GetEnemyPos() override
	{
		return m_enemyPos;
	}

	//敵のポインタの取得
	void SetEnemy(CharacterBase* pEnemy) override
	{
		m_pEnemy = pEnemy;
	}


	//座標の取得
	DirectX::SimpleMath::Vector3 GetPos() override
	{
		return m_pos;
	}

	//座標の設定
	void SetPos(const DirectX::SimpleMath::Vector3& pos) override
	{
		m_pos = pos;
	}

	//Y軸の角度の取得
	float GetAngleY() override
	{
		return m_angleY;
	}

	//Y軸の角度の設定
	void SetAngleY(const float& angleY) override
	{
		m_angleY = angleY;
	}

	//移動量の取得
	DirectX::SimpleMath::Vector3 GetVel() override
	{
		return m_vel;
	}

	//移動量の設定
	void SetVel(const DirectX::SimpleMath::Vector3& vel) override
	{
		m_vel = vel;
	}

	//キャラクターのステートの取得
	eCHARACTER_STATE GetCharaState() override
	{
		return m_charaState;
	}

	//キャラクターのステートの設定
	void SetCharaState(const eCHARACTER_STATE& charaState) override
	{
		m_charaState = charaState;
	}

	//接地フラグの取得
	bool GetLandingFlag() override
	{
		return m_landingFlag;
	}

	//接地フラグの設定
	void SetLandingFlag(const bool& landingFlag) override
	{
		m_landingFlag = landingFlag;
	}

	//ジャンプフラグの取得
	bool GetJumpFlag() override
	{
		return m_jumpFlag;
	}

	//ジャンプフラグの設定
	void SetJumpFlag(const bool& jumpFlag) override
	{
		m_jumpFlag = jumpFlag;
	}

	//ダメージタイマーの取得
	float GetDamageTimer() override
	{
		return m_damageTimer;
	}

	//ダメージタイマーの設定
	void SetDamageTimer(const float& damageTimer) override
	{
		m_damageTimer = damageTimer;
	}

	//前方向のベクトルの取得
	DirectX::SimpleMath::Vector3 GetFrontVector() override
	{
		return m_frontVector;
	}

	//攻撃用の前方向のベクトルの取得
	DirectX::SimpleMath::Vector3 GetAttackFront() override
	{
		return m_attackFront;
	}

	//攻撃用の前方向のベクトルの設定
	void SetAttackFront(const DirectX::SimpleMath::Vector3& attackFront) override
	{
		m_attackFront = attackFront;
	}

	//体力の取得
	int GetHP() override
	{
		return m_HP;
	}
	
	//体力の設定
	void SetHP(const int& hp) override
	{
		m_HP = hp;
	}

	//最大体力の取得
	int GetMaxHP()  override
	{
		return  Character2Params::GetInstance()->MAX_HP;
	}

	//ブースト容量の取得
	int GetBoostCap() override
	{
		return m_boostCap;
	}

	//ブースト容量の設定
	void SetBoostCap(const int& boostCap) override
	{
		m_boostCap = boostCap;
	}

	//最大ブースト容量の取得
	int GetMaxBoostCap() override
	{
		return  Character2Params::GetInstance()->BOOST_CAP_MAX;
	}

	//攻撃構造体のポインタの取得
	AttackStruct* GetAttack() override
	{
		return m_pAttack;
	}
	//攻撃構造体のポインタの設定
	void SetAttack(AttackStruct* pAttack) override
	{
		m_pAttack = pAttack;
	}

	//攻撃の使用中のフラグの取得
	bool GetIsAttackUse(const int& index) override
	{
		return m_isAttackUse[index];
	}

	//攻撃の使用中のフラグの設定
	void SetIsAttackUse(const int& index, bool flag) override
	{
		m_isAttackUse[index] = flag;
	}

	//攻撃の入力フラグの取得
	bool GetIsAttackInput(const int& index) override
	{
		return m_isAttackInput[index];
	}

	//攻撃の入力フラグの設定
	void SetIsAttackInput(const int& index, bool flag) override
	{
		m_isAttackInput[index] = flag;
	}

	//プレイヤーIDの取得
	ePLAYER_ID GetPlayerID()
	{
		return m_playerID;
	}


	//攻撃中のフラグの取得
	bool GetIsAttacking() override
	{
		return m_isAttacking;
	}
	//攻撃中のフラグの設定
	void SetIsAttacking(const bool& isAttacking) override
	{
		m_isAttacking = isAttacking;
	}

	//ステップタイマーの取得
	DX::StepTimer GetStepTimer()
	{
		return m_stepTimer;
	}


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

};