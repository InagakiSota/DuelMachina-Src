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

//シーンのステート
enum class eSCENE_STATE
{
	NONE = -1,

	FADE_IN,
	MAIN,
	FADE_OUT,

	OVER,
};

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

	//カメラの振動
	void CameraShake(DX::StepTimer const& timer, float duration, float magnitude) override;
	//カメラの振動を実行
	void DoShake(DX::StepTimer const& timer, float duration, float magnitude);

	//デバイスリソースの取得
	DX::DeviceResources* GetDeviceResources() override
	{
		return m_pDeviceResources;
	}
	//コモンステートの取得
	DirectX::CommonStates* GetState() override
	{
		return m_pStates;
	}
	//キートラッカーの取得
	DirectX::Keyboard::KeyboardStateTracker* GetKeyTracker() override
	{
		return m_pKeyTracker.get();
	}
	//シーンのステートの取得
	eSCENE_STATE GetSceneState() override
	{
		return m_sceneState;
	}
	//シーンのステートの設定
	void SetSceneState(const eSCENE_STATE& sceneState) override
	{
		m_sceneState = sceneState;
	}
	//フェードのタイマーの取得
	float GetFadeTimer() override
	{
		return m_fadeTimer;
	}
	//フェードのタイマーの設定
	void SetFadeTimer(const float& fadeTimer) override
	{
		m_fadeTimer = fadeTimer;
	}

	//ビュー行列の取得
	DirectX::SimpleMath::Matrix GetView() override
	{
		return m_view;
	}
	//ビュー行列の設定
	void SetView(const DirectX::SimpleMath::Matrix& view) override
	{
		m_view = view;
	}
	//射影行列の取得
	DirectX::SimpleMath::Matrix GetProj() override
	{
		return m_proj;
	}
	//射影行列の設定
	void SetProj(const  DirectX::SimpleMath::Matrix& proj) override
	{
		m_proj = proj;
	}
	//カメラの座標の取得
	DirectX::SimpleMath::Vector3 GetCameraPos() override
	{
		return m_cameraPos;
	}
	//カメラの座標の設定
	void SetCameraPos(const DirectX::SimpleMath::Vector3& cameraPos) override
	{
		m_cameraPos = cameraPos;
	}
	//カメラの注視点の取得
	DirectX::SimpleMath::Vector3 GetTargetPos() const override
	{
		return m_targetPos;
	}
	//カメラの注視点の設定
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetPos) override
	{
		m_targetPos = targetPos;
	}

	//音のIDの取得
	int GetSoundID() override
	{
		return m_soundID;
	}
	//音のIDの設定
	void SetSoundID(const int& soundID) override
	{
		m_soundID = soundID;
	}

private:

	//デバイスリソース
	DX::DeviceResources* m_pDeviceResources;
	//コモンステート
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
	//カメラの座標のバッファ
	DirectX::SimpleMath::Vector3 m_cameraPosBuf;
	//カメラの注視点
	DirectX::SimpleMath::Vector3 m_targetPos;
	//カメラの注視点のバッファ
	DirectX::SimpleMath::Vector3 m_targetPosBuf;


	//音のID
	int m_soundID;
	//カメラ振動のフラグ
	bool m_isShake;


};