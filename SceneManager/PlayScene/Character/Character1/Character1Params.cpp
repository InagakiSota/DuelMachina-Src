///////////////////////////////////
//キャラクター1の定数管理クラス
//作成者：GS3_05_稲垣颯太
//作成日：2020年12月18日
///////////////////////////////////

#include "pch.h"
#include "Character1Params.h"
#include <fstream>
#include <sstream>

Character1Params* Character1Params::m_pMyInstance = nullptr;

//体の当たり判定のサイズ(通常時)
const DirectX::SimpleMath::Vector3 Character1Params::BODY_COLL_SIZE_NORMAL = DirectX::SimpleMath::Vector3(0.30f, 1.02f, 0.3f);
//足の当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character1Params::LEG_COLL_SIZE = DirectX::SimpleMath::Vector3(0.25f, 0.25f, 0.25f);
//頭の当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character1Params::HEAD_COLL_SIZE = DirectX::SimpleMath::Vector3(0.25f, 0.15f, 0.25f);
//前方向のベクトル
const DirectX::SimpleMath::Vector3 Character1Params::FRONT_VECTOR = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);

//体のサイズ
const float Character1Params::BODY_SIZE = 0.003f;
//Y軸の角度
const float Character1Params::ANGLE_Y = 90.0f;

//カメラ座標
const DirectX::SimpleMath::Vector3 Character1Params::CAMERA_POS = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);


//移動できるX座標の最大値
const float Character1Params::MOVE_LIMIT_X = 3.0f;
//移動できるY座標の最大値
const float Character1Params::MOVE_LIMIT_Y = 2.5f;

//体の当たり判定のサイズ
const DirectX::SimpleMath::Vector3 Character1Params::MOVE_BOOST_BODY_SIDE = DirectX::SimpleMath::Vector3(0.30f, 0.5f, 0.2f);

//体の当たり判定のサイズ(しゃがみ)
const DirectX::SimpleMath::Vector3 Character1Params::BODY_COLL_SIZE_SQUAT = DirectX::SimpleMath::Vector3(0.30f, 0.78f, 0.3f);

//ブーストエフェクトのX座標
const float Character1Params::BOOST_EFFECT_POS_X = 0.8f;
//ブーストエフェクトのY座標
const float Character1Params::BOOST_EFFECT_POS_Y = 0.5f;
//ブーストエフェクトの角度(左側)	
const float Character1Params::BOOST_EFFECT_ANGLE_LEFT = 190.0f;
//ブーストエフェクトの角度(右側)
const float Character1Params::BOOST_EFFECT_ANGLE_RIGHT = -10.0f;


//コンストラクタ
Character1Params::Character1Params()
{
}

//デストラクタ
Character1Params::~Character1Params()
{
}


Character1Params * Character1Params::GetInstance()
{
	if (m_pMyInstance == nullptr)
	{
		m_pMyInstance = new Character1Params();
	}

	return m_pMyInstance;
}

void Character1Params::ReleaseInstance()
{
	if (m_pMyInstance != nullptr)
	{
		delete m_pMyInstance;
		m_pMyInstance = nullptr;
	}
}

void Character1Params::LoadStatusData()
{
	std::wstring str;

	//ファイルのオープン
	std::wifstream ifs(L"CharacterStatusData\\Character1_StatusData.csv");

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
