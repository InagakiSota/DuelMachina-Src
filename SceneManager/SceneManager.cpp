///////////////////////////////////
//シーンマネージャーのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////
#include "pch.h"
#include "SceneManager.h"
#include "TitleScene\TitleScene.h"
#include "PlayScene\PlayScene.h"
#include "CharaSelectScene\CharaSelectScene.h"
#include "ResultScene\ResultScene.h"

SceneManager* SceneManager::pMyInstance = nullptr;

//コンストラクタ
SceneManager::SceneManager()
	:m_pScene(nullptr),
	m_sceneId(eSCENE_ID::PLAY_SCENE)
{

}

//デストラクタ
SceneManager::~SceneManager()
{
	Finalize();
	if (m_pScene != nullptr)
	{
		m_pScene.reset();
	}
	m_pStates = nullptr;
}

///////////////////////////
//初期化
//引数:デバイスリソースのポインタ,ウインドウ
//戻り値:なし
//////////////////////////
void SceneManager::Initialize()
{
	//m_pDeviceResources = pDeviceResources;
	//m_window = window;
	//m_pStates = pStates;
	//if(m_pScene != nullptr)
	//	m_pScene->Initialize();
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void SceneManager::Update(DX::StepTimer const& timer)
{
	if (m_pScene != nullptr)
		m_pScene->Update(timer);

}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void SceneManager::Render()
{
	if (m_pScene != nullptr)
		m_pScene->Render();

}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void SceneManager::Finalize()
{
	if (m_pScene != nullptr)
		m_pScene->Finalize();

	m_pScene.reset();

}

///////////////////////////
//現在のシーンの取得
//引数:なし
//戻り値:現在のシーンのID
//////////////////////////
eSCENE_ID SceneManager::GetScene()
{
	return m_sceneId;
}

///////////////////////////
//シーンの設定
//引数:設定したいシーンのID
//戻り値:なし
//////////////////////////
void SceneManager::SetScene(const eSCENE_ID scene)
{
	//現在のシーンの終了処理
	if (m_pScene != nullptr)
	{
		m_pScene->Finalize();
	}

	m_sceneId = scene;

	m_pScene.reset();

	//各シーンの読み込み
	switch (m_sceneId)
	{
		//タイトル
		case eSCENE_ID::TITLE_SCENE:
		{
			m_pScene = std::make_unique<TitleScene>();

			break;
		}

		//キャラクターセレクト
		case eSCENE_ID::CHARA_SELECT_SCENE:
		{
			m_pScene = std::make_unique<CharaSelectScene>();
			break;
		}
			
		//プレイシーン
		case eSCENE_ID::PLAY_SCENE:
		{
			m_pScene = std::make_unique<PlayScene>();

			break;
		}

		//リザルト
		case eSCENE_ID::RESULT_SCENE:
		{
			m_pScene = std::make_unique<ResultScene>();
			break;
		}
		default:
			break;
	}
	//シーンの初期化
	if (m_pScene != nullptr)
		m_pScene->Initialize();
}

///////////////////////////
//インスタンスの取得
//引数:なし
//戻り値:自身のポインタ
//////////////////////////
SceneManager * SceneManager::GetInstance()
{
	if (pMyInstance == nullptr)
	{
		pMyInstance = new SceneManager();
	}

	return pMyInstance;
}

///////////////////////////
//インスタンスの解放
//引数:なし
//戻り値:なし
//////////////////////////
void SceneManager::ReleaseInstance()
{
	if (pMyInstance != nullptr)
	{
		delete pMyInstance;
	}
	pMyInstance = nullptr;
}


