///////////////////////////////////
//キャラクター２の中下攻撃
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月22日
///////////////////////////////////

#include "pch.h"
#include "Character2AttackMiddleBottom.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"

//攻撃力
const int Character2AttackMiddleBottom::POWER = 10;
//X座標
const float Character2AttackMiddleBottom::POS_X = 1.0f;
//Y座標
const float Character2AttackMiddleBottom::POS_Y = -1.0f;
//当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character2AttackMiddleBottom::COLL_SIZE = DirectX::SimpleMath::Vector3(0.6f, 0.5f, 0.3f);
//消えるまでの時間
const float Character2AttackMiddleBottom::VANISH_TIME = 40.0f;
//プレイヤーのX方向の移動量(上昇)
const float Character2AttackMiddleBottom::PLAYER_VEL_X_UP = 0.2f;
//プレイヤーのX方向の移動量(下降)
const float Character2AttackMiddleBottom::PLAYER_VEL_X_DOWN = -0.35f;

//コンストラクタ
Character2AttackMiddleBottom::Character2AttackMiddleBottom()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
}

//デストラクタ
Character2AttackMiddleBottom::~Character2AttackMiddleBottom()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクターのポインタ、デバイスリソース
//戻り値:なし
//////////////////////////
void Character2AttackMiddleBottom::Initialize(CharacterBase * pCharacter, DX::DeviceResources * pDeviceResources)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = pDeviceResources;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character2AttackMiddleBottom::Update()
{
	//キー入力を受け取る
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)))
	{
		for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
		{
			//攻撃のマネージャーから使用されていない攻撃の構造体のポインタを受け取る
			if (m_pCharacter->GetAttackManager()->GetAttackStruct(i)->isUsed == false)
			{
				if (m_pAttack == nullptr)
				{
					m_pAttack = m_pCharacter->GetAttackManager()->GetAttackStruct(i);
					break;
				}
			}

		}
		//プレイヤーの前方向のベクトルを保存
		m_pCharacter->SetAttackFront(m_pCharacter->GetFrontVector());
		//使用されている状態にする
		m_pAttack->isUsed = true;
		//威力を設定
		m_pAttack->power = Character2AttackMiddleBottom::POWER;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character2AttackMiddleBottom::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y + Character2AttackMiddleBottom::POS_Y,
			m_pCharacter->GetPos().z);
		//当たり判定のサイズを設定
		m_pAttack->attackColl.size_h = Character2AttackMiddleBottom::COLL_SIZE;
		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();

		//攻撃使用中のフラグの設定
		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), true);
		//攻撃入力のフラグの設定
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), false);
		//攻撃中のフラグの設定
		m_pCharacter->SetIsAttacking(true);
	}

	if (m_pAttack != nullptr && m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)) == true)
	{
		//タイマーを加算
		m_pAttack->timer++;
		//座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character2AttackMiddleBottom::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y + Character2AttackMiddleBottom::POS_Y,
			m_pCharacter->GetPos().z);
		m_pAttack->attackColl.pos = m_pAttack->pos;

		//プレイヤーの移動量を取得
		DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();

		//上昇
		if (m_pAttack->timer > 0.0f && m_pAttack->timer < Character2AttackMiddleBottom::VANISH_TIME * 0.5)
		{
			m_pCharacter->SetJumpFlag(true);
			vel.y = Character2AttackMiddleBottom::PLAYER_VEL_X_UP;
		}

		//下降
		else
		{
			vel.y = Character2AttackMiddleBottom::PLAYER_VEL_X_DOWN;
			if (m_pCharacter->GetLandingFlag() == true)vel.y = 0.0f;
		}

		//一定時間経ったら消す
		if (m_pAttack->timer >= Character2AttackMiddleBottom::VANISH_TIME)
		{

			m_pAttack->isUsed = false;
			m_pAttack->timer = 0;
			m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), false);
			m_pCharacter->SetIsAttacking(false);
			vel.x = 0.0f;

		}

		if (m_pAttack->isUsed == false)
		{
			m_pAttack->pos.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
		}
		//プレイヤーの移動量を設定
		m_pCharacter->SetVel(vel);
	}
}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character2AttackMiddleBottom::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	if (m_pBulletGP != nullptr)
	{
		if (m_pCharacter->GetPlayerID() == ePLAYER_ID::PLAYER_1)
			m_pBulletGP->Draw(m_world, view, proj, DirectX::Colors::Red);
		else if (m_pCharacter->GetPlayerID() == ePLAYER_ID::PLAYER_2)
			m_pBulletGP->Draw(m_world, view, proj, DirectX::Colors::Blue);
	}
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void Character2AttackMiddleBottom::Finalize()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}
	m_pDeviceResources = nullptr;
}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void Character2AttackMiddleBottom::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), false);
	m_pCharacter->SetIsAttacking(false);
}
