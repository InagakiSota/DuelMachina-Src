///////////////////////////////////
//キャラクター１の弱横攻撃
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月16日
///////////////////////////////////
#include "pch.h"
#include "Character1AttackWeakSide.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"

//攻撃力
const int Character1AttackWeakSide::POWER = 5;
//X座標
const float Character1AttackWeakSide::POS_X = 0.5f;
//Y座標
const float Character1AttackWeakSide::POS_Y = 0.85f;
//X方向の移動力
const float Character1AttackWeakSide::VEL_X = 0.15f;
//当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character1AttackWeakSide::COLL_SIZE = 
DirectX::SimpleMath::Vector3(0.2f,0.2f,0.2f);
//消えるまでの時間
const float Character1AttackWeakSide::DAVISH_TIME = 30.0f;
//ジオメトリプリミティブのサイズ
const float Character1AttackWeakSide::GP_SIZE = 0.4f;


//コンストラクタ
Character1AttackWeakSide::Character1AttackWeakSide()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
	m_pDeviceResources = nullptr;

}

//デストラクタ
Character1AttackWeakSide::~Character1AttackWeakSide()
{
	m_pCharacter = nullptr;
	m_pDeviceResources = nullptr;
	m_pAttack = nullptr;
}

///////////////////////////
//初期化
//引数:キャラクターのポインタ、デバイスリソース
//戻り値:なし
//////////////////////////
void Character1AttackWeakSide::Initialize(CharacterBase * pCharacter, DX::DeviceResources * pDeviceResources)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = pDeviceResources;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character1AttackWeakSide::Update()
{
	//攻撃の入力フラグが立ったら
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_SIDE)))
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
		m_pAttack->power = Character1AttackWeakSide::POWER;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character1AttackWeakSide::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y + Character1AttackWeakSide::POS_Y,
			m_pCharacter->GetPos().z);
		//当たり判定のサイズを設定
		m_pAttack->attackColl.size_h = Character1AttackWeakSide::COLL_SIZE;
		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();
		//タイマーの初期値設定
		m_pAttack->timer = 0.0f;

		//弾を読み込む
		m_pBulletGP = 
			DirectX::GeometricPrimitive::CreateSphere(m_pDeviceResources->GetD3DDeviceContext(), 
				Character1AttackWeakSide::GP_SIZE);

		//攻撃を使用しているフラグの設定
		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_SIDE), true);
		//攻撃の入力のフラグの設定
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_SIDE), false);
		//攻撃中のフラグの設定
		m_pCharacter->SetIsAttacking(true);
		//移動量を設定
		m_pAttack->vel = DirectX::SimpleMath::Vector3(
			Character1AttackWeakSide::VEL_X * m_pCharacter->GetAttackFront().x, 0.0f, 0.0f);
	}

	if (m_pAttack != nullptr && m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_SIDE)) == true)
	{
		//m_pAttack->vel.x = 0.5f * m_frontVector.x;
		//タイマーを加算
		m_pAttack->timer++;
		//座標を設定
		if (m_pAttack->timer > 0.0f)
		{
			m_pAttack->pos += m_pAttack->vel;
			m_pAttack->attackColl.pos = m_pAttack->pos;
			//座標を弾のワールド行列に変換
			m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pAttack->pos);

		}
		//一定時間超えたら消す
		if (m_pAttack->timer >= Character1AttackWeakSide::DAVISH_TIME)
		{
			m_pAttack->isUsed = false;
			m_pAttack->timer = 0;
			m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_SIDE), false);
			m_pCharacter->SetIsAttacking(false);
		}

		//未使用状態になったら消す
		if (m_pAttack->isUsed == false)
		{
			m_pAttack->pos.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
			m_pBulletGP.reset();

		}
	}
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void Character1AttackWeakSide::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	if (m_pBulletGP != nullptr)
	{
		if(m_pCharacter->GetPlayerID() == ePLAYER_ID::PLAYER_1)
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
void Character1AttackWeakSide::Finalize()
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
void Character1AttackWeakSide::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::WEAK_SIDE), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::WEAK_SIDE), false);
	m_pCharacter->SetIsAttacking(false);
}
