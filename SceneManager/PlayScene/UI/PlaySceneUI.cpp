//////////////////////////
//プレイシーンのUIクラス
//作成日：2021年4月30日
//作成者：GT2_04_稲垣颯太
/////////////////////////

#include "pch.h"
#include "PlaySceneUI.h"
#include "Src/SceneManager/PlayerID.h"
#include "../Character/CharacterBase.h"
#include "../Character/CharacterFactory.h"

//体力バーの横幅
const float PlaySceneUI::HP_BAR_WIDTH = 800.0f;
//体力バーの高さ
const float PlaySceneUI::HP_BAR_HEIGHT = 101.0f;
//体力バーのマージン
const float PlaySceneUI::HP_BAR_MARGIN = 20.0f;

//ブースト容量バーの横幅
const float PlaySceneUI::BOOST_BAR_WIDTH = 700.0f;
//体力バーの高さ
const float PlaySceneUI::BOOST_BAR_HEIGHT = 70.0f;

//カウントダウンの時間
const float PlaySceneUI::COUNT_DOWN_TIME = 3.0f;
//カウントダウン画像の座標
const DirectX::SimpleMath::Vector2 PlaySceneUI::COUNT_DOWN_SPRITE_POS = DirectX::SimpleMath::Vector2(640.0f, 300.0f);
//制限時間の最大値
const float PlaySceneUI::TIME_MAX = 60;
//勝利本数の画像の横幅
const float PlaySceneUI::WIN_NUM_SPRITE_WIDTH = 100.0f;
//勝利本数の画像の高さ
const float  PlaySceneUI::WIN_NUM_SPRITE_HEIGHT = 50.0f;
//制限時間の画像の横幅
const float PlaySceneUI::TIME_SPRITE_WIDTH = 100.0f;
//制限時間の画像の高さ
const float PlaySceneUI::TIME_SPRITE_HEIGHT = 200.0f;
//制限時間の画像のX座標
const float PlaySceneUI::TIME_SPRITE_POS_X = 960.0f;
//リザルトのpushSpaceの画像の座標
const DirectX::SimpleMath::Vector2  PlaySceneUI::PUSH_SPACE_RESULT_POS =
DirectX::SimpleMath::Vector2(1300.0f, 800.0f);


//体力バーの座標
const DirectX::SimpleMath::Vector2 PlaySceneUI::HP_BAR_POS[PlaySceneUI::PLAYER_NUM] =
{
	DirectX::SimpleMath::Vector2(20.0f,50.0f),
	DirectX::SimpleMath::Vector2(1900.0f - HP_BAR_WIDTH,50.0f)
};

//体力バーの座標
const DirectX::SimpleMath::Vector2 PlaySceneUI::BOOST_BAR_POS[PlaySceneUI::PLAYER_NUM] =
{
	DirectX::SimpleMath::Vector2(20.0f,900.0f),
	DirectX::SimpleMath::Vector2(1900.0f - BOOST_BAR_WIDTH,900.0f)
};


//コンストラクタ
PlaySceneUI::PlaySceneUI()
	: m_hpBarPos{},
	m_timeSpriteOneRect{},
	m_timeSpriteTenRect{},
	m_pTimeSpriteOne(nullptr),
	m_pTimeSpriteTen(nullptr),
	m_winSpritePos{}
{
}

//デストラクタ
PlaySceneUI::~PlaySceneUI()
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pHPBar[i].reset();
		m_pHPBar[i] = nullptr;
		m_pHPBarBack[i].reset();
		m_pHPBarBack[i] = nullptr;
		m_pWinSprite[i].reset();
		m_pWinSprite[i] = nullptr;
	}

	m_pTimeSpriteOne.reset();
	m_pTimeSpriteTen.reset();
}

/////////////////////
//初期化
//引数：なし
//戻り値：なし
/////////////////////
void PlaySceneUI::Initialize()
{
	//画像の読み込み
	PlaySceneUI::Load();

	//勝利画像の座標設定
	m_winSpritePos = DirectX::SimpleMath::Vector2(640.0f, 300.0f);

	for (int i = 0;i < PLAYER_NUM;i++)
	{
		//体力バーの切り取り位置設定
		m_hpBarRect[i].top = 0;
		m_hpBarRect[i].bottom = static_cast<LONG>(HP_BAR_HEIGHT);
		m_hpBarRect[i].left = 0;
		m_hpBarRect[i].right = static_cast<LONG>(HP_BAR_WIDTH);
		//体力バーの座標設定
		m_hpBarPos[i] = HP_BAR_POS[i];

		//ブースト容量バーの切り取り位置設定
		m_boostBarRect[i].top = 0;
		m_boostBarRect[i].bottom = static_cast<LONG>(BOOST_BAR_HEIGHT);
		m_boostBarRect[i].left = 0;
		m_boostBarRect[i].right = static_cast<LONG>(BOOST_BAR_WIDTH);
		//ブースト容量バーの座標設定
		m_boostBarPos[i] = BOOST_BAR_POS[i];

		for (int j = 0; j < WIN_NUM; j++)
		{
			//ラウンド取得本数の画像の切り取り位置設定
			m_winNumSpriteRect[i][j].top = 0;
			m_winNumSpriteRect[i][j].bottom = static_cast<LONG>(WIN_NUM_SPRITE_HEIGHT);
			m_winNumSpriteRect[i][j].left = 0;
			m_winNumSpriteRect[i][j].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH * 0.5);

		}

		//ラウンド取得本数の画像の座標設定
		//プレイヤー1用
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)][i].y = 170.0f;
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)][i].x = 70.0f + ((WIN_NUM_SPRITE_WIDTH * 0.5f +10.0f) * i);
		//プレイヤー2用
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)][i].y = 170.0f;
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)][i].x = 1920.0f - ((WIN_NUM_SPRITE_WIDTH * 0.5f + 10.0f) * (i + 1)) - 70.0f;


	}

}



/////////////////////
//更新
//引数：なし
//戻り値：なし
/////////////////////
void PlaySceneUI::Update()
{
	//UIの画像の座標設定
	for (int i = 0; i < static_cast<int>(PlayScene::eROUND::ROUND_NUM); i++)
	{
		m_pRoundSprite[i]->Update(COUNT_DOWN_SPRITE_POS);
	}
	m_pFightSprite->Update(COUNT_DOWN_SPRITE_POS);
	m_pTimeUpSprite->Update(COUNT_DOWN_SPRITE_POS);
	m_pDrawSprite->Update(COUNT_DOWN_SPRITE_POS);

	//体力バーの切り取り位置の更新
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = static_cast<LONG>(
		(HP_BAR_WIDTH /m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetMaxHP()) *
		(m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetMaxHP() - m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP()));
	m_hpBarPos[static_cast<int>(ePLAYER_ID::PLAYER_1)].x = m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left + HP_BAR_MARGIN;


	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(HP_BAR_WIDTH -
		((HP_BAR_WIDTH / m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetMaxHP()) *
			(m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetMaxHP() -
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP())));

	//ブースト容量バーの切り取り位置の更新
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = static_cast<LONG>(
		(BOOST_BAR_WIDTH / m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetMaxBoostCap()) *
		(m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetMaxBoostCap() - 
			m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetBoostCap()));
	m_boostBarPos[static_cast<int>(ePLAYER_ID::PLAYER_1)].x = m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left + HP_BAR_MARGIN;

	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(BOOST_BAR_WIDTH -
		((BOOST_BAR_WIDTH / m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetMaxBoostCap()) *
			(m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetMaxBoostCap() -
				m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetBoostCap())));

	//勝利本数の画像の切り取り位置の更新
	//プレイヤー１
	for (int i = 0; i < m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1)); i++)
	{
		if (m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1)) > 0)
		{
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_1)][m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1)) - 1].left = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH * 0.5);
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_1)][m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_1)) - 1].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH);
		}
	}
	//プレイヤー2
	for (int i = 0; i < m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2)); i++)
	{
		if (m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2)) > 0)
		{
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_2)][m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2)) - 1].left = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH * 0.5);
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_2)][m_pPlayScene->GetPlayerWinNum(static_cast<int>(ePLAYER_ID::PLAYER_2)) - 1].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH);
		}
	}

	//頭上の画像のY座標更新
	//プレイヤー1用
	switch (CharacterFactory::m_player1Chara)
	{
		//キャラクター１
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)].y =
				((m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetPos().y - 0.5f) * -1) * 260.0f;
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)].y =
				((m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetPos().y - 1.5f) * -1) * 260.0f;
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)].y =
				((m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetPos().y - 2.0f) * -1) * 260.0f;
			break;
		}

	default:
		break;
	}
	//プレイヤー2用
	switch (CharacterFactory::m_player2Chara)
	{
		//キャラクター１
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)].y =
				((m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetPos().y - 0.5f) * -1) * 260.0f;
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)].y =
				((m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetPos().y - 1.5f) * -1) * 260.0f;
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)].y =
				((m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetPos().y - 2.0f) * -1) * 260.0f;
			break;
		}

		default:
			break;
	}


	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//体力バーの更新	
		m_pHPBar[i]->Update(m_hpBarPos[i], m_hpBarRect[i]);
		m_pHPBarDanger[i]->Update(m_hpBarPos[i], m_hpBarRect[i]);
		m_pHPBarBack[i]->Update(HP_BAR_POS[i].x - 10.0f, HP_BAR_POS[i].y - 10.0f);
		//ブースト容量バーの更新
		m_pBoostBar[i]->Update(m_boostBarPos[i], m_boostBarRect[i]);
		m_pBoostBack[i]->Update(BOOST_BAR_POS[i].x - 10.0f, BOOST_BAR_POS[i].y - 10.0f);

		//頭上の画像の更新
		m_overHeadSpritePos[i].x = 960.0f - 50.0f + (m_pPlayScene->GetPlayer(i))->GetPos().x * 230.0f;
		m_pOverHeadSprite[i]->Update(m_overHeadSpritePos[i]);

		//勝利本数の画像の更新
		for (int j = 0; j < WIN_NUM; j++)
		{
			m_pWinNumSprtie[i][j]->Update(m_winNumSpritePos[i][j], m_winNumSpriteRect[i][j]);
		}

		//リザルトの勝利画像の更新
		if (m_pPlayScene->GetPlaySceneState() == PlayScene::ePLAY_SCENE_STATE::RESULT)
		{
			if (m_pWinSprite[i] != nullptr)
				m_pWinSprite[i]->Update(m_winSpritePos);
		}

	}

	//制限時間の画像の切り取り位置の設定
	m_timeSpriteOneRect.top = 0;
	m_timeSpriteOneRect.bottom = static_cast<LONG>(PlaySceneUI::TIME_SPRITE_HEIGHT);
	m_timeSpriteOneRect.left = static_cast<LONG>(PlaySceneUI::TIME_SPRITE_WIDTH * (static_cast<int>(m_pPlayScene->GetTime()) % 10));
	m_timeSpriteOneRect.right = m_timeSpriteOneRect.left + static_cast<LONG>(PlaySceneUI::TIME_SPRITE_WIDTH);

	m_timeSpriteTenRect.top = 0;
	m_timeSpriteTenRect.left = static_cast<LONG>(PlaySceneUI::TIME_SPRITE_WIDTH * (static_cast<int>(m_pPlayScene->GetTime()) / 10));
	m_timeSpriteTenRect.bottom = static_cast<LONG>(PlaySceneUI::TIME_SPRITE_HEIGHT);
	m_timeSpriteTenRect.right = m_timeSpriteTenRect.left + static_cast<LONG>(PlaySceneUI::TIME_SPRITE_WIDTH);

	m_pTimeSpriteOne->Update(PlaySceneUI::TIME_SPRITE_POS_X, 0.0f, m_timeSpriteOneRect);
	m_pTimeSpriteTen->Update(PlaySceneUI::TIME_SPRITE_POS_X - PlaySceneUI::TIME_SPRITE_WIDTH, 0.0f, m_timeSpriteTenRect);


}

/////////////////////
//描画
//引数：なし
//戻り値：なし
/////////////////////
void PlaySceneUI::Render()
{
	//プレイヤー１の勝利画像
	if (m_pPlayScene->GetPlaySceneState() == PlayScene::ePLAY_SCENE_STATE::RESULT &&
		m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP() >
		m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP())
	{
		m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Draw(true);
	}

	//プレイヤー2の勝利画像
	else if (m_pPlayScene->GetPlaySceneState() == PlayScene::ePLAY_SCENE_STATE::RESULT &&
		m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP() >
		m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP())
	{
		m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Draw(true);
	}

	//引き分け画像
	else if (m_pPlayScene->GetPlaySceneState() == PlayScene::ePLAY_SCENE_STATE::RESULT &&
		m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_1))->GetHP() ==
		m_pPlayScene->GetPlayer(static_cast<int>(ePLAYER_ID::PLAYER_2))->GetHP())
	{
		m_pDrawSprite->Draw(true);
	}

	if (m_pPlayScene->GetIsResult() == false)
	{
		//体力バー、ブースト容量バーの描画
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			//背景
			m_pHPBarBack[i]->Draw();
			m_pBoostBack[i]->Draw();

			//本体
			if (m_pPlayScene->GetPlayer(i)->GetHP() > m_pPlayScene->GetPlayer(i)->GetMaxHP() * 0.3f)
				m_pHPBar[i]->Draw(false, true);
			else m_pHPBarDanger[i]->Draw(false, true);

			m_pBoostBar[i]->Draw(false, true);

			//勝利本数の画像の描画
			for (int j = 0; j < WIN_NUM; j++)
			{
				m_pWinNumSprtie[i][j]->Draw(false, true);
			}
		}

		//頭上の画像の描画
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			//頭上の画像
			m_pOverHeadSprite[i]->Draw(true);
		}


		//制限時間の画像
		m_pTimeSpriteOne->Draw(true, true);
		m_pTimeSpriteTen->Draw(true, true);

		//カウントダウンの画像
		if (m_pPlayScene->GetPlaySceneState() == PlayScene::ePLAY_SCENE_STATE::COUNT_DOWN)
		{
			if (m_pPlayScene->GetCountDownTimer() <= COUNT_DOWN_TIME / 2)
			{
				//現在のラウンド数によって表示する画像を切り替える
				switch (m_pPlayScene->GetNowRound())
				{
					case PlayScene::eROUND::ROUND_1:
					{
						m_pRoundSprite[static_cast<int>(PlayScene::eROUND::ROUND_1)]->Draw(true);

						break;
					}
					case PlayScene::eROUND::ROUND_2:
					{
						m_pRoundSprite[static_cast<int>(PlayScene::eROUND::ROUND_2)]->Draw(true);

						break;
					}

					case PlayScene::eROUND::ROUND_3:
					{
						m_pRoundSprite[static_cast<int>(PlayScene::eROUND::ROUND_3)]->Draw(true);
						break;
					}

					default:
						break;
				}

			}
			else
			{
				m_pFightSprite->Draw(true);
			}
		}
	}
	else
	{
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			if (m_pWinSprite[i] != nullptr)
				m_pWinSprite[i]->Update(m_winSpritePos.x + 600.0f, m_winSpritePos.y - 300.0f);
		}

		m_pPushSpaceResult->Update(PUSH_SPACE_RESULT_POS);
		m_pPushSpaceResult->Draw(true);
	}

	//タイムアップの画像
	if (m_pPlayScene->GetPlaySceneState() == PlayScene::ePLAY_SCENE_STATE::TIME_UP)
	{
		m_pTimeUpSprite->Draw(true);
	}

}

/////////////////////
//終了処理
//引数：なし
//戻り値：なし
/////////////////////
void PlaySceneUI::Finalize()
{
	m_pPushSpaceResult->Reset();


	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pHPBar[i]->Reset();
		m_pHPBar[i].reset();

		m_pHPBarDanger[i]->Reset();
		m_pHPBarDanger[i].reset();


		m_pHPBarBack[i]->Reset();
		m_pHPBarBack[i].reset();

		m_pWinSprite[i]->Reset();
		m_pWinSprite[i].reset();

		for (int j = 0; j < WIN_NUM; j++)
		{
			m_pWinNumSprtie[i][j]->Reset();
			m_pWinNumSprtie[i][j].reset();
		}


		m_pOverHeadSprite[i]->Reset();
		m_pOverHeadSprite[i].reset();

	}
	for (int i = 0; i < static_cast<int>(PlayScene::eROUND::ROUND_NUM); i++)
	{
		m_pRoundSprite[i]->Reset();
		m_pRoundSprite[i].reset();
	}
	m_pFightSprite->Reset();
	m_pFightSprite.reset();

	m_pTimeUpSprite->Reset();
	m_pTimeUpSprite.reset();

	m_pDrawSprite->Reset();
	m_pDrawSprite.reset();

	m_pTimeSpriteOne->Reset();
	m_pTimeSpriteOne.reset();

	m_pTimeSpriteTen->Reset();
	m_pTimeSpriteTen.reset();


}


/////////////////////
//画像の読み込み
//引数：なし
//戻り値：なし
/////////////////////
void PlaySceneUI::Load()
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//勝利画像の読み込み
		m_pWinSprite[i] = std::make_unique<Sprite2D>();

		//体力バーの背景読み込み
		m_pHPBarBack[i] = std::make_unique<Sprite2D>();
		m_pHPBarBack[i]->Create(L"Resources/Textures/HPBar.jpg");

		//ブースト容量バーの背景読み込み
		m_pBoostBack[i] = std::make_unique<Sprite2D>();
		m_pBoostBack[i]->Create(L"Resources/Textures/BoostBar_Gray.jpg");

		//体力バーの読み込み
		m_pHPBar[i] = std::make_unique<Sprite2D>();
		m_pHPBar[i]->Create(L"Resources/Textures/HPBar.png");
		//危険状態の体力バーの読み込み
		m_pHPBarDanger[i] = std::make_unique<Sprite2D>();
		m_pHPBarDanger[i]->Create(L"Resources/Textures/HPBar_Red.jpg");

		//ブースト容量バーの読み込み
		m_pBoostBar[i] = std::make_unique<Sprite2D>();
		m_pBoostBar[i]->Create(L"Resources/Textures/BoostBar.jpg");

		//勝利本数の画像の描画
		for (int j = 0; j < WIN_NUM; j++)
		{
			m_pWinNumSprtie[i][j] = std::make_unique<Sprite2D>();
			m_pWinNumSprtie[i][j]->Create(L"Resources/Textures/winNum.png");


		}

		//プレイヤーの頭上の画像読み込み
		m_pOverHeadSprite[i] = std::make_unique<Sprite2D>();
	}

	//プレイヤーごとの勝利画像読み込み
	m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Create(L"Resources/Textures/p1win.png");
	m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Create(L"Resources/Textures/p2win.png");

	//プレイヤーごとの頭上の画像読み込み
	m_pOverHeadSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Create(L"Resources/Textures/p1Icon.png");
	m_pOverHeadSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Create(L"Resources/Textures/p2Icon.png");


	//ラウンド数の画像の読み込み
	for (int i = 0; i < static_cast<int>(PlayScene::eROUND::ROUND_NUM); i++)
	{
		m_pRoundSprite[i] = std::make_unique<Sprite2D>();
	}
	m_pRoundSprite[static_cast<int>(PlayScene::eROUND::ROUND_1)]->Create(L"Resources/Textures/Round1.png");
	m_pRoundSprite[static_cast<int>(PlayScene::eROUND::ROUND_2)]->Create(L"Resources/Textures/Round2.png");
	m_pRoundSprite[static_cast<int>(PlayScene::eROUND::ROUND_3)]->Create(L"Resources/Textures/Round3.png");



	//Fight画像の読み込み
	m_pFightSprite = std::make_unique<Sprite2D>();
	m_pFightSprite->Create(L"Resources/Textures/Fight.png");

	//タイムアップ画像の読み込み
	m_pTimeUpSprite = std::make_unique<Sprite2D>();
	m_pTimeUpSprite->Create(L"Resources/Textures/TimeUp.png");

	//引き分け画像の読み込み
	m_pDrawSprite = std::make_unique<Sprite2D>();
	m_pDrawSprite->Create(L"Resources/Textures/Draw.png");

	//制限時間の画像の読み込み
	m_pTimeSpriteOne = std::make_unique<Sprite2D>();
	m_pTimeSpriteOne->Create(L"Resources/Textures/Number_mini.png");
	m_pTimeSpriteTen = std::make_unique<Sprite2D>();
	m_pTimeSpriteTen->Create(L"Resources/Textures/Number_mini.png");

	//pushSpaceの画像読み込み
	m_pPushSpaceResult = std::make_unique<Sprite2D>();
	m_pPushSpaceResult->Create(L"Resources/Textures/pushSpace_result.png");

}


/////////////////////
//リセット
//引数：なし
//戻り値：なし
/////////////////////
void PlaySceneUI::Reset()
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//体力バーの座標のリセット
		m_hpBarPos[i] = HP_BAR_POS[i];
		//ブースト容量バーのリセット
		m_boostBarPos[i] = BOOST_BAR_POS[i];
	}

}
