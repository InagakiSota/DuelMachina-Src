///////////////////////////////////
//シーンのインターフェースクラス
//作成者：GT2_04_稲垣颯太
//作成日：2021年5月6日
///////////////////////////////////

#pragma once

#include <StepTimer.h>
#include "DeviceResources.h"

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
};
