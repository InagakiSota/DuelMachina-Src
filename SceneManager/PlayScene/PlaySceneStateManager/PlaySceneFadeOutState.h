///////////////////////////////////
//プレイシーンのフェードアウトステート
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#pragma once
#include "../PlayScene.h"
class PlaySceneFadeOutState : public PlayScene
{
public:
	//コンストラクタ
	PlaySceneFadeOutState();
	//デストラクタ
	~PlaySceneFadeOutState();

	//更新
	void Update(DX::StepTimer const& timer);

	//プレイシーンのポインタ取得
	void SetPlayScene(PlayScene* pPlayScene)
	{
		m_pPlayScene = pPlayScene;
	}


private:
	//プレイシーンのポインタ
	PlayScene* m_pPlayScene;
};

