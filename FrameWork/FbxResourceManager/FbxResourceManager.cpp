///////////////////////////////////
//Fbxのリソース管理クラス
//作成者：GS3_05_稲垣
//作成日：2021年3月30日
///////////////////////////////////

#include "pch.h"
#include "FbxResourceManager.h"
#include "../FbxModel/FbxModel.h"
#include "../../SceneManager/PlayScene/Character/CharacterID.h"


FbxResourceManager* FbxResourceManager::m_pMyInstance = nullptr;

///////////////////////////
//インスタンスの取得
//引数:なし
//戻り値:自身のポインタ
//////////////////////////
FbxResourceManager* FbxResourceManager::GetInstance()
{
	if (m_pMyInstance == nullptr)
	{
		m_pMyInstance = new FbxResourceManager();
	}

	return m_pMyInstance;
}

///////////////////////////
//インスタンスの解放
//引数:なし
//戻り値:なし
//////////////////////////
void FbxResourceManager::ReleaseInstance()
{
	if (m_pMyInstance != nullptr)
	{
		delete m_pMyInstance;
	}
	m_pMyInstance = nullptr;
}

///////////////////////////
//ロード
//引数:なし
//戻り値:なし
//////////////////////////
void FbxResourceManager::Load(DX::DeviceResources* pDeviceResources, HWND window)
{
	//キャラクター１のモデル読み込み
	m_pCharacter1Model = new FbxModel();
	m_pCharacter1Model->Load(
		window,
		pDeviceResources->GetD3DDevice(),
		pDeviceResources->GetD3DDeviceContext(),
		pDeviceResources->GetRenderTargetView(),
		"Resources/Models/robot1/robot1_animation3.fbx",
		true,
		0
	);

	//キャラクター２のモデル読み込み
	m_pCharacter2Model = new FbxModel();
	m_pCharacter2Model->Load(
		window,
		pDeviceResources->GetD3DDevice(),
		pDeviceResources->GetD3DDeviceContext(),
		pDeviceResources->GetRenderTargetView(),
		"Resources/Models/robot2/robot2_animation.fbx",
		true
	);

	//キャラクター３のモデル読み込み
	m_pCharacter3Model = new FbxModel();
	m_pCharacter3Model->Load(
		window,
		pDeviceResources->GetD3DDevice(),
		pDeviceResources->GetD3DDeviceContext(),
		pDeviceResources->GetRenderTargetView(),
		"Resources/Models/robot3/robot3_animation.fbx",
		true
	);

	//シールドのモデル読み込み
	m_pShieldModel = new FbxModel();
	m_pShieldModel->Load(
		window,
		pDeviceResources->GetD3DDevice(),
		pDeviceResources->GetD3DDeviceContext(),
		pDeviceResources->GetRenderTargetView(),
		"Resources/Models/Shield.fbx",
		false
	);

}

///////////////////////////
//モデルのポインタの取得
//引数:モデルのID
//戻り値:モデルのポインタ
//////////////////////////
FbxModel * FbxResourceManager::GetModel(const eCHARACTER_ID & characterID)
{
	FbxModel* pModel = nullptr;

	switch (characterID)
	{
		//キャラクター１
		case eCHARACTER_ID::CHARACTER_1:
		{
			if (m_pCharacter1Model != nullptr)
			{
				pModel = m_pCharacter1Model;
			}
			break;
		}
		//キャラクター２
		case eCHARACTER_ID::CHARACTER_2:
		{
			if (m_pCharacter2Model != nullptr)
			{
				pModel = m_pCharacter2Model;
			}
			break;
		}
		//キャラクター３
		case eCHARACTER_ID::CHARACTER_3:
		{
			if (m_pCharacter3Model != nullptr)
			{
				pModel = m_pCharacter3Model;
			}
			break;
		}

		default:
			break;
	}
	return pModel;
}

///////////////////////////
//シールドのモデルの取得
//引数:なし
//戻り値:シールドのモデルのポインタ
//////////////////////////
FbxModel * FbxResourceManager::GetShieldModel()
{
	return m_pShieldModel;
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void FbxResourceManager::Finalize()
{
	delete m_pCharacter1Model;
	m_pCharacter1Model = nullptr;

	delete m_pCharacter2Model;
	m_pCharacter2Model = nullptr;

	delete m_pCharacter3Model;
	m_pCharacter3Model = nullptr;
}

//コンストラクタ
FbxResourceManager::FbxResourceManager()
	:m_pCharacter1Model(nullptr),
	m_pCharacter2Model(nullptr),
	m_pCharacter3Model(nullptr)
{
}

//デストラクタ
FbxResourceManager::~FbxResourceManager()
{
	delete m_pCharacter1Model;
	m_pCharacter1Model = nullptr;

	delete m_pCharacter2Model;
	m_pCharacter2Model = nullptr;

	delete m_pCharacter3Model;
	m_pCharacter3Model = nullptr;

}
