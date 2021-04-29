///////////////////////////////////
//キャラクター3の状態管理クラス
//作成者：GS305_稲垣
//作成日：2020年10月17日
///////////////////////////////////

#include "pch.h"
#include "Character3StateManager.h"

#include "../../CharacterState.h"
#include "../../CharacterBase.h"

#include "Character3BoostMoveState.h"
#include "Character3MoveFrontState.h"
#include "Character3MoveBackState.h"
#include "Character3SquatState.h"
#include "Character3JumpState.h"
#include "Character3DamageState.h"
#include "Character3WaitState.h"

//コンストラクタ
Character3StateManager::Character3StateManager()
{
	for (int i = 0; i < static_cast<int>(eCHARACTER_STATE::MAX_NUM); i++)
	{
		m_pCharacterStateArray[i] = nullptr;
	}
}

//デストラクタ
Character3StateManager::~Character3StateManager()
{
	for (int i = 0; i < static_cast<int>(eCHARACTER_STATE::MAX_NUM); i++)
	{
		if (m_pCharacterStateArray[i] != nullptr)
		{
			m_pCharacterStateArray[i].reset();
		}
	}

}

///////////////////////////
//初期化
//引数:キャラクターのポインタ
//戻り値:なし
//////////////////////////
void Character3StateManager::Initialize(CharacterBase * pCharacterBase)
{
	//ブースト移動状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::BOOST_MOVE)] = 
		std::make_unique<Character3BoostMoveState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::BOOST_MOVE)]->Initialize(pCharacterBase);

	//前移動状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MOVE_FRONT)] = 
		std::make_unique<Character3MoveFrontState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MOVE_FRONT)]->Initialize(pCharacterBase);

	//後移動状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MOVE_BACK)] = 
		std::make_unique<Character3MoveBackState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MOVE_BACK)]->Initialize(pCharacterBase);

	//しゃがみ状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::SQUAT)] = 
		std::make_unique<Character3SquatState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::SQUAT)]->Initialize(pCharacterBase);

	//ジャンプ状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::JUMP)] = 
		std::make_unique<Character3JumpState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::JUMP)]->Initialize(pCharacterBase);

	//やられ状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::DAMAGE)] = 
		std::make_unique<Character3DamageState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::DAMAGE)]->Initialize(pCharacterBase);

	//待ち状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::WAIT)] =
		std::make_unique<Character3WaitState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::WAIT)]->Initialize(pCharacterBase);

}

///////////////////////////
//更新
//引数:キャラクターの状態
//戻り値:なし
//////////////////////////
void Character3StateManager::Update(const eCHARACTER_STATE& charaState)
{
	if (charaState != eCHARACTER_STATE::GUARD)
		m_pCharacterStateArray[static_cast<int>(charaState)]->Update();

}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void Character3StateManager::Finalize()
{
	for (int i = 0; i < static_cast<int>(eCHARACTER_STATE::MAX_NUM); i++)
	{
		m_pCharacterStateArray[i].reset();
	}
}
