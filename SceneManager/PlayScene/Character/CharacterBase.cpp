#include "pch.h"
#include "CharacterBase.h"

#include "../../PlayerID.h"
#include "Src/FrameWork/FbxResourceManager/FbxResourceManager.h"
#include "../Character/CharacterID.h"
#include "Src/Cgdi.h"
#include "Character1/State/Character1StateManager.h"
#include "Character1/Attack/Character1AttackManager.h"
#include "Character2/State/Character2StateManager.h"
#include "Character2/Attack/Character2AttackManager.h"
#include "Character3/State/Character3StateManager.h"
#include "Character3/Attack/Character3AttackManager.h"
#include "Src/FrameWork/FbxModel/FbxModel.h"
#include "ChracterAnimationNumber.h"
#include "Adx2.h"
#include "CueSheet_0.h"
#include "../AttackManager/AttackManager.h"

//重力
const float CharacterBase::GRAVITY = -0.007;
//移動制限
const float CharacterBase::MOVE_LIMIT_X = 3.0f;
//カメラの座標
 const DirectX::SimpleMath::Vector3 CharacterBase::CAMERA_POS = DirectX::SimpleMath::Vector3(0.00f,0.0f,6.0f);

//コンストラクタ
CharacterBase::CharacterBase(ePLAYER_ID playerID)
	: m_jumpFlag(false),
	m_landingFlag(false),
	m_legBoxWorld(DirectX::SimpleMath::Matrix::Identity),
	m_legCollBox{},
	m_pLegBox(nullptr),
	m_pModel(nullptr),
	m_pos{},
	m_vel{},
	m_frontVector{},
	m_pBodyBox(nullptr),
	m_bodyBoxWorld(DirectX::SimpleMath::Matrix::Identity),
	m_bodyCollBox{},
	m_startPos{},
	m_pKeyTracker(nullptr),
	m_playerID(playerID),
	m_isHitEnemyBody(false),
	m_posBuf{},
	m_pAttack(nullptr),
	m_pAttackManager(nullptr),
	m_enemyPos{},
	m_pHeadBox(nullptr),
	m_headCollBox{},
	m_headBoxWorld{},
	m_hp(0),
	m_hpBuffer(0),
	m_charaState(eCHARACTER_STATE::WAIT),
	m_damageTimer( 0.0f),
	m_angleY(0.0f),
	m_aiAttackTimer(0.0f),
	m_aiStateTiemer(0.0f),
	m_aiState(0),
	m_boostCap(0),
	m_pStateManager(nullptr),
	m_pCharacterAttackManager(nullptr),
	m_attackFront{},
	m_isAttacking(false),
	m_pHitEffectManager(nullptr),
	m_pEnemy(nullptr),
	m_characterID(eCHARACTER_ID::NONE)
{
}

CharacterBase::~CharacterBase()
{
	////リソースの解放
	//if (m_pBodyBox != nullptr)
	//{
	//	m_pBodyBox.reset();
	//}

	//if (m_pLegBox != nullptr)
	//{
	//	m_pLegBox.reset();
	//}

	//if (m_pHeadBox != nullptr)
	//{
	//	m_pHeadBox.reset();
	//}


	//if (m_pKeyTracker != nullptr)
	//{
	//	m_pKeyTracker.reset();
	//}

	//if (m_pFbxModel != nullptr)
	//{
	//	m_pFbxModel = nullptr;
	//}
}

//////////////////////////
//初期化
//引数：なし
//戻り値：なし
//////////////////////////
void CharacterBase::Initialize()
{
	//キートラッカーの読み込み
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//シールドのモデルの読み込み
	m_pShieldModel = FbxResourceManager::GetInstance()->GetShieldModel();
	//シールドのモデルのワールド行列を初期化
	m_shieldWorld = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero);
	m_shieldWorld = pos;

	//初期座標の設定	
	m_pos = m_startPos;

	//キャラのステート初期化
	m_charaState = eCHARACTER_STATE::WAIT;

	//攻撃フラグの初期化
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_isAttackUse[i] = false;
		m_isAttackInput[i] = false;
	}

	//攻撃の当たり判定の初期化
	m_pAttack = nullptr;

	//キャラクターごとに読み込むマネージャーを変える
	switch (m_characterID)
	{
		//キャラクター１
		case eCHARACTER_ID::CHARACTER_1:
		{	
			//ステートマネージャーの読み込み
			m_pStateManager = std::make_unique<Character1StateManager>();
			//攻撃マネージャーの読み込み
			m_pCharacterAttackManager = std::make_unique<Character1AttackManager>();
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			//ステートマネージャーの読み込み
			m_pStateManager = std::make_unique<Character2StateManager>();
			//攻撃マネージャーの読み込み
			m_pCharacterAttackManager = std::make_unique<Character2AttackManager>();
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			//ステートマネージャーの読み込み
			m_pStateManager = std::make_unique<Character3StateManager>();
			//攻撃マネージャーの読み込み
			m_pCharacterAttackManager = std::make_unique<Character3AttackManager>();
			break;
		}
		default:
			break;
	}
	//ステートマネージャーの読み込み、初期化
	m_pStateManager->Initialize(this);
	//攻撃マネージャーの読み込み、初期化
	m_pCharacterAttackManager->Initialize(this, gdi->GetDeviceResources());

	//ヒットエフェクトマネージャーの読み込み、作成
	m_pHitEffectManager = std::make_unique<HitEffectManager>();
	m_pHitEffectManager->Create(gdi->GetDeviceResources(), 50);

	//ブーストエフェクトマネージャーの読み込み、作成
	m_pBoostEffectManager = std::make_unique<BoostEffectManager>();
	m_pBoostEffectManager->Initialize(gdi->GetDeviceResources(), 1, 1, DirectX::SimpleMath::Vector3::Zero);

	//弾のワールド行列の初期化
	m_bulletWorld = DirectX::SimpleMath::Matrix::Identity;

}

//////////////////////////
//更新
//引数：タイマー
//戻り値：なし
//////////////////////////
void CharacterBase::Update(DX::StepTimer const& timer)
{
	m_stepTimer = timer;
	//キーボードの状態の取得
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//ステートの管理
	if (m_playerID == ePLAYER_ID::PLAYER_1 && m_hp > 0)
	{
		StateManager();
	}

	//プレイヤー２ならAI操作
	if (m_playerID == ePLAYER_ID::PLAYER_2 && m_hp > 0)
	{
		AI();
		//StateManager();
	}

	//重力落下
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;
	}
	else if (m_landingFlag == true && m_charaState == eCHARACTER_STATE::BOOST_MOVE)m_vel.y = 0.0f;

	//移動制限
	if (m_pos.x > -MOVE_LIMIT_X || m_pos.x < MOVE_LIMIT_X)m_vel.x = 0.0f;
	if (m_pos.x < -MOVE_LIMIT_X) m_pos.x = -MOVE_LIMIT_X;
	if (m_pos.x > MOVE_LIMIT_X) m_pos.x = MOVE_LIMIT_X;

	//キャラのステート更新
	m_pStateManager->Update(m_charaState);

	//回転行列を作成
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));

	//攻撃
	if (m_hp > 0)
	{
		Attack();
	}
	//攻撃との当たり判定
	HitAttack();
	//攻撃マネージャーの更新
	m_pCharacterAttackManager->Update();
	//ヒットエフェクトマネージャーの更新
	m_pHitEffectManager->Update(timer);

	//バッファに座標を格納
	m_posBuf = m_pos;
	//移動量を座標に加算
	m_pos += m_vel;

	//体力の更新
	if (m_hp > m_hpBuffer)m_hp--;

	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);


}

//////////////////////////
//描画
//引数：ビュー行列、射影行列
//戻り値：なし
//////////////////////////
void CharacterBase::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//ガード状態のモデルの描画
	if (m_charaState == eCHARACTER_STATE::GUARD)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::GUARD));

	}
	//しゃがみ状態のモデルの描画
	else if (m_charaState == eCHARACTER_STATE::SQUAT
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::SQUAT));
	}
	//ブースト移動状態のモデルの描画
	else if (m_charaState == eCHARACTER_STATE::BOOST_MOVE)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_BOOST));
		//m_pBoostEffectManager->SetRenderState(view, proj);
		m_pBoostEffectManager->Render(view, proj);
	}

	//前移動状態のモデルの描画
	else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_FRONT));

	}

	//後移動状態のモデルの描画
	else if (m_charaState == eCHARACTER_STATE::MOVE_BACK)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_BACK));
	}

	//ジャンプ状態のモデルの描画
	else if (m_charaState == eCHARACTER_STATE::JUMP)
	{
		//m_pFbxModel_Jump->Draw(m_world,view,proj);
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::JUMP));
	}

	//やられ
	else if (m_charaState == eCHARACTER_STATE::DAMAGE)
	{
		//m_pFbxModel_Damage->Draw(m_world, view, proj);
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::DAMAGE));
	}

	//弱ニュートラル攻撃の描画
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_WEAK_NEUTRAL));

	}

	//弱下攻撃の描画
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_WEAK_BOTTOM));

	}

	//弱横攻撃の描画
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_WEKA_SIDE));

	}

	//中ニュートラル攻撃の描画
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_MIDDLE_NEUTRAL));

	}

	//中下攻撃の描画
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_MIDDLE_BOTTOM));

	}

	//中横攻撃の描画
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_MIDDLE_SIDE));

	}

	//勝利
	else if (m_charaState == eCHARACTER_STATE::WIN)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::WIN_POSE));
	}

	//敗北
	else if (m_charaState == eCHARACTER_STATE::LOSE)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::LOSE_POSE));

	}

	//待機状態のモデルの描画
	else
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::WAIT));
	}


	if (m_playerID == ePLAYER_ID::PLAYER_1)
		m_pFbxModel->Draw(m_world, view, proj, DirectX::SimpleMath::Vector4(0.7f, 0.0f, 0.0f, 1.0f));
	else if (m_playerID == ePLAYER_ID::PLAYER_2)
		m_pFbxModel->Draw(m_world, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.7f, 1.0f));


	if (m_charaState == eCHARACTER_STATE::GUARD)
	{
		//シールドの描画
		if (m_playerID == ePLAYER_ID::PLAYER_1)
			m_pShieldModel->Draw(m_shieldWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.3f));
		else if (m_playerID == ePLAYER_ID::PLAYER_2)
			m_pShieldModel->Draw(m_shieldWorld, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.3f));
	}


	//攻撃マネージャーの描画
	m_pCharacterAttackManager->Render(view, proj);
	//ヒットエフェクトマネージャーの描画
	m_pHitEffectManager->SetRenderState(CAMERA_POS, view, proj);
	m_pHitEffectManager->Render();

}

//////////////////////////
//終了処理
//引数：なし
//戻り値：なし
//////////////////////////
void CharacterBase::Finalize()
{
	m_pFbxModel = nullptr;

	//delete m_pShieldModel;
	m_pShieldModel = nullptr;


	//ジオメトリプリミティブの解放
	m_pLegBox.reset();

	//delete m_pDeviceResources;

	//ステートマネージャーの終了処理
	m_pStateManager->Finalize();
	m_pStateManager.reset();
	m_pStateManager = nullptr;

	//攻撃マネージャの終了処理
	m_pCharacterAttackManager->Finalize();
	m_pCharacterAttackManager.reset();
	m_pCharacterAttackManager = nullptr;

	//ヒットエフェクトマネージャーの終了処理
	m_pHitEffectManager->Finalize();
	m_pHitEffectManager.reset();
	m_pHitEffectManager = nullptr;

	//ブーストエフェクトマネージャーの終了処理
	m_pBoostEffectManager->Finalize();
	m_pBoostEffectManager.reset();
	m_pBoostEffectManager = nullptr;
	m_pEnemy = nullptr;
}



//////////////////////////
//床との当たり判定
//引数：なし
//戻り値：なし
//////////////////////////
void CharacterBase::HitFloor(const Collision::BoxCollision& floorColl)
{
	if (Collision::CheckHitBox2Box(m_legCollBox, floorColl))
	{
		//Y方向の移動量を0にする
		m_vel.y = 0.0f;

		//着地フラグを立てる
		m_landingFlag = true;
		//m_pos.y += 0.0001f;
		m_jumpFlag = false;
		//ジャンプ状態なら待ち状態にする
		if (m_charaState == eCHARACTER_STATE::JUMP)
		{
			m_charaState = eCHARACTER_STATE::WAIT;
		}

	}
	else
	{
		m_landingFlag = false;
	}

}

//////////////////////////
//敵との当たり判定
//引数：なし
//戻り値：なし
//////////////////////////
void CharacterBase::HitEnemyBody(const Collision::BoxCollision& enemyBodyColl, const Collision::BoxCollision& enemyHeadColl)
{
	//体と体の当たり判定
	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyBodyColl))
	{
		//前フレームの座標に戻す
		if (m_landingFlag == true)
		{
			m_pos = m_posBuf;
		}
		else if (m_jumpFlag == true)
		{
			m_pos.x = m_posBuf.x;
		}
	}

	//体と頭の当たり判定(上から乗った時)
	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyHeadColl))
	{
		//着地フラグを立てる
		m_landingFlag = true;
		//待機状態にする
		m_charaState = eCHARACTER_STATE::WAIT;
		//Y方向の移動量を0にする
		m_vel.y = 0.0f;
		//敵が右端にいたら自身を左側に移動させる
		if (enemyHeadColl.pos.x > 2.0f || m_bodyCollBox.pos.x > 2.0f)
		{
			m_pos.x -= 0.25f;
		}
		//敵が左端にいたら自身を右側に移動させる
		else if (enemyHeadColl.pos.x < -2.0f || m_bodyCollBox.pos.x < -2.0f)
		{
			m_pos.x += 0.25f;
		}
		//敵の左側に乗ったら自身を左側に移動させる
		if (m_bodyCollBox.pos.x < enemyHeadColl.pos.x)
		{
			m_pos.x -= 0.15f;
		}
		//敵の右側に乗ったら自身を右側に移動させる
		else if (m_bodyCollBox.pos.x > enemyHeadColl.pos.x)
		{
			m_pos.x += 0.15f;

		}

	}
}

//////////////////////////
//攻撃との当たり判定
//引数：なし
//戻り値：なし
//////////////////////////
void CharacterBase::HitAttack()
{
	//キーの状態取得
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
	{
		//使用されていなければスルー
		if (m_pAttackManager->GetAttackStruct(i)->isUsed == false)continue;
		//攻撃のプレイヤーIDが自分のIDと一致していればスルー
		if (m_pAttackManager->GetAttackStruct(i)->playerID == m_playerID)continue;

		if (Collision::CheckHitBox2Box(m_bodyCollBox, m_pAttackManager->GetAttackStruct(i)->attackColl) && m_pAttackManager->GetAttackStruct(i)->isHit == false)
		{
			//ガードなしで当たったらやられ
			if (m_charaState != eCHARACTER_STATE::GUARD)
			{
				//体力を減らす
				m_hpBuffer = m_hp - m_pAttackManager->GetAttackStruct(i)->power;

				//やられ状態に切り替える
				m_charaState = eCHARACTER_STATE::DAMAGE;

				//当たったフラグを立てる
				m_pAttackManager->GetAttackStruct(i)->isHit = true;

				//ヒットエフェクトの再生
				m_pHitEffectManager->Play(20.0f, m_pos);

				//ヒット音の再生
				static int id;
				id = ADX2::GetInstance().Play(CRI_CUESHEET_0_DAMAGE);

				//攻撃力が5より大きければヒットストップ
				if (m_pAttackManager->GetAttackStruct(i)->power > 5)
				{
					Sleep(100);
				}
			}

			//ガード中に攻撃が当たったらダメージ量を減らす
			else if (m_charaState == eCHARACTER_STATE::GUARD)
			{
				//体力を減らす
				m_hpBuffer = m_hp - (m_pAttackManager->GetAttackStruct(i)->power / m_pAttackManager->GetAttackStruct(i)->power);

				//当たったフラグを立てる
				m_pAttackManager->GetAttackStruct(i)->isHit = true;
				//ヒットエフェクトの再生
				m_pHitEffectManager->Play(10.0f, m_pos);


			}
		}
	}

}

///////////////////////////
//準備
//引数:なし
//戻り値:なし
//////////////////////////
void CharacterBase::Ready(DX::StepTimer const& timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//ステートの管理
	if (m_hp > 0)StateManager();

	//重力落下
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;

	}
	else if (m_landingFlag == true)
	{
		m_vel.y = 0.0f;
	}
	//移動制限
	if (m_pos.x > -MOVE_LIMIT_X || m_pos.x < MOVE_LIMIT_X)m_vel.x = 0.0f;
	if (m_pos.x < -MOVE_LIMIT_X) m_pos.x = -MOVE_LIMIT_X;
	if (m_pos.x > MOVE_LIMIT_X) m_pos.x = MOVE_LIMIT_X;

	//キャラのステート更新
	m_pStateManager->Update(m_charaState);


	//バッファに座標を格納
	m_posBuf = m_pos;
	//移動量を座標に加算
	m_pos += m_vel;

	//攻撃マネージャーの更新
	m_pCharacterAttackManager->Update();
	//ヒットエフェクトマネージャーの更新
	m_pHitEffectManager->Update(timer);


}

///////////////////////////
//敗北
//引数:なし
//戻り値:なし
//////////////////////////
void CharacterBase::Lose(DX::StepTimer const& timer)
{
	//X方向の移動量を0にする
	m_vel.x = 0.0f;
	//ステートを負け状態にする
	m_charaState = eCHARACTER_STATE::LOSE;
	//重力落下
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;

	}
	else if (m_landingFlag == true)m_vel.y = 0.0f;

	//バッファに座標を格納
	m_posBuf = m_pos;
	//移動量を座標に加算
	m_pos += m_vel;

	//ヒットエフェクトマネージャーの更新
	m_pHitEffectManager->Update(timer);

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void CharacterBase::Reset()
{
	//初期座標の設定
	m_pos = m_startPos;
	m_posBuf = m_pos;
	//体力の設定
	m_hp = m_hpMax;
	m_hpBuffer = m_hp;
	//ステートの設定
	m_charaState = eCHARACTER_STATE::WAIT;

	m_pAttack = nullptr;

	//攻撃の当たり判定の初期化
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_isAttackInput[i] = false;
		m_isAttackUse[i] = false;
	}

	//弾のジオメトリプリミティブのリセット
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}
	//攻撃マネージャーのリセット
	m_pCharacterAttackManager->Reset();
	//ヒットエフェクトのリセット
	m_pHitEffectManager->Reset();
}

///////////////////////////
//アニメーションの切り替え
//引数:切り替えるアニメーションの番号
//戻り値:なし
//////////////////////////
void CharacterBase::ChangeAnimation(const int& animationStack)
{
	//モデルのアニメーションを切り替える
	m_pFbxModel->ChangeAnimation(animationStack);

}

///////////////////////////
//ステートの管理
//引数:なし
//戻り値:なし
//////////////////////////
void CharacterBase::StateManager()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//プレイヤー１の処理
	if (m_playerID == ePLAYER_ID::PLAYER_1)
	{
		//やられ状態でなければ各状態の処理をする
		if (m_charaState != eCHARACTER_STATE::DAMAGE)
		{
			if (m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::WAIT;
			}

			//左Shiftでガード
			if ((m_charaState == eCHARACTER_STATE::WAIT ||
				m_charaState == eCHARACTER_STATE::SQUAT ||
				m_charaState == eCHARACTER_STATE::MOVE_BACK ||
				m_charaState == eCHARACTER_STATE::MOVE_FRONT) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::LeftShift))
			{
				m_charaState = eCHARACTER_STATE::GUARD;
			}

			//スペースキー入力かつブースト容量があればブースト移動
			if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Space) && m_boostCap > 0)
			{
				m_charaState = eCHARACTER_STATE::BOOST_MOVE;
			}


			//待ち状態に下入力でしゃがみ
			if (m_charaState == eCHARACTER_STATE::WAIT && keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) && m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::SQUAT;
			}

			//待ち状態で左右入力で移動
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT &&
				m_charaState != eCHARACTER_STATE::GUARD &&
				m_isAttacking == false) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttacking == false))
			{
				//前移動
				m_charaState = eCHARACTER_STATE::MOVE_FRONT;
			}
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT &&
				m_charaState != eCHARACTER_STATE::GUARD &&
				m_isAttacking == false) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttacking == false))
			{
				//後移動
				m_charaState = eCHARACTER_STATE::MOVE_BACK;
			}
			//上入力でジャンプ
			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up) &&
				m_charaState != eCHARACTER_STATE::SQUAT &&
				m_charaState != eCHARACTER_STATE::BOOST_MOVE)
			{
				m_charaState = eCHARACTER_STATE::JUMP;
			}
		}
	}

	else if (m_playerID == ePLAYER_ID::PLAYER_2)
	{
		//やられ状態でなければ各状態の処理をする
		if (m_charaState != eCHARACTER_STATE::DAMAGE)
		{
			if (m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::WAIT;
			}


			//待ち状態に下入力でしゃがみ
			if (m_charaState == eCHARACTER_STATE::WAIT && keyState.IsKeyDown(DirectX::Keyboard::Keys::S) && m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::SQUAT;
			}

			//待ち状態で左右入力で移動
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::D) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::A) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD))
			{
				//前移動
				m_charaState = eCHARACTER_STATE::MOVE_FRONT;
			}
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::A) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::D) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT))
			{
				//後移動
				m_charaState = eCHARACTER_STATE::MOVE_BACK;

			}

			//上入力でジャンプ
			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::W) &&
				m_charaState != eCHARACTER_STATE::SQUAT &&
				m_charaState != eCHARACTER_STATE::BOOST_MOVE)
			{
				m_charaState = eCHARACTER_STATE::JUMP;
			}

			//左Shiftでガード
			if ((m_charaState == eCHARACTER_STATE::WAIT ||
				m_charaState == eCHARACTER_STATE::SQUAT ||
				m_charaState == eCHARACTER_STATE::MOVE_BACK ||
				m_charaState == eCHARACTER_STATE::MOVE_FRONT) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::Q))
			{
				m_charaState = eCHARACTER_STATE::GUARD;
			}


		}
	}

}

///////////////////////////
//攻撃
//引数:なし
//戻り値:なし
//////////////////////////
void CharacterBase::Attack()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//プレイヤー１の処理
	if (m_playerID == ePLAYER_ID::PLAYER_1)
	{
		//弱ニュートラル攻撃
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::WAIT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;
		}

		//弱下攻撃
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::SQUAT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = true;
		}

		//弱横攻撃
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = true;
		}

		//中ニュートラル攻撃
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::WAIT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = true;
		}

		//中下攻撃
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::SQUAT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = true;
		}

		//中横攻撃
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = true;
		}
	}

	//プレイヤー２の処理
	else if (m_playerID == ePLAYER_ID::PLAYER_2)
	{
		m_aiStateTiemer--;
		if (m_aiStateTiemer <= 0.0f)
		{
			int random = rand() % 2;
			//弱攻撃
			if (random == 0)
			{
				//ニュートラル
				if (m_charaState == eCHARACTER_STATE::WAIT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;

				}
				//横
				else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = true;

				}
				//下
				else if (m_charaState == eCHARACTER_STATE::SQUAT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = true;
				}
			}
			//中攻撃
			else if (random == 1)
			{
				//ニュートラル
				if (m_charaState == eCHARACTER_STATE::WAIT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = true;

				}
				//横
				else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = true;

				}
				//下
				else if (m_charaState == eCHARACTER_STATE::SQUAT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = true;
				}

			}
			m_aiStateTiemer = (rand() % 30) + 30.0f;
		}


		//弱ニュートラル攻撃
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::O) &&
			keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) == false &&
			keyState.IsKeyDown(DirectX::Keyboard::Keys::Up) == false &&
			keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) == false &&
			keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) == false &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;
		}

	}

}

///////////////////////////
//AI
//引数:なし
//戻り値:なし
//////////////////////////
void CharacterBase::AI()
{
	//敵との距離
	float distance = m_pos.x - m_enemyPos.x;
	//タイマーの値を減らす
	m_aiStateTiemer--;
	//タイマーの値が0になったらランダムに状態を切り替える
	if (m_aiStateTiemer <= 0.0f)
	{
		m_aiState = rand() % 7;
		m_aiStateTiemer = (rand() % 30) + 30.0f;
	}

	switch (m_aiState)
	{
		if (m_charaState != eCHARACTER_STATE::DAMAGE)
		{

			//待機
	case 0:
	{
		m_charaState = eCHARACTER_STATE::WAIT;
		break;
	}
	//前進
	case 1:
	{
		m_charaState = eCHARACTER_STATE::MOVE_FRONT;
		break;
	}
	//後退
	case 2:
	{
		m_charaState = eCHARACTER_STATE::MOVE_BACK;
		break;
	}
	//ジャンプ
	case 3:
	{
		m_charaState = eCHARACTER_STATE::JUMP;
		break;
	}
	//しゃがみ
	case 4:
	{
		m_charaState = eCHARACTER_STATE::SQUAT;
		break;
	}
	//ガード
	case 5:
	{
		m_charaState = eCHARACTER_STATE::GUARD;
		break;
	}
	//ブースト移動
	case 7:
	{
		m_charaState = eCHARACTER_STATE::BOOST_MOVE;
		break;

	}
	default:
		break;
		}
	}

}
