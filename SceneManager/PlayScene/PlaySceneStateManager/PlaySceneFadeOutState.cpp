///////////////////////////////////
//プレイシーンのフェードアウトステート
//作成者：2021年5月10日
//作成日：GT2_04_稲垣颯太
///////////////////////////////////

#include "pch.h"
#include "PlaySceneFadeOutState.h"
#include "Adx2.h"
#include "CueSheet_0.h"
//コンストラクタ
PlaySceneFadeOutState::PlaySceneFadeOutState()
	:m_pPlayScene(nullptr)
{
}

//デストラクタ
PlaySceneFadeOutState::~PlaySceneFadeOutState()
{
}

///////////////////////////
//更新
//引数:なし
//戻り値:なし
//////////////////////////
void PlaySceneFadeOutState::Update(DX::StepTimer const& timer)
{
}
