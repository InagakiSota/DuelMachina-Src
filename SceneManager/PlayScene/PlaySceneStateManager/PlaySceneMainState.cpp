///////////////////////////////////
//プレイシーンのメインステート
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////
#include "pch.h"
#include "PlaySceneMainState.h"
#include "Adx2.h"
#include "CueSheet_0.h"
#include "../Character/CharacterBase.h"
#include "../../PlayerID.h"
#include "../AttackManager/AttackManager.h"

 //カウントダウンの時間
const float PlaySceneMainState::COUNT_DOWN_TIME = 3.0f;
//制限時間の最大値
const float PlaySceneMainState::TIME_MAX = 60.0f;
//タイムアップシーンの制限時間
const float PlaySceneMainState::TIME_UP_TIME = 3.0f;
//ラウンド切り替え時の時間
const float PlaySceneMainState::ROUND_CHANGE_TIME = 1.0f;


//コンストラクタ
PlaySceneMainState::PlaySceneMainState()
	:m_pPlayScene(nullptr)
{
}

//デストラクタ
PlaySceneMainState::~PlaySceneMainState()
{
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void PlaySceneMainState::Update(DX::StepTimer const& timer)
{
	switch (m_pPlayScene->GetPlaySceneState())
	{
		//カウントダウン
		case PlayScene::ePLAY_SCENE_STATE::COUNT_DOWN:
		{

			for (int i = 0; i < PLAYER_NUM; i++)
			{
				//プレイヤーの床との当たり判定
				m_pPlayScene->GetPlayer(i)->HitFloor(m_pPlayScene->GetFloor());
				//プレイヤーの準備
				m_pPlayScene->GetPlayer(i)->Ready(timer);
			}
			//敵の座標の取得
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->SetEnemyPos(m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetPos());
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->SetEnemyPos(m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetPos());
			
			//プレイヤー同士の体の当たり判定
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->HitEnemyBody(
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetBodyCollBox(),
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHeadCollBox());
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->HitEnemyBody(
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetBodyCollBox(),
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHeadCollBox());

			//カウントダウンのタイマーを加算
			m_pPlayScene->SetCountDownTimer(m_pPlayScene->GetCountDownTimer() + static_cast<float>(timer.GetElapsedSeconds()));
			//一定時間になったらメイン状態に遷移
			if (m_pPlayScene->GetCountDownTimer() >= COUNT_DOWN_TIME)
			{
				m_pPlayScene->SetPlaySceneState(ePLAY_SCENE_STATE::MAIN);


				m_pPlayScene->SetCountDownTimer(0.0f);
			}
			break;
		}
		//メイン
		case PlayScene::ePLAY_SCENE_STATE::MAIN:
		{
			//攻撃のマネージャーの更新
			AttackManager::GetInstance()->Update();


			//時間を減らす
			m_pPlayScene->SetTime(m_pPlayScene->GetTime() + static_cast<float>(timer.GetElapsedSeconds()));
			//m_time -= static_cast<float>(timer.GetElapsedSeconds());
			//時間がゼロになったらタイムアップに遷移
			if (m_pPlayScene->GetTime() <= 0)
			{
				//m_playSceneState = ePLAY_SCENE_STATE::TIME_UP;
				m_pPlayScene->SetPlaySceneState(ePLAY_SCENE_STATE::TIME_UP);
				//プレイヤー１と２の勝利数を増やす
				if (m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP() ==
					m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP())
				{
					m_pPlayScene->SetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1),
						m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1) + 1));
					m_pPlayScene->SetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2),
						m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2) + 1));
				}
			}

			for (int i = 0; i < PLAYER_NUM; i++)
			{
				//プレイヤーの床との当たり判定
				m_pPlayScene->GetPlayer(i)->HitFloor(m_pPlayScene->GetFloor());
				//プレイヤーの準備
				m_pPlayScene->GetPlayer(i)->Ready(timer);
			}

			//敵の座標の取得
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->
				SetEnemyPos(m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetPos());
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->
				SetEnemyPos(m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetPos());
			//プレイヤー同士の体の当たり判定
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->HitEnemyBody(
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetBodyCollBox(),
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHeadCollBox());
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->HitEnemyBody(
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetBodyCollBox(),
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHeadCollBox());

			//プレイヤー１の体力がゼロになったらリザルトに遷移
			if (m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP() <= 0)
			{
				m_pPlayScene->SetPlaySceneState(ePLAY_SCENE_STATE::RESULT);
				//プレイヤー２の勝利数を増やす
				m_pPlayScene->SetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2),
					m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2) + 1));
			}
			//プレイヤー２の体力がゼロになったらリザルトに遷移
			if (m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP() <= 0)
			{
				m_pPlayScene->SetPlaySceneState(ePLAY_SCENE_STATE::RESULT);
				//プレイヤー１の勝利数を増やす
				m_pPlayScene->SetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1),
					m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1) + 1));
			}

			break;
		}

		case PlayScene::ePLAY_SCENE_STATE::TIME_UP:
		{
			//攻撃のマネージャーの更新
			//AttackManager::GetInstance()->Update();

			//カウントダウンのタイマーを加算
			m_pPlayScene->SetCountDownTimer(m_pPlayScene->GetCountDownTimer() + static_cast<float>(timer.GetElapsedSeconds()));
			if (m_pPlayScene->GetCountDownTimer() >= PlaySceneMainState::TIME_UP_TIME)
			{
				m_pPlayScene->SetPlaySceneState(ePLAY_SCENE_STATE::RESULT);

				//プレイヤー１の体力がプレイヤー２より多ければプレイヤー１の勝利数を増やす
				if (m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP() >
					m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP())
				{
					m_pPlayScene->SetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1),
						m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1) + 1));
				}
				//プレイヤー２の体力がプレイヤー１より多ければプレイヤー２の勝利数を増やす
				if (m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP() <
					m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP())
				{
					m_pPlayScene->SetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2),
						m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2) + 1));
				}

				m_pPlayScene->SetCountDownTimer(0.0f);
			}
			break;
		}


		//リザルト
		case PlayScene::ePLAY_SCENE_STATE::RESULT:
		{

			for (int i = 0; i < PLAYER_NUM; i++)
			{
				//プレイヤーの床との当たり判定
				m_pPlayScene->GetPlayer(i)->HitFloor(m_pPlayScene->GetFloor());
			}
			//プレイヤー１の体力がプレイヤー２より多ければプレイヤー１を動かす
			if (m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP() >
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP())
			{
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->Ready(timer);
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->SetCharaState(eCHARACTER_STATE::LOSE);
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->Lose(timer);
			}
			//プレイヤー２の体力がプレイヤー１より多ければプレイヤー２を動かす
			else
			{
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->Ready(timer);
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->SetCharaState(eCHARACTER_STATE::LOSE);
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->Lose(timer);
			}

			//プレイヤー同士の体の当たり判定
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->HitEnemyBody(
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetBodyCollBox(),
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHeadCollBox());

			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->HitEnemyBody(
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetBodyCollBox(),
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHeadCollBox());

			//カウントダウンのタイマーを加算
			m_pPlayScene->SetCountDownTimer(m_pPlayScene->GetCountDownTimer() + static_cast<float>(timer.GetElapsedSeconds()));
			if (m_pPlayScene->GetCountDownTimer() >= PlaySceneMainState::ROUND_CHANGE_TIME || GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Space))
			{
				SetSceneState(eSCENE_STATE::FADE_OUT);
			}

			break;
		}
		default:
			break;
	}

}
