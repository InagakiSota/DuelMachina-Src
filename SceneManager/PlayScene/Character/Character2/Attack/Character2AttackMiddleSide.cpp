///////////////////////////////////
//キャラクター２の中横攻撃クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月22日
///////////////////////////////////
#include "pch.h"
#include "Character2AttackMiddleSide.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"

//攻撃力
const int Character2AttackMiddleSide::POWER = 7;
//X座標
const float Character2AttackMiddleSide::POS_X = 1.0f;
//Y座標
const float Character2AttackMiddleSide::POS_Y = -1.0f;
//プレイヤーのX方向の移動量
const float Character2AttackMiddleSide::PLAYER_VEL_X = 0.2f;
//当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character2AttackMiddleSide::COLL_SIZE = DirectX::SimpleMath::Vector3(0.8f, 0.5, 0.3f);
//消えるまでの時間
const float Character2AttackMiddleSide::VANISH_TIME = 10.0f;

//コンストラクタ
Character2AttackMiddleSide::Character2AttackMiddleSide()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
}

//デストラクタ
Character2AttackMiddleSide::~Character2AttackMiddleSide()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクターのポインタ、デバイスリソース
//戻り値:
//////////////////////////
void Character2AttackMiddleSide::Initialize(CharacterBase * pCharacter, DX::DeviceResources * pDeviceResources)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = pDeviceResources;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character2AttackMiddleSide::Update()
{
	//キー入力を受け取る
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)) ==true)
	{
		for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
		{
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
		m_pAttack->power = Character2AttackMiddleSide::POWER;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + POS_X,
			m_pCharacter->GetPos().y + Character2AttackMiddleSide::POS_Y,
			m_pCharacter->GetPos().z);
		//当たり判定のサイズを設定
		m_pAttack->attackColl.size_h = Character2AttackMiddleSide::COLL_SIZE;
		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();

		//攻撃使用中のフラグの設定
		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), true);
		//攻撃入力のフラグの設定
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
		//攻撃中のフラグの設定
		m_pCharacter->SetIsAttacking(true);
	}

	if (m_pAttack != nullptr &&m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)) == true)
	{
		//タイマーを加算
		m_pAttack->timer++;
		//座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + POS_X *m_pCharacter->GetFrontVector().x,
			m_pCharacter->GetPos().y + Character2AttackMiddleSide::POS_Y,
			m_pCharacter->GetPos().z);
		m_pAttack->attackColl.pos = m_pAttack->pos;

		//プレイヤーの移動量を設定
		DirectX::SimpleMath::Vector3 vel = m_pCharacter->GetVel();
		vel.x = Character2AttackMiddleSide::PLAYER_VEL_X * m_pCharacter->GetAttackFront().x;

		//タイマーが一定時間経ったら消す
		if (m_pAttack->timer >= Character2AttackMiddleSide::VANISH_TIME)
		{
			m_pAttack->isUsed = false;
			m_pAttack->timer = 0;
			m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
			vel.x = 0.0f;
			m_pCharacter->SetIsAttacking(false);
		}

		if (m_pAttack->isUsed == false)
		{
			m_pAttack->pos.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
		}

		m_pCharacter->SetVel(vel);
	}
}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character2AttackMiddleSide::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
void Character2AttackMiddleSide::Finalize()
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
void Character2AttackMiddleSide::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
	m_pCharacter->SetIsAttacking(false);
}
