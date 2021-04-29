///////////////////////////////////
//シーンの基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////

#pragma once
#include "../../DeviceResources.h"
#include "../../StepTimer.h"
#include "Keyboard.h"
#include <SimpleMath.h>
#include <CommonStates.h>

class SceneBase
{
public:	
	//デストラクタ
	virtual ~SceneBase()
	{

	}

	//初期化
	virtual void Initialize(HWND window, DX::DeviceResources* pDeviceResources, DirectX::CommonStates* pStates) = 0;
	//更新
	virtual void Update(DX::StepTimer const& timer) = 0;
	//描画
	virtual void Render() = 0;
	//終了処理
	virtual void Finalize() = 0;

protected:
	//シーンのステート
	enum class eSCENE_STATE
	{
		NONE = -1,

		FADE_IN,
		MAIN,
		FADE_OUT,

		OVER,
	};

	//デバイス
	DX::DeviceResources* m_pDeviceResources;

	DirectX::CommonStates* m_pStates;

	//キートラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_pKeyTracker;
	//シーンのステート変数
	eSCENE_STATE m_sceneState;
	//フェードのタイマー
	float m_fadeTimer;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//射影行列
	DirectX::SimpleMath::Matrix m_proj;
	//カメラの座標
	DirectX::SimpleMath::Vector3 m_cameraPos;
	//音のID
	int soundID;

};	 