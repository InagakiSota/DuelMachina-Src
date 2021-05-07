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

#include "CueSheet_0.h"
#include "../../../../FrameWork/FbxResourceManager/FbxResourceManager.h"
#include "../CharacterID.h"
#include "Src/Cgdi.h"


//コンストラクタ
Character1::Character1(ePLAYER_ID playerID)
	:	
	//基底クラスのコンストラクタ
	CharacterBase::CharacterBase(playerID)
{
	//自身のキャラクターのIDを設定
	SetCharacterID(eCHARACTER_ID::CHARACTER_1);
}

//デストラクタ
Character1::~Character1()
{
	//基底クラスのデストラクタ
	CharacterBase::~CharacterBase();

}

///////////////////////////
//初期化
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Initialize()
{
	//基底クラスの初期化
	CharacterBase::Initialize();

	//3Dモデルの読み込み
	SetFbxModel(FbxResourceManager::GetInstance()->GetModel(eCHARACTER_ID::CHARACTER_1));	

	//キャラクターのステータスデータの読み込み
	Character1Params::GetInstance()->LoadStatusData();


	//足元の当たり判定の箱の設定
	Collision::BoxCollision legCollBox = GetLegCollBox();
	legCollBox.size_h = Character1Params::LEG_COLL_SIZE;
	legCollBox.pos = DirectX::SimpleMath::Vector3(GetPos().x, GetPos().y - 1.0f, GetPos().z);
	SetLegCollBox(legCollBox);

	//体の当たり判定の箱の読み込み
	Collision::BoxCollision bodyCollBox = GetBodyCollBox();
	bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
	bodyCollBox.pos = DirectX::SimpleMath::Vector3(GetPos().x, GetPos().y - 0.2f, GetPos().z);
	SetBodyCollBox(bodyCollBox);

	//頭の当たり判定の箱の読み込み
	Collision::BoxCollision headCollBox = GetBodyCollBox();
	headCollBox.size_h = Character1Params::HEAD_COLL_SIZE;
	headCollBox.pos = DirectX::SimpleMath::Vector3(GetPos().x, GetPos().y + 0.7f, GetPos().z);
	SetHeadCollBox(headCollBox);

	//前方向の設定
	SetFrontVector(Character1Params::FRONT_VECTOR);

	//体力の設定
	SetHP(Character1Params::GetInstance()->MAX_HP);
	//体力の最大値の設定
	SetHPMax(Character1Params::GetInstance()->MAX_HP);
	//ブースト容量の初期化
	SetBoostCap(Character1Params::GetInstance()->BOOST_CAP_MAX);
	//ブースト容量の最大値の設定
	SetMaxBoostCap(Character1Params::GetInstance()->BOOST_CAP_MAX);
	//体力のバッファの設定
	SetHPBuffer(GetHP());

	//Y軸の角度設定
	if (GetPlayerID() == ePLAYER_ID::PLAYER_1)SetAngleY(Character1Params::ANGLE_Y);
	if (GetPlayerID() == ePLAYER_ID::PLAYER_2)SetAngleY(-Character1Params::ANGLE_Y);

	//ヒットエフェクトの発生位置の座標設定
	SetHitEffectPos(DirectX::SimpleMath::Vector3(
		GetPos().x, GetPos().y, GetPos().z));


	//座標を行列に変換
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(GetPos());
	//サイズを行列に変換
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//Y軸の角度を行列に変換
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(GetAngleY());
	//ワールド行列に加算
	SetWorldMatrix(scale * rotY* trans);
	

	//モデルの更新
	//m_pModel->Update(m_world);



}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Update(DX::StepTimer const& timer)
{
	//基底クラスの更新
	CharacterBase::Update(timer);

	//ヒットエフェクトの発生位置の座標設定
	SetHitEffectPos(DirectX::SimpleMath::Vector3(
		GetPos().x, GetPos().y, GetPos().z));

	//ブースト移動でなければブースト容量を増やす
	if (GetCharaState() != eCHARACTER_STATE::BOOST_MOVE)
	{
		SetBoostCap(GetBoostCap() + 1);
		//最大値以上になったら最大値を代入
		if (GetBoostCap() >= Character1Params::GetInstance()->BOOST_CAP_MAX)
		{
			SetBoostCap(Character1Params::GetInstance()->BOOST_CAP_MAX);
		}
	}
	//ブーストエフェクトマネージャーの更新
	//キャラクターが右向きの場合
	if (GetFrontVector().x > 0)
	{
		GetBoostEffectManager()->Update(timer,
			DirectX::SimpleMath::Vector3(
				GetPos().x - Character1Params::BOOST_EFFECT_POS_X,
				GetPos().y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	//キャラクターが左向きの場合
	else
	{
		GetBoostEffectManager()->Update(timer,
			DirectX::SimpleMath::Vector3(
				GetPos().x + Character1Params::BOOST_EFFECT_POS_X,
				GetPos().y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_RIGHT);

	}

	//当たり判定の更新
	Character1::CollisionUpdate();

}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{	
	//基底クラスの描画
	CharacterBase::Render(view, proj);
}

///////////////////////////
//終了
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Finalize()
{
	//基底クラスの終了処理
	CharacterBase::Finalize();
}

///////////////////////////
//床との当たり判定
//引数:床の当たり判定構造体
//戻り値:なし
//////////////////////////
void Character1::HitFloor(const Collision::BoxCollision& floorColl)
{
	//床との当たり判定
	CharacterBase::HitFloor(floorColl);
}

///////////////////////////
//敵の体との当たり判定
//引数:敵の体の当たり判定
//戻り値:なし
//////////////////////////
void Character1::HitEnemyBody(const Collision::BoxCollision & enemyBodyColl, const Collision::BoxCollision& enemyHeadColl)
{
	//敵の体との当たり判定
	CharacterBase::HitEnemyBody(enemyBodyColl, enemyHeadColl);
}

///////////////////////////
//攻撃との当たり判定
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::HitAttack()
{
	//攻撃との当たり判定
	CharacterBase::HitAttack();
}


///////////////////////////
//準備
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Ready(DX::StepTimer const& timer)
{
	//基底クラスの準備関数
	CharacterBase::Ready(timer);

	//ブーストエフェクトマネージャーの更新
	if (GetFrontVector().x > 0)
	{
		GetBoostEffectManager()->Update(timer,
			DirectX::SimpleMath::Vector3(
				GetPos().x - Character1Params::BOOST_EFFECT_POS_X,
				GetPos().y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f),Character1Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		GetBoostEffectManager()->Update(timer,
			DirectX::SimpleMath::Vector3(
				GetPos().x + Character1Params::BOOST_EFFECT_POS_X,
				GetPos().y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_RIGHT);

	}

	//ブースト移動でなければブースト容量を増やす
	if (GetCharaState() != eCHARACTER_STATE::BOOST_MOVE)
	{
		SetBoostCap(GetBoostCap() + 1);
		//最大値以上になったら最大値を代入
		if (GetBoostCap() >= Character1Params::GetInstance()->BOOST_CAP_MAX)
		{
			SetBoostCap(Character1Params::GetInstance()->BOOST_CAP_MAX);
		}
	}

	//当たり判定の更新
	Character1::CollisionUpdate();
}

///////////////////////////
//敗北
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Lose(DX::StepTimer const & timer)
{
	//基底クラスの敗北関数
	CharacterBase::Lose(timer);

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Reset()
{
	//基底クラスのリセット関数
	CharacterBase::Reset();
	//ブースト容量の)
	SetBoostCap(Character1Params::GetInstance()->BOOST_CAP_MAX);

}

///////////////////////////
//アニメーションの切り替え
//引数:切り替えるアニメーションの番号
//戻り値:なし
//////////////////////////
void Character1::ChangeAnimation(const int & animationStack)
{
	CharacterBase::ChangeAnimation(animationStack);
}

///////////////////////////
//AI
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::AI()
{	
	//基底クラスのAI関数
	CharacterBase::AI();
}


///////////////////////////
//攻撃
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::Attack()
{
	//基底クラスの攻撃管理関数
	CharacterBase::Attack();
}

///////////////////////////
//ステートの管理
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::StateManager()
{
	//基底クラスのステート管理関数
	CharacterBase::StateManager();
}

///////////////////////////
//当たり判定の更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1::CollisionUpdate()
{
	//プレイヤーのY軸の角度
	if (GetPos().x < GetEnemyPos().x && (GetLandingFlag() == true || GetCharaState() == eCHARACTER_STATE::BOOST_MOVE) && GetCharaState() != eCHARACTER_STATE::SQUAT)
	{
		//向いている方向を変える
		SetAngleY(Character1Params::ANGLE_Y);
		//前方向のベクトルを変える
		DirectX::SimpleMath::Vector3 frontVector = GetFrontVector();
		frontVector.x = 1.0f;
		SetFrontVector(frontVector);
	}
	else if (GetPos().x > GetEnemyPos().x && (GetLandingFlag() == true || GetCharaState() == eCHARACTER_STATE::BOOST_MOVE) && GetCharaState() != eCHARACTER_STATE::SQUAT)
	{
		//向いている方向を変える
		SetAngleY(-Character1Params::ANGLE_Y);
		//前方向のベクトルを変える
		DirectX::SimpleMath::Vector3 frontVector = GetFrontVector();
		frontVector.x = -1.0f;
		SetFrontVector(frontVector);

	}

	//回転行列を作成
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(GetAngleY()));
	//座標を行列に変換
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(GetPos());
	//サイズを行列に変換
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//ワールド行列に加算
	SetWorldMatrix(scale * rotY * trans);

	//シールドのサイズ、座標の更新
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(GetPos());
	SetShieldWorldMatrix(Scale * pos);

	//足元の箱の座標の更新
	Collision::BoxCollision legCollBox = GetLegCollBox();
	legCollBox.pos = DirectX::SimpleMath::Vector3(GetPos().x, GetPos().y - 0.85f, GetPos().z);
	SetLegCollBox(legCollBox);

	if (GetCharaState() != eCHARACTER_STATE::SQUAT && GetCharaState() != eCHARACTER_STATE::BOOST_MOVE)
	{
		//体の箱の座標の更新
		Collision::BoxCollision bodyCollBox = GetBodyCollBox();
		bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
		bodyCollBox.pos = DirectX::SimpleMath::Vector3(GetPos().x, GetPos().y, GetPos().z);
		SetBodyCollBox(bodyCollBox);
		//頭の箱の座標の更新
		Collision::BoxCollision headCollBox = GetHeadCollBox();
		headCollBox.pos = DirectX::SimpleMath::Vector3(GetPos().x, GetPos().y + 0.9f, GetPos().z);
		SetHeadCollBox(headCollBox);
	}
}

