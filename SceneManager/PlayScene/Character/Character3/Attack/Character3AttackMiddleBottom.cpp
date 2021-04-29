///////////////////////////////////
//キャラクター３の中下攻撃クラス
//作成者：GS305_稲垣颯太
//作成日：2020年11月23日
///////////////////////////////////

#include "pch.h"
#include "Character3AttackMiddleBottom.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"

//X座標
const float Character3AttackMiddleBottom::POS_X = 0.5f;
//消えるまでの時間
const float Character3AttackMiddleBottom::VANISH_TIME = 40.0f;
//チャージに要する時間
const float Character3AttackMiddleBottom::CHARAGE_TIME = 10.0f;

//コンストラクタ
Character3AttackMiddleBottom::Character3AttackMiddleBottom()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
	m_isShot = false;
}

//デストラクタ
Character3AttackMiddleBottom::~Character3AttackMiddleBottom()
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
void Character3AttackMiddleBottom::Initialize(CharacterBase * pCharacter, DX::DeviceResources * pDeviceResources)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = pDeviceResources;

}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character3AttackMiddleBottom::Update()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	//攻撃の入力フラグが立ったら
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
		m_pAttack->power = 0;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character3AttackMiddleBottom::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetHeadCollBox().pos.y,
			m_pCharacter->GetPos().z);

		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();

		//タイマーの初期値設定
		m_pAttack->timer = 0.0f;

		//当たり判定のサイズ初期化
		m_pAttack->attackColl.size_h = DirectX::SimpleMath::Vector3::Zero;

		//移動量を設定
		m_pAttack->vel = DirectX::SimpleMath::Vector3(
			0.2f * m_pCharacter->GetAttackFront().x,
			0.0f, 
			0.0f);

		//攻撃使用中のフラグの設定
		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), true);
		//攻撃入力のフラグの設定
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), false);
		//攻撃中のフラグの設定
		m_pCharacter->SetIsAttacking(true);
	}

	if (m_pAttack != nullptr && m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)) == true)
	{
		//m_pAttack->vel.x = 0.5f * m_frontVector.x;
		//タイマーを加算
		if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) == true && keyState.IsKeyDown(DirectX::Keyboard::Keys::X) == true && m_isShot == false)
		{
			m_pAttack->timer++;
			if (m_pAttack->timer >= Character3AttackMiddleBottom::CHARAGE_TIME)
			{
				m_pAttack->timer = Character3AttackMiddleBottom::CHARAGE_TIME;
			}
			//初期座標を設定
			m_pAttack->attackColl.pos = m_pAttack->pos;
			m_pAttack->attackColl.size_h = DirectX::SimpleMath::Vector3::Zero;
			m_pAttack->vel.x = (0.2f + (m_pAttack->timer * 0.03f)) * m_pCharacter->GetAttackFront().x;
			//弾のジオメトリプリミティブ生成
			m_pBulletGP = DirectX::GeometricPrimitive::CreateSphere(m_pDeviceResources->GetD3DDeviceContext(), m_pAttack->timer * 0.06f);
			m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pAttack->pos);
			//攻撃力の設定
			m_pAttack->power = static_cast<int>(m_pAttack->timer);
		}

		else if ((keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) == false || keyState.IsKeyDown(DirectX::Keyboard::Keys::X) == false) && m_isShot == false)
		{
			float timerBuf = m_pAttack->timer;
			m_pAttack->attackColl.size_h = DirectX::SimpleMath::Vector3((timerBuf * 0.06f)*0.5f, (timerBuf * 0.06f)*0.5f, (timerBuf * 0.06f)*0.5f);
			m_isShot = true;
		}

		if (m_isShot == true)
		{
			m_pAttack->timer++;
			//座標を設定
			if (m_pAttack->timer > 0.0f)
			{
				m_pAttack->pos += m_pAttack->vel;
				m_pAttack->attackColl.pos = m_pAttack->pos;
				m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pAttack->pos);
			}
			//一定時間超えたら消す
			if (m_pAttack->timer >= Character3AttackMiddleBottom::VANISH_TIME)
			{
				m_pAttack->isUsed = false;
				m_pAttack->timer = 0;
				m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), false);
				m_pCharacter->SetIsAttacking(false);
			}

			//未使用状態になったら消す
			if (m_pAttack->isUsed == false)
			{
				m_isShot = false;
				m_pAttack->pos.Zero;
				m_pAttack->vel.Zero;
				m_pAttack = nullptr;
				m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
				m_world = DirectX::SimpleMath::Matrix::Identity;
				m_pBulletGP.reset();
			}
		}

	}
}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character3AttackMiddleBottom::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
void Character3AttackMiddleBottom::Finalize()
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
void Character3AttackMiddleBottom::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM), false);
	m_pCharacter->SetIsAttacking(false);
}
