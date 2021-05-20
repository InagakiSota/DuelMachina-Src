///////////////////////////////////
//プレイシーンのメインステート
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#pragma once

#include "../PlayScene.h"

class PlaySceneMainState
{	
public:
	//コンストラクタ
	PlaySceneMainState();
	//デストラクタ
	~PlaySceneMainState();

	//更新
	void Update(DX::StepTimer const& timer);

	//プレイシーンのポインタ取得
	void SetPlayScene(PlayScene* pPlayScene)
	{
		m_pPlayScene = pPlayScene;
	}


private:
	//制限時間の最大値
	static const float TIME_MAX;
	//タイムアップシーンの時間
	static const float TIME_UP_TIME;
	//ラウンド切り替えの時間
	static const float ROUND_CHANGE_TIME;
	//カウントダウンの時間
	static const float COUNT_DOWN_TIME;


	//プレイシーンのポインタ
	PlayScene* m_pPlayScene;

	//カウントダウン
	void CountDown(DX::StepTimer const& timer);

	//メイン
	void Main(DX::StepTimer const& timer);

	//タイムアップ
	void TimeUp(DX::StepTimer const& timer);

	//リザルト
	void Result(DX::StepTimer const& timer);
};

