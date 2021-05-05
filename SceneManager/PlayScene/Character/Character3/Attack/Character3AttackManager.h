///////////////////////////////////
//キャラクター３の攻撃管理クラス
//作成者：GS305_稲垣颯太
//作成日：2020年11月22日
///////////////////////////////////

#pragma once

#include "../../CharacterAttackManagerBase.h"

class Character3AttackManager : public CharacterAttackManagerBase
{
public:
	//コンストラクタ
	Character3AttackManager();
	//デストラクタ
	~Character3AttackManager();

	//初期化
	void Initialize(CharacterBase* pCharacter) override;
	//更新
	void Update() override;
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	//終了処理
	void Finalize() override;
	//リセット
	void Reset() override;
};