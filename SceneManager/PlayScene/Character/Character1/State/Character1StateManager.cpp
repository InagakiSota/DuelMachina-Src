///////////////////////////////////
//キャラクター１の状態管理クラス
//作成者：GS3_05_稲垣
//作成日：2020年10月17日
///////////////////////////////////

#include "pch.h"
#include "Character1StateManager.h"

#include "../../CharacterBase.h"

#include "Character1BoostMoveState.h"
#include "Character1MoveFrontState.h"
#include "Character1MoveBackState.h"
#include "Character1SquatState.h"
#include "Character1JumpState.h"
#include "Character1DamageState.h"
#include "Character1WaitState.h"

//コンストラクタ
Character1StateManager::Character1StateManager()
{
	for (int i = 0; i < static_cast<int>(eCHARACTER_STATE::MAX_NUM); i++)
	{
		m_pCharacterStateArray[i] = nullptr;
	}
}

//デストラクタ
Character1StateManager::~Character1StateManager()
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
void Character1StateManager::Initialize(CharacterBase * pCharacterBase)
{
	//ブースト移動状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::BOOST_MOVE)] = 
		std::make_unique<Character1BoostMoveState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::BOOST_MOVE)]->Initialize(pCharacterBase);

	//前移動状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MOVE_FRONT)] = 
		std::make_unique<Character1MoveFrontState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MOVE_FRONT)]->Initialize(pCharacterBase);

	//後移動状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MOVE_BACK)] = 
		std::make_unique<Character1MoveBackState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MOVE_BACK)]->Initialize(pCharacterBase);

	//しゃがみ状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::SQUAT)] = 
		std::make_unique<Character1SquatState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::SQUAT)]->Initialize(pCharacterBase);

	//ジャンプ状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::JUMP)] = 
		std::make_unique<Character1JumpState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::JUMP)]->Initialize(pCharacterBase);

	//やられ状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::DAMAGE)] = 
		std::make_unique<Character1DamageState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::DAMAGE)]->Initialize(pCharacterBase);

	//待ち状態の読み込み
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::WAIT)] = 
		std::make_unique<Character1WaitState>();
	m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::WAIT)]->Initialize(pCharacterBase);

}

///////////////////////////
//更新
//引数:キャラクターの状態
//戻り値:なし
//////////////////////////
void Character1StateManager::Update(const eCHARACTER_STATE& charaState)
{
	//ステートごとに更新をかける
	if(charaState != eCHARACTER_STATE::GUARD &&
		charaState != eCHARACTER_STATE::LOSE && 
		charaState != eCHARACTER_STATE::WIN)
		m_pCharacterStateArray[static_cast<int>(charaState)]->Update();
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void Character1StateManager::Finalize()
{
	for (int i = 0; i < static_cast<int>(eCHARACTER_STATE::MAX_NUM); i++)
	{
		m_pCharacterStateArray[i].reset();
	}
}
