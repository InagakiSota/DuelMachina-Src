#pragma once
#include "SimpleMath.h"


class FollowCamera
{
public:
	FollowCamera();
	~FollowCamera();


	//初期化
	void Initialize();

	//更新
	void Update();

	//座標の取得
	DirectX::SimpleMath::Vector3 GetPos();

	//座標の設定
	void SetPos(const DirectX::SimpleMath::Vector3& pos);

	//注視点の取得
	DirectX::SimpleMath::Vector3 GetTarget();

	//注視点の設定
	void SetTarget(const DirectX::SimpleMath::Vector3& target);

	//ビュー行列の取得
	DirectX::SimpleMath::Matrix GetViewMatrix();

	//射影行列の取得
	DirectX::SimpleMath::Matrix GetProjMatrix();

	//参照視点の取得
	DirectX::SimpleMath::Vector3 GetRefEyePos();

	//参照視点の設定
	void SetRefEyePos(const DirectX::SimpleMath::Vector3& refEyePos);

	//参照注視点の取得
	DirectX::SimpleMath::Vector3 GetRefTargetPos();

	//参照注視点の設定
	void SetRefTargetPos(const DirectX::SimpleMath::Vector3& refTargetPos);

	//追従度
	static const float REF_PERCENT;

private:
	// ビュー行列
	DirectX::SimpleMath::Matrix m_viewMatrix;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projectionMatrix;

	//座標
	DirectX::SimpleMath::Vector3 m_eye;
	//注視点
	DirectX::SimpleMath::Vector3 m_target;
	//上方向ベクトル
	DirectX::SimpleMath::Vector3 m_up;

	// 参照視点
	DirectX::SimpleMath::Vector3 m_refEye;

	// 参照注視点
	DirectX::SimpleMath::Vector3 m_refTarget;

};

