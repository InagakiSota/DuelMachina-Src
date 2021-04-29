///////////////////////////////////
//キャラクター２の攻撃マネージャークラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月19日
///////////////////////////////////

#pragma once

#include "../../CharacterAttackManagerBase.h"

class CharacterAttackBase;

class Character2AttackManager : public CharacterAttackManagerBase
{
public:
	//コンストラクタ
	Character2AttackManager();
	//デストラクタ
	~Character2AttackManager();

	//初期化
	void Initialize(CharacterBase* pCharacter, DX::DeviceResources* pDeviceResources) override;
	//更新
	void Update() override;
	//描画
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	//終了処理
	void Finalize() override;
	//リセット
	void Reset() override;

};
