///////////////////////////////////
//タイトルシーンのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////

#pragma once
#include "../SceneBase.h"
#include "../../FrameWork/ModelObject/ModelObject.h"

class Sprite2D;

class TitleScene :
	public SceneBase
{
public:
	TitleScene();
	~TitleScene();

	void Initialize() override;		//初期化
	void Update(DX::StepTimer const& timer)override;			//更新
	void Render()override;			//描画
	void Finalize()override;		//終了処理

private:

	//ロゴの画像
	std::unique_ptr<Sprite2D> m_pLogoSprite;
	//pushSpaceの画像
	std::unique_ptr<Sprite2D> m_pSpaceSprite;

	//pushSpaceの画像の座標
	static const DirectX::SimpleMath::Vector2 PUSH_SPACE_POS;
	//pushSpaceの画像の点滅タイマー
	float m_pushSpaceBlindTimer;
	//pushSpaceの画像の点滅フラグ
	bool m_pushSpaceBlindFlag;
	//pushSpaceの画像の点滅インターバル
	static const float PUSH_SPACE_BLIND_INTERBVAL;

	//天球のモデル
	std::unique_ptr<ModelObject> m_pSkyBox;
	//天球のワールド行列
	DirectX::SimpleMath::Matrix m_skyBoxWorld;
	
	//天球のY軸の角度
	float m_skyBoxAngleY;
	//天球の座標
	static const DirectX::SimpleMath::Vector3 SKYBOX_POS;

};

