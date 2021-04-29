#include "pch.h"
#include "Player.h"
#include "Keyboard.h"
#include "GamePad.h"

#include "../Character/CharacterBase.h"
#include "../Character/Character1/Character1.h"

//コンストラク
Player::Player()
{
}

//デストラクタ
Player::~Player()
{
	if (m_pCharacter != nullptr)
	{
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}
}

///////////////////////////
//初期化
//引数:デバイスリソースのポインタ
//戻り値:なし
//////////////////////////
void Player::Initialize(DX::DeviceResources* pDeviceResources)
{
	//キャラクター１の読み込み
	m_pCharacter = new Character1();
	//キャラクターの初期化
	m_pCharacter->Initialize(pDeviceResources);
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void Player::Update()
{
	////移動
	//Move();

	////移動量を座標に加算
	//m_pos += m_vel;
	//////Y軸回転
	////Matrix rotX = Matrix::CreateRotationX(time);
	//DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));
	////Matrix rotZ = Matrix::CreateRotationZ(XMConvertToRadians(30));

	//////m_world = rotX;
	////m_world = rotY *  m_world * rotX;
	////座標を行列に変換
	//DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	////サイズを行列に変換
	//DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(0.0015f, 0.0015f, 0.0015f);
	////ワールド行列に加算
	//m_world = scale * rotY* trans;

	////モデルの更新
	//m_pModel->Update(m_world);

	////足元の箱の座標の更新
	//m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x-0.1f, m_pos.y - 1.4f, m_pos.z);
	//m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);
	
	//キャラクターの更新
	m_pCharacter->Update();
}

///////////////////////////
//描画
//引数:ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void Player::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	////モデルの描画
	//m_pModel->Render(view, proj);

	////足元の箱の描画
	//m_pLegBox->Draw(m_legBoxWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f,0.0f,0.0f,0.4f));

	//キャラクターの描画
	m_pCharacter->Render(view, proj);
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void Player::Finalize()
{
	////モデルの解放
	//m_pModel->Lost();

	////ジオメトリプリミティブの解放
	//m_pLegBox.reset();

	////delete m_pDeviceResources;
	//m_pDeviceResources = nullptr;

	delete m_pCharacter;
	m_pCharacter = nullptr;
}

///////////////////////////
//当たり判定用の足元の箱の取得
//引数:なし
//戻り値:当たり判定用の足元の箱
//////////////////////////
Collision::BoxCollision Player::GetLegCollBox()
{
	return m_pCharacter->GetLegCollBox();
}

///////////////////////////
//床との当たり判定
//引数:床の当たり判定の箱
//戻り値:当たったかどうか
//////////////////////////
void Player::CheckFloorHit(const Collision::BoxCollision & floorBox)
{
	m_pCharacter->HitFloor(floorBox);
}

