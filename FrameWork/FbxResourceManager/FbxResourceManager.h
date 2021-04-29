///////////////////////////////////
//Fbxのリソース管理クラス
//作成者：GS3_05_稲垣
//作成日：2021年3月30日
///////////////////////////////////

#pragma once

class FbxModel;
enum class eCHARACTER_ID :int;

#include "DeviceResources.h"

class FbxResourceManager
{
public:
	//インスタンスの取得
	static FbxResourceManager* GetInstance();
	//インスタンスの解放
	static void ReleaseInstance();

	//ロード
	void Load(DX::DeviceResources* pDeviceResources, HWND window);
	//モデルのポインタ取得
	FbxModel* GetModel(const eCHARACTER_ID& characterID);
	//シールドのモデルのポインタ取得
	FbxModel* GetShieldModel();
	//終了処理
	void Finalize();

private:
	//自身のインスタンス
	static FbxResourceManager* m_pMyInstance;

	//コンストラクタ
	FbxResourceManager();
	//デストラクタ
	~FbxResourceManager();

	//キャラクター１のモデル
	FbxModel* m_pCharacter1Model;
	//キャラクター２のモデル
	FbxModel* m_pCharacter2Model;
	//キャラクター３のモデル
	FbxModel* m_pCharacter3Model;
	//シールドのモデル
	FbxModel* m_pShieldModel;
};
