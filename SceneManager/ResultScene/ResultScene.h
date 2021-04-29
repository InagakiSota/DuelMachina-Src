///////////////////////////////////
//リザルトシーンのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月28日
///////////////////////////////////

#pragma once
#include "../SceneBase.h"
#include "Src\SceneManager\PlayerID.h"

#include <SimpleMath.h>

class Sprite2D;

class ResultScene :public SceneBase
{
public:
	//コンストラクタ
	ResultScene();
	//デストラクタ
	~ResultScene();
	//初期化
	void Initialize(HWND window, DX::DeviceResources* pDeviceResources, DirectX::CommonStates* pStates) override;
	//更新
	void Update(DX::StepTimer const& timer) override;		
	//描画
	void Render() override;			
	//終了処理
	void Finalize() override;

	static ePLAYER_ID m_winPlayerID;

private:
	std::unique_ptr<Sprite2D> m_winPlayer1Sprite;
	std::unique_ptr<Sprite2D> m_winPlayer2Sprite;


};