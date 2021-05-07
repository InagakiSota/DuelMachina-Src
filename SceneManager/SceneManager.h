///////////////////////////////////
//シーンマネージャーのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////

#pragma once

class SceneBase;

#include "../../DeviceResources.h"
#include "../../StepTimer.h"
#include <CommonStates.h>

//各シーンのID
enum class eSCENE_ID
{
	NONE_SCENE = -1,
	TITLE_SCENE,							//タイトル
	CHARA_SELECT_SCENE,						//キャラクターセレクト
	PLAY_SCENE,								//プレイ
	RESULT_SCENE,							//リザルト

	OVER_ID,
};

class SceneManager
{
public:
	//初期化
	void Initialize();
	//更新
	void Update(DX::StepTimer const& timer);
	//描画
	void Render();
	//終了処理
	void Finalize();
		
	//現在のシーンの取得
	eSCENE_ID GetScene();	
	//シーンの設定
	void SetScene(const eSCENE_ID scene);

	//インスタンスの取得
	static SceneManager* GetInstance();
	//インスタンスの解放
	static void ReleaseInstance();


private:
	//シングルトン化
	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();

	//シーンのポインタ
	std::unique_ptr<SceneBase> m_pScene;
	//現在のシーンのID
	eSCENE_ID m_sceneId;
	//自身のインスタンス
	static SceneManager* pMyInstance;

	//デバイス
	DX::DeviceResources* m_pDeviceResources;

	DirectX::CommonStates* m_pStates;

	//ウインドウ
	HWND m_window;
};	

