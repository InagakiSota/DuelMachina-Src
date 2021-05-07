///////////////////////////////////
//キャラクター2の定数管理クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年12月18日
///////////////////////////////////

#pragma once

#include <SimpleMath.h>

class Character2Params
{
public:
	//最大体力
	int MAX_HP;
	//ブースト容量の最大値
	int BOOST_CAP_MAX;
	//ジャンプの移動量
	float JUMP_FORCE;
	//前移動の移動量
	float MOVE_FRONT_FORCE;
	//後移動の移動量
	float MOVE_BACK_FORCE;
	//ブースト移動の移動量
	float MOVE_BOOST_FORCE;
	

	//モデルのサイズ
	static const float MODEL_SIZE;
	//Y軸の角度
	static const float ANGLE_Y;
	//体の当たり判定のサイズ(通常時)
	static const DirectX::SimpleMath::Vector3 BODY_COLL_SIZE_NORMAL;
	//体の当たり判定のサイズ(しゃがみ)
	static const DirectX::SimpleMath::Vector3 BODY_COLL_SIZE_SQUAT;
	//足の当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 LEG_COLL_SIZE;
	//頭の当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 HEAD_COLL_SIZE;
	//体の当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 MOVE_BOOST_BODY_SIZE;
	//前方向のベクトル
	static const DirectX::SimpleMath::Vector3 FRONT_VECTOR;
	//カメラ座標
	static const DirectX::SimpleMath::Vector3 CAMERA_POS;
	//移動できるX座標の最大値
	static const float MOVE_LIMIT_X;
	//移動できるY座標の最大値
	static const float MOVE_LIMIT_Y;
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
	static Character2Params* GetInstance();
	//自身のインスタンスの解放
	void ReleaseInstance();

	//ステータスデータの読み込み
	void LoadStatusData();


private:
	//コンストラクタ
	Character2Params();
	//デストラクタ
	~Character2Params();
	//自身のインスタンス
	static Character2Params* m_pMyInstance;



};