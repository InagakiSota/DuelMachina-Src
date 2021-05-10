///////////////////////////////////
//プレイシーンのステートマネージャー
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#pragma once

#include "../../SceneBase.h"
#include <StepTimer.h>


class PlayScene;
class PlaySceneFadeInState;
class PlaySceneFadeOutState;
class PlaySceneMainState;

class PlaySceneStateManager
{
public:
	//コンストラクタ
	PlaySceneStateManager();
	//デストラクタ
	~PlaySceneStateManager();

	//初期化
	void Initialize(PlayScene* pPlayScene);
	//更新
	void Update(DX::StepTimer const& timer, const eSCENE_STATE& sceneState);

private:

	//プレイシーンの各ステートのクラス
	//フェードイン
	std::unique_ptr<PlaySceneFadeInState> m_pPlaySceneFadeInState;
	//フェードアウト
	std::unique_ptr<PlaySceneFadeOutState> m_pPlaySceneFadeOutState;
	//メイン
	std::unique_ptr<PlaySceneMainState> m_pPlaySceneMainState;


};

