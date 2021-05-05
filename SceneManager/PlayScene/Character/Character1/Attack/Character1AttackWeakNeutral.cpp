///////////////////////////////////
//キャラクター１の弱ニュートラル攻撃
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月13日
///////////////////////////////////
#include "pch.h"
#include "Character1AttackWeakNeutral.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"
#include "Src/Cgdi.h"

//当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character1AttackWeakNeutral::COLL_SIZE = DirectX::SimpleMath::Vector3(0.4f, 0.15f, 0.5f);
//攻撃力
const int Character1AttackWeakNeutral::POWER = 5;
//X座標
const float Character1AttackWeakNeutral::POS_X = 0.5f;
//Y座標
const float Character1AttackWeakNeutral::POS_Y = 0.7f;

//消えるまでの時間
const float Character1AttackWeakNeutral::VANISH_TIME = 15.0f;

//コンストラクタ
Character1AttackWeakNeutral::Character1AttackWeakNeutral()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
	m_pDeviceResources = nullptr;
}

//デストラクタ
Character1AttackWeakNeutral::~Character1AttackWeakNeutral()
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
//戻り値:
//////////////////////////
void Character1AttackWeakNeutral::Initialize(CharacterBase * pCharacter)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = gdi->GetDeviceResources();
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1AttackWeakNeutral::Update()
{
	//攻撃入力フラグが立ったら
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)))
	{
		//攻撃のマネージャーから使用されていない攻撃の構造体のポインタを受け取る
		for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
		{
			if (m_pCharacter ->GetAttackManager()->GetAttackStruct(i)->isUsed == false)
			{
				if (m_pAttack == nullptr)
				{
					m_pAttack =m_pCharacter->GetAttackManager()->GetAttackStruct(i);
					break;
				}
			}
		}
		//プレイヤーの前方向のベクトルを保存
		m_pCharacter->SetAttackFront(m_pCharacter->GetFrontVector());
		//使用されている状態にする
		m_pAttack->isUsed = true;
		//威力を設定
		m_pAttack->power = Character1AttackWeakNeutral::POWER;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character1AttackWeakNeutral::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y + Character1AttackWeakNeutral::POS_Y,
			m_pCharacter->GetPos().z);

		//当たり判定のサイズを設定
		m_pAttack->attackColl.size_h = Character1AttackWeakNeutral::COLL_SIZE;
		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();

		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL),true);

		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL), false);

		m_pCharacter->SetIsAttacking(true);
	}

	if (m_pAttack != nullptr && m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)) == true)
	{
		//m_pAttack->vel.x = 0.5f * m_frontVector.x;

		//座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(m_pCharacter->GetPos().x + Character1AttackWeakNeutral::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y + Character1AttackWeakNeutral::POS_Y,
			m_pCharacter->GetPos().z);
		m_pAttack->attackColl.pos = m_pAttack->pos;
		m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pAttack->pos);

		//タイマーを加算
		m_pAttack->timer++;

		//一定時間経ったら消す
		if (m_pAttack->timer >= Character1AttackWeakNeutral::VANISH_TIME)
		{
			m_pAttack->isUsed = false;
			m_pAttack->timer = 0;
			m_pCharacter ->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL),false);
			m_pCharacter->SetIsAttacking(false);

		}

		if (m_pAttack->isUsed == false)
		{
			m_pAttack->pos.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
			//m_pCharacter = nullptr;
			m_pBulletGP = nullptr;
		}
	}

}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character1AttackWeakNeutral::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
void Character1AttackWeakNeutral::Finalize()
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
void Character1AttackWeakNeutral::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL), false);
	m_pCharacter->SetIsAttacking(false);
}
