///////////////////////////////////
//DirectXTKで描画の際に必要なインスタンスを管理するクラス
//作成者：2021年4月30日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////
#pragma once

#include "DeviceResources.h"
#include <CommonStates.h>
#include <SimpleMath.h>

// 簡単に呼び出せるように定義しておく
#define gdi Cgdi::GetInstance()

class Cgdi
{
public:

	//デストラクタ
	virtual ~Cgdi();

	//自身のインスタンスの取得
	static Cgdi* GetInstance();

	//初期化
	void Initialize(DX::DeviceResources* deviceResources, float width, float height);

	// スクリーンサイズを設定する関数
	void SetScreenSize(float width, float height);

	// スクリーンサイズを取得する関数
	void GetScreenSize(float& width, float& height);

	// デバイスリソースを取得する関数
	DX::DeviceResources* GetDeviceResources() const 
	{
		return m_deviceResources; 
	}

	// コモンステートを取得する関数
	DirectX::CommonStates* GetStates() const
	{
		return m_states.get(); 
	}



private:

	// デバイスリソース管理インスタンスへのポインタ
	DX::DeviceResources* m_deviceResources;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// スクリーンサイズ
	float m_screenW, m_screenH;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;

	// コンストラクタ
	Cgdi();

	void operator=(const Cgdi& obj) = delete;	// 代入演算子削除
	Cgdi(const Cgdi& obj) = delete;				// コピーコンストラクタ削除




};
