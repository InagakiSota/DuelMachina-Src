///////////////////////////////////
//シーンのインターフェースクラス
//作成者：GT2_04_稲垣颯太
//作成日：2021年5月6日
///////////////////////////////////

#pragma once

#include <StepTimer.h>
#include "DeviceResources.h"
#include <CommonStates.h>

enum class eSCENE_STATE;

class IScene
{
public:
	//デストラクタ
	virtual ~IScene() {}

	//初期化
	virtual void Initialize() = 0;
	//更新
	virtual void Update(DX::StepTimer const& timer) = 0;
	//描画
	virtual void Render() = 0;
	//終了処理
	virtual void Finalize() = 0;

	///////////////////////////////
	//ゲッター、セッター
	//////////////////////////////
	//デバイスリソースの取得
	virtual DX::DeviceResources* GetDeviceResources() = 0;
	//コモンステートの取得
	virtual DirectX::CommonStates* GetState() = 0;
	//キートラッカーの取得
	virtual DirectX::Keyboard::KeyboardStateTracker* GetKeyTracker() = 0;
	//シーンのステートの取得
	virtual eSCENE_STATE GetSceneState() = 0;
	//シーンのステートの設定
	virtual void SetSceneState(const eSCENE_STATE& sceneState) = 0;
	//フェードのタイマーの取得
	virtual float GetFadeTimer() = 0;
	//フェードのタイマーの設定
	virtual void SetFadeTimer(const float& fadeTimer) = 0;
	//ビュー行列の取得
	virtual DirectX::SimpleMath::Matrix GetView() = 0;
	//ビュー行列の設定
	virtual void SetView(const DirectX::SimpleMath::Matrix& view) = 0;
	//射影行列の取得
	virtual DirectX::SimpleMath::Matrix GetProj() = 0;
	//射影行列の設定
	virtual void SetProj(const  DirectX::SimpleMath::Matrix& proj) = 0;
	//カメラの座標の取得
	virtual DirectX::SimpleMath::Vector3 GetCameraPos() = 0;
	//カメラの座標の設定
	virtual void SetCameraPos(const DirectX::SimpleMath::Vector3& cameraPos) = 0;
	//音のIDの取得
	virtual int GetSoundID() = 0;
	//音のIDの設定
	virtual void SetSoundID(const int& soundID) = 0;

};
