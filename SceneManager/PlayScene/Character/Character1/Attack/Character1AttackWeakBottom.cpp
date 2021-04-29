///////////////////////////////////
//キャラクター１の弱下攻撃
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月16日
///////////////////////////////////
#include "pch.h"
#include "Character1AttackWeakBottom.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"

//攻撃力
const int Character1AttackWeakBottom::POWER = 5;
//X座標
const float Character1AttackWeakBottom::POS_X = 0.5f;
//当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character1AttackWeakBottom::COLL_SIZE = DirectX::SimpleMath::Vector3(0.4f, 0.2f, 0.5f);
//消えるまでの時間
const float Character1AttackWeakBottom::VANISH_TIME = 15.0f;

//コンストラクタ
Character1AttackWeakBottom::Character1AttackWeakBottom()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
	m_pDeviceResources = nullptr;
}

//デストラクタ
Character1AttackWeakBottom::~Character1AttackWeakBottom()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
	m_pDeviceResources = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクターのポインタ、デバイスリソース
//戻り値:なし
//////////////////////////
void Character1AttackWeakBottom::Initialize(CharacterBase * pCharacter, DX::DeviceResources * pDeviceResources)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = pDeviceResources;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1AttackWeakBottom::Update()
{
	//攻撃の入力フラグが立ったら
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)) == true)
	{
		//攻撃のマネージャーから使用されていない攻撃の構造体のポインタを受け取る
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
		m_pAttack->power = Character1AttackWeakBottom::POWER;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character1AttackWeakBottom::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetLegCollBox().pos.y, 
			m_pCharacter->GetPos().z);
		//当たり判定のサイズを設定
		m_pAttack->attackColl.size_h = Character1AttackWeakBottom::COLL_SIZE;
		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();

	    m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM), true);
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM), false);
		m_pCharacter->SetIsAttacking(true);
	}

	if (m_pAttack != nullptr && m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)) ==  true)
	{
		//m_pAttack->vel.x = 0.5f * m_frontVector.x;

		//座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character1AttackWeakBottom::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetLegCollBox().pos.y, 
			m_pCharacter->GetPos().z);

		m_pAttack->attackColl.pos = m_pAttack->pos;

		//タイマーを加算
		m_pAttack->timer++;
		//一定時間経ったら消す
		if (m_pAttack->timer >= Character1AttackWeakBottom::VANISH_TIME)
		{
			m_pAttack->isUsed = false;
			m_pAttack->timer = 0;
			m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM), false);
			m_pCharacter->SetIsAttacking(false);
		}
		if (m_pAttack->isUsed == false)
		{
			m_pAttack->pos.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
			m_pBulletGP = nullptr;
		}
	}

}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character1AttackWeakBottom::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
//射影行列
//引数:なし
//戻り値:なし
//////////////////////////
void Character1AttackWeakBottom::Finalize()
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
void Character1AttackWeakBottom::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM), false);
	m_pCharacter->SetIsAttacking(false);
}
