///////////////////////////////////
//キャラクターの基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月13日
///////////////////////////////////
#pragma once

#include <SimpleMath.h>
#include "GeometricPrimitive.h"
#include "Src\FrameWork\ModelObject\ModelObject.h"
#include "Src\FrameWork\Collision\Collision.h"
#include "Keyboard.h"
#include "AttackType.h"
#include "CharacterStateManagerBase.h"
#include "CharacterAttackManagerBase.h"
#include "HitEffectManager.h"
#include "BoostEffectManager.h"
#include "../../../FrameWork/ModelObject/ModelObject.h"
#include "ICharacter.h"

//前方宣言
enum class ePLAYER_ID;
class AttackManager;
struct AttackStruct;
class FbxModel;
enum class eCHARACTER_STATE;
enum class eCHARACTER_ID;


class CharacterBase : public ICharacrter
{
public:

	//移動制限
	static const float MOVE_LIMIT_X;

	//攻撃の種類の数
	static const int ATTACK_NUM = 32;
	//重力
	static const float GRAVITY;
	//カメラの座標
	static const DirectX::SimpleMath::Vector3 CAMERA_POS;

	//コンストラクタ
	CharacterBase(ePLAYER_ID playerID);
	//デストラクタ
	~CharacterBase();
	
	//初期化
	void Initialize() override;

	//更新
	void Update(DX::StepTimer const& timer) override;
	//描画
	void  Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	//終了処理
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


	////////////////////////////////
	//ゲッター、セッター
	////////////////////////////////
	//3Dモデルのポインタの取得
	FbxModel* GetFbxModel() override
	{
		return m_pFbxModel;
	}
	//3Dモデルのポインタの設定
	void SetFbxModel(FbxModel* pFbxModel) override
	{
		m_pFbxModel = pFbxModel;
	}

	//初期座標の設定
	 void SetStartPos(const DirectX::SimpleMath::Vector3& startPos) override
	 {
		 m_startPos = startPos;
		 m_pos = m_startPos;
	 }
	 //初期座標の取得
	 DirectX::SimpleMath::Vector3 GetStartPos()override
	 {
		 return m_startPos;
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

	//攻撃マネージャーの取得
	 AttackManager* GetAttackManager() override
	 {
		 return m_pAttackManager;
	 }

	//攻撃マネージャーの設定
	 void SetAttackManager(AttackManager* pAttackManager) override
	 {
		 m_pAttackManager = pAttackManager;
	 }

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
	 //敵のポインタの設定
	 CharacterBase* GetEnemy() override
	 {
		 return m_pEnemy;
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
	 void SetCharaState(const eCHARACTER_STATE& state) override
	 {
		 m_charaState = state;
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

	 //前方向のベクトルの設定
	 void SetFrontVector(const DirectX::SimpleMath::Vector3& frontVector) override
	 {
		 m_frontVector = frontVector;
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
		 return m_hp;
	 }
	//体力の設定
	 void SetHP(const int& hp) override
	 {
		 m_hp = hp;
	 }
	 //体力のバッファの取得
	 int GetHPBuffer() override
	 {
		 return m_hpBuffer;
	 }
	 //体力のバッファの設定
	 void SetHPBuffer(const int& hpBuffer) override
	 {
		 m_hpBuffer = hpBuffer;
	 }

	 //最大体力の設定
	 void SetHPMax(const int& hpMax)override
	 {
		 m_hpMax = hpMax;
	 }
	//最大体力の取得
	 int GetMaxHP() override
	 {
		 return m_hpMax;
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
		 return m_boostCapMax;
	 }
	 //最大ブースト容量の設定
	 void SetMaxBoostCap(const int& boostCapMax) override
	 {
		 m_boostCapMax = boostCapMax;
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
	 ePLAYER_ID GetPlayerID() override
	 {
		 return m_playerID;
	 }
	 //キャラクターのIDの取得
	 eCHARACTER_ID GetCharacterID() override
	 {
		 return m_characterID;
	 }
	 //キャラクターのIDの設定
	 void SetCharacterID(const eCHARACTER_ID& characterID) override
	 {
		 m_characterID = characterID;
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
	 DX::StepTimer GetStepTimer() override
	 {
		 return m_stepTimer;
	 }
	 //ワールド行列の取得
	 DirectX::SimpleMath::Matrix GetWorldMatrix() override
	 {
		 return m_world;
	 }
	 //ワールド行列の設定
	 void SetWorldMatrix(const DirectX::SimpleMath::Matrix& worldMatrix) override
	 {
		 m_world = worldMatrix;
	 }
	 //シールドのワールド行列の取得
	 DirectX::SimpleMath::Matrix GetShieldWorldMatrix() override
	 {
		 return m_shieldWorld;
	 }
	 //シールドのワールド行列の設定
	 void SetShieldWorldMatrix(const DirectX::SimpleMath::Matrix& shieldWorldMatrix) override
	 {
		 m_shieldWorld = shieldWorldMatrix;
	 }

	 //ブーストエフェクトマネージャーのポインタ取得
	 BoostEffectManager* GetBoostEffectManager() override
	 {
		 return m_pBoostEffectManager.get();
	 }

	/////////////////////////////////////

public:
	//アニメーションの切り替え
	 void ChangeAnimation(const int& animationStack) override;

	 //キャラのステート管理
	 void StateManager() override;
	 //攻撃
	 void Attack() override;

	 //CPUの挙動
	 void AI();

	 //当たり判定の更新
	 void CollisionUpdate() {}



private:
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;						
	//座標
	DirectX::SimpleMath::Vector3 m_pos;
	//初期座標
	DirectX::SimpleMath::Vector3 m_startPos;
	//移動量
	DirectX::SimpleMath::Vector3 m_vel;
	//3Dモデル
	std::unique_ptr<ModelObject> m_pModel;
	//足元の箱
	std::unique_ptr<DirectX::GeometricPrimitive> m_pLegBox;		
	//足元の当たり判定用の構造体
	Collision::BoxCollision m_legCollBox;						
	//足元の箱のワールド行列
	DirectX::SimpleMath::Matrix m_legBoxWorld;
	//体の箱
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBodyBox;
	//体の箱(しゃがみ)
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBodySquatBox;
	//体の当たり判定用の構造体
	Collision::BoxCollision m_bodyCollBox;
	//体の箱のワールド行列
	DirectX::SimpleMath::Matrix m_bodyBoxWorld;
	//頭の箱
	std::unique_ptr<DirectX::GeometricPrimitive> m_pHeadBox;
	//頭の箱の当たり判定構造体
	Collision::BoxCollision m_headCollBox;
	//頭の箱のワールド行列
	DirectX::SimpleMath::Matrix m_headBoxWorld;
	//前方向のベクトル
	DirectX::SimpleMath::Vector3 m_frontVector;
	//攻撃用の前方向のベクトル
	DirectX::SimpleMath::Vector3 m_attackFront;
	//敵の体と当たった時のフラグ
	bool m_isHitEnemyBody;
	//座標のバッファ
	DirectX::SimpleMath::Vector3 m_posBuf;
	//攻撃のマネージャーのポインタ
	AttackManager* m_pAttackManager;
	//攻撃の構造体のポインタ
	AttackStruct* m_pAttack;
	//敵の座標
	DirectX::SimpleMath::Vector3 m_enemyPos;
	//敵のポインタ
	CharacterBase* m_pEnemy;

	DirectX::SimpleMath::Vector4 m_bodyColor;
	//各状態のモデルのポインタ
	//FBXモデル
	FbxModel* m_pFbxModel;
	//シールドのモデル
	FbxModel* m_pShieldModel;
	//シールドのモデルのワールド行列
	DirectX::SimpleMath::Matrix m_shieldWorld;
	//着地フラグ
	bool m_landingFlag;											
	//ジャンプフラグ
	bool m_jumpFlag;					
	//プレイヤーのID(プレイヤー１か２か)
	ePLAYER_ID m_playerID;
	//キートラッカーのポインタ
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_pKeyTracker;
	//体力
	int m_hp;
	//体力の最大値
	int m_hpMax;
	//体力のバッファ
	int m_hpBuffer;
	//やられ状態のタイマー
	float m_damageTimer;
	//Y軸の角度
	float m_angleY;
	//AIのステートタイマー
	float m_aiStateTiemer;
	//AIの攻撃タイマー
	float m_aiAttackTimer;
	//AIのステート
	int m_aiState;
	//攻撃を使用しているフラグ
	bool m_isAttackUse[static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM)];
	//攻撃の入力フラグ
	bool m_isAttackInput[static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM)];
	//攻撃中のフラグ
	bool m_isAttacking;

	//ブースト容量
	int m_boostCap;
	//ブースト容量の最大値
	int m_boostCapMax;

	//キャラのステート変数
	eCHARACTER_STATE m_charaState;

	//ステートマネージャーのポインタ
	std::unique_ptr<CharacterStateManagerBase> m_pStateManager;
	//攻撃マネージャーのポインタ
	std::unique_ptr<CharacterAttackManagerBase> m_pCharacterAttackManager;
	//ヒットエフェクトマネージャーのポインタ
	std::unique_ptr<HitEffectManager> m_pHitEffectManager;
	//ブーストエフェクトマネージャーのポインタ
	std::unique_ptr<BoostEffectManager> m_pBoostEffectManager;

	//弾のジオメトリプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBulletGP;
	//弾のワールド行列
	DirectX::SimpleMath::Matrix m_bulletWorld;
	//各キャラクターのID
	eCHARACTER_ID m_characterID;
	//タイマー
	DX::StepTimer m_stepTimer;

};
