///////////////////////////////////
//キャラクター3の定数管理クラス
//作成者：GS305_稲垣颯太
//作成日：2020年12月18日
///////////////////////////////////

#pragma once

#include <SimpleMath.h>

class Character3Params
{
public:

	//最大体力
	int MAX_HP;
	//ブースト容量の最大値
	int BOOST_CAP_MAX;
	//前移動の移動量
	float MOVE_FRONT_FORCE;
	//後移動の移動量
	float MOVE_BACK_FORCE;
	//ジャンプの移動量
	float JUMP_FORCE;
	//ブースト移動の移動量
	float MOVE_BOOST_FORCE;


	//体のサイズ
	static const float BODY_SIZE;
	//Y軸の角度
	static const float ANGLE_Y;
	//移動できるY座標の最大値
	static const float MOVE_LIMIT_Y;
	//体の当たり判定のサイズ(通常時)
	static const DirectX::SimpleMath::Vector3 BODY_COLL_SIZE_NORMAL;
	//体の当たり判定のサイズ(しゃがみ)
	static const DirectX::SimpleMath::Vector3 BODY_COLL_SIZE_SQUAT;
	//足の当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 LEG_COLL_SIZE;
	//頭の当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 HEAD_COLL_SIZE;
	//前方向のベクトル
	static const DirectX::SimpleMath::Vector3 FRONT_VECTOR;
	//カメラ座標
	static const DirectX::SimpleMath::Vector3 CAMERA_POS;
	//移動できるX座標の最大値
	static const float MOVE_LIMIT_X;
	//シールドのY座標
	static const float SHIELD_POS_Y;

	//ブーストエフェクトのX座標
	static const float BOOST_EFFECT_POS_X;
	//ブーストエフェクトのY座標
	static const float BOOST_EFFECT_POS_Y;
	//ブーストエフェクトの角度(左側)	
	static const float BOOST_EFFECT_ANGLE_LEFT;
	//ブーストエフェクトの角度(右側)
	static const float BOOST_EFFECT_ANGLE_RIGHT;

	//ヒットエフェクトのY座標
	static const float HIT_EFFECT_POS_Y;


	//シングルトン
public:
	//自身のインスタンスの取得
	static Character3Params* GetInstance();
	//自身のインスタンスの解放
	void ReleaseInstance();

	//ステータスデータの読み込み
	void LoadStatusData();


private:
	//コンストラクタ
	Character3Params();
	//デストラクタ
	~Character3Params();
	//自身のインスタンス
	static Character3Params* m_pMyInstance;


};
