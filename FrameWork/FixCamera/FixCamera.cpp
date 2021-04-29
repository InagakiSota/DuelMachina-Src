#include "pch.h"
#include "FixCamera.h"

//コンストラクタ
FixCamera::FixCamera()
	//:m_viewMatrix{},
	//m_projectionMatrix{},
	//m_eye{},
	//m_target{},
	//m_up{}
{
}

//デストラクタ
FixCamera::~FixCamera()
{
}

///////////////////////////
//初期化
//引数:なし
//戻り値:なし
//////////////////////////
void FixCamera::Initialize()
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
void FixCamera::Update()
{

	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);

}

///////////////////////////
//座標の取得
//引数:なし
//戻り値:座標
//////////////////////////
DirectX::SimpleMath::Vector3 FixCamera::GetPos()
{
	return m_eye;
}

///////////////////////////
//座標の設定
//引数:座標
//戻り値:なし
//////////////////////////
void FixCamera::SetPos(DirectX::SimpleMath::Vector3 pos)
{
	m_eye = pos;
}

///////////////////////////
//注視点の取得
//引数:なし
//戻り値:注視点
//////////////////////////
DirectX::SimpleMath::Vector3 FixCamera::GetTarget()
{
	return m_target;
}

///////////////////////////
//注視点の設定
//引数:注視点
//戻り値:なし
//////////////////////////
void FixCamera::SetTarget(DirectX::SimpleMath::Vector3 target)
{
	m_target = target;
}

///////////////////////////
//ビュー行列の取得
//引数:なし
//戻り値:ビュー行列
//////////////////////////
DirectX::SimpleMath::Matrix FixCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

///////////////////////////
//射影行列の取得
//引数:なし
//戻り値:射影行列
//////////////////////////
DirectX::SimpleMath::Matrix FixCamera::GetProjMatrix()
{
	return m_projectionMatrix;
}
