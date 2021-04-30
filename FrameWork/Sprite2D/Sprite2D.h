///////////////////////////////////
//2Dスプライト
//作成者：GS3_05_稲垣颯太
//作成日：2020年5月22日
///////////////////////////////////

#pragma once

#include "SpriteBatch.h"
#include "SimpleMath.h"
#include <CommonStates.h>

class Sprite2D
{
public:
	Sprite2D();
	~Sprite2D();

	//生成
	void Create(const wchar_t* fileName);
	//描画
	void Draw(bool isAlpha = false, bool isRect = false, DirectX::FXMVECTOR color = DirectX::SimpleMath::Vector4(1,1,1,1));
	//リセット
	void Reset();
	//更新
	void Update(const float& posX = 0, const float& posY = 0);
	void Update(const DirectX::SimpleMath::Vector2& pos);
	void Update(const float& posX, const float& posY,const RECT& rect);
	void Update(const DirectX::SimpleMath::Vector2& pos, const RECT& rect);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	DirectX::SpriteBatch* m_spriteBatch;
	//画像の座標
	DirectX::SimpleMath::Vector2 m_pos;
	//画像切り取り位置
	RECT m_rect;
	DirectX::CommonStates* m_pStates;
};

