///////////////////////////////////
//デバッグフォント
//作成者：GS3_05_稲垣颯太
//作成日：2020年5月20日
///////////////////////////////////

#include  "pch.h"

#include "DebugFont.h"

#if DEBUG_FONT_SINGLETON_TYPE == 0
DebugFont* DebugFont::m_pInstance = nullptr;
#elif DEBUG_FONT_SINGLETON_TYPE == 1
std::unique_ptr<DebugFont>DebugFont::m_pInstance = nullptr;
#else
DebugFont DebugFont::m_instance;
#endif

//コンストラクタ
DebugFont::DebugFont()
	:m_pos(0, 0),
	m_string(nullptr),
	m_color()
{
	m_string = new wchar_t[STRING_SIZE_MAX];
}

//デストラクタ
DebugFont::~DebugFont()
{
	delete[] m_string;

	m_spriteBatch.reset();
	m_spriteFont.reset();
}

///////////////////////////
//更新
//引数:device,context
//戻り値:なし
//////////////////////////
void DebugFont::Create(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	
	if (m_spriteBatch == nullptr)m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	if (m_spriteFont == nullptr)m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Font/SegoeUI_18.spritefont");

}


///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void DebugFont::Draw()
{
	m_spriteBatch->Begin();
	m_spriteFont->DrawString(m_spriteBatch.get(), m_string, m_pos, m_color);
	m_spriteBatch->End();

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void DebugFont::Reset()
{
	m_spriteBatch.reset();
	m_spriteFont.reset();
}

///////////////////////////
//表示情報登録
//引数:座標、文字列、色
//戻り値:なし
//////////////////////////
void DebugFont::Print(float posX, float posY, wchar_t const * const format, DirectX::XMVECTOR color, ...)
{
	m_pos.x = posX;
	m_pos.y = posY;


	m_color = color;

	va_list arg_ptr;

	va_start(arg_ptr, format);
	vswprintf(m_string, STRING_SIZE_MAX, format, arg_ptr);
	va_end(arg_ptr);
	

}

///////////////////////////
//表示情報登録
//引数:座標、文字列、色
//戻り値:なし
//////////////////////////
void DebugFont::Print(DirectX::SimpleMath::Vector2 pos, wchar_t const * const format, DirectX::XMVECTOR color, ...)
{
	m_pos = pos;

	m_color = color;


	va_list arg_ptr;

	va_start(arg_ptr, format);
	vswprintf(m_string, STRING_SIZE_MAX, format, arg_ptr);
	va_end(arg_ptr);

}

