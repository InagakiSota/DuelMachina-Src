///////////////////////////////
//プレイシーンの操作説明のクラス
//作成日：2021年5月9日
//作成者：GT2_04_稲垣颯太
///////////////////////////////

#include "pch.h"
#include "PlaySceneManual.h"
#include "../Character/CharacterID.h"
#include "../../PlayScene/Character/CharacterFactory.h"

#include "Adx2.h"
#include "CueSheet_0.h"

#include "../PlayScene.h"

//操作説明の画像の横幅
const float PlaySceneManual::MANUAL_SPRITE_WIDTH = 1920.0f;
//操作説明の画像の高さ
const float PlaySceneManual::MANUAL_SPRITE_HEIGHT = 1080.0f;

//コンストラクタ
PlaySceneManual::PlaySceneManual()
	:m_isManualDown(false),
	m_isManualLeft(false),
	m_isManualRight(false),
	m_isManualUp(false),
	m_pPlayScene(nullptr)
{
}

//デストラクタ
PlaySceneManual::~PlaySceneManual()
{
}

////////////////////////
//初期化
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneManual::Initialize()
{
	//操作説明の画像読み込み、初期座標設定
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i] = std::make_unique<Sprite2D>();

		m_ManualSpritePos[i].y = -MANUAL_SPRITE_HEIGHT;

	}
	m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = MANUAL_SPRITE_WIDTH;

	m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)]
		->Create(L"Resources/Textures/Manual_1.png");

	//操作説明のカーソル画像の読み込み
	m_pManualCursorSpriteRight = std::make_unique<Sprite2D>();
	m_pManualCursorSpriteRight->Create(L"Resources/Textures/ManualCursolr_Right.png");
	m_pManualCursorSpriteLeft = std::make_unique<Sprite2D>();
	m_pManualCursorSpriteLeft->Create(L"Resources/Textures/ManualCursolr_Left.png");

	//操作しているキャラクターごとに読み込む操作説明の画像を切り替える
	switch (CharacterFactory::m_player1Chara)
	{
		//キャラクター１
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(L"Resources/Textures/Manual_2_chara1.png");
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(L"Resources/Textures/Manual_2_chara2.png");
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(L"Resources/Textures/Manual_2_chara3.png");
			break;
		}

		default:
			break;
	}

}

////////////////////////
//更新
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneManual::Update()
{
	//操作説明表示中にESCキーでゲームに戻る
	if (m_pPlayScene->GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_pPlayScene->GetIsManualDisplay() == true)
	{
		//下降フラグを消す
		m_isManualDown = false;
		//上昇フラグを立てる
		m_isManualUp = true;

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
	}

	//左右入力で移動フラグを立てる
	//左入力
	if (m_pPlayScene->GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Left) &&
		m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
	{
		m_isManualLeft = false;
		m_isManualRight = true;

		//SE再生
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	//右入力
	else if (m_pPlayScene->GetKeyTracker()->IsKeyPressed(DirectX::Keyboard::Keys::Right) &&
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
	m_pManualCursorSpriteRight->Update(DirectX::SimpleMath::Vector2(1720.0f + cosf(m_pPlayScene->GetTotalTime() * 6.0f) * 10.0f, 540.0f));
	m_pManualCursorSpriteLeft->Update(DirectX::SimpleMath::Vector2(50.0f - cosf(m_pPlayScene->GetTotalTime() * 6.0f) * 10.0f, 540.0f));


	//右移動フラグが立ったら画像を右に移動させる
	if (m_isManualRight == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].x += MANUAL_SPRITE_WIDTH / 10.0f;

			//右に移動し終わったら右移動フラグを消す
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
			//左に移動し終わったら左移動フラグを消す
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

			//降下し終わったら降下フラグを消す
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
			//上昇し終わったら上昇フラグを消す
			if (m_ManualSpritePos[i].y <= -MANUAL_SPRITE_HEIGHT)
			{
				m_ManualSpritePos[i].y = -MANUAL_SPRITE_HEIGHT;
				m_isManualUp = false;
				//ゲーム停止フラグを消す
				m_pPlayScene->SetIsManualDisplay(false);
			}
		}
	}

	//画像の更新
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i]->Update(m_ManualSpritePos[i]);

	}

}

////////////////////////
//描画
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneManual::Render()
{
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i]->Draw(true);
	}

	//操作説明のカーソル画像の描画
	//左
	if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
	{
		m_pManualCursorSpriteLeft->Draw(true);
	}
	//右
	else
	{
		m_pManualCursorSpriteRight->Draw(true);
	}
}

////////////////////////
//終了処理
//引数：なし
//戻り値：なし
////////////////////////
void PlaySceneManual::Finalize()
{
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i]->Reset();

	}

	m_pManualCursorSpriteLeft->Reset();
	m_pManualCursorSpriteLeft.reset();

	m_pManualCursorSpriteRight->Reset();
	m_pManualCursorSpriteRight.reset();

}
