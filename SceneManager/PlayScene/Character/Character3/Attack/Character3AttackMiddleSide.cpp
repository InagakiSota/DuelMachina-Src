///////////////////////////////////
//キャラクター３の中横攻撃クラス
//作成者：GS305_稲垣颯太
//作成日：2020年11月23日
///////////////////////////////////
#include "pch.h"
#include "Character3AttackMiddleSide.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"
#include "Src/Cgdi.h"

//攻撃力
const int Character3AttackMiddleSide::POWER = 5;
//当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character3AttackMiddleSide::COLL_SIZE = DirectX::SimpleMath::Vector3(0.15f, 0.15f, 0.15f);
//消えるまでの時間
const float Character3AttackMiddleSide::VANISH_TIME = 60.0f;
//ジオメトリプリミティブのサイズ
const float Character3AttackMiddleSide::GP_SIZE = 2.0f;

//コンストラクタ
Character3AttackMiddleSide::Character3AttackMiddleSide()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
}

//デストラクタ
Character3AttackMiddleSide::~Character3AttackMiddleSide()
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
void Character3AttackMiddleSide::Initialize(CharacterBase * pCharacter)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = gdi->GetDeviceResources() ;
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character3AttackMiddleSide::Update()
{
	//キー入力を受け取る
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)) == true)
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
		m_pAttack->power = Character3AttackMiddleSide::POWER;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + 0.5f * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y - 0.2f,
			m_pCharacter->GetPos().z);
		//当たり判定のサイズを設定
		m_pAttack->attackColl.size_h = DirectX::SimpleMath::Vector3(Character3AttackMiddleSide::COLL_SIZE);
		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();
		//移動量を設定
		m_pAttack->vel = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetAttackFront().x * 0.2f, 
			0.2f, 
			0.0f);

		//弾のジオメトリプリミティブ生成
		m_pBulletGP = DirectX::GeometricPrimitive::CreateSphere(m_pDeviceResources->GetD3DDeviceContext(), m_pAttack->attackColl.size_h.x * Character3AttackMiddleSide::GP_SIZE);

		//弾道のジオメトリプリミティブの読み込み
		for (int i = 0; i < BALLISTIC_NUM; i++)
		{
			//後のほうになるにつれサイズを小さくする
			m_pBallisticGPArray[i] =
				DirectX::GeometricPrimitive::CreateSphere(
					m_pDeviceResources->GetD3DDeviceContext(),
					m_pAttack->attackColl.size_h.x * Character3AttackMiddleSide::GP_SIZE * (1 - i * 0.1f));;

			//弾道の座標の初期値を代入する
			m_ballisticPos[i] = m_pAttack->pos;
		}

		//攻撃使用中のフラグの設定
		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), true);
		//攻撃入力のフラグの設定
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
		//攻撃中のフラグの設定
		m_pCharacter->SetIsAttacking(true);
	}

	//攻撃のポインタが空ではないかつ、攻撃の使用フラグが立っていたら処理を更新処理を行う
	if (m_pAttack != nullptr &&m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)) == true)
	{
		//敵の座標へのベクトルを求める
		DirectX::SimpleMath::Vector3 toEnemyVec =
			DirectX::SimpleMath::Vector3(
				m_pCharacter->GetEnemyPos().x - m_pAttack->pos.x, 
				(m_pCharacter->GetEnemyPos().y - 1.5f) - m_pAttack->pos.y,
				m_pCharacter->GetEnemyPos().z - m_pAttack->pos.z);
		DirectX::SimpleMath::Vector3 addVec = m_pAttack->vel;
		//addVec.Normalize();
		//タイマーを加算
		m_pAttack->timer++;

		if (m_pAttack->timer >= 10.0f)
		{
			addVec.y += (toEnemyVec.y * 0.2f);
		}

		if (m_pAttack->timer > 0.0f)
		{
			//移動量を座標に加算
			m_pAttack->pos += addVec * 0.6f;
			//座標を設定
			m_pAttack->attackColl.pos = m_pAttack->pos;
			//座標から座標行列を生成
			m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pAttack->attackColl.pos);

			//弾道の座標を配列の前から後に更新(上書き)する
			for (int i = BALLISTIC_NUM - 2; i >= 0; --i)
			{
				m_ballisticPos[i + 1] = m_ballisticPos[i];
			}
			//先頭に現在の攻撃の座標を代入する
			m_ballisticPos[0] = m_pAttack->pos;

			//弾道の座標から行列を作成する
			for (int i = 0;i < BALLISTIC_NUM; i++)
			{
				m_ballisticWorld[i] = DirectX::SimpleMath::Matrix::CreateTranslation(m_ballisticPos[i]);
			}
		}

		//タイマーが一定時間超えたら消す
		if (m_pAttack->timer >= Character3AttackMiddleSide::VANISH_TIME)
		{
			m_pAttack->isUsed = false;

			m_pAttack->timer = 0;

			//m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = false;

		}
		//当たったら弾を消す
		if (m_pAttack->isHit == true)
		{
			m_pAttack->isUsed = false;
		}

		//未使用状態になったら消す
		if (m_pAttack->isUsed == false)
		{
			m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
			m_pCharacter->SetIsAttacking(false);
			m_pAttack->timer = 0;
			m_pAttack->pos.Zero;
			m_pAttack->vel.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
			m_pBulletGP.reset();
			m_world = DirectX::SimpleMath::Matrix::Identity;

		}

	}

}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character3AttackMiddleSide::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	if (m_pBulletGP != nullptr)
	{
		//弾道を配列の後の方から描画する(透明度は後の方なるほど上がっていく)
		for (int i = BALLISTIC_NUM - 1; i >= 0; i--)
		{
			m_pBallisticGPArray[i]->Draw(m_ballisticWorld[i], view, proj, DirectX::SimpleMath::Vector4(1, 1, 1, 1.0f - i * 0.1f));
		}

		//使用するプレイヤーごとに色を変える
		//プレイヤー１用(赤)
		if (m_pCharacter->GetPlayerID() == ePLAYER_ID::PLAYER_1)
			m_pBulletGP->Draw(m_world, view, proj, DirectX::Colors::Red);
		//プレイヤー２用(青)
		else if (m_pCharacter->GetPlayerID() == ePLAYER_ID::PLAYER_2)
			m_pBulletGP->Draw(m_world, view, proj, DirectX::Colors::Blue);
	}
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void Character3AttackMiddleSide::Finalize()
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
void Character3AttackMiddleSide::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
	m_pCharacter->SetIsAttacking(false);
}
