#pragma once

//キャラクターのアニメーション番号
enum class eCHARACTER_ANIMATION_NUMBER : int
{
	NONE = -1,

	WAIT,								//待機
	SQUAT,								//しゃがみ
	JUMP,								//ジャンプ
	GUARD,								//ガード
	MOVE_FRONT,							//前移動
	MOVE_BACK,							//後移動
	MOVE_BOOST,							//ブースト移動
	DAMAGE,								//やられ
	ATTACK_WEAK_NEUTRAL,				//弱ニュートラル攻撃
	ATTACK_WEAK_BOTTOM,					//弱下攻撃
	ATTACK_WEKA_SIDE,					//弱横攻撃
	ATTACK_MIDDLE_NEUTRAL,				//中ニュートラル攻撃
	ATTACK_MIDDLE_BOTTOM,				//中下攻撃
	ATTACK_MIDDLE_SIDE,					//中横攻撃
	WIN_POSE,							//勝利ポーズ
	LOSE_POSE,							//敗北ポーズ

	ANIMATION_NUM						//総数
};