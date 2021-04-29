///////////////////////////////////
//ヒットエフェクトのクラス
//作成者：
//作成日：
///////////////////////////////////

#include "pch.h"
#include "HitEffect.h"
#include "../../BinaryFile.h"

const std::vector<D3D11_INPUT_ELEMENT_DESC> HitEffect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


//コンストラクタ
HitEffect::HitEffect()
{
}

//デストラクタ
HitEffect::~HitEffect()
{
	Finalize();
}

///////////////////////////
//作成
//引数:デバイスリソース、テクスチャ、インプットレイアウト
//戻り値:なし
//////////////////////////
void HitEffect::Create(DX::DeviceResources * deviceresources, ID3D11ShaderResourceView * texture, ID3D11InputLayout * inputLayout)
{
	m_deviceResources = deviceresources;
	auto device = m_deviceResources->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/HitEffectVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/HitEffectPS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/HitEffectGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}


	//const wchar_t* name = L"Resources\\Textures\\shadow.png";
	//DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());

	//const wchar_t* name2 = L"Resources\\Textures\\ball.png";
	//DirectX::CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), name2, nullptr, m_texture2.GetAddressOf());

	m_texture = texture;

	// プリミティブバッチの作成
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_deviceResources->GetD3DDeviceContext());

	m_states = std::make_unique<DirectX::CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_world = DirectX::SimpleMath::Matrix::Identity;
	m_isGravity = false;
}

///////////////////////////
//初期化
//引数:寿命、座標、移動量
//戻り値:なし
//////////////////////////
void HitEffect::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity)
{
	m_startPos = pos;
	m_startVel = velocity;
	m_startLife = life;

	//int seed = 100;
	//m_wait = ((rand() % seed) / (float)seed) * m_startLife ;

	Reset();
	m_gravity = DirectX::SimpleMath::Vector3(0, 0.001f, 0);
}

///////////////////////////
//更新
//引数:タイマー
//戻り値:なし
//////////////////////////
void HitEffect::Update(DX::StepTimer timer)
{
	//処理に使う秒速計(1秒で1.0f)を取得する。
	float time = float(m_timer.GetElapsedSeconds());
	m_timer = timer;

	//座標計算
	m_pos += m_vel;

	//生存時間でリセットする
	m_life--;
	if (m_life < 0)
	{
		//Reset();
	}

	////距離でリセットする
	//Vector3 len = m_position - m_startPosition;
	//if (len.Length() > 3.0f) {
	//	Reset();
	//}



	//重力計算
	if (m_isGravity == true)
	{
		m_vel -= m_gravity;
	}
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void HitEffect::Render()
{
	//ビルボード化
	m_world =
		DirectX::SimpleMath::Matrix::CreateRotationZ(-atan2f(m_vel.y, m_vel.x))*
		DirectX::SimpleMath::Matrix::CreateBillboard(m_pos, m_camera, DirectX::SimpleMath::Vector3::UnitY);

	ShaderDraw();
}

///////////////////////////
//描画
//引数:Z軸の角度
//戻り値:なし
//////////////////////////
void HitEffect::Render(float angleZ, float scale)
{	
	//ビルボード化
	m_world =
		DirectX::SimpleMath::Matrix::CreateRotationZ(angleZ) *
		DirectX::SimpleMath::Matrix::CreateScale(scale) * 
		DirectX::SimpleMath::Matrix::CreateBillboard(m_pos, m_camera, DirectX::SimpleMath::Vector3::UnitY);

	ShaderDraw();

}

///////////////////////////
//リセット
//引数:なし
//戻り値:なし
//////////////////////////
void HitEffect::Reset()
{
	m_pos = m_startPos;
	m_vel = m_startVel;
	m_life = m_startLife;
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void HitEffect::Finalize()
{
	m_CBuffer.Reset();
	m_inputLayout.Reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_texture2.Reset();
	m_VertexShader.Reset();
	m_PixelShader.Reset();
}

///////////////////////////
//ビュー行列、射影行列の設定
//引数:カメラ座標、ビュー行列、射影行列
//戻り値:なし
//////////////////////////
void HitEffect::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;
}

///////////////////////////
//重力の設定
//引数:
//戻り値:
//////////////////////////
void HitEffect::SetGravity(bool isGravity)
{
	m_isGravity = isGravity;
}

///////////////////////////
//シェーダーの描画
//引数:なし
//戻り値:なし
//////////////////////////
void HitEffect::ShaderDraw()
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	DirectX::VertexPositionColorTexture vertex[4] =
	{
		DirectX::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
			DirectX::SimpleMath::Vector4::One, 
			DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
			DirectX::SimpleMath::Vector4::One,
			DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
			DirectX::SimpleMath::Vector4::One,
			DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(
			DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
			DirectX::SimpleMath::Vector4::One,
			DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
	};

	DirectX::VertexPositionColorTexture l_vertex =
		DirectX::VertexPositionColorTexture(m_pos, DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f));

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = m_view.Transpose();
	cbuff.matProj = m_proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(m_pos.x, m_pos.y, m_pos.z, m_life);

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	// カリングは左周り
	context->RSSetState(m_states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	//m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 4);

	m_batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);

}

void HitEffect::Draw()
{
}
