///////////////////////////////////
//キャラクター１のクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月13日
///////////////////////////////////
#include "pch.h"

#include "Character1.h"
#include "GamePad.h"
#include "../../../PlayerID.h"
#include "../../AttackManager/AttackManager.h"
#include "Src\FrameWork\FbxModel\FbxModel.h"
#include "../CharacterState.h"

#include "State\Character1StateManager.h"
#include "Attack\Character1AttackManager.h"
#include "../ChracterAnimationNumber.h"

#include "Adx2.h"

//#include "Adx2.h"
//#include "Test_acf.h"
#include "CueSheet_0.h"
#include "../../../../FrameWork/FbxResourceManager/FbxResourceManager.h"
#include "../CharacterID.h"
#include "Src/Cgdi.h"


//コンストラクタ
Character1::Character1(ePLAYER_ID playerID)
{
	m_jumpFlag = false;
	m_landingFlag = false;
	m_legBoxWorld.Identity;
	m_legCollBox = {};
	m_pDeviceResources = nullptr;
	m_pLegBox = nullptr;
	m_pModel = nullptr;
	m_pos.Zero;
	m_vel.Zero;
	m_frontVector.Zero;
	m_pBodyBox = nullptr;
	m_bodyBoxWorld.Identity;
	m_bodyCollBox = {};
	m_startPos.Zero;
	m_pKeyTracker = nullptr;
	m_playerID = playerID;
	m_isHitEnemyBody = false;
	m_posBuf.Zero;

	m_pAttack = nullptr;

	m_pAttackManager = nullptr;
	m_enemyPos.Zero;
	m_pHeadBox = nullptr;
	m_headCollBox = {};
	m_headBoxWorld.Identity;
	m_HP = 0;
	m_HPBuffer = 0;
	m_charaState = eCHARACTER_STATE::NONE;
	m_damageTimer = 0.0f;
	m_angleY = 0.0f;
	m_aiAttackTimer = 0.0f;
	m_aiStateTiemer = 0.0f;
	m_aiState = 0;
	m_boostCap = 0;

	m_pFbxModel = nullptr;

	m_pStateManager = nullptr;
	m_pCharacterAttackManager = nullptr;

	m_attackFront = DirectX::SimpleMath::Vector3::Zero;

	m_isAttacking = false;

	m_pHitEffectManager = nullptr;

	m_pEnemy = nullptr;

	thinkTimer = 0;
	toEnemyRange = 0.0f;
}

//デストラクタ
Character1::~Character1()
{
	//リソースの解放
	if (m_pBodyBox != nullptr)
	{
		m_pBodyBox.reset();
	}

	if (m_pLegBox != nullptr)
	{
		m_pLegBox.reset();
	}

	if (m_pHeadBox != nullptr)
	{
		m_pHeadBox.reset();
	}

	if (m_pDeviceResources != nullptr)
	{
		m_pDeviceResources = nullptr;
	}

	if (m_pKeyTracker != nullptr)
	{
		m_pKeyTracker.reset();
	}

	if (m_pFbxModel != nullptr)
	{
		delete m_pFbxModel;
		m_pFbxModel = nullptr;
	}

	
	if (m_pModel != nullptr)
	{
		m_pModel.reset();
	}
}

///////////////////////////
//初期化
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();

	//待機状態のモデルの読み込み
	//m_pFbxModel = new FbxModel();
	//m_pFbxModel->Load(
	//	window,
	//	pDeviceResources->GetD3DDevice(),
	//	pDeviceResources->GetD3DDeviceContext(),
	//	pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/robot1/robot1animation3.fbx",	
	//	true,
	//	0
	//);

	m_pFbxModel = FbxResourceManager::GetInstance()->GetModel(eCHARACTER_ID::CHARACTER_1);

	//m_pShieldModel = new FbxModel();
	//m_pShieldModel->Load(
	//	window,
	//	pDeviceResources->GetD3DDevice(),
	//	pDeviceResources->GetD3DDeviceContext(),
	//	pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/Shield.fbx",
	//	false
	//);
	
	m_pShieldModel = FbxResourceManager::GetInstance()->GetShieldModel();

	m_shieldWorld = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero);
	m_shieldWorld = pos;

	//キャラクターのステータスデータの読み込み
	Character1Params::GetInstance()->LoadStatusData();

	//初期座標の設定	
	m_pos = m_startPos;

	//足元の当たり判定の箱の読み込み
	m_legCollBox.size_h = Character1Params::LEG_COLL_SIZE;
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z);
	m_pLegBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_legCollBox.size_h * 2);
	m_legBoxWorld.Identity;

	//体の当たり判定の箱の読み込み
	m_bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
	m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.2f, m_pos.z);
	m_pBodyBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_bodyCollBox.size_h * 2);
	m_bodyBoxWorld.Identity;

	//頭の当たり判定の箱の読み込み
	m_headCollBox.size_h = Character1Params::HEAD_COLL_SIZE;
	m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.7f, m_pos.z);
	m_pHeadBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_headCollBox.size_h * 2);
	m_headBoxWorld.Identity;

	//キートラッカーの読み込み
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//前方向の設定
	m_frontVector = Character1Params::FRONT_VECTOR;

	//体力の設定
	m_HP = Character1Params::GetInstance()->MAX_HP;
	m_HPBuffer = m_HP;

	//体の色の設定
	m_bodyColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.4f);

	//キャラのステート初期化
	m_charaState = eCHARACTER_STATE::WAIT;

	if (m_playerID == ePLAYER_ID::PLAYER_1)m_angleY = Character1Params::ANGLE_Y;
	if (m_playerID == ePLAYER_ID::PLAYER_2)m_angleY = -Character1Params::ANGLE_Y;

	//座標を行列に変換
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//サイズを行列に変換
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//Y軸の角度を行列に変換
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_angleY);
	//ワールド行列に加算
	m_world = scale * rotY* trans;

	//モデルの更新
	//m_pModel->Update(m_world);
	 
	//攻撃フラグの初期化
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_isAttackUse[i] = false;
		m_isAttackInput[i] = false;
	}

	//エネルギー量の初期化
	m_boostCap = Character1Params::GetInstance()->BOOST_CAP_MAX;

	// サウンドの初期化
	//ADX2::GetInstance().Initialize("Test.acf", "CueSheet_0.acb");

	//攻撃の当たり判定の初期化
	for (int i = 0; i < CharacterBase::ATTACK_NUM; i++)
	{
		m_pAttack = nullptr;
	}

	//ステートマネージャーの読み込み、初期化
	m_pStateManager = std::make_unique<Character1StateManager>();
	m_pStateManager->Initialize(this);

	//攻撃マネージャーの読み込み、初期化
	m_pCharacterAttackManager = std::make_unique<Character1AttackManager>();
	m_pCharacterAttackManager->Initialize(this,m_pDeviceResources);

	//ヒットエフェクトマネージャーの読み込み、作成
	m_pHitEffectManager = std::make_unique<HitEffectManager>();
	m_pHitEffectManager->Create(m_pDeviceResources, 50);
	
	//ブーストエフェクトマネージャーの読み込み、作成
	m_pBoostEffectManager = std::make_unique<BoostEffectManager>();
	m_pBoostEffectManager->Initialize(m_pDeviceResources, 1,1,DirectX::SimpleMath::Vector3::Zero);

}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Update(DX::StepTimer const& timer)
{
	m_stepTimer = timer;
	//キーボードの状態の取得
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//// サウンドの更新
	//ADX2::GetInstance().Update();
	//// 音の再生
	//static int id = ADX2::GetInstance().Play(CRI_CUESHEET_0_ROBOT_FOOTSTEP2__ONLINE_AUDIO_CONVERTER_COM_);
	//ADX2::GetInstance().Stop(id);

	//ステートの管理
	if (m_playerID == ePLAYER_ID::PLAYER_1 && m_HP > 0)
	{
		StateManager();
	}

	//プレイヤー２ならAI操作
	if (m_playerID == ePLAYER_ID::PLAYER_2 && m_HP > 0)
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

	//ブースト移動でなければブースト容量を増やす
	if (m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	{
		m_boostCap++;
		//最大値以上になったら最大値を代入
		if (m_boostCap >= Character1Params::GetInstance()->BOOST_CAP_MAX)
		{
			m_boostCap = Character1Params::GetInstance()->BOOST_CAP_MAX;
		}
	}

	//プレイヤーのY軸の角度
	if (m_pos.x < m_pEnemy->GetPos().x && (m_landingFlag == true || m_charaState == eCHARACTER_STATE::BOOST_MOVE)&& m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//向いている方向を変える
		m_angleY = Character1Params::ANGLE_Y;
		//前方向のベクトルを変える
		m_frontVector.x = 1.0f;
	}
	else if(m_pos.x > m_enemyPos.x && (m_landingFlag == true || m_charaState == eCHARACTER_STATE::BOOST_MOVE) && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//向いている方向を変える
		m_angleY = -90.0f;
		//前方向のベクトルを変える
		m_frontVector.x = -1.0f;

	}
	//回転行列を作成
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));

	//攻撃
	if (m_HP > 0)
	{
		Attack();
	}
	//攻撃との当たり判定
	HitAttack();
	//攻撃マネージャーの更新
	m_pCharacterAttackManager->Update();
	//ヒットエフェクトマネージャーの更新
	m_pHitEffectManager->Update(timer);

	//ブーストエフェクトマネージャーの更新
	if (m_frontVector.x > 0)
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x - Character1Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x + Character1Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_RIGHT);

	}

	//バッファに座標を格納
	m_posBuf = m_pos;
	//移動量を座標に加算
	m_pos += m_vel;

	//体力の更新
	if (m_HP > m_HPBuffer)m_HP--; 

	//座標を行列に変換
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//サイズを行列に変換
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//ワールド行列に加算
	m_world = scale * rotY * trans;

	//シールドのサイズ、座標の更新
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	m_shieldWorld = Scale * pos;


	//モデルの更新
	//m_pModel->Update(m_world);


	//足元の箱の座標の更新
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.85f, m_pos.z);

	if (m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	{
		//体の箱の座標の更新
		m_bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);
		//頭の箱の座標の更新
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.9f, m_pos.z);
	}

	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);


}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{	
	//モデルの描画
	//m_pModel->Render(view, proj);

	//m_pFbxModel->Draw(
	//	m_world,
	//	view,
	//	proj
	//);


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


	if(m_playerID == ePLAYER_ID::PLAYER_1)
		m_pFbxModel->Draw(m_world, view, proj,DirectX::SimpleMath::Vector4(0.7f,0.0f,0.0f,1.0f));
	else if (m_playerID == ePLAYER_ID::PLAYER_2)
		m_pFbxModel->Draw(m_world, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.7f, 1.0f));


	if (m_charaState == eCHARACTER_STATE::GUARD)
	{
		//シールドの描画
		if (m_playerID == ePLAYER_ID::PLAYER_1)
			m_pShieldModel->Draw(m_shieldWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f,0.0f,0.0f,0.3f));
		else if (m_playerID == ePLAYER_ID::PLAYER_2)
			m_pShieldModel->Draw(m_shieldWorld, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.3f));
	}


	//足元の箱の描画
	//m_pLegBox->Draw(m_legBoxWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.4f));

	//体の箱の描画
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//m_pBodyBox->Draw(m_bodyBoxWorld, view, proj, m_bodyColor);
	}
	//しゃがみ中の体の箱の描画
	else
	{
		//m_pBodySquatBox->Draw(m_bodyBoxWorld, view, proj, m_bodyColor);
	}
	//頭の箱の描画
	//m_pHeadBox->Draw(m_headBoxWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.4f));

	////横弱攻撃の弾の描画
	//if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] && m_pBulletGP != nullptr)
	//{
	//	m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::Red);
	//}
	////中横攻撃の弾の描画
	//if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] && m_pBulletGP != nullptr)
	//{
	//	m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::Black);
	//}
	//攻撃マネージャーの描画
	m_pCharacterAttackManager->Render(view, proj);
	//ヒットエフェクトマネージャーの描画
	m_pHitEffectManager->SetRenderState(Character1Params::CAMERA_POS, view, proj);
	m_pHitEffectManager->Render();


}

///////////////////////////
//終了
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Finalize()
{
	//モデルの解放
	//m_pModel->Lost();
	//m_pModel.reset();
	// サウンドの終了処理
	//ADX2::GetInstance().Finalize();

	//delete m_pFbxModel;
	//m_pFbxModel = nullptr;

	//delete m_pFbxModel;
	m_pFbxModel = nullptr;

	//delete m_pShieldModel;
	m_pShieldModel = nullptr;


	//ジオメトリプリミティブの解放
	m_pLegBox.reset();

	//delete m_pDeviceResources;
	m_pDeviceResources = nullptr;

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

///////////////////////////
//床との当たり判定
//引数:床の当たり判定構造体
//戻り値:なし
//////////////////////////
void Character1::HitFloor(const Collision::BoxCollision& floorColl)
{
	if (Collision::CheckHitBox2Box(m_legCollBox, floorColl))
	{
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


///////////////////////////
//敵の体との当たり判定
//引数:敵の体の当たり判定
//戻り値:なし
//////////////////////////
void Character1::HitEnemyBody(const Collision::BoxCollision & enemyBodyColl, const Collision::BoxCollision& enemyHeadColl)
{
	//体と体の当たり判定
	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyBodyColl))
	{
		//前フレームの座標に戻す
		if (m_landingFlag == true)
		{
			m_pos = m_posBuf;
		}
		else if(m_jumpFlag == true)
		{
			m_pos.x = m_posBuf.x;
		}
	}

	//体と頭の当たり判定(上から乗った時)
	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyHeadColl))
	{
		m_landingFlag = true;
		m_charaState = eCHARACTER_STATE::WAIT;
		m_vel.y = 0.0f;
		if (enemyHeadColl.pos.x > 2.0f || m_bodyCollBox.pos.x > 2.0f)
		{
			m_pos.x -= 0.25f;
		}
		else if (enemyHeadColl.pos.x < -2.0f || m_bodyCollBox.pos.x < -2.0f)
		{
			m_pos.x += 0.25f;
		}

		if (m_bodyCollBox.pos.x < enemyHeadColl.pos.x)
		{
			m_pos.x -= 0.15f;
		}
		else if (m_bodyCollBox.pos.x > enemyHeadColl.pos.x)
		{
			m_pos.x += 0.15f;

		}

	}
	
}

///////////////////////////
//攻撃との当たり判定
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::HitAttack()
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
				m_bodyColor = DirectX::SimpleMath::Vector4(0.0f, 1.0f, 1.0f, 0.4f);
				//体力を減らす
				//m_HP -= m_pAttackManager->GetAttackStruct(i)->power;
				m_HPBuffer = m_HP - m_pAttackManager->GetAttackStruct(i)->power;

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
				//m_HP -= m_pAttackManager->GetAttackStruct(i)->power / m_pAttackManager->GetAttackStruct(i)->power;
				m_HPBuffer = m_HP - (m_pAttackManager->GetAttackStruct(i)->power / m_pAttackManager->GetAttackStruct(i)->power);

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
void Character1::Ready(DX::StepTimer const& timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//ステートの管理
	if (m_HP > 0)StateManager();

	//重力落下
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;

	}
	else if (m_landingFlag == true)m_vel.y = 0.0f;

	//移動制限
	if (m_pos.x > -MOVE_LIMIT_X || m_pos.x < MOVE_LIMIT_X)m_vel.x = 0.0f;
	if (m_pos.x < -MOVE_LIMIT_X) m_pos.x = -MOVE_LIMIT_X;
	if (m_pos.x > MOVE_LIMIT_X) m_pos.x = MOVE_LIMIT_X;

	//キャラのステート更新
	//if(m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	m_pStateManager->Update(m_charaState);
	//else m_pStateManager->Update(eCHARACTER_STATE)



	//プレイヤーのY軸の角度
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//向いている方向を変える
		m_angleY = Character1Params::ANGLE_Y;
		//前方向のベクトルを変える
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//向いている方向を変える
		m_angleY = -Character1Params::ANGLE_Y;
		//前方向のベクトルを変える
		m_frontVector.x = -1.0f;

	}
	//回転行列を作成
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));


	//バッファに座標を格納
	m_posBuf = m_pos;
	//移動量を座標に加算
	m_pos += m_vel;


	//座標を行列に変換
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//サイズを行列に変換
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//ワールド行列に加算
	m_world = scale * rotY* trans;

	//シールドのサイズ、座標の更新
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	m_shieldWorld = Scale * pos;



	//モデルの更新
	//m_pModel->Update(m_world);

	//足元の箱の座標の更新
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.85f, m_pos.z);
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);

	//体の箱の座標の更新
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y , m_pos.z);
	}
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);

	//頭の箱の座標の更新
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.9f, m_pos.z);
	}
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);

	//攻撃マネージャーの更新
	m_pCharacterAttackManager->Update();
	//ヒットエフェクトマネージャーの更新
	m_pHitEffectManager->Update(timer);

	//ブーストエフェクトマネージャーの更新
	if (m_frontVector.x > 0)
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x - Character1Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f),Character1Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x + Character1Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_RIGHT);

	}


}

///////////////////////////
//敗北
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Lose(DX::StepTimer const & timer)
{
	m_vel.x = 0.0f;
	m_charaState = eCHARACTER_STATE::LOSE;
	//重力落下
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;

	}
	else if (m_landingFlag == true)m_vel.y = 0.0f;
	//プレイヤーのY軸の角度
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//向いている方向を変える
		m_angleY = Character1Params::ANGLE_Y;
		//前方向のベクトルを変える
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//向いている方向を変える
		m_angleY = -Character1Params::ANGLE_Y;
		//前方向のベクトルを変える
		m_frontVector.x = -1.0f;

	}
	//回転行列を作成
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));


	//バッファに座標を格納
	m_posBuf = m_pos;
	//移動量を座標に加算
	m_pos += m_vel;


	//座標を行列に変換
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//サイズを行列に変換
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//ワールド行列に加算
	m_world = scale * rotY* trans;
	//足元の箱の座標の更新
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.85f, m_pos.z);
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);

	//体の箱の座標の更新
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);
	}
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);

	//頭の箱の座標の更新
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.9f, m_pos.z);
	}
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);
	//ヒットエフェクトマネージャーの更新
	m_pHitEffectManager->Update(timer);

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Reset()
{
	//初期座標の設定
	m_pos = m_startPos;
	m_posBuf = m_pos;
	//体力の設定
	m_HP = Character1Params::GetInstance()->MAX_HP;
	m_HPBuffer = m_HP;
	//ステートの設定
	m_charaState = eCHARACTER_STATE::WAIT;

	m_pAttack = nullptr;

	//攻撃の当たり判定の初期化
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_isAttackInput[i] = false;
		m_isAttackUse[i] = false;
	}
	//エネルギー量の初期化
	m_boostCap = Character1Params::GetInstance()->BOOST_CAP_MAX;

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
void Character1::ChangeAnimation(const int & animationStack)
{
	m_pFbxModel->ChangeAnimation(animationStack);
}

///////////////////////////
//AI
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::AI()
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

	//toEnemyRange = fabs(m_pos.x - m_pEnemy->GetPos().x);


	//if (m_pEnemy != nullptr)
	//{
	//	if (thinkTimer <= 0)
	//	{

	//		if (toEnemyRange <= 2.0f && m_isAttacking == false)
	//		{
	//			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;
	//		}
	//		//else if(toEnemyRange > 2.0f && m_isAttacking == false)
	//		//{
	//		//	m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = true;
	//		//}

	//		if (m_pEnemy->GetIsAttacking() == true)
	//		{
	//			m_charaState = eCHARACTER_STATE::GUARD;
	//		}
	//		else
	//		{
	//			m_charaState = eCHARACTER_STATE::WAIT;
	//		}

	//		thinkTimer = 60;
	//	}
	//	
	//}

	//thinkTimer--;
}

///////////////////////////
//攻撃
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Attack()
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
//ステートの管理
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::StateManager()
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

