//////////////////////////
//プレイシーンのUIクラス
//作成日：2021年4月30日
//作成者：GT2_04_稲垣颯太
/////////////////////////

#pragma once

#include <SimpleMath.h>
#include "../PlayScene.h"
#include "Src/FrameWork/Sprite2D/Sprite2D.h"

class PlayScene;

class PlaySceneUI
{
public:
	//コンストラクタ
	PlaySceneUI();
	//デストラクタ
	~PlaySceneUI();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Render();
	//終了処理
	void Finalize();
	//画像の読み込み
	void Load();
	//リセット
	void Reset();

	//プレイシーンの取得
	void SetPlayScene(PlayScene* pPlayScene)
	{
		m_pPlayScene = pPlayScene;
	}

private:
	//プレイヤーの数
	static const int PLAYER_NUM = 2;
	//勝利するための取得本数
	static const int WIN_NUM = 2;

	//体力バーの横幅
	static const float HP_BAR_WIDTH;
	//体力バーの高さ
	static const float HP_BAR_HEIGHT;
	//体力バーのマージン
	static const float HP_BAR_MARGIN;
	//ブースト容量バーの横幅
	static const float BOOST_BAR_WIDTH;
	//ブースト容量バーの高さ
	static const float BOOST_BAR_HEIGHT;
	//カウントダウンの時間
	static const float COUNT_DOWN_TIME;
	//カウントダウン画像の座標
	static const DirectX::SimpleMath::Vector2 COUNT_DOWN_SPRITE_POS;
	//制限時間の最大値
	static const float TIME_MAX;
	//勝利本数の画像の横幅
	static const float WIN_NUM_SPRITE_WIDTH;
	//勝利本数の画像の高さ
	static const float WIN_NUM_SPRITE_HEIGHT;
	//制限時間の画像の横幅
	static const float TIME_SPRITE_WIDTH;
	//制限時間の画像の高さ
	static const float TIME_SPRITE_HEIGHT;
	//制限時間の画像のX座標
	static const float TIME_SPRITE_POS_X;
	//体力バーの初期座標
	static const DirectX::SimpleMath::Vector2 HP_BAR_POS[PLAYER_NUM];
	//ブースト容量バーの初期座標
	static const DirectX::SimpleMath::Vector2 BOOST_BAR_POS[PLAYER_NUM];

	//体力バー
	std::unique_ptr<Sprite2D> m_pHPBar[PLAYER_NUM];
	//体力バー(体力低)	
	std::unique_ptr <Sprite2D> m_pHPBarDanger[PLAYER_NUM];
	//体力バーの背景
	std::unique_ptr<Sprite2D> m_pHPBarBack[PLAYER_NUM];

	//体力バーの座標
	DirectX::SimpleMath::Vector2 m_hpBarPos[PLAYER_NUM];
	//体力バーの切り取り位置
	RECT m_hpBarRect[PLAYER_NUM];

	//ブースト容量バー
	std::unique_ptr<Sprite2D> m_pBoostBar[PLAYER_NUM];
	//ブースト容量バー(オーバーヒート)
	std::unique_ptr<Sprite2D> m_pBoostOverHeatBar[PLAYER_NUM];
	//ブースト容量バーの背景
	std::unique_ptr<Sprite2D> m_pBoostBack[PLAYER_NUM];
	//ブースト容量バーの座標
	DirectX::SimpleMath::Vector2 m_boostBarPos[PLAYER_NUM];
	//ブースト容量バーの切り取り位置
	RECT m_boostBarRect[PLAYER_NUM];

	//勝利画像
	std::unique_ptr<Sprite2D> m_pWinSprite[PLAYER_NUM];
	//勝利画像の座標
	DirectX::SimpleMath::Vector2 m_winSpritePos;

	//ラウンド数の画像
	std::unique_ptr<Sprite2D> m_pRoundSprite[static_cast<int>(PlayScene::eROUND::ROUND_NUM)];
	//FIGHTの画像
	std::unique_ptr<Sprite2D> m_pFightSprite;
	//タイムアップの画像
	std::unique_ptr<Sprite2D> m_pTimeUpSprite;
	//引き分け時の画像
	std::unique_ptr<Sprite2D> m_pDrawSprite;
	//制限時間の画像(一の位)
	std::unique_ptr<Sprite2D> m_pTimeSpriteOne;
	//制限時間の画像(十の位)
	std::unique_ptr<Sprite2D> m_pTimeSpriteTen;

	//勝利本数の画像
	std::unique_ptr<Sprite2D> m_pWinNumSprtie[PLAYER_NUM][WIN_NUM];
	//勝利本数の画像の切り取り位置
	RECT m_winNumSpriteRect[PLAYER_NUM][WIN_NUM];
	//勝利本数の画像の座標
	DirectX::SimpleMath::Vector2 m_winNumSpritePos[PLAYER_NUM][WIN_NUM];
	//プレイヤーの頭上のカーソル
	std::unique_ptr<Sprite2D> m_pOverHeadSprite[PLAYER_NUM];
	//プレイヤーの頭上のカーソルの座標
	DirectX::SimpleMath::Vector2 m_overHeadSpritePos[PLAYER_NUM];
	//制限時間の画像の切り取り位置　一の位
	RECT m_timeSpriteOneRect;
	//制限時間の画像の切り取り位置　十の位
	RECT m_timeSpriteTenRect;
	//プレイシーンのポインタ
	PlayScene* m_pPlayScene;
	//リザルトのpushSpaceの画像
	std::unique_ptr<Sprite2D> m_pPushSpaceResult;
	//リザルトのpushSpaceの画像の座標
	static const DirectX::SimpleMath::Vector2 PUSH_SPACE_RESULT_POS;

};
