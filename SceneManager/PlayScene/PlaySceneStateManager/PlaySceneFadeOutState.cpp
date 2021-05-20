///////////////////////////////////
//プレイシーンのフェードアウトステート
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#include "pch.h"
#include "PlaySceneFadeOutState.h"
#include "Adx2.h"
#include "CueSheet_0.h"
#include "../../PlayerID.h"
#include "../../ResultScene/ResultScene.h"

const int PlaySceneFadeOutState::WIN_NUM = 2;

//コンストラクタ
PlaySceneFadeOutState::PlaySceneFadeOutState()
	:m_pPlayScene(nullptr)
{
}

//デストラクタ
PlaySceneFadeOutState::~PlaySceneFadeOutState()
{
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void PlaySceneFadeOutState::Update(DX::StepTimer const& timer)
{
	//リザルトに遷移しなければそのままフェードアウト
	if (m_pPlayScene->GetIsResult() == false)
	{
		m_pPlayScene->SetFadeTimer(m_pPlayScene->GetFadeTimer() + static_cast<float>(timer.GetElapsedSeconds()) * 2.0f);
	}
	//フェードアウトしたらプレイシーンに戻る
	if (m_pPlayScene->GetFadeTimer() >= 1.0f)
	{
		//どちらかのプレイヤーが２本勝利するまで繰り返す
		if (m_pPlayScene->GetPlayerWinNum(static_cast<int>( ePLAYER_ID::PLAYER_1)) < WIN_NUM &&
			m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2)) < WIN_NUM)
		{
			//ラウンド数を切り替える
			switch (m_pPlayScene->GetNowRound())
			{
				//ラウンド１
				case PlayScene::eROUND::ROUND_1:
				{
					//ラウンド2にする
					m_pPlayScene->SetNowRound(PlayScene::eROUND::ROUND_2);
					break;

				}
				//ラウンド２
				case PlayScene::eROUND::ROUND_2:
				{
					//ラウンド3にする
					m_pPlayScene->SetNowRound(PlayScene::eROUND::ROUND_3);
					break;
				}
				default:
					break;
			}
			//リセット
			m_pPlayScene->Reset();
		}
		//どちらかのプレイヤーが２本勝利したらリザルトに遷移
		else
		{
			m_pPlayScene->SetIsResult(true);

			//プレイヤー１が勝利したことにする
			if (m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1)) >= WIN_NUM)
			{
				ResultScene::m_winPlayerID = ePLAYER_ID::PLAYER_1;
			}
			//プレイヤー２が勝利したことにする
			else if (m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2)) >= WIN_NUM)
			{
				ResultScene::m_winPlayerID = ePLAYER_ID::PLAYER_2;
			}


		}
		//BGM停止
		ADX2::GetInstance().Stop(m_pPlayScene->GetSoundID());

	}

}
