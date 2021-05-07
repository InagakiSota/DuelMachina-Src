///////////////////////////////////
//キャラクター2の定数管理クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年12月18日
///////////////////////////////////

#include "pch.h"
#include "Character2Params.h"
#include <fstream>
#include <sstream>

Character2Params* Character2Params::m_pMyInstance = nullptr;

//体の当たり判定のサイズ(通常時)
const DirectX::SimpleMath::Vector3 Character2Params::BODY_COLL_SIZE_NORMAL = DirectX::SimpleMath::Vector3(0.3f, 1.02f, 0.3f);
//体の当たり判定のサイズ(しゃがみ)
const DirectX::SimpleMath::Vector3 Character2Params::BODY_COLL_SIZE_SQUAT = DirectX::SimpleMath::Vector3(0.3f, 0.75f, 0.3f);
//足の当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character2Params::LEG_COLL_SIZE = DirectX::SimpleMath::Vector3(0.25f, 0.25f, 0.25f);
//頭の当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character2Params::HEAD_COLL_SIZE = DirectX::SimpleMath::Vector3(0.25, 0.15f, 0.25f);
//前方向のベクトル
const DirectX::SimpleMath::Vector3 Character2Params::FRONT_VECTOR = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);


//モデルのサイズ
const float Character2Params::MODEL_SIZE = 0.015f;
//Y軸の角度
const float Character2Params::ANGLE_Y = 50.0f;

//カメラ座標
const DirectX::SimpleMath::Vector3 Character2Params::CAMERA_POS = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);

//移動できるX座標の最大値
const float Character2Params::MOVE_LIMIT_X = 3.0f;
//移動できるY座標の最大値
const float Character2Params::MOVE_LIMIT_Y = 2.5f;

//ブースト移動の体の当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character2Params::MOVE_BOOST_BODY_SIZE = DirectX::SimpleMath::Vector3(0.20f, 0.4f, 0.3f);

//シールドのY座標
const float Character2Params::SHIELD_POS_Y = 0.7f;

//ブーストエフェクトのX座標
const float Character2Params::BOOST_EFFECT_POS_X = 0.8f;
//ブーストエフェクトのY座標
const float Character2Params::BOOST_EFFECT_POS_Y = -0.5f;
//ブーストエフェクトの角度(左側)	
const float Character2Params::BOOST_EFFECT_ANGLE_LEFT = 190.0f;
//ブーストエフェクトの角度(右側)
const float Character2Params::BOOST_EFFECT_ANGLE_RIGHT = -10.0f;
//ヒットエフェクトのY座標
const float Character2Params::HIT_EFFECT_POS_Y = -0.9f;


//コンストラクタ
Character2Params::Character2Params()
{
}

//デストラクタ
Character2Params::~Character2Params()
{
}


Character2Params * Character2Params::GetInstance()
{
	if (m_pMyInstance == nullptr)
	{
		m_pMyInstance = new Character2Params();
	}

	return m_pMyInstance;
}

void Character2Params::ReleaseInstance()
{
	if (m_pMyInstance != nullptr)
	{
		delete m_pMyInstance;
		m_pMyInstance = nullptr;
	}
}

void Character2Params::LoadStatusData()
{
	std::wstring str;

	//ファイルのオープン
	std::wifstream ifs(L"CharacterStatusData\\Character2_StatusData.csv");

	//ヘッダ文字列
	std::wstring headName = L"HP";

	//ヘッダ部分までシーク
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, headName.size(), headName) == 0)
		{
			break;
		}
	}

	//体力の読み込み
	getline(ifs, str);
	std::wstring tmp1;
	std::wistringstream stream1(str);
	std::getline(stream1, tmp1);
	MAX_HP = std::stoi(tmp1);

	//ヘッダ文字列を変える
	headName = L"BOOST_CAP";
	//ファイルポインタを先頭へシーク
	ifs.clear();
	ifs.seekg(0);

	//ヘッダ部分までシーク
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, headName.size(), headName) == 0)
		{
			break;
		}
	}

	//ブースト容量の読み込み
	getline(ifs, str);
	std::wstring tmp2;
	std::wistringstream stream2(str);
	std::getline(stream2, tmp2);
	BOOST_CAP_MAX = std::stoi(tmp2);

	//ヘッダ文字列を変える
	headName = L"JUMP";
	//ファイルポインタを先頭へシーク
	ifs.clear();
	ifs.seekg(0);

	//ヘッダ部分までシーク
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, headName.size(), headName) == 0)
		{
			break;
		}
	}

	//ジャンプ力の読み込み
	getline(ifs, str);
	std::wstring tmp3;
	std::wistringstream stream3(str);
	std::getline(stream3, tmp3);
	JUMP_FORCE = std::stof(tmp3);

	//ヘッダ文字列を変える
	headName = L"MOVE_FRONT";
	//ファイルポインタを先頭へシーク
	ifs.clear();
	ifs.seekg(0);

	//ヘッダ部分までシーク
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, headName.size(), headName) == 0)
		{
			break;
		}
	}

	//前移動の速度の読み込み
	getline(ifs, str);
	std::wstring tmp4;
	std::wistringstream stream4(str);
	std::getline(stream4, tmp4);
	MOVE_FRONT_FORCE = std::stof(tmp4);

	//ヘッダ文字列を変える
	headName = L"MOVE_BACK";
	//ファイルポインタを先頭へシーク
	ifs.clear();
	ifs.seekg(0);

	//ヘッダ部分までシーク
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, headName.size(), headName) == 0)
		{
			break;
		}
	}

	//後移動の速度の読み込み
	getline(ifs, str);
	std::wstring tmp5;
	std::wistringstream stream5(str);
	std::getline(stream5, tmp5);
	MOVE_BACK_FORCE = std::stof(tmp5);

	//ヘッダ文字列を変える
	headName = L"MOVE_BOOST";
	//ファイルポインタを先頭へシーク
	ifs.clear();
	ifs.seekg(0);

	//ヘッダ部分までシーク
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, headName.size(), headName) == 0)
		{
			break;
		}
	}

	//ブースト移動の速度の読み込み
	getline(ifs, str);
	std::wstring tmp6;
	std::wistringstream stream6(str);
	std::getline(stream6, tmp6);
	MOVE_BOOST_FORCE = std::stof(tmp6);


	//ファイルのクローズ
	ifs.close();

}
