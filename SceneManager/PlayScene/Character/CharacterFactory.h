///////////////////////////////////
//キャラクターの生成クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月20日
///////////////////////////////////

#pragma once

class CharacterBase;

#include "CharacterID.h"

class CharacterFactory
{
public:
	//コンストラクタ
	CharacterFactory();
	//デストラクタ
	~CharacterFactory();

	//キャラクター生成 プレイヤー１
	static CharacterBase* CreatePlayer1();
	//キャラクター生成 プレイヤー2
	static CharacterBase* CreatePlayer2();

	//プレイヤー１の選択キャラ
	static eCHARACTER_ID m_player1Chara;
	//プレイヤー１の選択キャラ
	static eCHARACTER_ID m_player2Chara;


};

