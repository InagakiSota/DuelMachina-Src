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
#include "Menu/PlaySceneMenu.h"
#include "PlaySceneStateManager/PlaySceneStateManager.h"


 //カウントダウンの時間
 const float PlayScene::COUNT_DOWN_TIME = 3.0f;
 //制限時間の最大値
 const float PlayScene::TIME_MAX = 60.0f;
 //タイムアップシーンの制限時間
 const float PlayScene::TIME_UP_TIME = 3.0f;
 //ラウンド切り替え時の時間
 const float PlayScene::ROUND_CHANGE_TIME = 1.0f;

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
	m_isManualDisplay = false;
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
	SetTargetPos(DirectX::SimpleMath::Vector3::Zero);

	//プレイシーンのステートを初期化
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;

	//ラウンド数の初期化
	m_nowRound = eROUND::ROUND_1;

	//制限時間の初期化
	m_time = PlayScene::TIME_MAX;

	//敵のポインタ取得
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetEnemy(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]);
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetEnemy(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]);

	//メニュークラスの読み込み
	m_pPlaySceneMenu = std::make_unique<PlaySceneMenu>();
	//プレイシーンのポインタ取得
	m_pPlaySceneMenu->SetPlayScene(this);
	//メニュークラスの初期化
	m_pPlaySceneMenu->Initialize();
	//プレイシーンのステートマネージャーの初期化
	m_pPlaySceneStateManager = std::make_unique<PlaySceneStateManager>();
	m_pPlaySceneStateManager->Initialize(this);
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void PlayScene::Update(DX::StepTimer const& timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::O))
	{
		CameraShake(timer, 10.0f, 0.1f);
	}

	DoShake(timer, 10.0f, 0.1f);

	//基底クラスの更新関数
	SceneBase::Update(timer);

	//経過時間を加算する
	m_totalSeconds = static_cast<float>(timer.GetTotalSeconds());

	//ビュー行列を設定
	SetView(DirectX::SimpleMath::Matrix::CreateLookAt(GetCameraPos(), 
			GetTargetPos(), DirectX::SimpleMath::Vector3::UnitY));


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

	//停止フラグが立っていなければ更新処理
	if (m_isStop == false)
	{
		//天球の更新
		m_space->Update(m_spaceWorld);

		//プレイシーンのステートマネージャーの更新
		m_pPlaySceneStateManager->Update(timer, GetSceneState());
	}


	//メニュークラスの更新
	m_pPlaySceneMenu->Update();
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

	//メニューの描画
	m_pPlaySceneMenu->Render();

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
	SetTargetPos(DirectX::SimpleMath::Vector3::Zero);
	SetCameraPos(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f));

	//UIクラスのリセット
	m_pPlaySceneUI->Reset();
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
			SetTargetPos(DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().z));

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
			SetTargetPos(DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().z));

		}
	}
	//リザルト中にスペースキー入力でキャラクターセレクトに戻る
	if (GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Space) && m_isResult == true)
	{
		SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
	}
}
