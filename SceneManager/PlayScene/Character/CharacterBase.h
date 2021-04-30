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

//前方宣言
enum class ePLAYER_ID;
class AttackManager;
struct AttackStruct;
class FbxModel;
enum class eCHARACTER_STATE;


class CharacterBase
{
public:
	//移動制限
	static const float MOVE_LIMIT_X;

	//デストラクタ
	virtual ~CharacterBase();

	//初期化
	virtual void Initialize() = 0;
	//更新
	virtual void Update(DX::StepTimer const& timer) = 0;
	//描画
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	//終了
	virtual void Finalize() = 0;
	//床との当たり判定
	virtual void HitFloor(const Collision::BoxCollision& floorColl) = 0;

	////////////////////////////////
	//ゲッター、セッター
	////////////////////////////////
	//初期座標の設定
	virtual void SetStartPos(const DirectX::SimpleMath::Vector3& startPos) = 0;
	//足元の当たり判定の箱の取得
	virtual Collision::BoxCollision GetLegCollBox() = 0;
	//足元の当たり判定の箱の設定
	virtual void SetLegCollBox(const Collision::BoxCollision& legCollBox) = 0;
	//体の当たり判定の箱の取得
	virtual Collision::BoxCollision GetBodyCollBox() = 0;
	//体の当たり判定の箱の設定
	virtual void SetBodyCollBox(const Collision::BoxCollision& bodyCollBox) = 0;
	//頭の当たり判定の箱の取得
	virtual Collision::BoxCollision GetHeadCollBox() = 0;
	//頭の当たり判定の箱の設定
	virtual void SetHeadCollBox(const Collision::BoxCollision& headCollBox) = 0;
	//敵の体との当たり判定
	virtual void HitEnemyBody(const Collision::BoxCollision& enemyBodyColl, const Collision::BoxCollision& enemyHeadColl) = 0;
	//攻撃マネージャーの取得
	virtual AttackManager* GetAttackManager() = 0;
	//攻撃マネージャーの設定
	virtual void SetAttackManager(AttackManager* pAttackManager) = 0;
	//攻撃との当たり判定
	virtual void HitAttack() = 0;
	//敵の座標の設定
	virtual void SetEnemyPos(const DirectX::SimpleMath::Vector3& enemyPos) = 0;
	//敵の座標の取得
	virtual DirectX::SimpleMath::Vector3 GetEnemyPos() = 0;
	//敵のポインタの取得
	virtual void SetEnemy(CharacterBase* pEnemy) = 0;
	//座標の取得
	virtual DirectX::SimpleMath::Vector3 GetPos() = 0;
	//座標の設定
	virtual void SetPos(const DirectX::SimpleMath::Vector3& pos) = 0;
	//Y軸の角度の取得
	virtual float GetAngleY() = 0;
	//Y軸の角度の設定
	virtual void SetAngleY(const float& angleY) = 0;
	//移動量の取得
	virtual DirectX::SimpleMath::Vector3 GetVel() = 0;
	//移動量の設定
	virtual void SetVel(const DirectX::SimpleMath::Vector3& vel) = 0;
	//キャラクターのステートの取得
	virtual eCHARACTER_STATE GetCharaState() = 0;
	//キャラクターのステートの設定
	virtual void SetCharaState(const eCHARACTER_STATE& state) = 0;
	//接地フラグの取得
	virtual bool GetLandingFlag() = 0;
	//接地フラグの設定
	virtual void SetLandingFlag(const bool& landingFlag) = 0;
	//ジャンプフラグの取得
	virtual bool GetJumpFlag() = 0;
	//ジャンプフラグの設定
	virtual void SetJumpFlag(const bool& jumpFlag) = 0;
	//ダメージタイマーの取得
	virtual float GetDamageTimer() = 0;
	//ダメージタイマーの設定
	virtual void SetDamageTimer(const float& damageTimer) = 0;
	//前方向のベクトルの取得
	virtual DirectX::SimpleMath::Vector3 GetFrontVector() = 0;
	//攻撃用の前方向のベクトルの取得
	virtual DirectX::SimpleMath::Vector3 GetAttackFront() = 0;
	//攻撃用の前方向のベクトルの設定
	virtual void SetAttackFront(const DirectX::SimpleMath::Vector3& attackFront) = 0;
	//体力の取得
	virtual int GetHP() = 0;
	//体力の設定
	virtual void SetHP(const int& hp) = 0;
	//最大体力の取得
	virtual int GetMaxHP() = 0;
	//ブースト容量の取得
	virtual int GetBoostCap() = 0;
	//ブースト容量の設定
	virtual void SetBoostCap(const int& boostCap) = 0;
	//最大ブースト容量の取得
	virtual int GetMaxBoostCap() = 0;
	//攻撃構造体のポインタの取得
	virtual AttackStruct* GetAttack() = 0;
	//攻撃構造体のポインタの設定
	virtual void SetAttack(AttackStruct* pAttack) = 0;
	//攻撃の使用中のフラグの取得
	virtual bool GetIsAttackUse(const int& index) = 0;
	//攻撃の使用中のフラグの設定
	virtual void SetIsAttackUse(const int& index, bool flag) = 0;
	//攻撃の入力フラグの取得
	virtual bool GetIsAttackInput(const int& index) = 0;
	//攻撃の入力フラグの設定
	virtual void SetIsAttackInput(const int& index, bool flag) = 0;
	//プレイヤーIDの取得
	virtual ePLAYER_ID GetPlayerID() = 0;
	//攻撃中のフラグの取得
	virtual bool GetIsAttacking() = 0;
	//攻撃中のフラグの設定
	virtual void SetIsAttacking(const bool& isAttacking) = 0;
	//ステップタイマーの取得
	virtual DX::StepTimer GetStepTimer() = 0;

	/////////////////////////////////////

	//準備
	virtual void Ready(DX::StepTimer const& timer) = 0;
	//敗北
	virtual void Lose(DX::StepTimer const& timer) = 0;

	//リセット
	virtual void Reset() = 0;

	//アニメーションの切り替え
	virtual void ChangeAnimation(const int& animationStack) = 0;


protected:
	static const int ATTACK_NUM = 32;
	//重力
	static const float GRAVITY;	

	//デバイスリソースのポインタ
	DX::DeviceResources* m_pDeviceResources;		
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
	int m_HP;
	//体力のバッファ
	int m_HPBuffer;

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

	//キャラのステート変数
	eCHARACTER_STATE m_charaState;

	//キャラのステート管理
	virtual void StateManager() = 0;
	//攻撃
	virtual void Attack() = 0;

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

	//タイマー
	DX::StepTimer m_stepTimer;
};
