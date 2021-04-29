///////////////////////////////////
//Fbxファイルの表示クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年8月7日
///////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include <SimpleMath.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fbxsdk.h>
#include <Windows.h>
#include <map>
#include <CommonStates.h>

#pragma comment(lib, "d3dcompiler.lib")

class FbxModel
{
public:
	//1つの頂点情報を格納する構造体
	struct Vertex
	{
		DirectX::SimpleMath::Vector3 pos;
		DirectX::SimpleMath::Vector3 normal;
		DirectX::SimpleMath::Vector3 texturePos;
		int* indexBuffer;
		int uvSetCount;
		
	};

	//シェーダー側へ送る数値をまとめた構造体
	struct ConstantBuffer
	{
		DirectX::XMMATRIX mW;
		DirectX::XMMATRIX mWVP;
		DirectX::XMVECTOR lightPos;
		DirectX::XMVECTOR eyePos;
		DirectX::XMVECTOR color;
		DirectX::XMVECTOR texture;
	};

	FbxModel();
	~FbxModel();

	//ファイルの読み込み
	bool Load(HWND window, ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView,
		const char* fileName, bool isAnimation = false, int animStackNumber = 0);

	//描画
	void Draw(
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Vector4 color = DirectX::SimpleMath::Vector4(1.0f,1.0f,1.0f,1.0f));

	//アニメーションの切り替え
	void ChangeAnimation(const int& animeStack);

private:
	ID3D11RasterizerState* m_pRasterizerState;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;

	std::vector<Vertex> m_vertices;

	ID3D11Buffer* m_pVerBuffer;
	ID3D11Buffer* m_pIndBuffer;

	int m_AnimStackNumber;
	FbxTime m_frameTime;
	FbxTime m_timeCount;
	FbxTime m_start;
	FbxTime m_stop;

	HWND m_window;

	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
	IDXGISwapChain* m_pSwapChain;

	//メッシュの名前
	std::vector<std::string> m_fbxMeshNameArray;			
	std::vector<FbxMesh*> m_pfbxMeshArray;

	//テクスチャ
	std::vector<FbxFileTexture*> m_fbxTextureArray;
	//テクスチャのファイルパス
	std::vector<std::string> m_fbxTextureFilePath;
	//ファイルパスのキャラ
	std::vector<char> m_texturePathChar;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	ID3D11RenderTargetView* m_pRenderTargetView;

	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_pStates;
	
	//ビューポートの設定
	D3D11_VIEWPORT m_vp;			

	bool m_isAnimation;

	//色
	DirectX::SimpleMath::Vector4 m_color;

	//アニメーション用の描画
	bool DrawAnimation(FbxNode* pNode,
		DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//通常の描画
	bool DrawModel(FbxNode* pNode,
		DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);




};

