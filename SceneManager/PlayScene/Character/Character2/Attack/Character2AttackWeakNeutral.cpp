///////////////////////////////////
//キャラクター２の弱ニュートラル攻撃
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月20日
///////////////////////////////////

#include "pch.h"
#include "Character2AttackWeakNeutral.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"

//攻撃力
const int Character2AttackWeakNeutral::POWER = 5;
//X座標
const float Character2AttackWeakNeutral::POS_X = 1.4f;
//Y座標
const float Character2AttackWeakNeutral::POS_Y = -0.5f;
//当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character2AttackWeakNeutral::COLL_SIZE = DirectX::SimpleMath::Vector3(0.8f, 0.07f, 0.5f);
//消えるまでの時間
const float Character2AttackWeakNeutral::VANISH_TIME = 15.0f;


//コンストラクタ
Character2AttackWeakNeutral::Character2AttackWeakNeutral()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
}

//デストラクタ
Character2AttackWeakNeutral::~Character2AttackWeakNeutral()
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
void Character2AttackWeakNeutral::Initialize(CharacterBase * pCharacter, DX::DeviceResources * pDeviceResources)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = pDeviceResources;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character2AttackWeakNeutral::Update()
{
	//攻撃の入力フラグが立ったら
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)) == true)
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
		m_pAttack->power = Character2AttackWeakNeutral::POWER;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character2AttackWeakNeutral::POS_X * m_pCharacter->GetAttackFront().x, 
			m_pCharacter->GetPos().y + Character2AttackWeakNeutral::POS_Y,
			m_pCharacter->GetPos().z);
		//当たり判定のサイズを設定
		m_pAttack->attackColl.size_h = Character2AttackWeakNeutral::COLL_SIZE;
		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();
		//攻撃中のフラグの設定
		m_pCharacter->SetIsAttacking(true);

		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL), true);
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL), false);
	}

	if (m_pAttack != nullptr &&m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)) == true)
	{
		//m_pAttack->vel.x = 0.5f * m_frontVector.x;
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character2AttackWeakNeutral::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y + Character2AttackWeakNeutral::POS_Y,
			m_pCharacter->GetPos().z);
		m_pAttack->attackColl.pos = m_pAttack->pos;

		m_pAttack->timer++;

		if (m_pAttack->timer >= Character2AttackWeakNeutral::VANISH_TIME)
		{
			m_pAttack->isUsed = false;
			m_pAttack->timer = 0;
			m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL), false);
			m_pCharacter->SetIsAttacking(false);

		}

		if (m_pAttack->isUsed == false)
		{
			m_pAttack->pos.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
		}
	}

}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character2AttackWeakNeutral::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
void Character2AttackWeakNeutral::Finalize()
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
void Character2AttackWeakNeutral::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL), false);
	m_pCharacter->SetIsAttacking(false);
}
