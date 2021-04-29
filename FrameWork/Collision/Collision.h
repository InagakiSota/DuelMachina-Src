#pragma once

#include "../Math/Math.h"
#include <SimpleMath.h>

namespace Collision
{
	//AABBの当たった状態
	enum eAABB_HIT_STATE
	{
		eAABB_HIT_STATE_LEFT = 1,		//左から当たる
		eAABB_HIT_STATE_TOP = 4,		//上から当たる
		eAABB_HIT_STATE_RIGHT = 2,		//右から当たる
		eAABB_HIT_STATE_BOTTOM = 8,		//下から当たる
	};

	//円の当たり判定構造体
	struct CircleCollosion
	{
		//座標
		Vec2D pos;

		//半径
		float red;
	};

	//箱の当たり判定構造体
	struct BoxCollision
	{
		//座標
		DirectX::SimpleMath::Vector3 pos;
		//幅の半分
		DirectX::SimpleMath::Vector3 size_h;
	};

	//球の当たり判定構造体
	struct SphereCollision
	{
		//座標
		DirectX::SimpleMath::Vector3 pos;
		//半径
		float r;
	};

	//カプセルの当たり判定構造体
	struct CapsuleCollision
	{
		//中心座標
		DirectX::SimpleMath::Vector3 pos;
		//中心の線分の始点
		DirectX::SimpleMath::Vector3 startPos;
		//中心の線分の終点
		DirectX::SimpleMath::Vector3 endPos;
		//半径
		float r;
		//始点、終点までの距離
		float length;
		//クォータニオン
		DirectX::SimpleMath::Quaternion q;
		//回転軸
		DirectX::SimpleMath::Vector3 rotAxis;
		//角度
		float rot;
	};

	//円と円の当たり判定
	bool CheckCircleCollision(CircleCollosion c1, CircleCollosion c2);

	//AABB
	int AABBCheck(float a1X, float a1Y, float b1X, float b1Y, float a2X, float a2Y, float b2X, float b2Y);

	//3Dの箱と箱の当たり判定
	bool CheckHitBox2Box(Collision::BoxCollision box1, Collision::BoxCollision box2);

	//球と球の当たり判定
	bool CheckHitSphere2Sphere(Collision::SphereCollision sphere1, Collision::SphereCollision sphere2);

	//球とカプセルの当たり判定
	bool CheckHitSphere2Capsule(Collision::SphereCollision sphere, Collision::CapsuleCollision capsule);

	//カプセルとカプセルの当たり判定
	bool CheckHitCapsule2Capsule(Collision::CapsuleCollision capsule1, Collision::CapsuleCollision capsule2);

	// 点cと線分abの間の距離の平方を求める
	float SqDistPointSegment(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 endPos, DirectX::SimpleMath::Vector3 pos);

	// ２つの線分の最短距離の平方を返す関数
	float ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3  p1, DirectX::SimpleMath::Vector3  q1, DirectX::SimpleMath::Vector3  p2,
		DirectX::SimpleMath::Vector3  q2, float &s, float &t, 
		DirectX::SimpleMath::Vector3 & c1, DirectX::SimpleMath::Vector3 & c2);

	//カプセルの角度から始点、終点の座標を求める
	void SetCapsuleStartEndPos(Collision::CapsuleCollision* capsuleColl);
	// クランプ関数
	float Clamp(float n, float min, float max);

	//スクリーン座標をワールド座標に変換する行列を作成
	DirectX::SimpleMath::Matrix CreateMatrix_Screen2World(int screen_w, int screen_h,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);


}

