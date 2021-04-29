///////////////////////////////////
//�v���C�V�[���̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N6��19��
///////////////////////////////////

#include "pch.h"
#include "PlayScene.h"


#include "../../FrameWork/EffectManager/EffectManager.h"
#include "../../FrameWork/FbxModel/FbxModel.h"
#include "../../FrameWork/Sprite2D/Sprite2D.h"
#include "../../FrameWork/DebugFont/DebugFont.h"
#include "../../FrameWork/ModelObject/ModelObject.h"
#include "../../Fbx.h"
#include "../../FrameWork/DebugCamera/DebugCamera.h"
#include "Character\CharacterFactory.h"
#include "Character\CharacterBase.h"
#include "Character\CharacterID.h"
#include "../PlayerID.h"

#include "Keyboard.h"
#include "../SceneManager.h"
#include "AttackManager/AttackManager.h"

#include "../ResultScene/ResultScene.h"

#include "../../FrameWork/FbxModel/FbxModel.h"
#include "Character\ShadowManager.h"

#include "Character\HitEffectManager.h"
#include "Src\FrameWork\FadeManager\FadeManager.h"

#include "Adx2.h"
#include "CueSheet_0.h"

#include "../../FrameWork/LoadDisplay/LoadDisplay.h"

//�̗̓o�[�̉���
const float PlayScene::HP_BAR_WIDTH =800.0f;
//�̗̓o�[�̍���
const float PlayScene::HP_BAR_HEIGHT = 101.0f;
//�̗̓o�[�̃}�[�W��
const float PlayScene::HP_BAR_MARGIN = 20.0f;

//�u�[�X�g�e�ʃo�[�̉���
const float PlayScene::BOOST_BAR_WIDTH = 700.0f;
//�̗̓o�[�̍���
const float PlayScene::BOOST_BAR_HEIGHT = 70.0f;

//�J�E���g�_�E���̎���
const float PlayScene::COUNT_DOWN_TIME = 3.0f;
//�J�E���g�_�E���摜�̍��W
const DirectX::SimpleMath::Vector2 PlayScene::COUNT_DOWN_SPRITE_POS = DirectX::SimpleMath::Vector2(640.0f,300.0f);
 //�������Ԃ̍ő�l
 const float PlayScene::TIME_MAX = 60;
 //�^�C���A�b�v�V�[���̎���
 const float PlayScene::TIME_UP_TIME = 3.0f;
 //���E���h�؂�ւ��̎���
 const float PlayScene::ROUND_CHANGE_TIME = 2.0f;
 //�����{���̉摜�̉���
 const float PlayScene::WIN_NUM_SPRITE_WIDTH = 100.0f;
 //�����{���̉摜�̍���
 const float  PlayScene::WIN_NUM_SPRITE_HEIGHT = 50.0f;
 //�������Ԃ̉摜�̉���
 const float PlayScene::TIME_SPRITE_WIDTH = 100.0f;
 //�������Ԃ̉摜�̍���
 const float PlayScene::TIME_SPRITE_HEIGHT = 200.0f;
 //�������Ԃ̉摜��X���W
 const float PlayScene::TIME_SPRITE_POS_X = 960.0f;

 //��������̉摜�̉���
 const float PlayScene::MANUAL_SPRITE_WIDTH = 1920.0f;
 //��������̉摜�̍���
 const float PlayScene::MANUAL_SPRITE_HEIGHT = 1080.0f;
 //���U���g��pushSpace�̉摜�̍��W
 const DirectX::SimpleMath::Vector2  PlayScene::PUSH_SPACE_RESULT_POS = 
	 DirectX::SimpleMath::Vector2(1300.0f,800.0f);

 //�J�[�\���̍��W
 const DirectX::SimpleMath::Vector2 PlayScene::MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::OVER_ID)] =
 {
	DirectX::SimpleMath::Vector2(630.0f,300.0f),
	DirectX::SimpleMath::Vector2(630.0f,500.0f),
	DirectX::SimpleMath::Vector2(630.0f,700.0f)
 };

 //�̗̓o�[�̍��W
 const DirectX::SimpleMath::Vector2 PlayScene::HP_BAR_POS[PlayScene::PLAYER_NUM] =
 {
	 DirectX::SimpleMath::Vector2(20.0f,50.0f),
	 DirectX::SimpleMath::Vector2(1900.0f - HP_BAR_WIDTH,50.0f)
 };

 //�̗̓o�[�̍��W
 const DirectX::SimpleMath::Vector2 PlayScene::BOOST_BAR_POS[PlayScene::PLAYER_NUM] =
 {
	 DirectX::SimpleMath::Vector2(20.0f,900.0f),
	 DirectX::SimpleMath::Vector2(1900.0f - BOOST_BAR_WIDTH,900.0f)
 };


//�R���X�g���N�^
PlayScene::PlayScene()
	:m_floorBox{},
	m_pDebugCamera(nullptr),
	m_pFloor(nullptr),
	m_floorWorld{},
	m_space(nullptr),
	m_spaceWorld{},
	//m_pAttackManager(nullptr),
	m_hpBarPos{},
	m_timeSpriteOneRect{},
	m_timeSpriteTenRect{},
	m_pTimeSpriteOne(nullptr),
	m_pTimeSpriteTen(nullptr),
	m_pShadowManager{}
{
	m_pDeviceResources = nullptr;
	m_pKeyTracker = nullptr;

	m_sceneState = eSCENE_STATE::FADE_IN;
	m_countdownTimer = 0.0f;
	m_cameraPos.Zero;
	m_winSpritePos.Zero;
	m_fadeTimer = 1.0f;

	m_isStop = false;
	m_isManualUp = m_isManualDown = m_isManualRight = m_isManualLeft = false;
	m_isManualDisplay = false;
	m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);
	m_isResult = false;
}

//�f�X�g���N�^
PlayScene::~PlayScene()	
{
	//�O�̂��ߏI������
	//Finalize();

	m_pFloor.reset();
	//m_pFbx.reset();
	m_pDebugCamera.reset();
	m_space.reset();
	m_sprite2D.reset();

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pPlayer[i] = nullptr;
		m_pHPBar[i].reset();
		m_pHPBar[i] = nullptr;
		m_pHPBarBack[i].reset();
		m_pHPBarBack[i] = nullptr;
		m_pWinSprite[i].reset();
		m_pWinSprite[i] = nullptr;
		m_pShadowManager[i].reset();
	}

	//�U���}�l�[�W���[�̃C���X�^���X�̉��
	AttackManager::ReleaseInstance();


	m_pTimeSpriteOne.reset();
	m_pTimeSpriteTen.reset();


	m_pDeviceResources = nullptr;
}

///////////////////////////
//������
//����:�f�o�C�X���\�[�X�̃|�C���^
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Initialize(HWND window, DX::DeviceResources* pDeviceResources, DirectX::CommonStates* pStates)
{
	m_pDeviceResources = pDeviceResources;
	m_pStates = pStates;

	ID3D11Device1* device = m_pDeviceResources->GetD3DDevice();

	// TODO: Initialize device dependent objects here (independent of window size).
	device;

	auto context = m_pDeviceResources->GetD3DDeviceContext();


	m_spaceWorld = DirectX::SimpleMath::Matrix::Identity;

	//�r���[�s����쐬����
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

	//��ʃT�C�Y���擾����
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//�ˉe�s������
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);
	
	//m_pDebugCamera = std::make_unique<DebugCamera>();

	//�V�[���̃X�e�[�g�̏�����
	m_sceneState = eSCENE_STATE::FADE_IN;


	////�}�E�X�̍쐬
	//m_pMouse = std::make_unique<Mouse>();
	//m_pMouse->SetWindow(window);

	////�L�[�{�[�h�̍쐬
	//m_pKeyboard = std::make_unique<Keyboard>();


	//m_pFbx = std::make_unique<Fbx>();
	//m_pFbx->Load(
	//	window,
	//	m_pDeviceResources->GetD3DDevice(),
	//	m_pDeviceResources->GetD3DDeviceContext(),
	//	m_pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/robot_maya9.fbx",
	//	true);

	//�U���̃}�l�[�W���[�̍쐬
	//m_pAttackManager = std::make_unique<AttackManager>();
	//�U���̃}�l�[�W���[�̏�����
	AttackManager::GetInstance()->Initialize(pDeviceResources);

	m_space = std::make_unique<ModelObject>();
	m_space->Create(m_pDeviceResources, L"Resources/Models/Space.cmo");

	//�̗̓o�[�A�u�[�X�g�e�ʃo�[�̓ǂݍ���
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//�̗̓o�[
		m_pHPBar[i] = std::make_unique<Sprite2D>();
		m_pHPBar[i]->Create(device, context, L"Resources/Textures/HPBar.png", m_pStates);

		//�̗̓o�[(�̗͒�)
		m_pHPBarDanger[i] = std::make_unique<Sprite2D>();
		m_pHPBarDanger[i]->Create(device, context, L"Resources/Textures/HPBar_Red.jpg", m_pStates);

		//�̗̓o�[�̔w�i
		m_pHPBarBack[i] = std::make_unique<Sprite2D>();
		m_pHPBarBack[i]->Create(device, context, L"Resources/Textures/HPBar_Gray.jpg", m_pStates);

		//�u�[�X�g�e�ʃo�[
		m_pBoostBar[i] = std::make_unique<Sprite2D>();
		m_pBoostBar[i]->Create(device, context, L"Resources/Textures/BoostBar.jpg", m_pStates);

		//�u�[�X�g�e�ʃo�[(�I�[�o�[�q�[�g)
		m_pBoostOverHeatBar[i] = std::make_unique<Sprite2D>();
		m_pBoostOverHeatBar[i]->Create(device, context, L"Resources/Textures/BoostBar_OverHeat.jpg", m_pStates);

		//�u�[�X�g�e�ʃo�[�̔w�i
		m_pBoostBack[i] = std::make_unique<Sprite2D>();
		m_pBoostBack[i]->Create(device, context, L"Resources/Textures/BoostBar_Gray.jpg", m_pStates);

		//����摜
		m_pOverHeadSprite[i] = std::make_unique<Sprite2D>();

		m_overHeadSpritePos[i] = DirectX::SimpleMath::Vector2::Zero;

		//�����{���̉摜
		for(int j = 0; j < WIN_NUM;j++)
		{
			m_pWinNumSprtie[i][j] = std::make_unique<Sprite2D>();
			m_pWinNumSprtie[i][j]->Create(device, context, L"Resources/Textures/winNum.png", m_pStates);

			//�؂���ʒu�̐ݒ�
			m_winNumSpriteRect[i][j].top = 0;
			m_winNumSpriteRect[i][j].bottom = static_cast<LONG>(WIN_NUM_SPRITE_HEIGHT);
			m_winNumSpriteRect[i][j].left = 0;
			m_winNumSpriteRect[i][j].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH* 0.5);
		}

		//�����̉e�̃G�t�F�N�g
		m_pShadowManager[i] = std::make_unique<ShadowManager>();
		m_pShadowManager[i]->Initialize(m_pDeviceResources, 1, 10, DirectX::SimpleMath::Vector3::Zero);

	}
	//����摜�̓ǂݍ���
	m_pOverHeadSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Create(device, context, L"Resources/Textures/p1Icon.png", m_pStates);
	m_pOverHeadSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Create(device, context, L"Resources/Textures/p2Icon.png", m_pStates);


	//�̗̓o�[�̐؂���ʒu�̏�����
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].top = 0;
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].bottom = static_cast<LONG>(HP_BAR_HEIGHT);
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = 0;
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].right = static_cast<LONG>(HP_BAR_WIDTH);

	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].top = 0;
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].bottom = static_cast<LONG>(HP_BAR_HEIGHT);
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].left = 0;
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(HP_BAR_WIDTH);

	//�u�[�X�g�e�ʃo�[�̐؂���ʒu�̏�����
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].top = 0;
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].bottom = static_cast<LONG>(BOOST_BAR_HEIGHT);
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = 0;
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].right = static_cast<LONG>(BOOST_BAR_WIDTH);

	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].top = 0;
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].bottom = static_cast<LONG>(BOOST_BAR_HEIGHT);
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].left = 0;
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(BOOST_BAR_WIDTH);


	//�̗̓o�[�A�u�[�X�g�e�ʃo�[�̏������W�ݒ�
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_hpBarPos[i] = HP_BAR_POS[i];
		m_boostBarPos[i] = BOOST_BAR_POS[i];
	}

	//�����{���̉摜�̏������W�ݒ�
	for (int i = 0; i < WIN_NUM; i++)
	{
		//�v���C���[1
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)][i].y = 170.0f;
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)][i].x = 70.0f + ((WIN_NUM_SPRITE_WIDTH * 0.5f + 10) * i);
		//�v���C���[2
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)][i].y = 170.0f;
		m_winNumSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)][i].x = 1920.0f - ((WIN_NUM_SPRITE_WIDTH * 0.5f + 10) * (i + 1)) - 70.0f;
	}

	DebugFont::GetInstance()->Create(device, context);

	m_isResult = false;

	//���̍쐬
	m_floorBox.pos = DirectX::SimpleMath::Vector3(0.0f, -2.0f, 0.0f);
	m_floorBox.size_h = DirectX::SimpleMath::Vector3(5.0f, 0.25f, 1.0f);
	m_pFloor = DirectX::GeometricPrimitive::CreateBox(pDeviceResources->GetD3DDeviceContext(), m_floorBox.size_h*2);
	m_floorWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_floorBox.pos);

	//�v���C���[�̍쐬
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)] = CharacterFactory::CreatePlayer1();
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)] = CharacterFactory::CreatePlayer2();

	//�������W�̐ݒ�
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetStartPos(DirectX::SimpleMath::Vector3(-2.0f, 1.0f, 0.0f));
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetStartPos(DirectX::SimpleMath::Vector3(2.0f, 1.0f, 0.0f));

	//�����摜�̓ǂݍ���
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pWinSprite[i] = std::make_unique<Sprite2D>();
	}
	m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Create(device, context, L"Resources/Textures/p1win.png", m_pStates);
	m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Create(device, context, L"Resources/Textures/p2win.png", m_pStates);

	//���E���h���̉摜�̓ǂݍ���
	for (int i = 0; i < static_cast<int>(eROUND::ROUND_NUM); i++)
	{
		m_pRoundSprite[i] = std::make_unique<Sprite2D>();
	}
	m_pRoundSprite[static_cast<int>(eROUND::ROUND_1)]->Create(device, context, L"Resources/Textures/Round1.png", m_pStates);
	m_pRoundSprite[static_cast<int>(eROUND::ROUND_2)]->Create(device, context, L"Resources/Textures/Round2.png", m_pStates);
	m_pRoundSprite[static_cast<int>(eROUND::ROUND_3)]->Create(device, context, L"Resources/Textures/Round3.png", m_pStates);

	//Fight�摜�̓ǂݍ���
	m_pFightSprite = std::make_unique<Sprite2D>();
	m_pFightSprite->Create(device, context, L"Resources/Textures/Fight.png", m_pStates);

	//�^�C���A�b�v�摜�̓ǂݍ���
	m_pTimeUpSprite = std::make_unique<Sprite2D>();
	m_pTimeUpSprite->Create(device, context, L"Resources/Textures/TimeUp.png", m_pStates);

	//���������摜�̓ǂݍ���
	m_pDrawSprite = std::make_unique<Sprite2D>();
	m_pDrawSprite->Create(device, context, L"Resources/Textures/Draw.png", m_pStates);

	//�������Ԃ̉摜�̓ǂݍ���
	m_pTimeSpriteOne = std::make_unique<Sprite2D>();
	m_pTimeSpriteOne->Create(device, context, L"Resources/Textures/Number_mini.png", m_pStates);
	m_pTimeSpriteTen= std::make_unique<Sprite2D>();
	m_pTimeSpriteTen->Create(device, context, L"Resources/Textures/Number_mini.png", m_pStates);

	//���j���[�̉摜�ǂݍ���
	m_pMenuSprite = std::make_unique<Sprite2D>();
	m_pMenuSprite->Create(device, context, L"Resources/Textures/Menu.png", m_pStates);
	m_pMenuCursorSprite = std::make_unique<Sprite2D>();
	m_pMenuCursorSprite->Create(device, context, L"Resources/Textures/menuCursol.png", m_pStates);

	//pushSpace�̉摜�ǂݍ���
	m_pPushSpaceResult = std::make_unique<Sprite2D>();
	m_pPushSpaceResult->Create(device, context, L"Resources/Textures/pushSpace_result.png", m_pStates);


	//�v���C���[�̏�����
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pPlayer[i]->Initialize(pDeviceResources,window);
		m_pPlayer[i]->SetAttackManager(AttackManager::GetInstance());
		//�v���C���[�̏����{���̏�����
		m_playerWinNum[i] = 0;

	}
	//�L�[�g���b�J�[�̍쐬
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//�J�����̒����_�̏�����
	m_targetPos = DirectX::SimpleMath::Vector3::Zero;

	//�v���C�V�[���̃X�e�[�g��������
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;

	//�����摜�̍��W�ݒ�
	m_winSpritePos = DirectX::SimpleMath::Vector2(640.0f, 300.0f);

	////FBX���f���̓ǂݍ���
	//m_pFbxModel = new FbxModel();
	//m_pFbxModel->Load(
	//	window,
	//	pDeviceResources->GetD3DDevice(),
	//	pDeviceResources->GetD3DDeviceContext(),
	//	pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/robot_maya10.fbx",
	//	true
	//);

	m_fbxModelWorld = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(0, -0.8f, 0);
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(0.08f, 0.08f, 0.08f);

	m_fbxModelWorld = scale * trans;

	//���E���h���̏�����
	m_nowRound = eROUND::ROUND_1;

	//�������Ԃ̏�����
	m_time = PlayScene::TIME_MAX;

	m_fadeTimer = 1.0f;

	//�G�̃|�C���^�擾
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetEnemy(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]);
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetEnemy(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]);


	//��������̉摜�ǂݍ��݁A�������W�ݒ�
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i] = std::make_unique<Sprite2D>();

		m_ManualSpritePos[i].y = -MANUAL_SPRITE_HEIGHT;

	}
	m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = MANUAL_SPRITE_WIDTH;

	m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)]
		->Create(device, context, L"Resources/Textures/Manual_1.png", m_pStates);

	//��������̃J�[�\���摜�̓ǂݍ���
	m_pManualCursorSpriteRight = std::make_unique<Sprite2D>();
	m_pManualCursorSpriteRight->Create(device, context, L"Resources/Textures/ManualCursolr_Right.png", m_pStates);
	m_pManualCursorSpriteLeft = std::make_unique<Sprite2D>();
	m_pManualCursorSpriteLeft->Create(device, context, L"Resources/Textures/ManualCursolr_Left.png", m_pStates);

	switch (CharacterFactory::m_player1Chara)
	{
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(device, context, L"Resources/Textures/Manual_2_chara1.png", m_pStates);
			break;
		}

		case eCHARACTER_ID::CHARACTER_2:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(device, context, L"Resources/Textures/Manual_2_chara2.png", m_pStates);
			break;
		}

		case eCHARACTER_ID::CHARACTER_3:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(device, context, L"Resources/Textures/Manual_2_chara3.png", m_pStates);
			break;
		}

		default:
			break;
	}


}

///////////////////////////
//�X�V
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Update(DX::StepTimer const& timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	m_totalSeconds = static_cast<float>(timer.GetTotalSeconds());

	//m_pDebugCamera->update();

	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, m_targetPos, DirectX::SimpleMath::Vector3::UnitY);
	//m_view = m_pDebugCamera->getViewMatrix();

	float time = float(timer.GetTotalSeconds());


	//m_pHitEffectManager->SetRenderState(m_view, m_proj);

	//UI�̉摜�̍��W�ݒ�
	for (int i = 0; i < static_cast<int>(eROUND::ROUND_NUM); i++)
	{
		m_pRoundSprite[i]->Update(COUNT_DOWN_SPRITE_POS);
	}
	m_pFightSprite->Update(COUNT_DOWN_SPRITE_POS);
	m_pTimeUpSprite->Update(COUNT_DOWN_SPRITE_POS);
	m_pDrawSprite->Update(COUNT_DOWN_SPRITE_POS);

	//�̗͂̃o�b�t�@�̓���
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_playerHpBuffer[i] = m_pPlayer[i]->GetHP();
	}

	//�̗̓o�[�̐؂���ʒu�̍X�V
	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = static_cast<LONG>(
		(HP_BAR_WIDTH / m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetMaxHP()) *
		(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetMaxHP() - m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP()));
	m_hpBarPos[static_cast<int>(ePLAYER_ID::PLAYER_1)].x = m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left + HP_BAR_MARGIN;


	m_hpBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(HP_BAR_WIDTH -
		((HP_BAR_WIDTH / m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetMaxHP()) *
		(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetMaxHP() - m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())));

	//�u�[�X�g�e�ʃo�[�̐؂���ʒu�̍X�V
	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left = static_cast<LONG>(
		(BOOST_BAR_WIDTH / m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetMaxBoostCap()) *
		(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetMaxBoostCap() - m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetBoostCap()));
	m_boostBarPos[static_cast<int>(ePLAYER_ID::PLAYER_1)].x = m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_1)].left + HP_BAR_MARGIN;

	m_boostBarRect[static_cast<int>(ePLAYER_ID::PLAYER_2)].right = static_cast<LONG>(BOOST_BAR_WIDTH -
		((BOOST_BAR_WIDTH / m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetMaxBoostCap()) *
		(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetMaxBoostCap() - m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetBoostCap())));

	//�����{���̉摜�̐؂���ʒu�̍X�V
	//�v���C���[�P
	for (int i = 0; i < m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)]; i++)
	{
		if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] > 0)
		{
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_1)][m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] - 1].left = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH * 0.5);
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_1)][m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] - 1].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH);
		}
	}
	//�v���C���[2
	for (int i = 0; i < m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)]; i++)
	{
		if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] > 0)
		{
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_2)][m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] - 1].left = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH * 0.5);
			m_winNumSpriteRect[static_cast<int>(ePLAYER_ID::PLAYER_2)][m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] - 1].right = static_cast<LONG>(WIN_NUM_SPRITE_WIDTH);
		}
	}

	//����̉摜��Y���W�X�V
	//�v���C���[1�p
	switch (CharacterFactory::m_player1Chara)
	{
		//�L�����N�^�[�P
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 0.5f)*-1) * 260.0f;
			break;
		}
		//�L�����N�^�[�Q
		case eCHARACTER_ID::CHARACTER_2:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 1.5f)*-1) * 260.0f;
			break;
		}
		//�L�����N�^�[�R
		case eCHARACTER_ID::CHARACTER_3:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_1)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 2.0f)*-1) * 260.0f;
			break;
		}

		default:
			break;
	}
	//�v���C���[2�p
	switch (CharacterFactory::m_player2Chara)
	{
		//�L�����N�^�[�P
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 0.5f)*-1) * 260.0f;
			break;
		}
		//�L�����N�^�[�Q
		case eCHARACTER_ID::CHARACTER_2:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 1.5f)*-1) * 260.0f;
			break;
		}
		//�L�����N�^�[�R
		case eCHARACTER_ID::CHARACTER_3:
		{
			m_overHeadSpritePos[static_cast<int>(ePLAYER_ID::PLAYER_2)].y =
				((m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 2.0f)*-1) * 260.0f;
			break;
		}

		default:
			break;
	}


	//�̗̓o�[�̍X�V	
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pHPBar[i]->Update(m_hpBarPos[i], m_hpBarRect[i]);
		m_pHPBarBack[i]->Update(HP_BAR_POS[i].x - 10.0f,HP_BAR_POS[i].y - 10.0f);

		m_pHPBarDanger[i]->Update(m_hpBarPos[i], m_hpBarRect[i]);
		m_pHPBarBack[i]->Update(HP_BAR_POS[i].x - 10.0f, HP_BAR_POS[i].y - 10.0f);

		m_pBoostBar[i]->Update(m_boostBarPos[i], m_boostBarRect[i]);
		m_pBoostBack[i]->Update(BOOST_BAR_POS[i].x - 10.0f, BOOST_BAR_POS[i].y - 10.0f);

		//����̉摜�̍X�V
		m_overHeadSpritePos[i].x = 960.0f - 50.0f + m_pPlayer[i]->GetPos().x * 230.0f;
		m_pOverHeadSprite[i]->Update(m_overHeadSpritePos[i]);

		//�����{���̉摜�̍X�V
		for (int j = 0; j < WIN_NUM; j++)
		{
			m_pWinNumSprtie[i][j]->Update(m_winNumSpritePos[i][j], m_winNumSpriteRect[i][j]);
		}

		//�����̉e�̃G�t�F�N�g�̍X�V
		m_pShadowManager[i]->Update(timer, m_pPlayer[i]->GetPos());

	}

	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(0.001);
	m_spaceWorld *= rotY;

	//�������Ԃ̉摜�̐؂���ʒu�̐ݒ�
	m_timeSpriteOneRect.top = 0;
	m_timeSpriteOneRect.bottom = static_cast<LONG>(PlayScene::TIME_SPRITE_HEIGHT);
	m_timeSpriteOneRect.left = static_cast<LONG>(PlayScene::TIME_SPRITE_WIDTH * (static_cast<int>(m_time) % 10));
	m_timeSpriteOneRect.right = m_timeSpriteOneRect.left + static_cast<LONG>(PlayScene::TIME_SPRITE_WIDTH);

	m_timeSpriteTenRect.top = 0;
	m_timeSpriteTenRect.left = static_cast<LONG>(PlayScene::TIME_SPRITE_WIDTH * (static_cast<int>(m_time) / 10));
	m_timeSpriteTenRect.bottom = static_cast<LONG>(PlayScene::TIME_SPRITE_HEIGHT);
	m_timeSpriteTenRect.right = m_timeSpriteTenRect.left + static_cast<LONG>(PlayScene::TIME_SPRITE_WIDTH);

	m_pTimeSpriteOne->Update(PlayScene::TIME_SPRITE_POS_X, 0.0f, m_timeSpriteOneRect);
	m_pTimeSpriteTen->Update(PlayScene::TIME_SPRITE_POS_X - PlayScene::TIME_SPRITE_WIDTH, 0.0f, m_timeSpriteTenRect);


	//�V�[���̃X�e�[�g
	if (m_isStop == false)
	{
		//�V���̍X�V
		m_space->Update(m_spaceWorld);

		switch (m_sceneState)
		{
			//�t�F�[�h�C��
			case SceneBase::eSCENE_STATE::FADE_IN:
			{
				m_fadeTimer -= static_cast<float>(timer.GetElapsedSeconds()) * 2.0f;

				if (m_fadeTimer <= 0.0f)
				{	
					//BGM�̍Đ�
					soundID = ADX2::GetInstance().Play(CRI_CUESHEET_0_PLAYSCENE_BGM);

					//LoadDisplay::GetInstance()->Update(false);
					m_sceneState = eSCENE_STATE::MAIN;
				}

				for (int i = 0; i < PLAYER_NUM; i++)
				{
					//�v���C���[�̏��Ƃ̓����蔻��
					m_pPlayer[i]->HitFloor(m_floorBox);
					//�v���C���[�̏���
					m_pPlayer[i]->Ready(timer);
				}

				break;
			}
			//���C��
			case SceneBase::eSCENE_STATE::MAIN:
			{
				switch (m_playSceneState)
				{
					//�J�E���g�_�E��
					case PlayScene::ePLAY_SCENE_STATE::COUNT_DOWN:
					{

						for (int i = 0; i < PLAYER_NUM; i++)
						{
							//�v���C���[�̏��Ƃ̓����蔻��
							m_pPlayer[i]->HitFloor(m_floorBox);
							//�v���C���[�̏���
							m_pPlayer[i]->Ready(timer);
						}
						//�G�̍��W�̎擾
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetEnemyPos(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos());
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetEnemyPos(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos());
						//�v���C���[���m�̑̂̓����蔻��
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHeadCollBox());

						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHeadCollBox());


						m_countdownTimer += static_cast<float>(timer.GetElapsedSeconds());
						if (m_countdownTimer >= COUNT_DOWN_TIME)
						{
							m_playSceneState = ePLAY_SCENE_STATE::MAIN;
							m_countdownTimer = 0.0f;
						}
						break;
					}
					//���C��
					case PlayScene::ePLAY_SCENE_STATE::MAIN:
					{
						//�U���̃}�l�[�W���[�̍X�V
						AttackManager::GetInstance()->Update();


						//���Ԃ����炷
						m_time -= static_cast<float>(timer.GetElapsedSeconds());
						//���Ԃ��[���ɂȂ�����^�C���A�b�v�ɑJ��
						if (m_time <= 0)
						{
							m_playSceneState = ePLAY_SCENE_STATE::TIME_UP;
							//�v���C���[�P�ƂQ�̏������𑝂₷
							if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() ==
								m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
							{
								m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)]++;
								m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)]++;
							}
						}

						for (int i = 0; i < PLAYER_NUM; i++)
						{
							//�v���C���[�̏��Ƃ̓����蔻��
							m_pPlayer[i]->HitFloor(m_floorBox);
							//�v���C���[�̍X�V
							m_pPlayer[i]->Update(timer);
						}
						//�G�̍��W�̎擾
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetEnemyPos(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos());
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetEnemyPos(m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos());
						//�v���C���[���m�̑̂̓����蔻��
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHeadCollBox());

						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHeadCollBox());


						//�v���C���[�P�̗̑͂��[���ɂȂ����烊�U���g�ɑJ��
						if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() <= 0)
						{
							m_playSceneState = ePLAY_SCENE_STATE::RESULT;
							//�v���C���[�Q�̏������𑝂₷
							m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)]++;
						}
						//�v���C���[�Q�̗̑͂��[���ɂȂ����烊�U���g�ɑJ��
						if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP() <= 0)
						{
							m_playSceneState = ePLAY_SCENE_STATE::RESULT;
							//�v���C���[�P�̏������𑝂₷
							m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)]++;
						}

						break;
					}

					case PlayScene::ePLAY_SCENE_STATE::TIME_UP:
					{
						//�U���̃}�l�[�W���[�̍X�V
						//AttackManager::GetInstance()->Update();

						m_countdownTimer += static_cast<float>(timer.GetElapsedSeconds());
						if (m_countdownTimer >= PlayScene::TIME_UP_TIME)
						{
							m_playSceneState = ePLAY_SCENE_STATE::RESULT;

							//�v���C���[�P�̗̑͂��v���C���[�Q��葽����΃v���C���[�P�̏������𑝂₷
							if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() >
								m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
							{
								m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)]++;
							}
							//�v���C���[�Q�̗̑͂��v���C���[�P��葽����΃v���C���[�Q�̏������𑝂₷
							else if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() <
								m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
							{
								m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)]++;
							}

							m_countdownTimer = 0.0f;
						}
						break;
					}


					//���U���g
					case PlayScene::ePLAY_SCENE_STATE::RESULT:
					{

						for (int i = 0; i < PLAYER_NUM; i++)
						{
							//�v���C���[�̏��Ƃ̓����蔻��
							m_pPlayer[i]->HitFloor(m_floorBox);

							
							//m_pPlayer[i]->Ready(timer);

							if (m_pWinSprite[i] != nullptr)
								m_pWinSprite[i]->Update(m_winSpritePos);
						}
						if (m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() > m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
						{
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Ready(timer);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetCharaState(eCHARACTER_STATE::LOSE);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Lose(timer);
						}
						else
						{
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Ready(timer);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetCharaState(eCHARACTER_STATE::LOSE);
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Lose(timer);
						}

						//�v���C���[���m�̑̂̓����蔻��
						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHeadCollBox());

						m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->HitEnemyBody(
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetBodyCollBox(),
							m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHeadCollBox());

						m_countdownTimer += static_cast<float>(timer.GetElapsedSeconds());
						if (m_countdownTimer >= PlayScene::ROUND_CHANGE_TIME || keyState.IsKeyDown(DirectX::Keyboard::Keys::Space))
						{
							m_sceneState = eSCENE_STATE::FADE_OUT;
						}

						break;
					}
					default:
						break;
				}


				//m_sprite2D->Update(0, 0);//�X�v���C�g�̍X�V

				break;
			}
			//�t�F�[�h�A�E�g
			case SceneBase::eSCENE_STATE::FADE_OUT:
			{
				if(m_isResult == false)m_fadeTimer += static_cast<float>(timer.GetElapsedSeconds())* 2.0f;
				if (m_fadeTimer >= 1.0f)
				{
					//SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
					//m_sceneState = eSCENE_STATE::FADE_IN;
					//�ǂ��炩�̃v���C���[���Q�{��������܂ŌJ��Ԃ�
					if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] < WIN_NUM &&
						m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] < WIN_NUM)
					{
						//���E���h����؂�ւ���
						switch (m_nowRound)
						{
							case PlayScene::eROUND::ROUND_1:
							{
								m_nowRound = eROUND::ROUND_2;
								break;

							}
							case PlayScene::eROUND::ROUND_2:
							{
								m_nowRound = eROUND::ROUND_3;
								break;
							}
							default:
								break;
						}
						//���Z�b�g
						Reset();
					}
					//�ǂ��炩�̃v���C���[���Q�{���������烊�U���g�ɑJ��
					else
					{
						//SceneManager::GetInstance()->SetScene(eSCENE_ID::RESULT_SCENE);
						m_isResult = true;

						if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] >= WIN_NUM)
						{
							ResultScene::m_winPlayerID = ePLAYER_ID::PLAYER_1;
						}
						else if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] >= WIN_NUM)
						{
							ResultScene::m_winPlayerID = ePLAYER_ID::PLAYER_2;
						}


					}
					ADX2::GetInstance().Stop(soundID);

				}
				break;
			}
			default:
				break;
		}
	}

	////ESC�L�[���͂Ń^�C�g���ɖ߂�
	//if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape))
	//{
	//	SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
	//	//SE�Đ�
	//	ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
	//	ADX2::GetInstance().Stop(soundID);


	//}

	//���j���[
	Menu();
	//���U���g
	if(m_isResult == true)Result(timer);

		

	//�t�F�[�h�}�l�[�W���[�̍X�V
	FadeManager::GetInstance()->Update(timer, m_fadeTimer);
}

///////////////////////////
//�`��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Render()
{
	//�V���̕`��
	m_space->Render(m_view, m_proj);
	//m_sprite2D->Draw();//2d�X�v���C�g�̕`��

	////wchar_t string[25] = {};

	////if (m_pPlayer[1] != nullptr)
	////{
	////	//�̗͂𕶎���ɕϊ�
	////	_itow(m_pPlayer[1]->GetHP(), string, 10);
	////}
	////�f�o�b�O�����̍X�V
	//DebugFont::GetInstance()->Print(0, 0, string, DirectX::Colors::Aqua);
	//DebugFont::GetInstance()->Print(0, 0, string, DirectX::Colors::Aqua);


	//DebugFont::GetInstance()->Draw();//�f�o�b�O�����̕`��



	//���̕`��
	if(m_isResult == false)
		m_pFloor->Draw(m_floorWorld, m_view, m_proj, DirectX::Colors::Brown);

	////���f���̕`��
	//m_pFbxModel->Draw(
	//	m_fbxModelWorld,
	//	m_view,
	//	m_proj
	//);
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//�����̉e�̃G�t�F�N�g�̕`��
		m_pShadowManager[i]->Render(m_view,m_proj);
	}

	//�v���C���[�̕`��
	//for (int i = 0; i < PLAYER_NUM; i++)
	//{
	//	m_pPlayer[i]->Render(m_view, m_proj);

	//}

	//�v���C���[�P�̕`��
	if (m_isResult == false || m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)] < WIN_NUM)
	{
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Render(m_view, m_proj);
	}
	//�v���C���[�Q�̕`��
	if (m_isResult == false || m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] < WIN_NUM)
	{
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Render(m_view, m_proj);
	}

	//�v���C���[�P�̏����摜
	if (m_playSceneState == ePLAY_SCENE_STATE::RESULT && 
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() >
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
	{
		m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_1)]->Draw(true);
	}

	//�v���C���[2�̏����摜
	else if (m_playSceneState == ePLAY_SCENE_STATE::RESULT && 
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP() >
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP())
	{
		m_pWinSprite[static_cast<int>(ePLAYER_ID::PLAYER_2)]->Draw(true);
	}

	//���������摜
	else if (m_playSceneState == ePLAY_SCENE_STATE::RESULT &&
		m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetHP() == m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetHP())
	{
	}

	if (m_isResult == false)
	{
		//�̗̓o�[�A�u�[�X�g�e�ʃo�[�̕`��
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			//�w�i
			m_pHPBarBack[i]->Draw();
			m_pBoostBack[i]->Draw();

			//�{��
			if (m_pPlayer[i]->GetHP() > m_pPlayer[i]->GetMaxHP() * 0.3f)
				m_pHPBar[i]->Draw(false, true);
			else m_pHPBarDanger[i]->Draw(false, true);

			m_pBoostBar[i]->Draw(false, true);

			//�����{���̉摜�̕`��
			for (int j = 0; j < WIN_NUM; j++)
			{
				m_pWinNumSprtie[i][j]->Draw(false, true);
			}
		}

		//����̉摜�̕`��
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			//����̉摜
			m_pOverHeadSprite[i]->Draw(true);
		}


		//�������Ԃ̉摜
		m_pTimeSpriteOne->Draw(true, true);
		m_pTimeSpriteTen->Draw(true, true);

		//�J�E���g�_�E���̉摜
		if (m_playSceneState == ePLAY_SCENE_STATE::COUNT_DOWN)
		{
			if (m_countdownTimer <= COUNT_DOWN_TIME / 2)
			{
				//���݂̃��E���h���ɂ���ĕ\������摜��؂�ւ���
				switch (m_nowRound)
				{
					case PlayScene::eROUND::ROUND_1:
					{
						m_pRoundSprite[static_cast<int>(eROUND::ROUND_1)]->Draw(true);

						break;
					}
					case PlayScene::eROUND::ROUND_2:
					{
						m_pRoundSprite[static_cast<int>(eROUND::ROUND_2)]->Draw(true);

						break;
					}

					case PlayScene::eROUND::ROUND_3:
					{
						m_pRoundSprite[static_cast<int>(eROUND::ROUND_3)]->Draw(true);
						break;
					}

					default:
						break;
				}

			}
			else
			{
				m_pFightSprite->Draw(true);
			}
		}
	}
	else
	{
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			if (m_pWinSprite[i] != nullptr)
				m_pWinSprite[i]->Update(m_winSpritePos.x + 600.0f,m_winSpritePos.y - 300.0f);
		}

		m_pPushSpaceResult->Update(PUSH_SPACE_RESULT_POS);
		m_pPushSpaceResult->Draw(true);
	}

	//�^�C���A�b�v�̉摜
	if (m_playSceneState == ePLAY_SCENE_STATE::TIME_UP)
	{
		m_pTimeUpSprite->Draw(true);
	}

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	//���j���[�̉摜�̕`��
	if (m_isStop == true)
	{
		m_pMenuSprite->Draw(true);
		m_pMenuCursorSprite->Draw(true);

	}

	//��������̉摜�̕`��
	if (m_isManualDisplay == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_pManualSprite[i]->Draw(true);

		}
	}
	//��������̃J�[�\���摜�̕`��
	if (m_isManualDisplay == true && m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
	{
		m_pManualCursorSpriteLeft->Draw(true);
	}
	else if (m_isManualDisplay == true && m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x >= 0.0f)
	{
		m_pManualCursorSpriteRight->Draw(true);
	}

	//�U���̕`��
	//AttackManager::GetInstance()->Render(m_view, m_proj);

}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Finalize()
{
	//�v���C���[�̏I������
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		if (m_pPlayer[i] != nullptr)
		{
			m_pPlayer[i]->Finalize();
		}
	}

	//���\�[�X�̉��
	m_pFloor.reset();
	//m_pFbx.reset();
	m_pDebugCamera.reset();
	m_space.reset();
	m_sprite2D.reset();
	m_pPushSpaceResult->Reset();
	//
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i]->Reset();

	}

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pHPBar[i]->Reset();
		m_pHPBar[i].reset();

		m_pHPBarDanger[i]->Reset();
		m_pHPBarDanger[i].reset();


		m_pHPBarBack[i]->Reset();
		m_pHPBarBack[i].reset();

		m_pWinSprite[i]->Reset();
		m_pWinSprite[i].reset();

		for (int j = 0; j < WIN_NUM; j++)
		{
			m_pWinNumSprtie[i][j]->Reset();
			m_pWinNumSprtie[i][j].reset();
		}

		if (m_pPlayer[i] != nullptr)
		{
			delete m_pPlayer[i];
			m_pPlayer[i] = nullptr;

		}

		m_pOverHeadSprite[i]->Reset();
		m_pOverHeadSprite[i].reset();
		//�����̉e�̃G�t�F�N�g�̏I������
		m_pShadowManager[i]->Finalize();

	}

	for (int i = 0; i < static_cast<int>(eROUND::ROUND_NUM); i++)
	{
		m_pRoundSprite[i]->Reset();
		m_pRoundSprite[i].reset();
	}
	m_pFightSprite->Reset();
	m_pFightSprite.reset();

	m_pTimeUpSprite->Reset();
	m_pTimeUpSprite.reset();

	m_pDrawSprite->Reset();
	m_pDrawSprite.reset();

	m_pTimeSpriteOne->Reset();
	m_pTimeSpriteOne.reset();

	m_pTimeSpriteTen->Reset();
	m_pTimeSpriteTen.reset();

	m_pManualCursorSpriteLeft->Reset();
	m_pManualCursorSpriteLeft.reset();

	m_pManualCursorSpriteRight->Reset();
	m_pManualCursorSpriteRight.reset();


	DebugFont::Dispose();

	//�U���}�l�[�W���[�̏I������
	AttackManager::GetInstance()->Finalize();
	//�U���}�l�[�W���[�̃C���X�^���X�̉��
	AttackManager::ReleaseInstance();

	//delete m_pFbxModel;
	//m_pFbxModel = nullptr;
	ADX2::GetInstance().Stop(soundID);

}

///////////////////////////
//���Z�b�g
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Reset()
{
	//�v���C���[�̍��W���̃��Z�b�g
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pPlayer[i]->Reset();

		m_hpBarPos[i] = HP_BAR_POS[i];

		m_boostBarPos[i] = BOOST_BAR_POS[i];
	}

	//�U���}�l�[�W���[�̃��Z�b�g
	for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
	{
		AttackManager::GetInstance()->Reset(i);
	}
	m_sceneState = eSCENE_STATE::FADE_IN;
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;

	m_fadeTimer = 1.0f;
	m_countdownTimer = 0.0f;

	m_time = PlayScene::TIME_MAX;

	//�J�����̒����_�̏�����
	m_targetPos = DirectX::SimpleMath::Vector3::Zero;
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);

	//BGM�̍Đ�
	//soundID = ADX2::GetInstance().Play(CRI_CUESHEET_0_PLAYSCENE_BGM);

}


///////////////////////////
//�������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Manual()
{
	//��������\������ESC�L�[�ŃQ�[���ɖ߂�
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_isManualDisplay == true)
	{
		//���~�t���O������
		m_isManualDown = false;
		//�㏸�t���O�𗧂Ă�
		m_isManualUp = true;

		//SE�Đ�
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);

	}
	//
	////ESC�L�[�ő�������\��
	//if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_isManualDisplay == false)
	//{
	//	m_isManualDisplay = true;
	//	//���~�t���O�𗧂Ă�
	//	m_isManualDown = true;
	//}


	////��������\�����ɃX�y�[�X�L�[���͂ŃL�����N�^�[�Z���N�g�ɖ߂�
	//if (m_isManualDisplay == true)
	//{
	//	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	//	{
	//		SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
	//		//SE�Đ�
	//		ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
	//		ADX2::GetInstance().Stop(soundID);
	//	}
	//}

	//���E���͂ňړ��t���O�𗧂Ă�
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left) && 
		m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
	{
		m_isManualLeft = false;
		m_isManualRight = true;

		//SE�Đ�
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	else if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right) &&
		m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x >= 0.0f)
	{
		m_isManualRight = false;
		m_isManualLeft = true;

		//SE�Đ�
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	
	//�J�[�\���X�V
	if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x < 0.0f)
	{
		m_pManualCursorSpriteRight->Update(DirectX::SimpleMath::Vector2(100.0f, 540.0f));
	}

	//��������̃J�[�\���̍X�V
	m_pManualCursorSpriteRight->Update(DirectX::SimpleMath::Vector2(1720.0f + cosf(m_totalSeconds * 6.0f)*10.0f, 540.0f));
	m_pManualCursorSpriteLeft->Update(DirectX::SimpleMath::Vector2(50.0f - cosf(m_totalSeconds * 6.0f) * 10.0f, 540.0f));


	//�E�ړ��t���O����������摜���E�Ɉړ�������
	if (m_isManualRight == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].x += MANUAL_SPRITE_WIDTH / 10.0f;

			if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x >= 0.0f)
			{
				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x = 0.0f;
				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = MANUAL_SPRITE_WIDTH;
				m_isManualRight = false;
			}
		}
	}

	//���ړ��t���O����������摜�����Ɉړ�������
	if (m_isManualLeft == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].x -= MANUAL_SPRITE_WIDTH / 10.0f;

			if (m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x <= -MANUAL_SPRITE_WIDTH)
			{
				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::KEYBOARD)].x = -MANUAL_SPRITE_WIDTH;
				m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)].x = 0.0f;
				m_isManualLeft = false;
			}
		}

	}

	//���~�t���O����������摜�����~������
	if (m_isManualDown == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].y += MANUAL_SPRITE_HEIGHT / 10.0f;

			if (m_ManualSpritePos[i].y >= 0.0f)
			{
				m_ManualSpritePos[i].y = 0.0f;
				m_isManualDown = false;
			}
		}
	}

	//�㏸�t���O����������摜���㏸������
	if (m_isManualUp == true)
	{
		for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
		{
			m_ManualSpritePos[i].y -= MANUAL_SPRITE_HEIGHT / 10.0f;

			if (m_ManualSpritePos[i].y <= -MANUAL_SPRITE_HEIGHT)
			{
				m_ManualSpritePos[i].y = -MANUAL_SPRITE_HEIGHT;
				m_isManualUp = false;
				//�Q�[����~�t���O������
				m_isManualDisplay = false;
			}
		}
	}

	//�摜�̍X�V
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i]->Update(m_ManualSpritePos[i]);

	}

}

///////////////////////////
//���j���[
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Menu()
{
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape) && m_isManualDisplay == false && m_isResult == false)
	{
		//SE�Đ�
		if (m_isStop == false)
			ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);
		else if(m_isStop == true)
			ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);

		m_isStop = !m_isStop;

	}

	//�J�[�\���̍��W�ݒ�
	switch (static_cast<eMENU_CURSOR>(m_menuCursor))
	{
		case PlayScene::eMENU_CURSOR::MANUAL:
		{
			m_pMenuCursorSprite->
				Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::MANUAL)].x - cosf(m_totalSeconds * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::MANUAL)].y);
			break;
		}
		case PlayScene::eMENU_CURSOR::CHARA_SELECT:
		{
			m_pMenuCursorSprite->
				Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::CHARA_SELECT)].x - cosf(m_totalSeconds * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::CHARA_SELECT)].y);
			break;
		}
		case PlayScene::eMENU_CURSOR::EXIT:
		{
			m_pMenuCursorSprite->
				Update(MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::EXIT)].x - cosf(m_totalSeconds * 6.0f) * 10.0f, MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::EXIT)].y);
			break;
		}
		default:
			break;
	}

	//�L�[���͂ŃJ�[�\���ړ�
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Down) && m_isStop == true && m_isManualDisplay == false)
	{
		m_menuCursor++;
		if (m_menuCursor >= static_cast<int>(eMENU_CURSOR::OVER_ID))
		{
			m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);
		}

		//SE�Đ�
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);

	}
	else if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up) && m_isStop == true && m_isManualDisplay == false)
	{
		m_menuCursor--;
		if (m_menuCursor <= static_cast<int>(eMENU_CURSOR::NONE))
		{
			m_menuCursor = static_cast<int>(eMENU_CURSOR::EXIT);
		}

		//SE�Đ�
		ADX2::GetInstance().Play(CRI_CUESHEET_0_CURSOL);
	}
	
	//�X�y�[�X�L�[���͂Ō���
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space) && m_isStop == true && m_isManualDisplay == false)
	{
		//SE�Đ�
		ADX2::GetInstance().Play(CRI_CUESHEET_0_SUBMIT);

		switch (static_cast<eMENU_CURSOR>(m_menuCursor))
		{
			//��������\��
			case eMENU_CURSOR::MANUAL:
			{
				if (m_isManualDisplay == false)
				{
					m_isManualDisplay = true;
					m_isManualDown = true;
				}
				break;
			}

			//�L�����N�^�[�Z���N�g�ɖ߂�
			case eMENU_CURSOR::CHARA_SELECT:
			{
				SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
				//SE�Đ�
				ADX2::GetInstance().Play(CRI_CUESHEET_0_CANCEL);
				ADX2::GetInstance().Stop(soundID);
				break;
			}

			//�Q�[���I��
			case eMENU_CURSOR::EXIT:
			{
				PostQuitMessage(0);
				break;
			}

			default:
				break;
		}
	}

	//��������̕\���t���O���������瑀������̍X�V
	if (m_isManualDisplay == true)Manual();
}

///////////////////////////
//���U���g
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Result(DX::StepTimer const& timer)
{
	if (m_isResult == true)
	{
		m_fadeTimer = 0.0f;

		if (m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_1)] >
			m_playerWinNum[static_cast<int>(ePLAYER_ID::PLAYER_2)])
		{
			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetPos(
				DirectX::SimpleMath::Vector3(10.0f, 10.0f, 10.0f));

			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetCharaState(eCHARACTER_STATE::WIN);

			//m_pPlayer[0]->ChangeAnimation(14);
			m_cameraPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y - 1.0f,
				3.0f
			);

			m_targetPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().y,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->GetPos().z);

		}
		else
		{
			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetPos(
				DirectX::SimpleMath::Vector3(10.0f, 10.0f, 10.0f));


			m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetCharaState(eCHARACTER_STATE::WIN);
			//m_pPlayer[0]->ChangeAnimation(14);
			m_cameraPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y - 1.0f,
				3.0f
			);

			m_targetPos = DirectX::SimpleMath::Vector3(
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().x,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().y,
				m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->GetPos().z);

		}
	}
	//���U���g���ɃX�y�[�X�L�[���͂ŃL�����N�^�[�Z���N�g�ɖ߂�
	if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space) && m_isResult == true)
	{
		SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);
	}
}
