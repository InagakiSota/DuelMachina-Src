///////////////////////////////////
//キャラクター3の定数管理クラス
//作成者：GS305_稲垣颯太
//作成日：2020年12月18日
///////////////////////////////////

#include "pch.h"
#include "Character3Params.h"
#include <fstream>
#include <sstream>

Character3Params* Character3Params::m_pMyInstance = nullptr;


//体のサイズ
const float Character3Params::BODY_SIZE = 0.015f;
//Y軸の角度
const float Character3Params::ANGLE_Y = 30.0f;
//移動できるX座標の最大値
const float Character3Params::MOVE_LIMIT_X = 3.0f;
//移動できるY座標の最大値
const float Character3Params::MOVE_LIMIT_Y = 2.5f;
//体の当たり判定のサイズ(通常時)
const DirectX::SimpleMath::Vector3 Character3Params::BODY_COLL_SIZE_NORMAL = DirectX::SimpleMath::Vector3(0.40f, 1.0f, 0.30f);
//体の当たり判定のサイズ(しゃがみ)
const DirectX::SimpleMath::Vector3 Character3Params::BODY_COLL_SIZE_SQUAT = DirectX::SimpleMath::Vector3(0.40f, 0.75f, 0.30f);
//足の当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character3Params::LEG_COLL_SIZE = DirectX::SimpleMath::Vector3(0.35f, 0.25f, 0.25f);
//頭の当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character3Params::HEAD_COLL_SIZE = DirectX::SimpleMath::Vector3(0.35f, 0.15f, 0.25f);
//前方向のベクトル
const DirectX::SimpleMath::Vector3 Character3Params::FRONT_VECTOR = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
//カメラ座標
const DirectX::SimpleMath::Vector3 Character3Params::CAMERA_POS = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);
//シールドのY座標
const float Character3Params::SHIELD_POS_Y = 1.3f;

//ブーストエフェクトのX座標
const float Character3Params::BOOST_EFFECT_POS_X = 0.8f;
//ブーストエフェクトのY座標
const float Character3Params::BOOST_EFFECT_POS_Y = -1.1f;
//ブーストエフェクトの角度(左側)	
const float Character3Params::BOOST_EFFECT_ANGLE_LEFT = 190.0f;
//ブーストエフェクトの角度(右側)
const float Character3Params::BOOST_EFFECT_ANGLE_RIGHT = -10.0f;


//コンストラクタ
Character3Params::Character3Params()
{
}

//デストラクタ
Character3Params::~Character3Params()
{
}


Character3Params * Character3Params::GetInstance()
{
	if (m_pMyInstance == nullptr)
	{
		m_pMyInstance = new Character3Params();
	}

	return m_pMyInstance;
}

void Character3Params::ReleaseInstance()
{
	if (m_pMyInstance != nullptr)
	{
		delete m_pMyInstance;
		m_pMyInstance = nullptr;
	}
}

void Character3Params::LoadStatusData()
{
	std::wstring str;

	//ファイルのオープン
	std::wifstream ifs(L"CharacterStatusData\\Character3_StatusData.csv");

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
