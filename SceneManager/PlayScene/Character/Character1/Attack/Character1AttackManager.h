///////////////////////////////////
//キャラクター１の攻撃マネージャークラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年11月13日
///////////////////////////////////

#pragma once

#include "../../CharacterAttackManagerBase.h"

class CharacterAttackBase;

class Character1AttackManager : public CharacterAttackManagerBase
{
public:
	//コンストラクタ
	Character1AttackManager();
	//デストラクタ
	~Character1AttackManager();

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

private:
	std::unique_ptr<CharacterAttackBase> m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM)];


};


