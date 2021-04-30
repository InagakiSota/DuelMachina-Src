//////////////////////////
//プレイシーンのUIクラス
//作成日：2021年4月30日
//作成者：GT2_04_稲垣颯太
/////////////////////////

#pragma once

class PlaySceneUI
{
public:
	//コンストラクタ
	PlaySceneUI();
	//デストラクタ
	~PlaySceneUI();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Render();
	//終了処理
	void Finalize();

private:
	//プレイヤーの数
	static const int PLAYER_NUM = 2;


};
