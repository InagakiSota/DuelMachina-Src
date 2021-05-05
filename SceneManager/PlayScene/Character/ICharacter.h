///////////////////////////////
//キャラクターのインターフェースクラス
//作成日：2021年5月4日
//作成者：GT2_04_稲垣颯太
//////////////////////////////

#pragma once

#include <SimpleMath.h>
#include "StepTimer.h"
#include "Src/FrameWork/Collision/Collision.h"

//前方宣言
class AttackManager;
class CharacterBase;
enum class eCHARACTER_STATE;
enum class ePLAYER_ID;
enum class eCHARACTER_ID;
struct AttackStruct;
class FbxModel;
class BoostEffectManager;

class ICharacrter
{
public:
	//デストラクタ
	virtual ~ICharacrter() {}

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
	//準備
	virtual void Ready(DX::StepTimer const& timer) = 0;
	//敗北
	virtual void Lose(DX::StepTimer const& timer) = 0;

	//リセット
	virtual void Reset() = 0;

	//アニメーションの切り替え
	virtual void ChangeAnimation(const int& animationStack) = 0;

	//キャラのステート管理
	virtual void StateManager() = 0;
	//攻撃
	virtual void Attack() = 0;
	
	//当たり判定の更新
	virtual void CollisionUpdate() = 0;


	////////////////////////////////
	//ゲッター、セッター
	////////////////////////////////
	//3Dモデルのポインタの取得
	virtual FbxModel* GetFbxModel() = 0;
	//3Dモデルのポインタの設定
	virtual void SetFbxModel( FbxModel* pFbxModel) = 0;
	//初期座標の設定
	virtual void SetStartPos(const DirectX::SimpleMath::Vector3& startPos) = 0;
	//初期座標の取得
	virtual DirectX::SimpleMath::Vector3 GetStartPos() = 0;
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
	//敵のポインタの設定
	virtual CharacterBase* GetEnemy() = 0;
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
	//前方向のベクトルの設定
	virtual void SetFrontVector(const DirectX::SimpleMath::Vector3& frontVector) = 0;
	//攻撃用の前方向のベクトルの取得
	virtual DirectX::SimpleMath::Vector3 GetAttackFront() = 0;
	//攻撃用の前方向のベクトルの設定
	virtual void SetAttackFront(const DirectX::SimpleMath::Vector3& attackFront) = 0;
	//体力の取得
	virtual int GetHP() = 0;
	//体力の設定
	virtual void SetHP(const int& hp) = 0;
	//体力のバッファの取得
	virtual int GetHPBuffer() = 0;
	//体力のバッファの設定
	virtual void SetHPBuffer(const int& hpBuffer) = 0;
	//最大体力の設定
	virtual void SetHPMax(const int& hpMax) = 0;
	//最大体力の取得
	virtual int GetMaxHP() = 0;
	//ブースト容量の取得
	virtual int GetBoostCap() = 0;
	//ブースト容量の設定
	virtual void SetBoostCap(const int& boostCap) = 0;
	//最大ブースト容量の取得
	virtual int GetMaxBoostCap() = 0;
	//最大ブースト容量の設定
	virtual void SetMaxBoostCap(const int& boostCapMax) = 0;
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
	//キャラクターのIDの取得
	virtual eCHARACTER_ID GetCharacterID() = 0;
	//キャラクターのIDの設定
	virtual void SetCharacterID(const eCHARACTER_ID& characterID) = 0;
	//攻撃中のフラグの取得
	virtual bool GetIsAttacking() = 0;
	//攻撃中のフラグの設定
	virtual void SetIsAttacking(const bool& isAttacking) = 0;
	//ステップタイマーの取得
	virtual DX::StepTimer GetStepTimer() = 0;
	//ワールド行列の取得
	virtual DirectX::SimpleMath::Matrix GetWorldMatrix() = 0;
	//ワールド行列の設定
	virtual void SetWorldMatrix(const DirectX::SimpleMath::Matrix& worldMatrix) = 0;
	//シールドのワールド行列の取得
	virtual DirectX::SimpleMath::Matrix GetShieldWorldMatrix() = 0;
	//シールドのワールド行列の設定
	virtual void SetShieldWorldMatrix(const DirectX::SimpleMath::Matrix& shieldWorldMatrix) = 0;
	//ブーストエフェクトマネージャーのポインタ取得
	virtual BoostEffectManager* GetBoostEffectManager() = 0;

	/////////////////////////////////////



};
