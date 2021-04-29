///////////////////////////////////
//デバッグフォント
//作成者：GS3_05_稲垣颯太
//作成日：2020年5月20日
///////////////////////////////////

#pragma once

#include "SpriteBatch.h"
#include "SimpleMath.h"

#include "SpriteFont.h"

#include "AlignedAllocationPolicy.h"



#define DEBUG_FONT_SINGLETON_TYPE 1

class DebugFont : public AlignedAllocationPolicy<16>
{
private:
	//コンストラクタ
	DebugFont();


public:
	//文字列最大数サイズ
	static const size_t STRING_SIZE_MAX = 256;

	//デストラクタ
	~DebugFont();

	//生成
	void Create(ID3D11Device1* device, ID3D11DeviceContext1* context);
	//描画
	void Draw();
	//リセット
	void Reset();
	//表示情報登録
	void Print(float posX, float posY, wchar_t const* const format , DirectX::XMVECTOR color, ...);
	void Print(DirectX::SimpleMath::Vector2 pos, wchar_t const* const format, DirectX::XMVECTOR color, ...);

#if DEBUG_FONT_SINGLETON_TYPE == 0


public:
	//オブジェクトへのポインタを返す
	static DebugFont* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			//オブジェクトを生成し、m_pInstance変数に格納する
			m_pInstance = new DebugFont();
		}
		//オブジェクトへのポインタを返す
		return m_pInstance;
	}

	//オブジェクトへの参照を返す
	static DebugFont& getRefInstance()
	{
		if (m_pInstance == nullptr)
		{
			//オブジェクトを生成し、m_pInstance変数に格納する
			m_pInstance = new DebugFont();
		}
		//オブジェクトへのポインタを返す
		return *m_pInstance;
	}

	//オブジェクトを破棄する
	static void Dispose()
	{
		if (m_pInstance != nullptr)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static DebugFont* m_pInstance;


#elif DEBUG_FONT_SINGLETON_TYPE == 1
public:
	//オブジェクトへのポインタを返す
	static DebugFont* GetInstance()
	{
		if (m_pInstance.get() == nullptr)
		{
			//オブジェクトを生成し、m_pInstance変数に格納する
			m_pInstance.reset(new DebugFont());

		}
		//オブジェクトへのポインタを返す
		return m_pInstance.get();
	}
	//オブジェクトを破棄する
	static void Dispose()
	{
		if (m_pInstance)m_pInstance.reset();
	}

private:
	//オブジェクトへのポインタ
	static std::unique_ptr<DebugFont> m_pInstance;



#else
private:
	//オブジェクト
	static DebugFont m_instance;

public:
	//オブジェクトへのポインタを返す
	static DebugFont* GetInstance()
	{
		//オブジェクトへのポインタを返す
		return &m_instance;
	}
	//オブジェクトの参照を返す
	static DebugFont& getRefInstance()
	{
		//オブジェクトへのポインタを返す
		return m_instance;
	}

#endif
private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>  m_spriteFont;
	wchar_t* m_string;
	DirectX::SimpleMath::Vector2 m_pos;//座標

	DirectX::XMVECTOR m_color;			//色

	////override the operator new/delete.
	//void* operator new(size_t i) {
	//	return _mm_malloc(i, 16);
	//}
	//void operator delete(void* p) {
	//	_mm_free(p);
	//}

};