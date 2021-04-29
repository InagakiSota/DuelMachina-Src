#include "pch.h"
#include "Collision.h"
#include "../Math/Math.h"	

////////////////////////
//円と円の判定
//引数:円の情報
//戻り値:当たったかどうか
////////////////////////
bool Collision::CheckCircleCollision(Collision::CircleCollosion c1, Collision::CircleCollosion c2)
{
	//二つの円の距離を測る
	Vec2D v = c1.pos;			//別の変数にコピー
	SubVec2D(&v, c2.pos);		//ベクトルの引き算をして原点に戻す
	float len = LengthVec2D(v);	//長さの取得

	//半径の合計と距離を比較
	float rad = c1.red + c2.red;

	if (len < rad)//二点の距離が半径の合計より小さければ当たっている
	{
		return true;
	}
	return false;
}

//----------------------------------------------------------------------
//! @brief AABBの当たり判定を行う
//!
//! @param[in]
//!	int a1X, int a1Y		1つ目のオブジェクトの左上座標
//!	int b1X, int b1Y		1つ目のオブジェクトの右下座標
//!	int a2X, int a2Y		2つ目のオブジェクトの左上座標
//!	int b2X, int b2Y		2つ目のオブジェクトの右下座標
//!
//! @return int				0 = 当たっていない
//!							または、以下の合計
//!							1 = 1が2から見て左から当たっている
//!							2 = 1が2から見て上から当たっている
//!							4 = 1が2から見て右から当たっている
//!							8 = 1が2から見て下から当たっている
//----------------------------------------------------------------------
int Collision::AABBCheck(float a1X, float a1Y, float b1X, float b1Y, float a2X, float a2Y, float b2X, float b2Y)
{
	int ret = 0;

	if (a1X < b2X &&

		a1Y < b2Y &&
		b1X > a2X &&
		b1Y > a2Y)
	{
		int checkFlag = 0;
		float checkXRatio = 0.0f;
		float checkYRatio = 0.0f;
		float obj1Widht = b1X - a1X;
		float obj1Height = b1Y - a1Y;
		float objRightRatio = (float)(b2X - a1X) / (float)obj1Widht;
		float objLeftRatio = (float)(b1X - a2X) / (float)obj1Widht;
		float objUpRatio = (float)(b1Y - a2Y) / (float)obj1Height;
		float objDownRatio = (float)(b2Y - a1Y) / (float)obj1Height;

		//	左右の比率から小さい方をチェックに入れる
		if (objLeftRatio < objRightRatio)
		{
			checkFlag = 1;
			checkXRatio = objLeftRatio;
		}
		else if (objLeftRatio > objRightRatio)
		{
			checkFlag = 2;
			checkXRatio = objRightRatio;
		}
		else
		{
			checkFlag = 3;
			checkXRatio = objLeftRatio;
		}

		//	上下の比率から小さい方をチェックに入れる
		if (objUpRatio < objDownRatio)
		{
			checkFlag += 4;
			checkYRatio = objUpRatio;
		}
		else if (objUpRatio > objDownRatio)
		{
			checkFlag += 8;
			checkYRatio = objDownRatio;
		}
		else
		{
			checkFlag += 12;
			checkYRatio = objUpRatio;
		}

		//	上下と左右で比率の小さい方を戻り値に入れる
		if (checkXRatio < checkYRatio)
		{
			ret = checkFlag & 3;
		}
		else if (checkXRatio > checkYRatio)
		{
			ret = checkFlag & 12;
		}
		else
		{
			ret = checkFlag & 15;
		}
		//	retが15になった場合、中心座標が完全に重なっている
	}

	return ret;
}

///////////////////////////
//3Dの箱と箱の当たり判定
//引数:箱
//戻り値:当たったかどうか
//////////////////////////
bool Collision::CheckHitBox2Box(Collision::BoxCollision box1, Collision::BoxCollision box2)
{
	if (fabsf(box1.pos.x - box2.pos.x) > (box1.size_h.x + box2.size_h.x)) return false;

	if (fabsf(box1.pos.y - box2.pos.y) > (box1.size_h.y + box2.size_h.y)) return false;

	if (fabsf(box1.pos.z - box2.pos.z) > (box1.size_h.z + box2.size_h.z)) return false;

	return true;

}

///////////////////////////
//球と球の当たり判定
//引数:球の構造体
//戻り値:当たったかどうか
//////////////////////////
bool Collision::CheckHitSphere2Sphere(Collision::SphereCollision sphere1, Collision::SphereCollision sphere2)
{
	//2つの球の中心点のベクトルを求める
	DirectX::SimpleMath::Vector3 v3 = sphere1.pos - sphere2.pos;
	//2つの球の中心点の長さの２乗を求める
	float l2 = v3.Dot(v3);
	//２点間の距離の２乗が２つの球の半径の和の２乗より小さければ当たっている
	float radiusSum = sphere1.r + sphere2.r;
	if (l2 <= radiusSum * radiusSum)
	{
		return true;
	}
	
	return false;
}

///////////////////////////
//球とカプセルの当たり判定
//引数:球の構造体、カプセルの構造体
//戻り値:当たったかどうか
//////////////////////////
bool Collision::CheckHitSphere2Capsule(Collision::SphereCollision sphere, Collision::CapsuleCollision capsule)
{
	// 球の中心とカプセルの中心の線分との距離の平方を計算
	float dist2 = SqDistPointSegment(capsule.startPos, capsule.endPos, sphere.pos);

	float radius = sphere.r + capsule.r;

	return dist2 <= radius * radius;
}


///////////////////////////
//カプセルとカプセルの当たり判定
//引数:カプセルの構造体１、カプセルの構造体２
//戻り値:当たったかどうか
//////////////////////////
bool Collision::CheckHitCapsule2Capsule(Collision::CapsuleCollision capsule1, Collision::CapsuleCollision capsule2)
{
	float s, t;

	DirectX::SimpleMath::Vector3 c1, c2;

	// カプセルの中心の線分間の距離の平方を計算

	float dist2 = ClosestPtSegmentSegment(capsule1.startPos, capsule1.endPos, capsule2.startPos, capsule2.endPos, s, t, c1, c2);

	float radius = capsule1.r + capsule2.r;

	return dist2 <= radius * radius;
}

///////////////////////////
// 点cと線分abの間の距離の平方を求める
//引数:a:線分の開点、b:線分の終点、c:点の座標
//戻り値:点cと線分abの間の距離の平方
//////////////////////////
float Collision::SqDistPointSegment(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 endPos, DirectX::SimpleMath::Vector3 pos)
{
	DirectX::SimpleMath::Vector3 ab = endPos - startPos;

	DirectX::SimpleMath::Vector3 ac = pos - startPos;

	DirectX::SimpleMath::Vector3 bc = pos - endPos;

	float e = ac.Dot(ab);

	if (e <= 0.0f) return ac.Dot(ac);

	float f = ab.Dot(ab);

	if (e >= f) return bc.Dot(bc);

	return ac.Dot(ac) - e * e / f;
}
// クランプ関数

float Collision::Clamp(float n, float min, float max)
{

	if (n < min) return min;

	if (n > max) return max;

	return n;

}
///////////////////////////
//２つの線分の最短距離の平方を返す関数
//引数: p1:線分1の始点, q1:線分1の終点
//		p2:線分2の始点, q2:線分2の終点
//		s: 線分１上の最短位置を表す係数 t: 線分2上の最短位置を表す係数
//		c1: 線分１上の最短距離の位置 c2: 線分2上の最短距離の位置
//戻り値:２つの線分の最短距離の平方
//////////////////////////
float  Collision::ClosestPtSegmentSegment(DirectX::SimpleMath::Vector3  p1, DirectX::SimpleMath::Vector3  q1, DirectX::SimpleMath::Vector3  p2,
	DirectX::SimpleMath::Vector3  q2, float &s, float &t,
	DirectX::SimpleMath::Vector3 & c1, DirectX::SimpleMath::Vector3 & c2)
{
	DirectX::SimpleMath::Vector3 d1 = q1 - p1; DirectX::SimpleMath::Vector3 d2 = q2 - p2; DirectX::SimpleMath::Vector3 r = p1 - p2;

	float a = d1.Dot(d1); float e = d2.Dot(d2); float f = d2.Dot(r);

	if (a <= FLT_EPSILON && e <= FLT_EPSILON)

	{

		s = t = 0.0f;

		c1 = p1;

		c2 = p2;

		return (c1 - c2).Dot(c1 - c2);

	}

	if (a <= FLT_EPSILON)

	{

		s = 0.0f;

		t = f / e;

		t = Clamp(t, 0.0f, 1.0f);

	}

	else

	{

		float c = d1.Dot(r);

		if (e <= FLT_EPSILON)

		{

			t = 0.0f;

			s = Clamp(-c / a, 0.0f, 1.0f);

		}

		else

		{

			float b = d1.Dot(d2);

			float denom = a * e - b * b;

			if (denom != 0.0f)

			{

				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);

			}

			else

			{

				s = 0.0f;

			}

			float tnom = (b * s + f);

			if (tnom < 0.0f)

			{

				t = 0.0f;

				s = Clamp(-c / a, 0.0f, 1.0f);

			}

			else if (tnom > e)

			{

				t = 1.0f;

				s = Clamp((b - c) / a, 0.0f, 1.0f);

			}

			else

			{

				t = tnom / e;

			}

		}

	}

	c1 = p1 + d1 * s;

	c2 = p2 + d2 * t;

	return (c1 - c2).Dot(c1 - c2);
}

///////////////////////////
//カプセルの始点と終点の座標を求める
//引数:カプセルの当たり判定構造体
//戻り値:なし
//////////////////////////
void Collision::SetCapsuleStartEndPos(Collision::CapsuleCollision* capsuleColl)
{
	
	capsuleColl->q = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(capsuleColl->rotAxis, DirectX::XMConvertToRadians(capsuleColl->rot));

	//中心点から始点まで向かうベクトル
	DirectX::SimpleMath::Vector3 vec1(0.0f, capsuleColl->length, 0.0f);
	//中心点から終点まで向かうベクトル
	DirectX::SimpleMath::Vector3 vec2(0.0f, -capsuleColl->length, 0.0f);

	//始点と終点の座標を求める
	capsuleColl->startPos = capsuleColl->pos + DirectX::SimpleMath::Vector3::Transform(vec1, capsuleColl->q);
	capsuleColl->endPos = capsuleColl->pos + DirectX::SimpleMath::Vector3::Transform(vec2, capsuleColl->q);


}

///////////////////////////
//スクリーン座標をワールド座標に変換する行列を作成
//引数:スクリーンの幅、スクリーンの高さ、ビュー行列、射影行列
//戻り値:変換した行列
//////////////////////////
DirectX::SimpleMath::Matrix Collision::CreateMatrix_Screen2World(int screen_w, int screen_h, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{

	// ビューポートスケーリング行列を作成

	DirectX::SimpleMath::Matrix viewport;

	viewport._11 = screen_w / 2.0f;

	viewport._22 = -screen_h / 2.0f;

	viewport._41 = screen_w / 2.0f;

	viewport._42 = screen_h / 2.0f;

	// 逆行列を作成

	DirectX::SimpleMath::Matrix invS = viewport.Invert();

	DirectX::SimpleMath::Matrix invP = projection.Invert();

	DirectX::SimpleMath::Matrix invV = view.Invert();

	// 『ビューポートスケーリング行列の逆行列』 × 『射影行列の逆行列』 × 『ビュー行列の逆行列』

	return invS * invP * invV;

}