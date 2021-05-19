///////////////////////////////////
//プレイシーンのフェードインステート
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#include "pch.h"
#include "PlaySceneFadeInState.h"
#include "Adx2.h"
#include "CueSheet_0.h"
#include "Src/SceneManager/PlayScene/Character/CharacterBase.h"

//コンストラクタ
PlaySceneFadeInState::PlaySceneFadeInState()
	:m_pPlayScene(nullptr)
{
}

//デストラクタ
PlaySceneFadeInState::~PlaySceneFadeInState()
{
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void PlaySceneFadeInState::Update(DX::StepTimer const& timer)
{
	//フェードのタイマーを加算する
	SetFadeTimer(GetFadeTimer() - static_cast<float>(timer.GetElapsedSeconds()) * 2.0f);

	if (GetFadeTimer() <= 0.0f)
	{
		//BGMの再生
		SetSoundID(ADX2::GetInstance().Play(CRI_CUESHEET_0_PLAYSCENE_BGM));
		//メイン状態に遷移
		SetSceneState(eSCENE_STATE::MAIN);
	}

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//プレイヤーの床との当たり判定
		m_pPlayScene->GetPlayer(i)->HitFloor(m_pPlayScene->GetFloor());

		//プレイヤーの準備
		m_pPlayScene->GetPlayer(i)->Ready(timer);

	}

}
