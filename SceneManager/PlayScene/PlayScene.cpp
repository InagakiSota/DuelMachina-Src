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

//体力バーの横幅
const float PlayScene::HP_BAR_WIDTH =800.0f;
//体力バーの高さ
const float PlayScene::HP_BAR_HEIGHT = 101.0f;
//体力バーのマージン
const float PlayScene::HP_BAR_MARGIN = 20.0f;

//ブースト容量バーの横幅
const float PlayScene::BOOST_BAR_WIDTH = 700.0f;
//体力バーの高さ
const float PlayScene::BOOST_BAR_HEIGHT = 70.0f;

//カウントダウンの時間
const float PlayScene::COUNT_DOWN_TIME = 3.0f;
//カウントダウン画像の座標
const DirectX::SimpleMath::Vector2 PlayScene::COUNT_DOWN_SPRITE_POS = DirectX::SimpleMath::Vector2(640.0f,300.0f);
 //制限時間の最大値
 const float PlayScene::TIME_MAX = 60;
 //タイムアップシーンの時間
 const float PlayScene::TIME_UP_TIME = 3.0f;
 //ラウンド切り替えの時間
 const float PlayScene::ROUND_CHANGE_TIME = 2.0f;
 //勝利本数の画像の横幅
 const float PlayScene::WIN_NUM_SPRITE_WIDTH = 100.0f;
 //勝利本数の画像の高さ
 const float  PlayScene::WIN_NUM_SPRITE_HEIGHT = 50.0f;
 //制限時間の画像の横幅
 const float PlayScene::TIME_SPRITE_WIDTH = 100.0f;
 //制限時間の画像の高さ
 const float PlayScene::TIME_SPRITE_HEIGHT = 200.0f;
 //制限時間の画像のX座標
 const float PlayScene::TIME_SPRITE_POS_X = 960.0f;

 //操作説明の画像の横幅
 const float PlayScene::MANUAL_SPRITE_WIDTH = 1920.0f;
 //操作説明の画像の高さ
 const float PlayScene::MANUAL_SPRITE_HEIGHT = 1080.0f;
 //リザルトのpushSpaceの画像の座標
 const DirectX::SimpleMath::Vector2  PlayScene::PUSH_SPACE_RESULT_POS = 
	 DirectX::SimpleMath::Vector2(1300.0f,800.0f);

 //カーソルの座標
 const DirectX::SimpleMath::Vector2 PlayScene::MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::OVER_ID)] =
 {
	DirectX::SimpleMath::Vector2(630.0f,300.0f),
	DirectX::SimpleMath::Vector2(630.0f,500.0f),
	DirectX::SimpleMath::Vector2(630.0f,700.0f)
 };

 //体力バーの座標
 const DirectX::SimpleMath::Vector2 PlayScene::HP_BAR_POS[PlayScene::PLAYER_NUM] =
 {
	 DirectX::SimpleMath::Vector2(20.0f,50.0f),
	 DirectX::SimpleMath::Vector2(1900.0f - HP_BAR_WIDTH,50.0f)
 };

 //体力バーの座標
 const DirectX::SimpleMath::Vector2 PlayScene::BOOST_BAR_POS[PlayScene::PLAYER_NUM] =
 {
	 DirectX::SimpleMath::Vector2(20.0f,900.0f),
	 DirectX::SimpleMath::Vector2(1900.0f - BOOST_BAR_WIDTH,900.0f)
 };


//コンストラクタ
PlayScene::PlayScene()
	:m_floorBox{},
	m_pDebugCamera(nullptr),
	m_pFloor(nullptr),
	m_floorWorld{},
	m_space(nullptr),
	m_spaceWorld{},
	//m_pAttackManager(nullptr),
	m_hpBarPos{},
	m_timeSpriteOneRect{},
	m_timeSpriteTenRect{},
	m_pTimeSpriteOne(nullptr),
	m_pTimeSpriteTen(nullptr),
	m_pShadowManager{}
{
	m_pDeviceResources = nullptr;
	m_pKeyTracker = nullptr;

	m_sceneState = eSCENE_STATE::FADE_IN;
	m_countdownTimer = 0.0f;
	m_cameraPos.Zero;
	m_winSpritePos.Zero;
	m_fadeTimer = 1.0f;

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
		m_pHPBar[i].reset();
		m_pHPBar[i] = nullptr;
		m_pHPBarBack[i].reset();
		m_pHPBarBack[i] = nullptr;
		m_pWinSprite[i].reset();
		m_pWinSprite[i] = nullptr;
		m_pShadowManager[i].reset();
	}

	//攻撃マネージャーのインスタンスの解放
	AttackManager::ReleaseInstance();


	m_pTimeSpriteOne.reset();
	m_pTimeSpriteTen.reset();


	m_pDeviceResources = nullptr;
}

///////////////////////////
//初期化
//引数:デバイスリソースのポインタ
//戻り値:なし
//////////////////////////
void PlayScene::Initialize(HWND window, DX::DeviceResources* pDeviceResources, DirectX::CommonStates* pStates)
{
	m_pDeviceResources = pDeviceResources;
	m_pStates = pStates;

	ID3D11Device1* device = m_pDeviceResources->GetD3DDevice();

	// TODO: Initialize device dependent objects here (independent of window size).
	device;

	auto context = m_pDeviceResources->GetD3DDeviceContext();


	m_spaceWorld = DirectX::SimpleMath::Matrix::Identity;

	//ビュー行列を作成する
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

	//画面サイズを取得する
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//射影行列を作る
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);
	
	//m_pDebugCamera = std::make_unique<DebugCamera>();

	//シーンのステートの初期化
	m_sceneState = eSCENE_STATE::FADE_IN;


	////マウスの作成
	//m_pMouse = std::make_unique<Mouse>();
	//m_pMouse->SetWindow(window);

	////キーボードの作成
	//m_pKeyboard = std::make_unique<Keyboard>();


	//m_pFbx = std::make_unique<Fbx>();
	//m_pFbx->Load(
	//	window,
	//	m_pDeviceResources->GetD3DDevice(),
	//	m_pDeviceResources->GetD3DDeviceContext(),
	//	m_pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/robot_maya9.fbx",
	//	true);

	//攻撃のマネージャーの作成
	//m_pAttackManager = std::make_unique<AttackManager>();
	//攻撃のマネージャーの初期化
	AttackManager::GetInstance()->Initialize(pDeviceResources);

	m_space = std::make_unique<ModelObject>();
	m_space->Create(m_pDeviceResources, L"Resources/Models/Space.cmo");

	//体力バー、ブースト容量バーの読み込み
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//体力バー
		m_pHPBar[i] = std::make_unique<Sprite2D>();
		m_pHPBar[i]->Create(device, context, L"Resources/Textures/HPBar.png", m_pStates);

		//体力バー(体力低)
		m_pHPBarDanger[i] = std::make_unique<Sprite2D>();
		m_pHPBarDanger[i]->Create(device, context, L"Resources/Textures/HPBar_Red.jpg", m_pStates);

		//体力バーの背景
		m_pHPBarBack[i] = std::make_unique<Sprite2D>();
		m_pHPBarBack[i]->Create(device, context, L"Resources/Textures/HPBar_Gray.jpg", m_pStates);

		//ブースト容量バー
		m_pBoostBar[i] = std::make_unique<Sprite2D>();
		m_pBoostBar[i]->Create(device, context, L"Resources/Textures/BoostBar.jpg", m_pStates);

		//ブースト容量バー(オーバーヒート)
		m_pBoostOverHeatBar[i] = std::make_unique<Sprite2D>();
		m_pBoostOverHeatBar[i]->Create(device, context, L"Resources/Textures/BoostBar_OverHeat.jpg", m_pStates);

		//ブースト容量バーの背景
		m_pBoostBack[i] = std::make_unique<Sprite2D>();
		m_pBoostBack[i]->Create(device, context, L"Resources/Textures/BoostBar_Gray.jpg", m_pStates);

		//頭上画像
		m_pOverHeadSprite[i] = std::make_unique<Sprite2D>();

		m_overHeadSpritePos[i] = DirectX::SimpleMath::Vector2::Zero;

		//勝利本数の画像
		for(int j = 0; j < WIN_NUM;j++)
		{
			m_pWinNumSprtie[i][j] = std::make_unique<Sprite2D>();
			m_pWinNumSprtie[i][j]->Create(device, context, L"Resources/Textures/winNum.png", m_pStates);

			//切り取り位置の設定
			m_winNumSpriteRect[i][j].top = 0;
			m_winNumSpriteRect[i][j].bottom = static_cast<LONG>(WIN_NUM_SPRITE_HEIGHT);
			m_winNumSpriteRect[i][j].left = 0;
			m_winNumSpriteRect[i][j].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH* 0.5);
		}

		//足元の影のエフェクト
		m_pShadowManager[i] = std::make_unique<ShadowManager>();
		m_pShadowManager[i]->Initialize(m_pDeviceResources, 1, 10, DirectX::SimpleMath::Vector3::Zero);

	}
	//頭上画像の読み込み
	m_pOverHeadSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Create(device, context, L"Resources/Textures/p1Icon.png", m_pStates);
	m_pOverHeadSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Create(device, context, L"Resources/Textures/p2Icon.png", m_pStates);


	//体力バーの切り取り位置の初期化
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].top = 0;
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].bottom = static_cast<LONG>(HP_BAR_HEIGHT);
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = 0;
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].right = static_cast<LONG>(HP_BAR_WIDTH);

	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].top = 0;
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].bottom = static_cast<LONG>(HP_BAR_HEIGHT);
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].left = 0;
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(HP_BAR_WIDTH);

	//ブースト容量バーの切り取り位置の初期化
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].top = 0;
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].bottom = static_cast<LONG>(BOOST_BAR_HEIGHT);
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = 0;
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].right = static_cast<LONG>(BOOST_BAR_WIDTH);

	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].top = 0;
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].bottom = static_cast<LONG>(BOOST_BAR_HEIGHT);
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].left = 0;
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(BOOST_BAR_WIDTH);


	//体力バー、ブースト容量バーの初期座標設定
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_hpBarPos[i] = HP_BAR_POS[i];
		m_boostBarPos[i] = BOOST_BAR_POS[i];
	}

	//勝利本数の画像の初期座標設定
	for (int i = 0; i < WIN_NUM; i++)
	{
		//プレイヤー1
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)][i].y = 170.0f;
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)][i].x = 70.0f + ((WIN_NUM_SPRITE_WIDTH * 0.5f + 10) * i);
		//プレイヤー2
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)][i].y = 170.0f;
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)][i].x = 1920.0f - ((WIN_NUM_SPRITE_WIDTH * 0.5f + 10) * (i + 1)) - 70.0f;
	}

	DebugFont::GetInstance()->Create(device, context);

	m_isResult = false;

	//床の作成
	m_floorBox.pos = DirectX::SimpleMath::Vector3(0.0f, -2.0f, 0.0f);
	m_floorBox.size_h = DirectX::SimpleMath::Vector3(5.0f, 0.25f, 1.0f);
	m_pFloor = DirectX::GeometricPrimitive::CreateBox(pDeviceResources->GetD3DDeviceContext(), m_floorBox.size_h*2);
	m_floorWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_floorBox.pos);

	//プレイヤーの作成
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)] = CharacterFactory::CreatePlayer1();
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)] = CharacterFactory::CreatePlayer2();

	//初期座標の設定
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetStartPos(DirectX::SimpleMath::Vector3(-2.0f, 1.0f, 0.0f));
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetStartPos(DirectX::SimpleMath::Vector3(2.0f, 1.0f, 0.0f));

	//勝利画像の読み込み
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pWinSprite[i] = std::make_unique<Sprite2D>();
	}
	m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Create(device, context, L"Resources/Textures/p1win.png", m_pStates);
	m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Create(device, context, L"Resources/Textures/p2win.png", m_pStates);

	//ラウンド数の画像の読み込み
	for (int i = 0; i < static_cast<int>(eROUND::ROUND_NUM); i++)
	{
		m_pRoundSprite[i] = std::make_unique<Sprite2D>();
	}
	m_pRoundSprite[static_cast<int>(eROUND::ROUND_1)]->Create(device, context, L"Resources/Textures/Round1.png", m_pStates);
	m_pRoundSprite[static_cast<int>(eROUND::ROUND_2)]->Create(device, context, L"Resources/Textures/Round2.png", m_pStates);
	m_pRoundSprite[static_cast<int>(eROUND::ROUND_3)]->Create(device, context, L"Resources/Textures/Round3.png", m_pStates);

	//Fight画像の読み込み
	m_pFightSprite = std::make_unique<Sprite2D>();
	m_pFightSprite->Create(device, context, L"Resources/Textures/Fight.png", m_pStates);

	//タイムアップ画像の読み込み
	m_pTimeUpSprite = std::make_unique<Sprite2D>();
	m_pTimeUpSprite->Create(device, context, L"Resources/Textures/TimeUp.png", m_pStates);

	//引き分け画像の読み込み
	m_pDrawSprite = std::make_unique<Sprite2D>();
	m_pDrawSprite->Create(device, context, L"Resources/Textures/Draw.png", m_pStates);

	//制限時間の画像の読み込み
	m_pTimeSpriteOne = std::make_unique<Sprite2D>();
	m_pTimeSpriteOne->Create(device, context, L"Resources/Textures/Number_mini.png", m_pStates);
	m_pTimeSpriteTen= std::make_unique<Sprite2D>();
	m_pTimeSpriteTen->Create(device, context, L"Resources/Textures/Number_mini.png", m_pStates);

	//メニューの画像読み込み
	m_pMenuSprite = std::make_unique<Sprite2D>();
	m_pMenuSprite->Create(device, context, L"Resources/Textures/Menu.png", m_pStates);
	m_pMenuCursorSprite = std::make_unique<Sprite2D>();
	m_pMenuCursorSprite->Create(device, context, L"Resources/Textures/menuCursol.png", m_pStates);

	//pushSpaceの画像読み込み
	m_pPushSpaceResult = std::make_unique<Sprite2D>();
	m_pPushSpaceResult->Create(device, context, L"Resources/Textures/pushSpace_result.png", m_pStates);


	//プレイヤーの初期化
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pPlayer[i]->Initialize(pDeviceResources,window);
		m_pPlayer[i]->SetAttackManager(AttackManager::GetInstance());
		//プレイヤーの勝利本数の初期化
		m_playerWinNum[i] = 0;

	}
	//キートラッカーの作成
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//カメラの注視点の初期化
	m_targetPos = DirectX::SimpleMath::Vector3::Zero;

	//プレイシーンのステートを初期化
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;

	//勝利画像の座標設定
	m_winSpritePos = DirectX::SimpleMath::Vector2(640.0f, 300.0f);

	////FBXモデルの読み込み
	//m_pFbxModel = new FbxModel();
	//m_pFbxModel->Load(
	//	window,
	//	pDeviceResources->GetD3DDevice(),
	//	pDeviceResources->GetD3DDeviceContext(),
	//	pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/robot_maya10.fbx",
	//	true
	//);

	m_fbxModelWorld = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(0, -0.8f, 0);
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(0.08f, 0.08f, 0.08f);

	m_fbxModelWorld = scale * trans;

	//ラウンド数の初期化
	m_nowRound = eROUND::ROUND_1;

	//制限時間の初期化
	m_time = PlayScene::TIME_MAX;

	m_fadeTimer = 1.0f;

	//敵のポインタ取得
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetEnemy(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]);
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetEnemy(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]);


	//操作説明の画像読み込み、初期座標設定
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i] = std::make_unique<Sprite2D>();

		m_ManualSpritePos[i].y = -MANUAL_SPRITE_HEIGHT;

	}
	m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = MANUAL_SPRITE_WIDTH;

	m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)]
		->Create(device, context, L"Resources/Textures/Manual_1.png", m_pStates);

	//操作説明のカーソル画像の読み込み
	m_pManualCursorSpriteRight = std::make_unique<Sprite2D>();
	m_pManualCursorSpriteRight->Create(device, context, L"Resources/Textures/ManualCursolr_Right.png", m_pStates);
	m_pManualCursorSpriteLeft = std::make_unique<Sprite2D>();
	m_pManualCursorSpriteLeft->Create(device, context, L"Resources/Textures/ManualCursolr_Left.png", m_pStates);

	switch (CharacterFactory::m_player1Chara)
	{
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(device, context, L"Resources/Textures/Manual_2_chara1.png", m_pStates);
			break;
		}

		case eCHARACTER_ID::CHARACTER_2:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(device, context, L"Resources/Textures/Manual_2_chara2.png", m_pStates);
			break;
		}

		case eCHARACTER_ID::CHARACTER_3:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(device, context, L"Resources/Textures/Manual_2_chara3.png", m_pStates);
			break;
		}

		default:
			break;
	}


}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Update(DX::StepTimer const& timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	m_totalSeconds = static_cast<float>(timer.GetTotalSeconds());

	//m_pDebugCamera->update();

	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, m_targetPos, DirectX::SimpleMath::Vector3::UnitY);
	//m_view = m_pDebugCamera->getViewMatrix();

	float time = float(timer.GetTotalSeconds());


	//m_pHitEffectManager->SetRenderState(m_view, m_proj);

	//UIの画像の座標設定
	for (int i = 0; i < static_cast<int>(eROUND::ROUND_NUM); i++)
	{
		m_pRoundSprite[i]->Update(COUNT_DOWN_SPRITE_POS);
	}
	m_pFightSprite->Update(COUNT_DOWN_SPRITE_POS);
	m_pTimeUpSprite->Update(COUNT_DOWN_SPRITE_POS);
	m_pDrawSprite->Update(COUNT_DOWN_SPRITE_POS);

	//体力のバッファの同期
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_playerHpBuffer[i] = m_pPlayer[i]->GetHP();
	}

	//体力バーの切り取り位置の更新
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = static_cast<LONG>(
		(HP_BAR_WIDTH / m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetMaxHP()) *
		(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetMaxHP() - m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP()));
	m_hpBarPos[static_cast<int>(ePLAYER_ID::PLAYER_1)].x = m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left + HP_BAR_MARGIN;


	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(HP_BAR_WIDTH -
		((HP_BAR_WIDTH / m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetMaxHP()) *
		(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetMaxHP() - m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())));

	//ブースト容量バーの切り取り位置の更新
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = static_cast<LONG>(
		(BOOST_BAR_WIDTH / m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetMaxBoostCap()) *
		(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetMaxBoostCap() - m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetBoostCap()));
	m_boostBarPos[static_cast<int>(ePLAYER_ID::PLAYER_1)].x = m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left + HP_BAR_MARGIN;

	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(BOOST_BAR_WIDTH -
		((BOOST_BAR_WIDTH / m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetMaxBoostCap()) *
		(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetMaxBoostCap() - m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetBoostCap())));

	//勝利本数の画像の切り取り位置の更新
	//プレイヤー１
	for (int i = 0; i < m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)]; i++)
	{
		if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] > 0)
		{
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_1)][m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] - 1].left = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH * 0.5);
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_1)][m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] - 1].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH);
		}
	}
	//プレイヤー2
	for (int i = 0; i < m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)]; i++)
	{
		if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] > 0)
		{
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_2)][m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] - 1].left = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH * 0.5);
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_2)][m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] - 1].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH);
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
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 0.5f)*-1) * 260.0f;
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 1.5f)*-1) * 260.0f;
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 2.0f)*-1) * 260.0f;
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
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 0.5f)*-1) * 260.0f;
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 1.5f)*-1) * 260.0f;
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 2.0f)*-1) * 260.0f;
			break;
		}

		default:
			break;
	}


	//体力バーの更新	
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pHPBar[i]->Update(m_hpBarPos[i], m_hpBarRect[i]);
		m_pHPBarBack[i]->Update(HP_BAR_POS[i].x - 10.0f,HP_BAR_POS[i].y - 10.0f);

		m_pHPBarDanger[i]->Update(m_hpBarPos[i], m_hpBarRect[i]);
		m_pHPBarBack[i]->Update(HP_BAR_POS[i].x - 10.0f, HP_BAR_POS[i].y - 10.0f);

		m_pBoostBar[i]->Update(m_boostBarPos[i], m_boostBarRect[i]);
		m_pBoostBack[i]->Update(BOOST_BAR_POS[i].x - 10.0f, BOOST_BAR_POS[i].y - 10.0f);

		//頭上の画像の更新
		m_overHeadSpritePos[i].x = 960.0f - 50.0f + m_pPlayer[i]->GetPos().x * 230.0f;
		m_pOverHeadSprite[i]->Update(m_overHeadSpritePos[i]);

		//勝利本数の画像の更新
		for (int j = 0; j < WIN_NUM; j++)
		{
			m_pWinNumSprtie[i][j]->Update(m_winNumSpritePos[i][j], m_winNumSpriteRect[i][j]);
		}

		//足元の影のエフェクトの更新
		m_pShadowManager[i]->Update(timer, m_pPlayer[i]->GetPos());

	}

	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(0.001);
	m_spaceWorld *= rotY;

	//制限時間の画像の切り取り位置の設定
	m_timeSpriteOneRect.top = 0;
	m_timeSpriteOneRect.bottom = static_cast<LONG>(PlayScene::TIME_SPRITE_HEIGHT);
	m_timeSpriteOneRect.left = static_cast<LONG>(PlayScene::TIME_SPRITE_WIDTH * (static_cast<int>(m_time) % 10));
	m_timeSpriteOneRect.right = m_timeSpriteOneRect.left + static_cast<LONG>(PlayScene::TIME_SPRITE_WIDTH);

	m_timeSpriteTenRect.top = 0;
	m_timeSpriteTenRect.left = static_cast<LONG>(PlayScene::TIME_SPRITE_WIDTH * (static_cast<int>(m_time) / 10));
	m_timeSpriteTenRect.bottom = static_cast<LONG>(PlayScene::TIME_SPRITE_HEIGHT);
	m_timeSpriteTenRect.right = m_timeSpriteTenRect.left + static_cast<LONG>(PlayScene::TIME_SPRITE_WIDTH);

	m_pTimeSpriteOne->Update(PlayScene::TIME_SPRITE_POS_X, 0.0f, m_timeSpriteOneRect);
	m_pTimeSpriteTen->Update(PlayScene::TIME_SPRITE_POS_X - PlayScene::TIME_SPRITE_WIDTH, 0.0f, m_timeSpriteTenRect);


	//シーンのステート
	if (m_isStop == false)
	{
		//天球の更新
		m_space->Update(m_spaceWorld);

		switch (m_sceneState)
		{
			//フェードイン
			case SceneBase::eSCENE_STATE::FADE_IN:
			{
				m_fadeTimer -= static_cast<float>(timer.GetElapsedSeconds()) * 2.0f;

				if (m_fadeTimer <= 0.0f)
				{	
					//BGMの再生
					soundID = ADX2::GetInstance().Play(CRI_CUESHEET_0_PLAYSCENE_BGM);

					//LoadDisplay::GetInstance()->Update(false);
					m_sceneState = eSCENE_STATE::MAIN;
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
			case SceneBase::eSCENE_STATE::MAIN:
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

							
							//m_pPlayer[i]->Ready(timer);

							if (m_pWinSprite[i] != nullptr)
								m_pWinSprite[i]->Update(m_winSpritePos);
						}
						if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() > m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
						{
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Ready(timer);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetCharaState(eCHARACTER_STATE::LOSE);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Lose(timer);
						}
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
						if (m_countdownTimer >= PlayScene::ROUND_CHANGE_TIME || keyState.IsKeyDown(DirectX::Keyboard::Keys::Space))
						{
							m_sceneState = eSCENE_STATE::FADE_OUT;
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
			case SceneBase::eSCENE_STATE::FADE_OUT:
			{
				if(m_isResult == false)m_fadeTimer += static_cast<float>(timer.GetElapsedSeconds())* 2.0f;
				if (m_fadeTimer >= 1.0f)
				{
					//SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
					//m_sceneState = eSCENE_STATE::FADE_IN;
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
					ADX2::GetInstance().Stop(soundID);

				}
				break;
			}
			default:
				break;
		}
	}

	////ESCキー入力でタイトルに戻る
	//if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape))
	//{
	//	SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
	//	//SE再生
	//	ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
	//	ADX2::GetInstance().Stop(soundID);


	//}

	//メニュー
	Menu();
	//リザルト
	if(m_isResult == true)Result(timer);

		

	//フェードマネージャーの更新
	FadeManager::GetInstance()->Update(timer, m_fadeTimer);
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Render()
{
	//天球の描画
	m_space->Render(m_view, m_proj);
	//m_sprite2D->Draw();//2dスプライトの描画

	////wchar_t string[25] = {};

	////if (m_pPlayer[1] != nullptr)
	////{
	////	//体力を文字列に変換
	////	_itow(m_pPlayer[1]->GetHP(), string, 10);
	////}
	////デバッグ文字の更新
	//DebugFont::GetInstance()->Print(0, 0, string, DirectX::Colors::Aqua);
	//DebugFont::GetInstance()->Print(0, 0, string, DirectX::Colors::Aqua);


	//DebugFont::GetInstance()->Draw();//デバッグ文字の描画



	//床の描画
	if(m_isResult == false)
		m_pFloor->Draw(m_floorWorld, m_view, m_proj, DirectX::Colors::Brown);

	////モデルの描画
	//m_pFbxModel->Draw(
	//	m_fbxModelWorld,
	//	m_view,
	//	m_proj
	//);
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//足元の影のエフェクトの描画
		m_pShadowManager[i]->Render(m_view,m_proj);
	}

	//プレイヤーの描画
	//for (int i = 0; i < PLAYER_NUM; i++)
	//{
	//	m_pPlayer[i]->Render(m_view, m_proj);

	//}

	//プレイヤー１の描画
	if (m_isResult == false || m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] < WIN_NUM)
	{
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Render(m_view, m_proj);
	}
	//プレイヤー２の描画
	if (m_isResult == false || m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] < WIN_NUM)
	{
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Render(m_view, m_proj);
	}

	//プレイヤー１の勝利画像
	if (m_playSceneState == ePLAY_SCENE_STATE::RESULT && 
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() >
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
	{
		m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Draw(true);
	}

	//プレイヤー2の勝利画像
	else if (m_playSceneState == ePLAY_SCENE_STATE::RESULT && 
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP() >
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP())
	{
		m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Draw(true);
	}

	//引き分け画像
	else if (m_playSceneState == ePLAY_SCENE_STATE::RESULT &&
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() == m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
	{
	}

	if (m_isResult == false)
	{
		//体力バー、ブースト容量バーの描画
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			//背景
			m_pHPBarBack[i]->Draw();
			m_pBoostBack[i]->Draw();

			//本体
			if (m_pPlayer[i]->GetHP() > m_pPlayer[i]->GetMaxHP() * 0.3f)
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
		if (m_playSceneState == ePLAY_SCENE_STATE::COUNT_DOWN)
		{
			if (m_countdownTimer <= COUNT_DOWN_TIME / 2)
			{
				//現在のラウンド数によって表示する画像を切り替える
				switch (m_nowRound)
				{
					case PlayScene::eROUND::ROUND_1:
					{
						m_pRoundSprite[static_cast<int>(eROUND::ROUND_1)]->Draw(true);

						break;
					}
					case PlayScene::eROUND::ROUND_2:
					{
						m_pRoundSprite[static_cast<int>(eROUND::ROUND_2)]->Draw(true);

						break;
					}

					case PlayScene::eROUND::ROUND_3:
					{
						m_pRoundSprite[static_cast<int>(eROUND::ROUND_3)]->Draw(true);
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
				m_pWinSprite[i]->Update(m_winSpritePos.x + 600.0f,m_winSpritePos.y - 300.0f);
		}

		m_pPushSpaceResult->Update(PUSH_SPACE_RESULT_POS);
		m_pPushSpaceResult->Draw(true);
	}

	//タイムアップの画像
	if (m_playSceneState == ePLAY_SCENE_STATE::TIME_UP)
	{
		m_pTimeUpSprite->Draw(true);
	}

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
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_pManualSprite[i]->Draw(true);

		}
	}
	//操作説明のカーソル画像の描画
	if (m_isManualDisplay == true && m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
	{
		m_pManualCursorSpriteLeft->Draw(true);
	}
	else if (m_isManualDisplay == true && m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x >= 0.0f)
	{
		m_pManualCursorSpriteRight->Draw(true);
	}

	//攻撃の描画
	//AttackManager::GetInstance()->Render(m_view, m_proj);

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
	m_pPushSpaceResult->Reset();
	//
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i]->Reset();

	}

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

		if (m_pPlayer[i] != nullptr)
		{
			delete m_pPlayer[i];
			m_pPlayer[i] = nullptr;

		}

		m_pOverHeadSprite[i]->Reset();
		m_pOverHeadSprite[i].reset();
		//足元の影のエフェクトの終了処理
		m_pShadowManager[i]->Finalize();

	}

	for (int i = 0; i < static_cast<int>(eROUND::ROUND_NUM); i++)
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

	m_pManualCursorSpriteLeft->Reset();
	m_pManualCursorSpriteLeft.reset();

	m_pManualCursorSpriteRight->Reset();
	m_pManualCursorSpriteRight.reset();


	DebugFont::Dispose();

	//攻撃マネージャーの終了処理
	AttackManager::GetInstance()->Finalize();
	//攻撃マネージャーのインスタンスの解放
	AttackManager::ReleaseInstance();

	//delete m_pFbxModel;
	//m_pFbxModel = nullptr;
	ADX2::GetInstance().Stop(soundID);

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Reset()
{
	//プレイヤーの座標等のリセット
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pPlayer[i]->Reset();

		m_hpBarPos[i] = HP_BAR_POS[i];

		m_boostBarPos[i] = BOOST_BAR_POS[i];
	}

	//攻撃マネージャーのリセット
	for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
	{
		AttackManager::GetInstance()->Reset(i);
	}
	m_sceneState = eSCENE_STATE::FADE_IN;
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;

	m_fadeTimer = 1.0f;
	m_countdownTimer = 0.0f;

	m_time = PlayScene::TIME_MAX;

	//カメラの注視点の初期化
	m_targetPos = DirectX::SimpleMath::Vector3::Zero;
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);

	//BGMの再生
	//soundID = ADX2::GetInstance().Play(CRI_CUESHEET_0_PLAYSCENE_BGM);

}


///////////////////////////
//操作説明
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Manual()
{
	//操作説明表示中にESCキーでゲームに戻る
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_isManualDisplay == true)
	{
		//下降フラグを消す
		m_isManualDown = false;
		//上昇フラグを立てる
		m_isManualUp = true;

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);

	}
	//
	////ESCキーで操作説明表示
	//if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_isManualDisplay == false)
	//{
	//	m_isManualDisplay = true;
	//	//下降フラグを立てる
	//	m_isManualDown = true;
	//}


	////操作説明表示中にスペースキー入力でキャラクターセレクトに戻る
	//if (m_isManualDisplay == true)
	//{
	//	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	//	{
	//		SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
	//		//SE再生
	//		ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
	//		ADX2::GetInstance().Stop(soundID);
	//	}
	//}

	//左右入力で移動フラグを立てる
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left) && 
		m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
	{
		m_isManualLeft = false;
		m_isManualRight = true;

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	else if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right) &&
		m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x >= 0.0f)
	{
		m_isManualRight = false;
		m_isManualLeft = true;

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	
	//カーソル更新
	if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
	{
		m_pManualCursorSpriteRight->Update(DirectX::SimpleMath::Vector2(100.0f, 540.0f));
	}

	//操作説明のカーソルの更新
	m_pManualCursorSpriteRight->Update(DirectX::SimpleMath::Vector2(1720.0f + cosf(m_totalSeconds * 6.0f)*10.0f, 540.0f));
	m_pManualCursorSpriteLeft->Update(DirectX::SimpleMath::Vector2(50.0f - cosf(m_totalSeconds * 6.0f) * 10.0f, 540.0f));


	//右移動フラグが立ったら画像を右に移動させる
	if (m_isManualRight == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].x += MANUAL_SPRITE_WIDTH / 10.0f;

			if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x >= 0.0f)
			{
				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x = 0.0f;
				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = MANUAL_SPRITE_WIDTH;
				m_isManualRight = false;
			}
		}
	}

	//左移動フラグが立ったら画像を左に移動させる
	if (m_isManualLeft == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].x -= MANUAL_SPRITE_WIDTH / 10.0f;

			if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x <= -MANUAL_SPRITE_WIDTH)
			{
				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x = -MANUAL_SPRITE_WIDTH;
				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = 0.0f;
				m_isManualLeft = false;
			}
		}

	}

	//下降フラグが立ったら画像を下降させる
	if (m_isManualDown == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].y += MANUAL_SPRITE_HEIGHT / 10.0f;

			if (m_ManualSpritePos[i].y >= 0.0f)
			{
				m_ManualSpritePos[i].y = 0.0f;
				m_isManualDown = false;
			}
		}
	}

	//上昇フラグが立ったら画像を上昇させる
	if (m_isManualUp == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].y -= MANUAL_SPRITE_HEIGHT / 10.0f;

			if (m_ManualSpritePos[i].y <= -MANUAL_SPRITE_HEIGHT)
			{
				m_ManualSpritePos[i].y = -MANUAL_SPRITE_HEIGHT;
				m_isManualUp = false;
				//ゲーム停止フラグを消す
				m_isManualDisplay = false;
			}
		}
	}

	//画像の更新
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i]->Update(m_ManualSpritePos[i]);

	}

}

///////////////////////////
//メニュー
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Menu()
{
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_isManualDisplay == false && m_isResult == false)
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
		case PlayScene::eMENU_CURSOR::MANUAL:
		{
			m_pMenuCursorSprite->
				Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::MANUAL)].x - cosf(m_totalSeconds * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::MANUAL)].y);
			break;
		}
		case PlayScene::eMENU_CURSOR::CHARA_SELECT:
		{
			m_pMenuCursorSprite->
				Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::CHARA_SELECT)].x - cosf(m_totalSeconds * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::CHARA_SELECT)].y);
			break;
		}
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
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Down) && m_isStop == true && m_isManualDisplay == false)
	{
		m_menuCursor++;
		if (m_menuCursor >= static_cast<int>(eMENU_CURSOR::OVER_ID))
		{
			m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);
		}

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	else if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up) && m_isStop == true && m_isManualDisplay == false)
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
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space) && m_isStop == true && m_isManualDisplay == false)
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
				ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
				ADX2::GetInstance().Stop(soundID);
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
	if (m_isManualDisplay == true)Manual();
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
		m_fadeTimer = 0.0f;

		if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] >
			m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)])
		{
			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetPos(
				DirectX::SimpleMath::Vector3(10.0f, 10.0f, 10.0f));

			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetCharaState(eCHARACTER_STATE::WIN);

			//m_pPlayer[0]->ChangeAnimation(14);
			m_cameraPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 1.0f,
				3.0f
			);

			m_targetPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().z);

		}
		else
		{
			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetPos(
				DirectX::SimpleMath::Vector3(10.0f, 10.0f, 10.0f));


			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetCharaState(eCHARACTER_STATE::WIN);
			//m_pPlayer[0]->ChangeAnimation(14);
			m_cameraPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 1.0f,
				3.0f
			);

			m_targetPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().z);

		}
	}
	//リザルト中にスペースキー入力でキャラクターセレクトに戻る
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space) && m_isResult == true)
	{
		SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
	}
}
