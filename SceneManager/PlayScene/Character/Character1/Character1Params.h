///////////////////////////////////
//キャラクター1の定数管理クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年12月18日
///////////////////////////////////

#pragma once

#include <SimpleMath.h>

class Character1Params
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


	//Y軸の角度
	static const float ANGLE_Y;
	//体のサイズ
	static const float BODY_SIZE;
	//体の当たり判定のサイズ(通常時)
	static const DirectX::SimpleMath::Vector3 BODY_COLL_SIZE_NORMAL;
	//足の当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 LEG_COLL_SIZE;
	//頭の当たり判定のサイズ
	static const DirectX::SimpleMath::Vector3 HEAD_COLL_SIZE;
	//前方向のベクトル
	static const DirectX::SimpleMath::Vector3 FRONT_VECTOR;
	//カメラ座標
	static const DirectX::SimpleMath::Vector3 CAMERA_POS;
	//体の当たり判定
	static const DirectX::SimpleMath::Vector3 MOVE_BOOST_BODY_SIDE;
	//移動できるX座標の最大値
	static const float MOVE_LIMIT_X;
	//移動できるY座標の最大値
	static const float MOVE_LIMIT_Y;

	//しゃがみ状態の体のサイズ
	static const DirectX::SimpleMath::Vector3 BODY_COLL_SIZE_SQUAT;

	//ブーストエフェクトのX座標
	static const float BOOST_EFFECT_POS_X;
	//ブーストエフェクトのY座標
	static const float BOOST_EFFECT_POS_Y;
	//ブーストエフェクトの角度(左側)	
	static const float BOOST_EFFECT_ANGLE_LEFT;
	//ブーストエフェクトの角度(右側)
	static const float BOOST_EFFECT_ANGLE_RIGHT;



//シングルトン
public:
	//自身のインスタンスの取得
	static Character1Params* GetInstance();
	//自身のインスタンスの解放
	void ReleaseInstance();

	//ステータスデータの読み込み
	void LoadStatusData();


private:
	//コンストラクタ
	Character1Params();
	//デストラクタ
	~Character1Params();
	//自身のインスタンス
	static Character1Params* m_pMyInstance;


};