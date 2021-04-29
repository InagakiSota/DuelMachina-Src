#include "pch.h"
#include "FollowCamera.h"

const float FollowCamera::REF_PERCENT = 0.05f;
//コンストラクタ
FollowCamera::FollowCamera()
//:m_viewMatrix{},
//m_projectionMatrix{},
//m_eye{},
//m_target{},
//m_up{}
{
}

//デストラクタ
FollowCamera::~FollowCamera()
{
}

///////////////////////////
//初期化
//引数:なし
//戻り値:なし
//////////////////////////
void FollowCamera::Initialize()
{
	m_eye = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 10.0f);
	m_target = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_up = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);

}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void FollowCamera::Update()
{
	m_eye += (m_refEye - m_eye) * REF_PERCENT;

	m_target += (m_refTarget - m_target) * REF_PERCENT;

	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);

}

///////////////////////////
//座標の取得
//引数:なし
//戻り値:座標
//////////////////////////
DirectX::SimpleMath::Vector3 FollowCamera::GetPos()
{
	return m_eye;
}

///////////////////////////
//座標の設定
//引数:座標
//戻り値:なし
//////////////////////////
void FollowCamera::SetPos(const DirectX::SimpleMath::Vector3& pos)
{
	m_eye = pos;
}

///////////////////////////
//注視点の取得
//引数:なし
//戻り値:注視点
//////////////////////////
DirectX::SimpleMath::Vector3 FollowCamera::GetTarget()
{
	return m_target;
}

///////////////////////////
//注視点の設定
//引数:注視点
//戻り値:なし
//////////////////////////
void FollowCamera::SetTarget(const DirectX::SimpleMath::Vector3& target)
{
	m_target = target;
}

///////////////////////////
//ビュー行列の取得
//引数:なし
//戻り値:ビュー行列
//////////////////////////
DirectX::SimpleMath::Matrix FollowCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

///////////////////////////
//射影行列の取得
//引数:なし
//戻り値:射影行列
//////////////////////////
DirectX::SimpleMath::Matrix FollowCamera::GetProjMatrix()
{
	return m_projectionMatrix;
}

///////////////////////////
//参照視点の取得
//引数:なし
//戻り値:参照視点
//////////////////////////
DirectX::SimpleMath::Vector3 FollowCamera::GetRefEyePos()
{
	return m_refEye;
}

///////////////////////////
//参照視点の設定
//引数:参照視点
//戻り値:なし
//////////////////////////
void FollowCamera::SetRefEyePos(const DirectX::SimpleMath::Vector3& refEyePos)
{
	m_refEye = refEyePos;

}

///////////////////////////
//参照注視点の取得
//引数:なし
//戻り値:参照
//////////////////////////
DirectX::SimpleMath::Vector3 FollowCamera::GetRefTargetPos()
{
	return m_refTarget;
}

///////////////////////////
//参照注視点の設定
//引数:参照注視点
//戻り値:なし
//////////////////////////
void FollowCamera::SetRefTargetPos(const DirectX::SimpleMath::Vector3& refTargetPos)
{
	m_refTarget = refTargetPos;
}
