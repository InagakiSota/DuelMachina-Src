///////////////////////////////////
//プレイシーンのフェードアウトステート
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#pragma once

#include "../PlayScene.h"

class PlaySceneFadeOutState
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
	//勝利するための取得本数
	static const int WIN_NUM;

	//プレイシーンのポインタ
	PlayScene* m_pPlayScene;


};

