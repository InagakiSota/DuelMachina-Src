///////////////////////////////////
//プレイシーンのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////

#pragma once
#include "../SceneBase.h"
#include <SimpleMath.h>
#include "Src\FrameWork\Collision\Collision.h"

#include "GeometricPrimitive.h"
#include "Manual/PlaySceneManual.h"

//クラスの前方宣言
class EffectManager;
class FbxModel;
class Sprite2D;
class DebugFont;
class ModelObject;
class Fbx;
class CharacterBase;
class DebugCamera;
class AttackManager;
class PlaySceneUI;

class FbxModel;
class ShadowManager;
class HitEffectManager;
class PlaySceneMenu;

class PlayScene :
	public SceneBase
{
////////////////////////////////////
//関数
public:

	//プレイシーンのステート
	enum class ePLAY_SCENE_STATE
	{
		NONE = -1,

		COUNT_DOWN,		//カウントダウン
		MAIN,			//メイン
		TIME_UP,		//タイムアップ
		RESULT,			//リザルト

		OVER_ID
	};

	//ラウンド
	enum class eROUND
	{
		NONE = -1,

		ROUND_1,		//ラウンド１
		ROUND_2,		//ラウンド２
		ROUND_3,		//ラウンド３

		ROUND_NUM,		//全ラウンド数
	};

	//操作説明の画像の種類
	enum class eMANUAL_SPRITE_TYPE
	{
		NONE = -1,

		KEYBOARD,		//キーボード
		COMMAND,		//コマンド

		SPRITE_NUM,		//画像枚数
	};


	//コンストラクタ
	PlayScene();
	//デストラクタ
	~PlayScene();			
	//初期化
	void Initialize() override;	
	//更新
	void Update(DX::StepTimer const& timer) override;			
	//描画
	void Render() override;		
	//終了処理
	void Finalize() override;

	//プレイヤーのポインタのゲッター
	CharacterBase* GetPlayer(const int& index)const
	{
		return m_pPlayer[index];
	}

	//プレイヤーの勝利本数のゲッター
	int GetPlayerWinNum(const int& index)	const
	{
		return m_playerWinNum[index];
	}

	//時間のゲッター
	float GetTime() const
	{
		return m_time;
	}
	//プレイシーンの状態のゲッター
	PlayScene::ePLAY_SCENE_STATE GetPlaySceneState()const
	{
		return m_playSceneState;
	}
	//リザルト状態かどうかのフラグのゲッター
	bool GetIsResult()const
	{
		return m_isResult;
	}
	//カウントダウンのタイマーのゲッター
	float GetCountDownTimer()const
	{
		return m_countdownTimer;
	}
	//現在のラウンドのゲッター
	eROUND GetNowRound()const
	{
		return m_nowRound;
	}

	//時間の取得
	float GetTotalTime() const
	{
		return m_totalSeconds;
	}

	//操作説明画面の表示フラグの取得
	bool GetIsManualDisplay()const
	{
		return m_isManualDisplay;
	}
	//操作説明画面の表示フラグの設定
	void SetIsManualDisplay(const bool& isManualDisplay)
	{
		m_isManualDisplay = isManualDisplay;
	}

	//停止フラグの取得
	bool GetIsStop()const
	{
		return m_isStop;
	}
	//停止フラグの設定
	void SetIsStop(const bool& isStop)
	{
		m_isStop = isStop;
	}

	//プレイヤーの取得
	CharacterBase* GetPlayer(const int& index)
	{
		return m_pPlayer[index];
	}
private:
	//リセット
	void Reset();
	//操作説明
	void Manual();
	//メニュー
	void Menu();
	//リザルト
	void Result(DX::StepTimer const& timer);

////////////////////////////////////
//変数・定数・列挙体

private:
	//プレイシーンのステート変数
	ePLAY_SCENE_STATE m_playSceneState;

	//プレイヤーの数
	static const int PLAYER_NUM = 2;		
	//勝利するための取得本数
	static const int WIN_NUM = 2;

	DirectX::SimpleMath::Matrix m_spaceWorld;	//天球のワールド行列

	//制限時間の最大値
	static const float TIME_MAX;
	//タイムアップシーンの時間
	static const float TIME_UP_TIME;
	//ラウンド切り替えの時間
	static const float ROUND_CHANGE_TIME;
	//カウントダウンの時間
	static const float COUNT_DOWN_TIME;

	//std::unique_ptr<DirectX::CommonStates>m_states;
	//std::unique_ptr<DirectX::IEffectFactory>m_fxFactory;
	//std::unique_ptr<DirectX::Model>m_model;
	
	//3dモデル
	//std::unique_ptr<ModelObject>m_modelObject;//プレイヤー(ロボ)

	//天球
	std::unique_ptr<ModelObject>m_space;		
	//スプライト								
	std::unique_ptr<Sprite2D> m_sprite2D;		


	//fbx
	//std::unique_ptr<Fbx> m_pFbx;

	//床
	std::unique_ptr<DirectX::GeometricPrimitive> m_pFloor;
	//床のワールド行列
	DirectX::SimpleMath::Matrix m_floorWorld;
	//床の当たり判定用の箱
	Collision::BoxCollision m_floorBox;

	//プレイヤーのポインタ
	CharacterBase* m_pPlayer[PLAYER_NUM];

	//デバッグカメラ
	std::unique_ptr<DebugCamera> m_pDebugCamera;

	//カメラの注視点
	DirectX::SimpleMath::Vector3 m_targetPos;

	//足元の影のエフェクト
	std::unique_ptr<ShadowManager> m_pShadowManager[PLAYER_NUM];
	
	//攻撃のマネージャー
	//std::unique_ptr<AttackManager> m_pAttackManager;

	FbxModel* m_pFbxModel;
	DirectX::SimpleMath::Matrix m_fbxModelWorld;

	//現在のラウンド
	eROUND m_nowRound;
	//各プレイヤーの勝利数(ラウンド取得本数)
	int m_playerWinNum[PLAYER_NUM];

	//制限時間
	float m_time;

	//カウントダウンタイマー
	float m_countdownTimer;

	//体力のバッファ
	int m_playerHpBuffer[PLAYER_NUM];
	//ゲーム停止フラグ
	bool m_isStop;
	
	//メニュー表示フラグ
	bool m_isMenuDisplay;
	//操作説明表示フラグ
	bool m_isManualDisplay;

	int m_menuCursor;
	//時間
	float m_totalSeconds;
	//リザルトに遷移したフラグ
	bool m_isResult;

	//UIクラスのポインタ
	std::unique_ptr<PlaySceneUI> m_pPlaySceneUI;
	//メニュークラスのポインタ
	std::unique_ptr<PlaySceneMenu> m_pPlaySceneMenu;



};

