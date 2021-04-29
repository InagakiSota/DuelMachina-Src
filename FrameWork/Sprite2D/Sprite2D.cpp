///////////////////////////////////
//2Dスプライト
//作成者：GS3_05_稲垣颯太
//作成日：2020年5月22日
///////////////////////////////////

#include  "pch.h"
#include "Sprite2D.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

//コンストラクタ
Sprite2D::Sprite2D()
	:m_pos(0,0),
	m_pStates(nullptr),
	m_rect{}
{
}

//デストラクタ
Sprite2D::~Sprite2D()
{
	if (m_spriteBatch != nullptr)
	{
		m_spriteBatch.reset();
	}
	m_pStates = nullptr;
}

//生成
void Sprite2D::Create(ID3D11Device1 * device, ID3D11DeviceContext1 * context, const wchar_t* fileName, DirectX::CommonStates* pStates)
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	DirectX::CreateWICTextureFromFile(device, fileName, NULL, m_texture.ReleaseAndGetAddressOf());
	m_pStates = pStates;

	m_rect.top = 0;
	m_rect.left = 0;
	m_rect.bottom = 0;
	m_rect.right = 0;
}

///////////////////////////
//描画
//引数:isAlpa:透過させるかどうか(true = 透過させる、false = 透過させない)
//     isRect:切り取るかどうか(true = 切り取る、false = 切り取らない)
//戻り値:
//////////////////////////
void Sprite2D::Draw(bool isAlpha, bool isRect, DirectX::FXMVECTOR color)
{
	//透過させない
	if (isAlpha == false)
	{
		m_spriteBatch->Begin();
		//切り取りしない
		if (isRect == false)
		{
			m_spriteBatch->Draw(m_texture.Get(), m_pos, color);
		}
		//切り取りする
		else if(isRect == true)
		{
			m_spriteBatch->Draw(m_texture.Get(), m_pos, &m_rect, color);
		}
		m_spriteBatch->End();
	}
	//透過させる
	else
	{
		m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred,m_pStates->NonPremultiplied());
		//切り取りしない
		if (isRect == false)
		{
			m_spriteBatch->Draw(m_texture.Get(), m_pos, color);
		}
		//切り取りする
		else if(isRect == true)
		{
			m_spriteBatch->Draw(m_texture.Get(), m_pos, &m_rect, color);
		}
		m_spriteBatch->End();

	}
}

//リセット
void Sprite2D::Reset()
{
	m_texture.Reset();
	m_spriteBatch.reset();
}

//更新
void Sprite2D::Update(const float& posX, const float& posY)
{
	//座標の設定
	m_pos.x = posX;
	m_pos.y = posY;
}

void Sprite2D::Update(const DirectX::SimpleMath::Vector2 & pos)
{
	//座標の設定
	m_pos = pos;
}

void Sprite2D::Update(const float & posX, const float & posY, const RECT & rect)
{
	//座標の設定
	m_pos.x = posX;
	m_pos.y = posY;

	//画像切り取り位置の設定
	m_rect = rect;
}

void Sprite2D::Update(const DirectX::SimpleMath::Vector2 & pos, const RECT & rect)
{
	//座標の設定
	m_pos = pos;

	//画像切り取り位置の設定
	m_rect = rect;

}

