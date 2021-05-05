///////////////////////////////////
//キャラクター３の中ニュートラル攻撃クラス
//作成者：GS305_稲垣颯太
//作成日：2020年11月23日
///////////////////////////////////

#include "pch.h"
#include "Character3AttackMiddleNeutral.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"
#include "Src/Cgdi.h"
//攻撃力
const int Character3AttackMiddleNeutral::POWER = 5;
//X座標
const float Character3AttackMiddleNeutral::POS_X = 0.5f;
//Y座標
const float Character3AttackMiddleNeutral::POS_Y = -0.3f;
//X方向の移動量
const float Character3AttackMiddleNeutral::VEL_X = 0.2f;
//Y方向の移動量
const float Character3AttackMiddleNeutral::VEL_Y = -0.02f;
//消えるまでの時間
const float Character3AttackMiddleNeutral::VANISH_TIME = 30.0f;
//ジオメトリプリミティブのサイズ
const float Character3AttackMiddleNeutral::GP_SIZE = 2.0f;
//当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character3AttackMiddleNeutral::COLL_SIZE = DirectX::SimpleMath::Vector3(0.15f, 0.15f, 0.5f);

//コンストラクタ
Character3AttackMiddleNeutral::Character3AttackMiddleNeutral()
{
	//変数の初期化
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;

}

//デストラクタ
Character3AttackMiddleNeutral::~Character3AttackMiddleNeutral()
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
void Character3AttackMiddleNeutral::Initialize(CharacterBase * pCharacter)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = gdi->GetDeviceResources();

	for (int i = 0; i < BALLISTIC_NUM;i++)
	{
		m_pBallisticGPArray[i] = nullptr;
		m_ballisticPos[i] = DirectX::SimpleMath::Vector3::Zero;
		m_ballisticWorld[i] = DirectX::SimpleMath::Matrix::Identity;
	}


}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character3AttackMiddleNeutral::Update()
{
	//攻撃の入力フラグが立ったら
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)))
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
		m_pAttack->power = Character3AttackMiddleNeutral::POWER;
		//初期座標を設定
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + Character3AttackMiddleNeutral::POS_X * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y + Character3AttackMiddleNeutral::POS_Y,
			m_pCharacter->GetPos().z);
		//当たり判定のサイズを設定
		m_pAttack->attackColl.size_h = DirectX::SimpleMath::Vector3(COLL_SIZE);
		//弾のジオメトリプリミティブ生成
		m_pBulletGP = DirectX::GeometricPrimitive::CreateSphere(m_pDeviceResources->GetD3DDeviceContext(), m_pAttack->attackColl.size_h.x * Character3AttackMiddleNeutral::GP_SIZE);
		//弾道のジオメトリプリミティブの読み込み
		for (int i = 0; i < BALLISTIC_NUM; i++)
		{
			//後のほうになるにつれサイズを小さくする
			m_pBallisticGPArray[i] = 
				DirectX::GeometricPrimitive::CreateSphere(
					m_pDeviceResources->GetD3DDeviceContext(), 
					m_pAttack->attackColl.size_h.x * Character3AttackMiddleNeutral::GP_SIZE * (1- i*0.1f));;
		
			//弾道の座標の初期値を代入する
			m_ballisticPos[i] = m_pAttack->pos;
		}

		//プレイヤーのIDを設定(使用者のプレイヤーIDと同期)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();

		//タイマーの初期値設定
		m_pAttack->timer = 0.0f;

		//移動量を設定
		m_pAttack->vel = DirectX::SimpleMath::Vector3(
			Character3AttackMiddleNeutral::VEL_X * m_pCharacter->GetAttackFront().x,
			Character3AttackMiddleNeutral::VEL_X,
			0.0f);

		//攻撃使用中のフラグの設定
		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL), true);
		//攻撃入力のフラグの設定
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL), false);
		//攻撃中のフラグの設定
		m_pCharacter->SetIsAttacking(true);
	}

	//攻撃のポインタが空ではないかつ、攻撃の使用フラグが立っていたら処理を更新処理を行う
	if (m_pAttack != nullptr &&m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)) == true)
	{
		//タイマーを加算
		m_pAttack->timer++;

		//座標を設定
		if (m_pAttack->timer > 0.0f)
		{
			//Y移動量を与える
			m_pAttack->vel.y += Character3AttackMiddleNeutral::VEL_Y;
			//移動量を座標に咥える
			m_pAttack->pos += m_pAttack->vel;
			//当たり判定と座標を同期させる
			m_pAttack->attackColl.pos = m_pAttack->pos;
			//ワールド行列に座標行列を代入する
			m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pAttack->pos);

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
		//一定時間超えたら消す
		if (m_pAttack->timer >= Character3AttackMiddleNeutral::VANISH_TIME)
		{
			m_pAttack->isUsed = false;
			m_pAttack->timer = 0;
			m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL), false);
			m_pCharacter->SetIsAttacking(false);
		}

		//未使用状態になったら消す
		if (m_pAttack->isUsed == false)
		{
			m_pAttack->pos.Zero;
			m_pAttack->vel.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
			m_world = DirectX::SimpleMath::Matrix::Identity;
			m_pBulletGP.reset();

			for (int i = 0; i < BALLISTIC_NUM; i++)
			{
				m_pBallisticGPArray[i].reset();
				m_ballisticPos[i] = DirectX::SimpleMath::Vector3::Zero;
				m_ballisticWorld[i] = DirectX::SimpleMath::Matrix::Identity;
			}
		}
	}
}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character3AttackMiddleNeutral::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	if (m_pBulletGP != nullptr)
	{
		//弾道を配列の後の方から描画する(透明度は後の方なるほど上がっていく)
		for (int i = BALLISTIC_NUM - 1; i >= 0; i--)
		{
			m_pBallisticGPArray[i]->Draw(m_ballisticWorld[i], view, proj, DirectX::SimpleMath::Vector4( 1, 1, 1, 1.0f- i *0.1f));
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
void Character3AttackMiddleNeutral::Finalize()
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
void Character3AttackMiddleNeutral::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL), false);
	m_pCharacter->SetIsAttacking(false);
}
