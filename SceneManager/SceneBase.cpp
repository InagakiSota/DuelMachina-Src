///////////////////////////////////
//シーンの基底クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年6月19日
///////////////////////////////////
#include "pch.h"
#include "SceneBase.h"

//コンストラクタ
SceneBase::SceneBase()
	:m_pDeviceResources(nullptr),
	m_pStates(nullptr),
	m_pKeyTracker(nullptr),
	m_fadeTimer(1.0f),
	m_sceneState(eSCENE_STATE::FADE_IN),
	m_view{},
	m_proj{},
	m_cameraPos{}
{
}

//デストラクタ
SceneBase::~SceneBase()
{
}

///////////////////////////
//初期化
//引数:なし
//戻り値:なし
//////////////////////////
void SceneBase::Initialize()
{
}

///////////////////////////
//更新
//引数:ステップタイマー
//戻り値:なし
//////////////////////////
void SceneBase::Update(DX::StepTimer const& timer)
{
}

///////////////////////////
//描画
//引数:なし
//戻り値:なし
//////////////////////////
void SceneBase::Render()
{
}

///////////////////////////
//終了処理
//引数:なし
//戻り値:なし
//////////////////////////
void SceneBase::Finalize()
{
}
