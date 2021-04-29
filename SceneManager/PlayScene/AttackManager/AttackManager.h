#pragma once
#include "../../../FrameWork/Collision/Collision.h"
#include "GeometricPrimitive.h"
#include "DeviceResources.h"
#include "../../PlayerID.h"

//攻撃の構造体
struct AttackStruct
{
	//コンストラクタ
	AttackStruct()
		:power(0),
		pos{},
		attackColl{},
		geoBox(nullptr),
		isUsed(false),
		vel{},
		world{},
		timer(0),
		playerID(ePLAYER_ID::NONE),
		isHit(false)
	{
	}

	//威力
	int power;
	//当たり判定
	Collision::BoxCollision attackColl;
	//ジオメトリックプリミティブ(箱)
	std::unique_ptr<DirectX::GeometricPrimitive> geoBox;
	//使用されているフラグ
	bool isUsed;
	//座標
	DirectX::SimpleMath::Vector3 pos;
	//移動量
	DirectX::SimpleMath::Vector3 vel;
	//ワールド行列
	DirectX::SimpleMath::Matrix world;
	//タイマー
	float timer;
	//使用するプレイヤーのID
	ePLAYER_ID playerID;
	//当たったフラグ
	bool isHit;
};

class AttackManager
{
public:
	//攻撃の総数
	static const int ATTACK_NUM = 128;


	//初期化
	void Initialize(DX::DeviceResources* pDeviceResources);
	//更新
	void Update();
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//終了処理
	void Finalize();

	//攻撃の構造体の取得
	AttackStruct* GetAttackStruct(const int& index);

	//リセット
	void Reset(const int& index);

	//インスタンスの取得
	static AttackManager* GetInstance();
	//インスタンスの解放
	static void ReleaseInstance();

private:
	//シングルトン化
	//コンストラクタ
	AttackManager();
	//デストラクタ
	~AttackManager();
	//自身のインスタンス
	static AttackManager* m_pMyInstance;


	//攻撃の配列
	std::unique_ptr<AttackStruct> m_pAttackArray[ATTACK_NUM];
	//デバイス
	DX::DeviceResources* m_pDeviceResources;
	
};

