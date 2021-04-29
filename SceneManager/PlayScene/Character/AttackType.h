#pragma once

//攻撃の種類
enum class eATTACK_TYPE : int
{
	NONE = -1,

	WEAK_NEUTRAL,		//弱ニュートラル
	WEAK_BOTTOM,		//弱下
	WEAK_SIDE,			//弱横
	WEAK_TOP,			//弱上
	WEAK_AERIAL,		//弱空

	MIDDLE_NEUTRAL,		//中ニュートラル
	MIDDLE_BOTTOM,		//中下
	MIDDLE_SIDE,		//中横
	MIDDLE_TOP,			//中上
	MIDDLE_AERIAL,		//中空

	STRONG_NEUTRAL,		//強ニュートラル
	STRONG_BOTTOM,		//強下
	STRONG_SIDE,		//強横
	STRONG_TOP,			//強上
	STRONG_AERIAL,		//強空

	ATTACK_TYPE_NUM,
};
