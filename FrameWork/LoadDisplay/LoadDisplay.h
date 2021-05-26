///////////////////////////////////
//ロード画面の管理クラス
//作成者：GS3_05_稲垣
//作成日：2021年3月25日
///////////////////////////////////

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include <CommonStates.h>
#include "../Sprite2D/Sprite2D.h"
#include <memory>

class LoadDisplay
{
public:
	//初期化
	void Initialize(DX::DeviceResources* pDeviceResources, DirectX::CommonStates* pStates);
	//更新
	void Update(const bool& isLoad, float alpha = 1.0f);
	//描画
	void Render();
	//終了処理
	void Finalize();

	//インスタンスの取得
	static LoadDisplay* GetInstance();

	//インスタンスの解放
	static void ReleaseInstance();

private:
	//コンストラクタ
	LoadDisplay();
	//デストラクタ
	~LoadDisplay();

	//ロード画面の画像
	std::unique_ptr<Sprite2D> m_pLoadSprite;
	//ロード画面の表示フラグ
	bool m_isLoadDisplay;
	//ロード画面の座標
	DirectX::SimpleMath::Vector2 m_loadSpritePos;

	//自身のインスタンス
	static LoadDisplay* m_pMyInstance;

	DirectX::SimpleMath::Vector4 m_color;
};