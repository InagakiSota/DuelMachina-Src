#pragma once

// PI
#define PI		3.14159265358979323846

// 2Dベクトルの構造体
struct Vec2D
{
	float x;
	float y;
};

typedef struct Vec2D Vec2D;


// ベクトルの足し算
void SumVec2D(Vec2D* a, Vec2D b);

// ベクトルの引き算
void SubVec2D(Vec2D* a, Vec2D b);

// ベクトルを伸ばす
void ScaleVec2D(Vec2D* pv, float s);

// ベクトルの長さ
float LengthVec2D(Vec2D pv);

// ベクトルの正規化
void NormalizeVec2D(Vec2D* pv);

//ラジアン→ディグリーへの変換(弧度法→度数法)
double RagianToDegree(double rad);

//ディグリー→ラジアンへの変換(度数法→弧度法)
double DegreeToRagian(double deg);
