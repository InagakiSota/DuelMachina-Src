///////////////////////////////////
//プレイシーンのステートマネージャー
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#include "pch.h"
#include "PlaySceneStateManager.h"

#include "PlaySceneFadeInState.h"
#include "PlaySceneFadeOutState.h"
#include "PlaySceneMainState.h"



PlaySceneStateManager::PlaySceneStateManager()
{
}

PlaySceneStateManager::~PlaySceneStateManager()
{
}

///////////////////////////
//初期化
//引数:プレイシーンのポインタ
//戻り値:なし
//////////////////////////
void PlaySceneStateManager::Initialize(PlayScene* pPlayScene)
{
	//プレイシーンの各ステートクラスの読み込み
	//フェードイン
	m_pPlaySceneFadeInState = std::make_unique<PlaySceneFadeInState>();
	m_pPlaySceneFadeInState->SetPlayScene(pPlayScene);
	//フェードアウト
	m_pPlaySceneFadeOutState = std::make_unique<PlaySceneFadeOutState>();
	m_pPlaySceneFadeOutState->SetPlayScene(pPlayScene);
	//メイン
	m_pPlaySceneMainState = std::make_unique<PlaySceneMainState>();
	m_pPlaySceneMainState->SetPlayScene(pPlayScene);

}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void PlaySceneStateManager::Update(DX::StepTimer const& timer, const eSCENE_STATE& sceneState)
{
	//各ステートのクラスの更新
	switch (sceneState)
	{
		//フェードイン
		case eSCENE_STATE::FADE_IN:
		{
			m_pPlaySceneFadeInState->Update(timer);

			break;
		}
		//フェードアウト
		case eSCENE_STATE::FADE_OUT:
		{
			m_pPlaySceneFadeOutState->Update(timer);

			break;
		}

		//メイン
		case eSCENE_STATE::MAIN:
		{
			m_pPlaySceneMainState->Update(timer);

			break;
		}


		default:
			break;
	}
}
