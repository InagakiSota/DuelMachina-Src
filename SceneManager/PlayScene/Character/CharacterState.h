///////////////////////////////////
//キャラクターのステートの列挙体
//作成者：GS3_05_稲垣
//作成日：2020年10月17日
///////////////////////////////////

#pragma once

enum class eCHARACTER_STATE : int
{
	NONE = -1,

	WAIT,			//待機
	JUMP,			//ジャンプ
	MOVE_FRONT,		//前移動
	MOVE_BACK,		//後ろ移動
	SQUAT,			//しゃがみ
	DAMAGE,			//やられ
	GUARD,			//ガード
	SQUAT_GUARD,	//しゃがみガード
	BOOST_MOVE,		//ブースト移動
	WIN,			//勝利
	LOSE,			//敗北

	MAX_NUM
};
