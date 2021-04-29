#include "pch.h"
#include "CharaSelectScene.h"
#include "Src\FrameWork\DebugFont\DebugFont.h"
#include "Src\SceneManager\SceneManager.h"
#include "Keyboard.h"

#include "../PlayScene/Character/CharacterID.h"
#include "../PlayScene/Character/CharacterFactory.h"

#include "Src\FrameWork\FadeManager\FadeManager.h"

#include "Src\FrameWork\Sprite2D\Sprite2D.h"

#include "Adx2.h"
#include "CueSheet_0.h"

#include "../../FrameWork/LoadDisplay/LoadDisplay.h"

#include "../../FrameWork/FbxResourceManager/FbxResourceManager.h"
#include "../PlayScene/Character/ChracterAnimationNumber.h"

const DirectX::SimpleMath::Vector2 CharaSelectScene::CHARA1_ICON_POS = DirectX::SimpleMath::Vector2(250.0f, 260.0f);
const DirectX::SimpleMath::Vector2 CharaSelectScene::CHARA2_ICON_POS = DirectX::SimpleMath::Vector2(810.0f, 260.0f);
const DirectX::SimpleMath::Vector2 CharaSelectScene::CHARA3_ICON_POS = DirectX::SimpleMath::Vector2(1370.0f, 260.0f);
const float CharaSelectScene::CURSOL_POS = 10.0f;
//キャラクターの情報画像の座標
const DirectX::SimpleMath::Vector2 CharaSelectScene::CHARA_INFO_POS = DirectX::SimpleMath::Vector2(0,580.0f);
//操作説明の画像の横幅
const float CharaSelectScene::MANUAL_SPRITE_WIDTH = 1920.0f;
//操作説明の画像の高さ
const float CharaSelectScene::MANUAL_SPRITE_HEIGHT = 1080.0f;
//各キャラクターのモデルのサイズ
const float CharaSelectScene::MODEL_SIZE[CharaSelectScene::CHARA_NUM] =
{
	0.003f,0.015f,0.015f
};

//モデルの座標
const DirectX::SimpleMath::Vector3 CharaSelectScene::MODEL_POS = DirectX::SimpleMath::Vector3(-3.7f, -1.0f, 0.0f);
//モデルの色						 
const DirectX::SimpleMath::Vector4 CharaSelectScene::MODEL_COLOR = DirectX::SimpleMath::Vector4(0.3f, 0.3f, 0.3f, 1.0f);

//コンストラクタ
CharaSelectScene::CharaSelectScene()
{
	m_pDeviceResources = nullptr;
	m_pKeyTracker = nullptr;
	m_pSelectSprite = nullptr;
	m_sceneState = eSCENE_STATE::FADE_IN;
	m_isStop = false;
	m_isManualDown = m_isManualUp = false;
	m_modelAngleY = 0.0f;
}

//デストラクタ
CharaSelectScene::~CharaSelectScene()
{
	m_pSelectSprite.reset();

	for (int i = 0; i < static_cast<int>(eCHARACTER_ID::OVER_ID); i++)
	{
		m_pCharacterSprite[i].reset();
	}

	m_pCursolSprite.reset();

}

///////////////////////////
//初期化
//引数:ウィンドウ、デバイス
//戻り値:なし
//////////////////////////
void CharaSelectScene::Initialize(HWND window, DX::DeviceResources * pDeviceResources, DirectX::CommonStates* pStates)
{
	m_pDeviceResources = pDeviceResources;
	m_pStates = pStates;

	auto device = m_pDeviceResources->GetD3DDevice();
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	//ビュー行列を作成する
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

	//画面サイズを取得する
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//射影行列を作る
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);

	//各キャラクターのモデルの読み込み
	m_pCharacter1Model = FbxResourceManager::GetInstance()->GetModel(eCHARACTER_ID::CHARACTER_1);
	m_pCharacter2Model = FbxResourceManager::GetInstance()->GetModel(eCHARACTER_ID::CHARACTER_2);
	m_pCharacter3Model = FbxResourceManager::GetInstance()->GetModel(eCHARACTER_ID::CHARACTER_3);

	m_pCharacter1Model->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::WAIT));
	m_pCharacter2Model->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::WAIT));
	m_pCharacter3Model->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::WAIT));
		
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_modelAngleY);
	//
	for (int i = 0; i < CHARA_NUM; i++)
	{
		//m_modelWorld[i] *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero);
		//m_modelWorld[i] *= rotY;// *DirectX::SimpleMath::Matrix::CreateTranslation(MODEL_POS);
		//m_modelWorld[i] *= DirectX::SimpleMath::Matrix::CreateTranslation(MODEL_POS);
		m_modelWorld[i] = DirectX::SimpleMath::Matrix::CreateScale(MODEL_SIZE[i]) *  DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero) *
			rotY * DirectX::SimpleMath::Matrix::CreateTranslation(MODEL_POS) * rotY;
	}

	DebugFont::GetInstance()->Create(device, context);

	//キートラッカーの読み込み
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//背景画像の読み込み
	m_pSelectSprite = std::make_unique<Sprite2D>();
	m_pSelectSprite->Create(device, context, L"Resources/Textures/CharacterSelect2.png", m_pStates);

	//キャラクターのアイコンの読み込み
	for (int i = 0; i < static_cast<int>(eCHARACTER_ID::OVER_ID); i++)
	{
		m_pCharacterSprite[i] = std::make_unique<Sprite2D>();
		m_pCharaInfoSprite[i] = std::make_unique<Sprite2D>();
	}
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_1)]->Create(
		device, context, L"Resources/Textures/character1_icon.png", m_pStates);
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_2)]->Create(
		device, context, L"Resources/Textures/character2_icon.png", m_pStates);
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_3)]->Create(
		device, context, L"Resources/Textures/character3_icon.png", m_pStates);
	
	m_pCharaInfoSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_1)]->Create(
		device, context, L"Resources/Textures/charaInfo_1.png", m_pStates);
	m_pCharaInfoSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_2)]->Create(
		device, context, L"Resources/Textures/charaInfo_2.png", m_pStates);
	m_pCharaInfoSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_3)]->Create(
		device, context, L"Resources/Textures/charaInfo_3.png", m_pStates);

	//操作説明の画像の読み込み
	m_pManualSprite = std::make_unique<Sprite2D>();
	m_pManualSprite->Create(device, context, L"Resources/Textures/Manual_1_charaSelect.png", m_pStates);
	//操作説明の画像の初期座標設定
	m_ManualSpritePos = DirectX::SimpleMath::Vector2(0, -MANUAL_SPRITE_HEIGHT);


	//キャラクターの情報画像の座標設定
	for (int i = 0; i < static_cast<int>(eCHARACTER_ID::OVER_ID); i++)
	{
		m_pCharaInfoSprite[i]->Update(CHARA_INFO_POS);
	}

	//キャラクターのアイコンの初期座標設定
	//キャラクターのアイコンの座標設定
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_1)]->Update(CHARA1_ICON_POS);
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_2)]->Update(CHARA2_ICON_POS);
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_3)]->Update(CHARA3_ICON_POS);



	//カーソルの画像の読み込み
	m_pCursolSprite = std::make_unique<Sprite2D>();
	m_pCursolSprite->Create(device,context, L"Resources/Textures/SelectCursol.png", m_pStates);
	//カーソルの画像の初期座標設定
	m_pCursolSprite->Update(CHARA1_ICON_POS.x - CURSOL_POS, CHARA1_ICON_POS.y - CURSOL_POS);

	m_fadeTimer = 1.0f;
	m_sceneState = eSCENE_STATE::FADE_IN;

	//現在選択しているキャラクターの初期設定
	m_nowSelect = static_cast<int>(eCHARACTER_ID::CHARACTER_1);

	soundID = ADX2::GetInstance().Play(CRI_CUESHEET_0_SELECTSCENE_BGM);
	m_cursorColor = DirectX::SimpleMath::Vector4(0.0f, 1.0f, 1.0f, 1.0f);
}

///////////////////////////
//更新
//引数:タイマー
//戻り値:なし
//////////////////////////
void CharaSelectScene::Update(DX::StepTimer const & timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//キャラクターのアイコンの座標設定
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_1)]->Update(CHARA1_ICON_POS);
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_2)]->Update(CHARA2_ICON_POS);
	m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_3)]->Update(CHARA3_ICON_POS);

	static int id;

	m_cursorColor.x = cosf((float)timer.GetTotalSeconds()*3.0f);

	//シーンのステート
	switch (m_sceneState)
	{
		//フェードイン
		case SceneBase::eSCENE_STATE::FADE_IN:
		{
			m_fadeTimer -= static_cast<float>(timer.GetElapsedSeconds())* 2.0f;
			if (m_fadeTimer <= 0.0f)m_sceneState = eSCENE_STATE::MAIN;

			break;
		}
		//メイン
		case SceneBase::eSCENE_STATE::MAIN:
		{

			//操作説明
			CharaSelectScene::Manual();
			
			m_modelAngleY += 0.01f;
			DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_modelAngleY);
			//
			for (int i = 0; i < CHARA_NUM; i++)
			{
				//m_modelWorld[i] *= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero);
				//m_modelWorld[i] *= rotY;// *DirectX::SimpleMath::Matrix::CreateTranslation(MODEL_POS);
				//m_modelWorld[i] *= DirectX::SimpleMath::Matrix::CreateTranslation(MODEL_POS);
				m_modelWorld[i] = DirectX::SimpleMath::Matrix::CreateScale(MODEL_SIZE[i]) *  DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero) *
					rotY * DirectX::SimpleMath::Matrix::CreateTranslation(MODEL_POS);
				
			}
			//m_modelWorld = modelSize;

			if (m_isStop != true)
			{
				//左右キー入力でカーソル移動
				if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right))
				{
					m_nowSelect++;
					//SE再生
					ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

				}
				else if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left))
				{
					m_nowSelect--;
					//SE再生
					ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

				}

				switch (static_cast<eCHARACTER_ID>(m_nowSelect))
				{
					case eCHARACTER_ID::NONE:
					{
						m_nowSelect = static_cast<int>(eCHARACTER_ID::CHARACTER_3);
						break;
					}
					case eCHARACTER_ID::CHARACTER_1:
					{
						CharacterFactory::m_player1Chara = eCHARACTER_ID::CHARACTER_1;
						//カーソルの画像の座標設定
						m_pCursolSprite->Update(CHARA1_ICON_POS.x - CURSOL_POS, CHARA1_ICON_POS.y - CURSOL_POS);


						break;
					}
					case eCHARACTER_ID::CHARACTER_2:
					{
						CharacterFactory::m_player1Chara = eCHARACTER_ID::CHARACTER_2;
						//カーソルの画像の座標設定
						m_pCursolSprite->Update(CHARA2_ICON_POS.x - CURSOL_POS, CHARA2_ICON_POS.y - CURSOL_POS);

						break;
					}
					case eCHARACTER_ID::CHARACTER_3:
					{
						CharacterFactory::m_player1Chara = eCHARACTER_ID::CHARACTER_3;
						//カーソルの画像の座標設定
						m_pCursolSprite->Update(CHARA3_ICON_POS.x - CURSOL_POS, CHARA3_ICON_POS.y - CURSOL_POS);

						break;
					}
					case eCHARACTER_ID::OVER_ID:
					{
						m_nowSelect = static_cast<int>(eCHARACTER_ID::CHARACTER_1);
						break;
					}
					default:
						break;
				}

				//スペースキー入力でフェードアウト		
				if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
				{
					m_sceneState = eSCENE_STATE::FADE_OUT;
					//SE再生
					ADX2::GetInstance().Play(CRI_CUESHEET_0_SUBMIT);

					//LoadDisplay::GetInstance()->Update(true);
				}

				//エスケープキー入力でタイトルに戻る
				if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape))
				{
					SceneManager::GetInstance()->SetScene(eSCENE_ID::TITLE_SCENE);
					//SE再生
					ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);

					ADX2::GetInstance().Stop(soundID);

				}

				
			}
			break; 
		}
		//フェードアウト
		case SceneBase::eSCENE_STATE::FADE_OUT:
		{
			m_fadeTimer += static_cast<float>(timer.GetElapsedSeconds())* 2.0f;
			if (m_fadeTimer >= 1.0f)
			{
				//プレイヤ−２のキャラクターをランダムで選択
				switch (rand() % static_cast<int>(eCHARACTER_ID::OVER_ID))
					//switch(0)
				{
					//キャラクター１
					case static_cast<int>(eCHARACTER_ID::CHARACTER_1) :
					{
						CharacterFactory::m_player2Chara = eCHARACTER_ID::CHARACTER_1;
						break;
					}

																		//キャラクター２
					case static_cast<int>(eCHARACTER_ID::CHARACTER_2) :
					{
						CharacterFactory::m_player2Chara = eCHARACTER_ID::CHARACTER_2;
						break;
					}

																		//キャラクター３
					case static_cast<int>(eCHARACTER_ID::CHARACTER_3) :
					{
						CharacterFactory::m_player2Chara = eCHARACTER_ID::CHARACTER_3;
						break;
					}

					default:
						break;
				}
				ADX2::GetInstance().Stop(soundID);

				//プレイシーンへの遷移
				SceneManager::GetInstance()->SetScene(eSCENE_ID::PLAY_SCENE);


			}
			break;
		}
		default:
			break;
	}
	

	//フェードマネージャーの更新
	FadeManager::GetInstance()->Update(timer, m_fadeTimer);
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void CharaSelectScene::Render()
{
	DebugFont::GetInstance()->Print(500, 500, L"CharacterSelect", DirectX::Colors::Gainsboro);

	DebugFont::GetInstance()->Draw();

	//背景画像の描画
	m_pSelectSprite->Draw();

	//カーソルの画像の描画
	m_pCursolSprite->Draw(false,false,m_cursorColor);

	//キャラクターのアイコンの描画
	for (int i = 0; i < static_cast<int>(eCHARACTER_ID::OVER_ID); i++)
	{
		m_pCharacterSprite[i]->Draw();
	}

	//キャラクターの情報画像の描画
	switch (static_cast<eCHARACTER_ID>(m_nowSelect))
	{
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_pCharaInfoSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_1)]
				->Draw(true);
			//モデルの描画
			m_pCharacter1Model->Draw(m_modelWorld[static_cast<int>(eCHARACTER_ID::CHARACTER_1)], m_view, m_proj, MODEL_COLOR);

			break;
		}

		case eCHARACTER_ID::CHARACTER_2:
		{
			m_pCharaInfoSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_2)]
				->Draw(true);
			//モデルの描画
			m_pCharacter2Model->Draw(m_modelWorld[static_cast<int>(eCHARACTER_ID::CHARACTER_2)], m_view, m_proj, MODEL_COLOR);
			break;
		}

		case eCHARACTER_ID::CHARACTER_3:
		{
			m_pCharaInfoSprite[static_cast<int>(eCHARACTER_ID::CHARACTER_3)]
				->Draw(true);
			//モデルの描画
			m_pCharacter3Model->Draw(m_modelWorld[static_cast<int>(eCHARACTER_ID::CHARACTER_3)], m_view, m_proj, MODEL_COLOR);
			break;
		}

		default:
			break;
	}

	//操作説明の画像の描画
	if(m_isStop == true && m_pManualSprite != nullptr)
		m_pManualSprite->Draw(true);

}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void CharaSelectScene::Finalize()
{
	DebugFont::Dispose();

	m_pSelectSprite->Reset();


	for (int i = 0; i < static_cast<int>(eCHARACTER_ID::OVER_ID); i++)
	{
		m_pCharacterSprite[i]->Reset();
		m_pCharaInfoSprite[i]->Reset();
	}

	m_pCursolSprite->Reset();
	m_pManualSprite->Reset();


	ADX2::GetInstance().Stop(soundID);

}

///////////////////////////
//操作説明
//引数:なし
//戻り値:なし
//////////////////////////
void CharaSelectScene::Manual()
{
	//操作説明表示中にXキー入力で元の画面に戻る
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) && m_isStop == true)
	{
		m_isManualDown = false;
		m_isManualUp = true;

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);

	}

	//Xキー入力で操作説明の表示
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) && m_isStop == false)
	{
		m_isStop = true;
		m_isManualDown = true;

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}

	//下降フラグが立ったら画像を下降させる
	if (m_isManualDown == true)
	{
		m_ManualSpritePos.y += MANUAL_SPRITE_HEIGHT / 10.0f;

		if (m_ManualSpritePos.y >= 0.0f)
		{
			m_ManualSpritePos.y = 0.0f;
			m_isManualDown = false;
		}
	}

	//上昇フラグが立ったら画像を上昇させる
	if (m_isManualUp == true)
	{
		m_ManualSpritePos.y -= MANUAL_SPRITE_HEIGHT / 10.0f;

		if (m_ManualSpritePos.y <= -MANUAL_SPRITE_HEIGHT)
		{
			m_ManualSpritePos.y = -MANUAL_SPRITE_HEIGHT;
			m_isManualUp = false;
			//ゲーム停止フラグを消す
			m_isStop = false;
		}
	}

	//画像の更新
	if(m_pManualSprite != nullptr)
		m_pManualSprite->Update(m_ManualSpritePos);


}

