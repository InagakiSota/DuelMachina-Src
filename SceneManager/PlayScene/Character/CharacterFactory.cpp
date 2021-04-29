///////////////////////////////////
//キャラクターの生成クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月20日
///////////////////////////////////
#include "pch.h"
#include "CharacterFactory.h"

#include "CharacterBase.h"
#include "Character1\Character1.h"
#include "Character2\Character2.h"
#include "Character3\Character3.h"

#include "../../PlayerID.h"

eCHARACTER_ID CharacterFactory::m_player1Chara = eCHARACTER_ID::CHARACTER_1;
eCHARACTER_ID CharacterFactory::m_player2Chara = eCHARACTER_ID::CHARACTER_1;

//コンストラクタ
CharacterFactory::CharacterFactory()
{
}

//デストラクタ
CharacterFactory::~CharacterFactory()
{
}

///////////////////////////
//キャラクター生成 プレイヤー１用
//引数:キャラクターのID
//戻り値:生成したキャラクターのポインタ
//////////////////////////
CharacterBase * CharacterFactory::CreatePlayer1()
{
	//キャラクターのポインタ
	CharacterBase* chara;

	//IDに対応したキャラクターを読み込む
	switch (m_player1Chara)
	{
		//キャラクター１
		case eCHARACTER_ID::CHARACTER_1:
		{
			chara = new Character1(ePLAYER_ID::PLAYER_1);
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			chara = new Character2(ePLAYER_ID::PLAYER_1);
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			chara = new Character3(ePLAYER_ID::PLAYER_1);
			break;
		}

		default:
			break;
	}

	return chara;
}

///////////////////////////
//キャラクター生成 プレイヤー2用
//引数:キャラクターのID
//戻り値:生成したキャラクターのポインタ
//////////////////////////
CharacterBase * CharacterFactory::CreatePlayer2()
{
	//キャラクターのポインタ
	CharacterBase* chara;

	//IDに対応したキャラクターを読み込む
	switch (m_player2Chara)
	{
		//キャラクター１
		case eCHARACTER_ID::CHARACTER_1:
		{
			chara = new Character1(ePLAYER_ID::PLAYER_2);
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			chara = new Character2(ePLAYER_ID::PLAYER_2);
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			chara = new Character3(ePLAYER_ID::PLAYER_2);
			break;
		}

		default:
			break;
	}

	return chara;
}
