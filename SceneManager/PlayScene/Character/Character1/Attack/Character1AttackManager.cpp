///////////////////////////////////
//キャラクター１の攻撃マネージャークラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月13日
///////////////////////////////////

#include "pch.h"
#include "Character1AttackManager.h"
#include "../../CharacterBase.h"
#include "../../CharacterAttackBase.h"

#include "Character1AttackWeakNeutral.h"
#include "Character1AttackWeakSide.h"
#include "Character1AttackWeakBottom.h"
#include "Character1AttackMiddleNeutral.h"
#include "Character1AttackMiddleBottom.h"
#include "Character1AttackMiddleSide.h"


//コンストラクタ
Character1AttackManager::Character1AttackManager()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_pCharacterAttackArray[i] = nullptr;
	}
}

//デストラクタ
Character1AttackManager::~Character1AttackManager()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if(m_pCharacterAttackArray[i] != nullptr)m_pCharacterAttackArray[i].reset();
		m_pCharacterAttackArray[i] = nullptr;
	}
}

///////////////////////////
//初期化
//引数:キャラクターのポインタ
//戻り値:なし
//////////////////////////
void Character1AttackManager::Initialize(CharacterBase * pCharacter)
{
	//弱ニュートラル
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = std::make_unique< Character1AttackWeakNeutral>();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)]->Initialize(pCharacter);
	//弱横
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = std::make_unique < Character1AttackWeakSide>();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)]->Initialize(pCharacter);
	//弱下
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = std::make_unique < Character1AttackWeakBottom>();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)]->Initialize(pCharacter);
	//中ニュートラル攻撃
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = std::make_unique < Character1AttackMiddleNeutral>();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)]->Initialize(pCharacter);
	//中下攻撃
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = std::make_unique < Character1AttackMiddleBottom>();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)]->Initialize(pCharacter);
	//中横攻撃
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = std::make_unique < Character1AttackMiddleSide>();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)]->Initialize(pCharacter);

}

///////////////////////////
//更新
//引数:攻撃の入力フラグ
//戻り値:なし
//////////////////////////
void Character1AttackManager::Update()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{	
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			//攻撃の配列の更新
			m_pCharacterAttackArray[i]->Update();
		}
	}
}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character1AttackManager::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			//攻撃の配列の描画
			m_pCharacterAttackArray[i]->Render(view,proj);
		}
	}
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void Character1AttackManager::Finalize()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			//攻撃の配列の終了処理
			m_pCharacterAttackArray[i]->Finalize();
		}

		m_pCharacterAttackArray[i].reset();
		m_pCharacterAttackArray[i] = nullptr;
	}
}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void Character1AttackManager::Reset()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			//攻撃の配列のリセット関数
			m_pCharacterAttackArray[i]->Reset();
		}
	}

}
