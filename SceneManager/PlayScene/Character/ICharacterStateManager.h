///////////////////////////////
//キャラクターの状態マネージャーのインターフェースクラス
//作成日：2021年5月4日
//作成者：GT2_04_稲垣颯太
//////////////////////////////


#pragma once

class CharacterBase;
enum class eCHARACTER_STATE;

class ICharacterStateManager
{
public:

	//デストラクタ
	virtual ~ICharacterStateManager() {};

	//初期化
	virtual void Initialize(CharacterBase* pCharacterBase) = 0;
	//更新
	virtual void Update(const eCHARACTER_STATE& charaState) = 0;
	//終了処理
	virtual void Finalize() = 0;

};
