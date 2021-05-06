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
#include "IScene.h"

class SceneBase : public IScene
{
public:	
	//コンストラクタ
	SceneBase();
	//デストラクタ
	~SceneBase();

	//初期化
	void Initialize() override;
	//更新
	void Update(DX::StepTimer const& timer) override;
	//描画
	void Render() override;
	//終了処理
	void Finalize() override;

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