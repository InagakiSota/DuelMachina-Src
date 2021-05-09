///////////////////////////////////
//プレイシーンのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////

#include "pch.h"
#include "PlayScene.h"


#include "../../FrameWork/EffectManager/EffectManager.h"
#include "../../FrameWork/FbxModel/FbxModel.h"
#include "../../FrameWork/Sprite2D/Sprite2D.h"
#include "../../FrameWork/DebugFont/DebugFont.h"
#include "../../FrameWork/ModelObject/ModelObject.h"
#include "../../Fbx.h"
#include "../../FrameWork/DebugCamera/DebugCamera.h"
#include "Character\CharacterFactory.h"
#include "Character\CharacterBase.h"
#include "Character\CharacterID.h"
#include "../PlayerID.h"

#include "Keyboard.h"
#include "../SceneManager.h"
#include "AttackManager/AttackManager.h"

#include "../ResultScene/ResultScene.h"

#include "../../FrameWork/FbxModel/FbxModel.h"
#include "Character\ShadowManager.h"

#include "Character\HitEffectManager.h"
#include "Src\FrameWork\FadeManager\FadeManager.h"

#include "Adx2.h"
#include "CueSheet_0.h"

#include "../../FrameWork/LoadDisplay/LoadDisplay.h"
#include "Src/Cgdi.h"
#include "UI/PlaySceneUI.h"


 //カウントダウンの時間
 const float PlayScene::COUNT_DOWN_TIME = 3.0f;
 //制限時間の最大値
 const float PlayScene::TIME_MAX = 60.0f;
 //タイムアップシーンの制限時間
 const float PlayScene::TIME_UP_TIME = 3.0f;
 //ラウンド切り替え時の時間
 const float PlayScene::ROUND_CHANGE_TIME = 1.0f;

 //カーソルの座標
 const DirectX::SimpleMath::Vector2 PlayScene::MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::OVER_ID)] =
 {
	DirectX::SimpleMath::Vector2(630.0f,300.0f),
	DirectX::SimpleMath::Vector2(630.0f,500.0f),
	DirectX::SimpleMath::Vector2(630.0f,700.0f)
 };


//コンストラクタ
PlayScene::PlayScene()
	:m_floorBox{},
	m_pFloor(nullptr),
	m_floorWorld{},
	m_space(nullptr),
	m_spaceWorld{},
	//m_pAttackManager(nullptr),
	m_pShadowManager{},
	//基底クラスのコンストラクタ
	SceneBase::SceneBase()
{

	m_countdownTimer = 0.0f;

	m_isStop = false;
	m_isManualUp = m_isManualDown = m_isManualRight = m_isManualLeft = false;
	m_isManualDisplay = false;
	m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);
	m_isResult = false;
}

//デストラクタ
PlayScene::~PlayScene()	
{
	//念のため終了処理
	//Finalize();

	m_pFloor.reset();
	//m_pFbx.reset();
	m_pDebugCamera.reset();
	m_space.reset();
	m_sprite2D.reset();

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pPlayer[i] = nullptr;
		m_pShadowManager[i].reset();
	}

	//攻撃マネージャーのインスタンスの解放
	AttackManager::ReleaseInstance();




}

///////////////////////////
//初期化
//引数:デバイスリソースのポインタ
//戻り値:なし
//////////////////////////
void PlayScene::Initialize()
{
	//基底クラスの初期化関数
	SceneBase::Initialize();

	//m_pDebugCamera = std::make_unique<DebugCamera>();

	//攻撃のマネージャーの初期化
	AttackManager::GetInstance()->Initialize(gdi->GetDeviceResources());
	
	//天球の読み込み
	m_space = std::make_unique<ModelObject>();
	m_space->Create(gdi->GetDeviceResources(), L"Resources/Models/Space.cmo");
	
	m_isResult = false;

	//床の作成
	m_floorBox.pos = DirectX::SimpleMath::Vector3(0.0f, -2.0f, 0.0f);
	m_floorBox.size_h = DirectX::SimpleMath::Vector3(5.0f, 0.25f, 1.0f);
	m_pFloor = DirectX::GeometricPrimitive::CreateBox(gdi->GetDeviceResources()->GetD3DDeviceContext(), m_floorBox.size_h*2);
	m_floorWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_floorBox.pos);

	//プレイヤーの作成
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)] = CharacterFactory::CreatePlayer1();
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)] = CharacterFactory::CreatePlayer2();

	//初期座標の設定
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetStartPos(DirectX::SimpleMath::Vector3(-2.0f, 1.0f, 0.0f));
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetStartPos(DirectX::SimpleMath::Vector3(2.0f, 1.0f, 0.0f));

	//メニューの画像読み込み
	m_pMenuSprite = std::make_unique<Sprite2D>();
	m_pMenuSprite->Create(L"Resources/Textures/Menu.png");
	m_pMenuCursorSprite = std::make_unique<Sprite2D>();
	m_pMenuCursorSprite->Create(L"Resources/Textures/menuCursol.png");

	//UIクラスの読み込み
	m_pPlaySceneUI = std::make_unique < PlaySceneUI>();
	//プレイシーンのポインタを渡す
	m_pPlaySceneUI->SetPlayScene(this);
	//UIクラスの初期化
	m_pPlaySceneUI->Initialize();

	//プレイヤーの初期化
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pPlayer[i]->Initialize();
		m_pPlayer[i]->SetAttackManager(AttackManager::GetInstance());
		//プレイヤーの勝利本数の初期化
		m_playerWinNum[i] = 0;
		
		//足元の影の読み込み、初期化
		m_pShadowManager[i] = std::make_unique<ShadowManager>();
		m_pShadowManager[i]->Initialize(1,10,DirectX::SimpleMath::Vector3::Zero);

	}

	//カメラの注視点の初期化
	m_targetPos = DirectX::SimpleMath::Vector3::Zero;

	//プレイシーンのステートを初期化
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;

	//ラウンド数の初期化
	m_nowRound = eROUND::ROUND_1;

	//制限時間の初期化
	m_time = PlayScene::TIME_MAX;

	//敵のポインタ取得
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetEnemy(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]);
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetEnemy(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]);

	//操作説明画面クラスのポインタ
	m_pPlaySceneManual = std::make_unique<PlaySceneManual>();
	//初期化
	m_pPlaySceneManual->Initialize();
	m_pPlaySceneManual->SetPlayScene(this);

	////操作説明の画像読み込み、初期座標設定
	//for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	//{
	//	m_pManualSprite[i] = std::make_unique<Sprite2D>();

	//	m_ManualSpritePos[i].y = -MANUAL_SPRITE_HEIGHT;

	//}
	//m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = MANUAL_SPRITE_WIDTH;

	//m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)]
	//	->Create(L"Resources/Textures/Manual_1.png");

	////操作説明のカーソル画像の読み込み
	//m_pManualCursorSpriteRight = std::make_unique<Sprite2D>();
	//m_pManualCursorSpriteRight->Create( L"Resources/Textures/ManualCursolr_Right.png");
	//m_pManualCursorSpriteLeft = std::make_unique<Sprite2D>();
	//m_pManualCursorSpriteLeft->Create( L"Resources/Textures/ManualCursolr_Left.png");

	////操作しているキャラクターごとに読み込む操作説明の画像を切り替える
	//switch (CharacterFactory::m_player1Chara)
	//{
	//	//キャラクター１
	//	case eCHARACTER_ID::CHARACTER_1:
	//	{
	//		m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
	//			->Create( L"Resources/Textures/Manual_2_chara1.png");
	//		break;
	//	}
	//	//キャラクター２
	//	case eCHARACTER_ID::CHARACTER_2:
	//	{
	//		m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
	//			->Create( L"Resources/Textures/Manual_2_chara2.png");
	//		break;
	//	}
	//	//キャラクター３
	//	case eCHARACTER_ID::CHARACTER_3:
	//	{
	//		m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
	//			->Create(L"Resources/Textures/Manual_2_chara3.png");
	//		break;
	//	}

	//	default:
	//		break;
	//}


}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Update(DX::StepTimer const& timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//基底クラスの更新関数
	SceneBase::Update(timer);

	//経過時間を加算する
	m_totalSeconds = static_cast<float>(timer.GetTotalSeconds());


	SetView(DirectX::SimpleMath::Matrix::CreateLookAt(GetCameraPos(), 
			m_targetPos, DirectX::SimpleMath::Vector3::UnitY));

	//m_view = m_pDebugCamera->getViewMatrix();

	float time = float(timer.GetTotalSeconds());


	for (int i = 0; i < PLAYER_NUM; i++)
	{	
		//足元の影のエフェクトの更新
		m_pShadowManager[i]->Update(timer, m_pPlayer[i]->GetPos());
		//体力のバッファの同期
		m_playerHpBuffer[i] = m_pPlayer[i]->GetHP();
	}


	//天球をY軸で回転させる
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(0.001);
	m_spaceWorld *= rotY;

	//UIクラスの更新
	m_pPlaySceneUI->Update();

	//フェードマネージャーの更新
	FadeManager::GetInstance()->Update(timer, GetFadeTimer());

	//シーンのステート
	if (m_isStop == false)
	{
		//天球の更新
		m_space->Update(m_spaceWorld);

		switch (GetSceneState())
		{
			//フェードイン
			case eSCENE_STATE::FADE_IN:
			{

				//フェードのタイマーを加算する
				SetFadeTimer(GetFadeTimer() - static_cast<float>(timer.GetElapsedSeconds()) * 2.0f);

				if (GetFadeTimer() <= 0.0f)
				{	
					//BGMの再生
					SetSoundID(ADX2::GetInstance().Play(CRI_CUESHEET_0_PLAYSCENE_BGM));

					//LoadDisplay::GetInstance()->Update(false);
					SetSceneState(eSCENE_STATE::MAIN);
				}

				for (int i = 0; i < PLAYER_NUM; i++)
				{
					//プレイヤーの床との当たり判定
					m_pPlayer[i]->HitFloor(m_floorBox);
					//プレイヤーの準備
					m_pPlayer[i]->Ready(timer);
				}

				break;
			}

			//メイン
			case eSCENE_STATE::MAIN:
			{
				switch (m_playSceneState)
				{
					//カウントダウン
					case PlayScene::ePLAY_SCENE_STATE::COUNT_DOWN:
					{

						for (int i = 0; i < PLAYER_NUM; i++)
						{
							//プレイヤーの床との当たり判定
							m_pPlayer[i]->HitFloor(m_floorBox);
							//プレイヤーの準備
							m_pPlayer[i]->Ready(timer);
						}
						//敵の座標の取得
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetEnemyPos(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos());
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetEnemyPos(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos());
						//プレイヤー同士の体の当たり判定
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHeadCollBox());

						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHeadCollBox());


						m_countdownTimer += static_cast<float>(timer.GetElapsedSeconds());
						if (m_countdownTimer >= COUNT_DOWN_TIME)
						{
							m_playSceneState = ePLAY_SCENE_STATE::MAIN;
							m_countdownTimer = 0.0f;
						}
						break;
					}
					//メイン
					case PlayScene::ePLAY_SCENE_STATE::MAIN:
					{
						//攻撃のマネージャーの更新
						AttackManager::GetInstance()->Update();


						//時間を減らす
						m_time -= static_cast<float>(timer.GetElapsedSeconds());
						//時間がゼロになったらタイムアップに遷移
						if (m_time <= 0)
						{
							m_playSceneState = ePLAY_SCENE_STATE::TIME_UP;
							//プレイヤー１と２の勝利数を増やす
							if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() ==
								m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
							{
								m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)]++;
								m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)]++;
							}
						}

						for (int i = 0; i < PLAYER_NUM; i++)
						{
							//プレイヤーの床との当たり判定
							m_pPlayer[i]->HitFloor(m_floorBox);
							//プレイヤーの更新
							m_pPlayer[i]->Update(timer);
						}
						//敵の座標の取得
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetEnemyPos(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos());
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetEnemyPos(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos());
						//プレイヤー同士の体の当たり判定
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHeadCollBox());

						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHeadCollBox());


						//プレイヤー１の体力がゼロになったらリザルトに遷移
						if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() <= 0)
						{
							m_playSceneState = ePLAY_SCENE_STATE::RESULT;
							//プレイヤー２の勝利数を増やす
							m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)]++;
						}
						//プレイヤー２の体力がゼロになったらリザルトに遷移
						if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP() <= 0)
						{
							m_playSceneState = ePLAY_SCENE_STATE::RESULT;
							//プレイヤー１の勝利数を増やす
							m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)]++;
						}

						break;
					}

					case PlayScene::ePLAY_SCENE_STATE::TIME_UP:
					{
						//攻撃のマネージャーの更新
						//AttackManager::GetInstance()->Update();

						m_countdownTimer += static_cast<float>(timer.GetElapsedSeconds());
						if (m_countdownTimer >= PlayScene::TIME_UP_TIME)
						{
							m_playSceneState = ePLAY_SCENE_STATE::RESULT;

							//プレイヤー１の体力がプレイヤー２より多ければプレイヤー１の勝利数を増やす
							if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() >
								m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
							{
								m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)]++;
							}
							//プレイヤー２の体力がプレイヤー１より多ければプレイヤー２の勝利数を増やす
							else if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() <
								m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
							{
								m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)]++;
							}

							m_countdownTimer = 0.0f;
						}
						break;
					}


					//リザルト
					case PlayScene::ePLAY_SCENE_STATE::RESULT:
					{

						for (int i = 0; i < PLAYER_NUM; i++)
						{
							//プレイヤーの床との当たり判定
							m_pPlayer[i]->HitFloor(m_floorBox);

						}
						//プレイヤー１の体力がプレイヤー２より多ければプレイヤー１を動かす
						if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() > m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
						{
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Ready(timer);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetCharaState(eCHARACTER_STATE::LOSE);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Lose(timer);
						}
						//プレイヤー２の体力がプレイヤー１より多ければプレイヤー２を動かす
						else
						{
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Ready(timer);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetCharaState(eCHARACTER_STATE::LOSE);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Lose(timer);
						}

						//プレイヤー同士の体の当たり判定
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHeadCollBox());

						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHeadCollBox());

						m_countdownTimer += static_cast<float>(timer.GetElapsedSeconds());
						if (m_countdownTimer >= PlayScene::ROUND_CHANGE_TIME || GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Space))
						{
							SetSceneState(eSCENE_STATE::FADE_OUT);
						}

						break;
					}
					default:
						break;
				}


				//m_sprite2D->Update(0, 0);//スプライトの更新

				break;
			}
			//フェードアウト
			case eSCENE_STATE::FADE_OUT:
			{
				//リザルトに遷移しなければそのままフェードアウト
				if (m_isResult == false)
				{
					SetFadeTimer(GetFadeTimer() + static_cast<float>(timer.GetElapsedSeconds()) * 2.0f);
				}
				//フェードアウトしたらプレイシーンに戻る
				if (GetFadeTimer() >= 1.0f)
				{
					//どちらかのプレイヤーが２本勝利するまで繰り返す
					if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] < WIN_NUM &&
						m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] < WIN_NUM)
					{
						//ラウンド数を切り替える
						switch (m_nowRound)
						{
							case PlayScene::eROUND::ROUND_1:
							{
								m_nowRound = eROUND::ROUND_2;
								break;

							}
							case PlayScene::eROUND::ROUND_2:
							{
								m_nowRound = eROUND::ROUND_3;
								break;
							}
							default:
								break;
						}
						//リセット
						Reset();
					}
					//どちらかのプレイヤーが２本勝利したらリザルトに遷移
					else
					{
						//SceneManager::GetInstance()->SetScene(eSCENE_ID::RESULT_SCENE);
						m_isResult = true;

						if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] >= WIN_NUM)
						{
							ResultScene::m_winPlayerID = ePLAYER_ID::PLAYER_1;
						}
						else if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] >= WIN_NUM)
						{
							ResultScene::m_winPlayerID = ePLAYER_ID::PLAYER_2;
						}


					}
					//BGM停止
					ADX2::GetInstance().Stop(GetSoundID());

				}
				break;
			}

			default:
				break;
		}
	}

	//メニュー
	Menu();
	//リザルト
	if(m_isResult == true)Result(timer);
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Render()
{
	//天球の描画
	m_space->Render(GetView(), GetProj());

	//床の描画
	if(m_isResult == false)
		m_pFloor->Draw(m_floorWorld, GetView(), GetProj(), DirectX::Colors::Brown);

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//足元の影のエフェクトの描画
		m_pShadowManager[i]->Render(GetView(), GetProj());
	}


	//プレイヤー１の描画
	if (m_isResult == false || m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] < WIN_NUM)
	{
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Render(GetView(), GetProj());
	}
	//プレイヤー２の描画
	if (m_isResult == false || m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] < WIN_NUM)
	{
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Render(GetView(), GetProj());
	}

	//UIの描画
	m_pPlaySceneUI->Render();

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	//メニューの画像の描画
	if (m_isStop == true)
	{
		m_pMenuSprite->Draw(true);
		m_pMenuCursorSprite->Draw(true);

	}

	//操作説明の画像の描画
	if (m_isManualDisplay == true)
	{
		m_pPlaySceneManual->Render();
	}
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Finalize()
{
	//プレイヤーの終了処理
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		if (m_pPlayer[i] != nullptr)
		{
			m_pPlayer[i]->Finalize();
		}
	}

	//リソースの解放
	m_pFloor.reset();
	//m_pFbx.reset();
	m_pDebugCamera.reset();
	m_space.reset();
	m_sprite2D.reset();
	for (int i = 0; i < PLAYER_NUM; i++)
	{

		if (m_pPlayer[i] != nullptr)
		{
			//delete m_pPlayer[i];
			m_pPlayer[i] = nullptr;

		}
		//足元の影のエフェクトの終了処理
		m_pShadowManager[i]->Finalize();

	}



	DebugFont::Dispose();

	//攻撃マネージャーの終了処理
	AttackManager::GetInstance()->Finalize();
	//攻撃マネージャーのインスタンスの解放
	AttackManager::ReleaseInstance();

	//delete m_pFbxModel;
	//m_pFbxModel = nullptr;
	ADX2::GetInstance().Stop(GetSoundID());

	//UIクラスの終了処理
	m_pPlaySceneUI->Finalize();
	m_pPlaySceneUI.reset();

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Reset()
{
	//各プレイヤーのリセット
	for (int i = 0; i < PLAYER_NUM;i++)
	{
		m_pPlayer[i]->Reset();
	}

	//攻撃マネージャーのリセット
	for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
	{
		AttackManager::GetInstance()->Reset(i);
	}
	//フェードイン状態にする
	SetSceneState(eSCENE_STATE::FADE_IN);
	//プレイシーンの状態をカウントダウンにする
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;
	//フェードのタイマーを設定
	SetFadeTimer(1.0f);
	//カウントダウンのタイマーを初期化
	m_countdownTimer = 0.0f;
	//制限時間を最大値に設定
	m_time = PlayScene::TIME_MAX;

	//カメラの注視点の初期化
	m_targetPos = DirectX::SimpleMath::Vector3::Zero;
	SetCameraPos(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f));


	//UIクラスのリセット
	m_pPlaySceneUI->Reset();
}


///////////////////////////
//操作説明
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Manual()
{
//	//操作説明表示中にESCキーでゲームに戻る
//	if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_isManualDisplay == true)
//	{
//		//下降フラグを消す
//		m_isManualDown = false;
//		//上昇フラグを立てる
//		m_isManualUp = true;
//
//		//SE再生
//		ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
//	}
//
//	//左右入力で移動フラグを立てる
//	//左入力
//	if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Left) &&
//		m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
//	{
//		m_isManualLeft = false;
//		m_isManualRight = true;
//
//		//SE再生
//		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);
//
//	}
//	//右入力
//	else if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Right) &&
//		m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x >= 0.0f)
//	{
//		m_isManualRight = false;
//		m_isManualLeft = true;
//
//		//SE再生
//		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);
//
//	}
//	
//	//カーソル更新
//	if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
//	{
//		m_pManualCursorSpriteRight->Update(DirectX::SimpleMath::Vector2(100.0f, 540.0f));
//	}
//
//	//操作説明のカーソルの更新
//	m_pManualCursorSpriteRight->Update(DirectX::SimpleMath::Vector2(1720.0f + cosf(m_totalSeconds * 6.0f)*10.0f, 540.0f));
//	m_pManualCursorSpriteLeft->Update(DirectX::SimpleMath::Vector2(50.0f - cosf(m_totalSeconds * 6.0f) * 10.0f, 540.0f));
//
//
//	//右移動フラグが立ったら画像を右に移動させる
//	if (m_isManualRight == true)
//	{
//		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
//		{
//			m_ManualSpritePos[i].x += MANUAL_SPRITE_WIDTH / 10.0f;
//
//			if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x >= 0.0f)
//			{
//				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x = 0.0f;
//				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = MANUAL_SPRITE_WIDTH;
//				m_isManualRight = false;
//			}
//		}
//	}
//
//	//左移動フラグが立ったら画像を左に移動させる
//	if (m_isManualLeft == true)
//	{
//		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
//		{
//			m_ManualSpritePos[i].x -= MANUAL_SPRITE_WIDTH / 10.0f;
//
//			if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x <= -MANUAL_SPRITE_WIDTH)
//			{
//				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x = -MANUAL_SPRITE_WIDTH;
//				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = 0.0f;
//				m_isManualLeft = false;
//			}
//		}
//
//	}
//
//	//下降フラグが立ったら画像を下降させる
//	if (m_isManualDown == true)
//	{
//		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
//		{
//			m_ManualSpritePos[i].y += MANUAL_SPRITE_HEIGHT / 10.0f;
//
//			if (m_ManualSpritePos[i].y >= 0.0f)
//			{
//				m_ManualSpritePos[i].y = 0.0f;
//				m_isManualDown = false;
//			}
//		}
//	}
//
//	//上昇フラグが立ったら画像を上昇させる
//	if (m_isManualUp == true)
//	{
//		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
//		{
//			m_ManualSpritePos[i].y -= MANUAL_SPRITE_HEIGHT / 10.0f;
//
//			if (m_ManualSpritePos[i].y <= -MANUAL_SPRITE_HEIGHT)
//			{
//				m_ManualSpritePos[i].y = -MANUAL_SPRITE_HEIGHT;
//				m_isManualUp = false;
//				//ゲーム停止フラグを消す
//				m_isManualDisplay = false;
//			}
//		}
//	}
//
//	//画像の更新
//	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
//	{
//		m_pManualSprite[i]->Update(m_ManualSpritePos[i]);
//
//	}
//
}

///////////////////////////
//メニュー
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Menu()
{
	if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_isManualDisplay == false && m_isResult == false)
	{
		//SE再生
		if (m_isStop == false)
			ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);
		else if(m_isStop == true)
			ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);

		m_isStop = !m_isStop;
	}

	//カーソルの座標設定
	switch (static_cast<eMENU_CURSOR>(m_menuCursor))
	{
		//マニュアル
		case PlayScene::eMENU_CURSOR::MANUAL:
		{
			m_pMenuCursorSprite->
				Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::MANUAL)].x - cosf(m_totalSeconds * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::MANUAL)].y);
			break;
		}
		//キャラクターセレクト
		case PlayScene::eMENU_CURSOR::CHARA_SELECT:
		{
			m_pMenuCursorSprite->
				Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::CHARA_SELECT)].x - cosf(m_totalSeconds * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::CHARA_SELECT)].y);
			break;
		}
		//ゲーム終了
		case PlayScene::eMENU_CURSOR::EXIT:
		{
			m_pMenuCursorSprite->
				Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::EXIT)].x - cosf(m_totalSeconds * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::EXIT)].y);
			break;
		}
		default:
			break;
	}

	//キー入力でカーソル移動
	//上入力
	if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Down) && m_isStop == true && m_isManualDisplay == false)
	{
		m_menuCursor++;
		if (m_menuCursor >= static_cast<int>(eMENU_CURSOR::OVER_ID))
		{
			m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);
		}

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	//下入力
	else if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Up) && m_isStop == true && m_isManualDisplay == false)
	{
		m_menuCursor--;
		if (m_menuCursor <= static_cast<int>(eMENU_CURSOR::NONE))
		{
			m_menuCursor = static_cast<int>(eMENU_CURSOR::EXIT);
		}

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);
	}
	
	//スペースキー入力で決定
	if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Space) && m_isStop == true && m_isManualDisplay == false)
	{
		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_SUBMIT);

		switch (static_cast<eMENU_CURSOR>(m_menuCursor))
		{
			//操作説明表示
			case eMENU_CURSOR::MANUAL:
			{
				if (m_isManualDisplay == false)
				{
					m_isManualDisplay = true;
					m_isManualDown = true;
				}
				break;
			}

			//キャラクターセレクトに戻る
			case eMENU_CURSOR::CHARA_SELECT:
			{
				SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
				//SE再生
				//ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
				//ADX2::GetInstance().Stop(GetSoundID());
				break;
			}

			//ゲーム終了
			case eMENU_CURSOR::EXIT:
			{
				PostQuitMessage(0);
				break;
			}

			default:
				break;
		}
	}

	//操作説明の表示フラグが立ったら操作説明の更新
	if (m_isManualDisplay == true)m_pPlaySceneManual->Update();
}

///////////////////////////
//リザルト
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Result(DX::StepTimer const& timer)
{
	if (m_isResult == true)
	{
		SetFadeTimer(0.0f);

		//プレイヤー１勝利
		if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] >
			m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)])
		{
			//プレイヤー２の座標をカメラ外に持っていく
			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetPos(
				DirectX::SimpleMath::Vector3(10.0f, 10.0f, 10.0f));
			//プレイヤー１を勝利状態にする
			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetCharaState(eCHARACTER_STATE::WIN);

			//m_pPlayer[0]->ChangeAnimation(14);
			//カメラの座標設定
			SetCameraPos(DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 1.0f,
				3.0f
			));
			//注視点を変える
			m_targetPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().z);

		}
		//プレイヤー２勝利
		else
		{
			//プレイヤー１の座標をカメラ外に持っていく
			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetPos(
				DirectX::SimpleMath::Vector3(10.0f, 10.0f, 10.0f));

			//プレイヤー２を勝利状態にする
			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetCharaState(eCHARACTER_STATE::WIN);
			//m_pPlayer[0]->ChangeAnimation(14);
			SetCameraPos(DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 1.0f,
				3.0f
			));
			//注視点を変える
			m_targetPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().z);

		}
	}
	//リザルト中にスペースキー入力でキャラクターセレクトに戻る
	if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Space) && m_isResult == true)
	{
		SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
	}
}
