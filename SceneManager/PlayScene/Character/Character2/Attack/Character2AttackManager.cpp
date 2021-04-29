///////////////////////////////////
//キャラクター２の攻撃マネージャークラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月19日
///////////////////////////////////
#include "pch.h"
#include "Character2AttackManager.h"
#include "../../CharacterAttackBase.h"

#include "Character2AttackWeakNeutral.h"
#include "Character2AttackWeakBottom.h"
#include "Character2AttackWeakSide.h"
#include "Character2AttackMiddleNeutral.h"
#include "Character2AttackMiddleSide.h"
#include "Character2AttackMiddleBottom.h"

//コンストラクタ
Character2AttackManager::Character2AttackManager()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_pCharacterAttackArray[i] = nullptr;
	}
}

//デストラクタ
Character2AttackManager::~Character2AttackManager()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)delete m_pCharacterAttackArray[i];
		m_pCharacterAttackArray[i] = nullptr;
	}

}

///////////////////////////
//初期化
//引数:キャラクターのポインタ、デバイスリソース
//戻り値:なし
//////////////////////////
void Character2AttackManager::Initialize(CharacterBase * pCharacter, DX::DeviceResources * pDeviceResources)
{
	//各攻撃の読み込み
	//弱ニュートラル
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = new Character2AttackWeakNeutral();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)]->Initialize(pCharacter, pDeviceResources);
	//弱下
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = new Character2AttackWeakBottom();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)]->Initialize(pCharacter, pDeviceResources);
	//弱横
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = new Character2AttackWeakSide();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)]->Initialize(pCharacter, pDeviceResources);
	//中ニュートラル
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = new Character2AttackMiddleNeutral();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)]->Initialize(pCharacter, pDeviceResources);
	//中横
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = new Character2AttackMiddleSide();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)]->Initialize(pCharacter, pDeviceResources);
	//中下
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = new Character2AttackMiddleBottom();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)]->Initialize(pCharacter, pDeviceResources);

}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Character2AttackManager::Update()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			m_pCharacterAttackArray[i]->Update();
		}
	}
}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Character2AttackManager::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			m_pCharacterAttackArray[i]->Render(view, proj);
		}
	}

}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void Character2AttackManager::Finalize()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			m_pCharacterAttackArray[i]->Finalize();
		}

		delete m_pCharacterAttackArray[i];
		m_pCharacterAttackArray[i] = nullptr;
	}

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void Character2AttackManager::Reset()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			m_pCharacterAttackArray[i]->Reset();
		}
	}
}
