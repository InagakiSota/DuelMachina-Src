///////////////////////////////////
//プレイヤーのクラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月25日
///////////////////////////////////

#pragma once

#include <SimpleMath.h>
#include "GeometricPrimitive.h"
#include "Src\FrameWork\ModelObject\ModelObject.h"
#include "Src\FrameWork\Collision\Collision.h"
//#include "../Character/CharacterBase.h"

//class ModelObject;
class CharacterBase;


class Player
{
public:
	//コンストラクタ
	Player();
	//デストラクタ
	~Player();	

	//初期化
	void Initialize(DX::DeviceResources* pDeviceResources);
	//更新
	void Update();
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//終了処理
	void Finalize();
	//当たり判定用の足元の箱の取得
	Collision::BoxCollision GetLegCollBox();								
	//床との当たり判定
	void CheckFloorHit(const Collision::BoxCollision& floorBox);			


private:
	//static const float WALK_FORCE;								//歩きの移動量
	//static const float JUMP_FORCE;								//ジャンプの移動量
	//static const float GRAVITY;									//重力

	//DX::DeviceResources* m_pDeviceResources;					//デバイスリソースのポインタ
	//DirectX::SimpleMath::Matrix m_world;						//ワールド行列
	//DirectX::SimpleMath::Vector3 m_pos;							//座標
	//DirectX::SimpleMath::Vector3 m_vel;							//移動量
	//std::unique_ptr<ModelObject> m_pModel;						//3Dモデル
	//std::unique_ptr<DirectX::GeometricPrimitive> m_pLegBox;		//足元の箱
	//Collision::BoxCollision m_legCollBox;						//当たり判定用の足元の箱
	//DirectX::SimpleMath::Matrix m_legBoxWorld;					//足元の箱のワールド行列

	//bool m_landingFlag;											//着地フラグ
	//bool m_jumpFlag;											//ジャンプフラグ

	//キャラクターのポインタ
	CharacterBase* m_pCharacter;
};