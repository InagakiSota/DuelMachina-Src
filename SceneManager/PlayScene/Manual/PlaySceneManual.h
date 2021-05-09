///////////////////////////////
//プレイシーンの操作説明のクラス
//作成日：2021年5月9日
//作成者：GT2_04_稲垣颯太
///////////////////////////////

#pragma once

#include <SimpleMath.h>
#include "Src/FrameWork/Sprite2D/Sprite2D.h"

class PlayScene;

class PlaySceneManual
{
public:
	//コンストラクタ
	PlaySceneManual();
	//デストラクタ
	~PlaySceneManual();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Render();
	//終了処理
	void Finalize();
	//プレイシーンのポインタ設定
	void SetPlayScene(PlayScene* pPlayScene)
	{
		m_pPlayScene = pPlayScene;
	}

private:
	//操作説明の画像の横幅
	static const float MANUAL_SPRITE_WIDTH;
	//操作説明の画像の高さ
	static const float MANUAL_SPRITE_HEIGHT;

	//操作説明の画像の種類
	enum class eMANUAL_SPRITE_TYPE
	{
		NONE = -1,

		KEYBOARD,		//キーボード
		COMMAND,		//コマンド

		SPRITE_NUM,		//画像枚数
	};

	//操作説明画像
	std::unique_ptr<Sprite2D> m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM)];
	//操作説明の画像の座標
	DirectX::SimpleMath::Vector2 m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM)];
	//操作説明上昇フラグ
	bool m_isManualUp;
	//操作説明下降フラグ
	bool m_isManualDown;
	//操作説明右移動フラグ
	bool m_isManualRight;
	//操作説明左移動フラグ
	bool m_isManualLeft;
	//操作説明のカーソルの画像(右)
	std::unique_ptr<Sprite2D> m_pManualCursorSpriteRight;
	//操作説明のカーソルの画像(左)
	std::unique_ptr<Sprite2D> m_pManualCursorSpriteLeft;

	//プレイシーンのポインタ
	PlayScene* m_pPlayScene;
};

