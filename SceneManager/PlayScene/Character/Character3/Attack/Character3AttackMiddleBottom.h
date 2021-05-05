///////////////////////////////////
//キャラクター３の中下攻撃クラス
//作成者：GS305_稲垣颯太
//作成日：2020年11月23日
///////////////////////////////////

#pragma once

#include "../../CharacterAttackBase.h"

class Character3AttackMiddleBottom : public CharacterAttackBase
{
public:
	//コンストラクタ
	Character3AttackMiddleBottom();
	//デストラクタ
	~Character3AttackMiddleBottom();

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
	//発射したフラグ
	bool m_isShot;

	//X座標
	static const float POS_X;
	//消えるまでの時間
	static const float VANISH_TIME;
	//チャージに要する時間
	static const float CHARAGE_TIME;

};