#include "pch.h"
#include "AttackManager.h"

AttackManager* AttackManager::m_pMyInstance = nullptr;

//コンストラクタ
AttackManager::AttackManager()
	:m_pAttackArray{}
{
}

//デストラクタ
AttackManager::~AttackManager()
{
	m_pDeviceResources = nullptr;
	for (int i = 0; i < ATTACK_NUM; i++)
	{
		//if (m_pAttackArray[i]->geoBox != nullptr)
		//{
		//	//m_pAttackArray[i]->geoBox = nullptr;
		//}

		//if (m_pAttackArray[i] != nullptr)
		//{
		//	m_pAttackArray[i].reset();
		//}
	}

}

///////////////////////////
//初期化
//引数:デバイス
//戻り値:なし
//////////////////////////
void AttackManager::Initialize(DX::DeviceResources* pDeviceResources)
{
	m_pDeviceResources = pDeviceResources;
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	//値を初期化
	for (int i = 0; i < ATTACK_NUM; i++)
	{
		//ポインタの読み込み
		m_pAttackArray[i] = std::make_unique<AttackStruct>();

		//ジオメトリックプリミティブの作成
		//m_pAttackArray[i]->geoBox = DirectX::GeometricPrimitive::CreateCube(context);
		//値の初期化
		Reset(i);
	}

}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void AttackManager::Update()
{

	for (int i = 0; i < ATTACK_NUM; i++)
	{
		if (m_pAttackArray[i]->isUsed == false)
		{
			Reset(i);
			continue;
		}
		//if (m_pAttackArray[i]->geoBox == nullptr)
		{
			m_pAttackArray[i]->geoBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_pAttackArray[i]->attackColl.size_h * 2);
		}
		DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pAttackArray[i]->pos);
		m_pAttackArray[i]->world = trans;

	}

}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void AttackManager::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	for (int i = 0; i < ATTACK_NUM; i++)
	{
		//使用されていなければスルー
		if (m_pAttackArray[i]->isUsed == false)continue;

		if (m_pAttackArray[i]->geoBox != nullptr)
		{
			m_pAttackArray[i]->geoBox->Draw(m_pAttackArray[i]->world, view, proj, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void AttackManager::Finalize()
{
	for (int i = 0; i < ATTACK_NUM; i++)
	{
		m_pAttackArray[i]->geoBox.reset();
		m_pAttackArray[i].reset();
	}
}

///////////////////////////
//攻撃の構造体の取得
//引数:要素数
//戻り値:攻撃の構造体のポインタ
//////////////////////////
AttackStruct * AttackManager::GetAttackStruct(const int& index)
{
	////攻撃の構造体のポインタ変数
	//AttackStruct* pAttack;
	//for (int i = 0; i < ATTACK_NUM; i++)
	//{
	//	//使用されていたらスルー
	//	if (m_pAttackArray[i]->isUsed == true)continue;

	//	else
	//	{
	//		//使用されていなければポイントに格納
	//		pAttack = m_pAttackArray[i].get();
	//		break;
	//	}
	//}

	//return pAttack;

	return m_pAttackArray[index].get();
}

///////////////////////////
//リセット
//引数:
//戻り値:
//////////////////////////
void AttackManager::Reset(const int & index)
{
	m_pAttackArray[index]->isUsed = false;
	m_pAttackArray[index]->pos = DirectX::SimpleMath::Vector3::Zero;
	m_pAttackArray[index]->vel = DirectX::SimpleMath::Vector3::Zero;
	m_pAttackArray[index]->attackColl = {};
	m_pAttackArray[index]->power = 0;
	m_pAttackArray[index]->world.Identity;
	m_pAttackArray[index]->isHit = false;
	m_pAttackArray[index]->playerID = ePLAYER_ID::NONE;


	if (m_pAttackArray[index]->geoBox != nullptr)
	{
		//m_pAttackArray[index].reset();
		m_pAttackArray[index]->geoBox = nullptr;

	}

}

///////////////////////////
//インスタンスの取得
//引数:なし
//戻り値:自身(攻撃マネージャー)のポインタ
//////////////////////////
AttackManager * AttackManager::GetInstance()
{
	if (m_pMyInstance == nullptr)
	{
		m_pMyInstance = new AttackManager();
	}

	return m_pMyInstance;
}

///////////////////////////
//インスタンスの解放
//引数:なし
//戻り値:なし
//////////////////////////
void AttackManager::ReleaseInstance()
{
	if (m_pMyInstance != nullptr)
	{
		delete m_pMyInstance;
	}
	m_pMyInstance = nullptr;
}
