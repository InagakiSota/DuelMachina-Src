///////////////////////////////////
//キャラクターセレクトシーンのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年7月15日
///////////////////////////////////

#pragma once
#include "../SceneBase.h"
#include "Src\SceneManager\PlayScene\Character\CharacterID.h"
#include "../../FrameWork/FbxModel/FbxModel.h"

class Sprite2D;

class CharaSelectScene :
	public SceneBase
{
public:
	//コンストラクタ
	CharaSelectScene();
	//デストラクタ
	~CharaSelectScene();

	//初期化
	void Initialize() override;		
	//更新
	void Update(DX::StepTimer const& timer)override;
	//描画
	void Render()override;
	//終了処理
	void Finalize()override;

	//キャラクターの数
	static const int CHARA_NUM = 3;


private:
	//画像
	std::unique_ptr<Sprite2D> m_pSelectSprite;
	std::unique_ptr<Sprite2D> m_pCharacterSprite[static_cast<int>(eCHARACTER_ID::OVER_ID)];
	std::unique_ptr<Sprite2D> m_pCharaInfoSprite[static_cast<int>(eCHARACTER_ID::OVER_ID)];
	std::unique_ptr<Sprite2D> m_pCursolSprite;

	//各キャラクターのモデル
	FbxModel* m_pCharacter1Model;
	FbxModel* m_pCharacter2Model;
	FbxModel* m_pCharacter3Model;

	//モデルのワールド行列
	DirectX::SimpleMath::Matrix m_modelWorld[CHARA_NUM];
	//モデルの座標
	static const DirectX::SimpleMath::Vector3 MODEL_POS;
	//モデルの色
	static const DirectX::SimpleMath::Vector4 MODEL_COLOR;

	//モデルの回転角度(Y)
	float m_modelAngleY;

	//各キャラクターのモデルのサイズ
	static const float MODEL_SIZE[CHARA_NUM];


	//キャラクターのアイコンの座標定数
	static const DirectX::SimpleMath::Vector2 CHARA1_ICON_POS;
	static const DirectX::SimpleMath::Vector2 CHARA2_ICON_POS;
	static const DirectX::SimpleMath::Vector2 CHARA3_ICON_POS;

	//キャラクターの情報画像の座標
	static const  DirectX::SimpleMath::Vector2 CHARA_INFO_POS;

	//現在選択しているキャラクター
	int m_nowSelect;

	//カーソルの座標
	static const float CURSOL_POS;

	int count;

	//操作説明
	void Manual();

	//操作説明画像
	std::unique_ptr<Sprite2D> m_pManualSprite;
	//操作説明の画像の座標
	DirectX::SimpleMath::Vector2 m_ManualSpritePos;
	//ゲーム停止フラグ
	bool m_isStop;
	//操作説明上昇フラグ
	bool m_isManualUp;
	//操作説明下降フラグ
	bool m_isManualDown;
	//操作説明の画像の横幅
	static const float MANUAL_SPRITE_WIDTH;
	//操作説明の画像の高さ
	static const float MANUAL_SPRITE_HEIGHT;
	//カーソル画像の色
	DirectX::SimpleMath::Vector4 m_cursorColor;

	//音のID
	int m_soundID;
};

