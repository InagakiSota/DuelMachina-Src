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
#include "Src/Cgdi.h"
#include "UI/PlaySceneUI.h"


 //��������̉摜�̉���
 const float PlayScene::MANUAL_SPRITE_WIDTH = 1920.0f;
 //��������̉摜�̍���
 const float PlayScene::MANUAL_SPRITE_HEIGHT = 1080.0f;
 //�J�E���g�_�E���̎���
 const float PlayScene::COUNT_DOWN_TIME = 3.0f;
 //�������Ԃ̍ő�l
 const float PlayScene::TIME_MAX = 60.0f;
 //�^�C���A�b�v�V�[���̐�������
 const float PlayScene::TIME_UP_TIME = 3.0f;
 //���E���h�؂�ւ����̎���
 const float PlayScene::ROUND_CHANGE_TIME = 1.0f;

 //�J�[�\���̍��W
 const DirectX::SimpleMath::Vector2 PlayScene::MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::OVER_ID)] =
 {
	DirectX::SimpleMath::Vector2(630.0f,300.0f),
	DirectX::SimpleMath::Vector2(630.0f,500.0f),
	DirectX::SimpleMath::Vector2(630.0f,700.0f)
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
	m_pShadowManager{}
{
	m_pDeviceResources = nullptr;
	m_pKeyTracker = nullptr;

	m_sceneState = eSCENE_STATE::FADE_IN;
	m_countdownTimer = 0.0f;
	m_cameraPos.Zero;
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
		m_pShadowManager[i].reset();
	}

	//�U���}�l�[�W���[�̃C���X�^���X�̉��
	AttackManager::ReleaseInstance();




	m_pDeviceResources = nullptr;
}

///////////////////////////
//������
//����:�f�o�C�X���\�[�X�̃|�C���^
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();
	m_pStates = gdi->GetStates();

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

	//�U���̃}�l�[�W���[�̏�����
	AttackManager::GetInstance()->Initialize(m_pDeviceResources);

	m_space = std::make_unique<ModelObject>();
	m_space->Create(m_pDeviceResources, L"Resources/Models/Space.cmo");


	DebugFont::GetInstance()->Create(device, context);

	m_isResult = false;

	//���̍쐬
	m_floorBox.pos = DirectX::SimpleMath::Vector3(0.0f, -2.0f, 0.0f);
	m_floorBox.size_h = DirectX::SimpleMath::Vector3(5.0f, 0.25f, 1.0f);
	m_pFloor = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_floorBox.size_h*2);
	m_floorWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_floorBox.pos);

	//�v���C���[�̍쐬
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)] = CharacterFactory::CreatePlayer1();
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)] = CharacterFactory::CreatePlayer2();

	//�������W�̐ݒ�
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_1)]->SetStartPos(DirectX::SimpleMath::Vector3(-2.0f, 1.0f, 0.0f));
	m_pPlayer[static_cast<int>(ePLAYER_ID::PLAYER_2)]->SetStartPos(DirectX::SimpleMath::Vector3(2.0f, 1.0f, 0.0f));


	//���j���[�̉摜�ǂݍ���
	m_pMenuSprite = std::make_unique<Sprite2D>();
	m_pMenuSprite->Create(L"Resources/Textures/Menu.png");
	m_pMenuCursorSprite = std::make_unique<Sprite2D>();
	m_pMenuCursorSprite->Create(L"Resources/Textures/menuCursol.png");


	//UI�N���X�̓ǂݍ���
	m_pPlaySceneUI = std::make_unique < PlaySceneUI>();
	//�v���C�V�[���̃|�C���^��n��
	m_pPlaySceneUI->SetPlayScene(this);
	//UI�N���X�̏�����
	m_pPlaySceneUI->Initialize();

	//�v���C���[�̏�����
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		m_pPlayer[i]->Initialize();
		m_pPlayer[i]->SetAttackManager(AttackManager::GetInstance());
		//�v���C���[�̏����{���̏�����
		m_playerWinNum[i] = 0;
	    
		//�����̉e�̓ǂݍ��݁A������
		m_pShadowManager[i] = std::make_unique<ShadowManager>();
		m_pShadowManager[i]->Initialize(1,10,DirectX::SimpleMath::Vector3::Zero);

	}
	//�L�[�g���b�J�[�̍쐬
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//�J�����̒����_�̏�����
	m_targetPos = DirectX::SimpleMath::Vector3::Zero;

	//�v���C�V�[���̃X�e�[�g��������
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;


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
		->Create(L"Resources/Textures/Manual_1.png");

	//��������̃J�[�\���摜�̓ǂݍ���
	m_pManualCursorSpriteRight = std::make_unique<Sprite2D>();
	m_pManualCursorSpriteRight->Create( L"Resources/Textures/ManualCursolr_Right.png");
	m_pManualCursorSpriteLeft = std::make_unique<Sprite2D>();
	m_pManualCursorSpriteLeft->Create( L"Resources/Textures/ManualCursolr_Left.png");

	//���삵�Ă���L�����N�^�[���Ƃɓǂݍ��ޑ�������̉摜��؂�ւ���
	switch (CharacterFactory::m_player1Chara)
	{
		//�L�����N�^�[�P
		case eCHARACTER_ID::CHARACTER_1:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create( L"Resources/Textures/Manual_2_chara1.png");
			break;
		}
		//�L�����N�^�[�Q
		case eCHARACTER_ID::CHARACTER_2:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create( L"Resources/Textures/Manual_2_chara2.png");
			break;
		}
		//�L�����N�^�[�R
		case eCHARACTER_ID::CHARACTER_3:
		{
			m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::COMMAND)]
				->Create(L"Resources/Textures/Manual_2_chara3.png");
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


	for (int i = 0; i < PLAYER_NUM; i++)
	{	
		//�����̉e�̃G�t�F�N�g�̍X�V
		m_pShadowManager[i]->Update(timer, m_pPlayer[i]->GetPos());
		//�̗͂̃o�b�t�@�̓���
		m_playerHpBuffer[i] = m_pPlayer[i]->GetHP();
	}


	//�V����Y���ŉ�]������
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(0.001);
	m_spaceWorld *= rotY;

	//UI�N���X�̍X�V
	m_pPlaySceneUI->Update();

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
					m_soundID = ADX2::GetInstance().Play(CRI_CUESHEET_0_PLAYSCENE_BGM);

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
					ADX2::GetInstance().Stop(m_soundID);

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

	//���̕`��
	if(m_isResult == false)
		m_pFloor->Draw(m_floorWorld, m_view, m_proj, DirectX::Colors::Brown);

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		//�����̉e�̃G�t�F�N�g�̕`��
		m_pShadowManager[i]->Render(m_view,m_proj);
	}


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

	//UI�̕`��
	m_pPlaySceneUI->Render();

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
	for (int i = 0; i < static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM); i++)
	{
		m_pManualSprite[i]->Reset();

	}

	for (int i = 0; i < PLAYER_NUM; i++)
	{

		if (m_pPlayer[i] != nullptr)
		{
			//delete m_pPlayer[i];
			m_pPlayer[i] = nullptr;

		}
		//�����̉e�̃G�t�F�N�g�̏I������
		m_pShadowManager[i]->Finalize();

	}


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
	ADX2::GetInstance().Stop(m_soundID);

	//UI�N���X�̏I������
	m_pPlaySceneUI->Finalize();
	m_pPlaySceneUI.reset();

}

///////////////////////////
//���Z�b�g
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void PlayScene::Reset()
{
	//�e�v���C���[�̃��Z�b�g
	for (int i = 0; i < PLAYER_NUM;i++)
	{
		m_pPlayer[i]->Reset();
	}


	//�U���}�l�[�W���[�̃��Z�b�g
	for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
	{
		AttackManager::GetInstance()->Reset(i);
	}
	//�t�F�[�h�C����Ԃɂ���
	m_sceneState = eSCENE_STATE::FADE_IN;
	//�v���C�V�[���̏�Ԃ��J�E���g�_�E���ɂ���
	m_playSceneState = ePLAY_SCENE_STATE::COUNT_DOWN;
	//�t�F�[�h�̃^�C�}�[��ݒ�
	m_fadeTimer = 1.0f;
	//�J�E���g�_�E���̃^�C�}�[��������
	m_countdownTimer = 0.0f;
	//�������Ԃ��ő�l�ɐݒ�
	m_time = PlayScene::TIME_MAX;

	//�J�����̒����_�̏�����
	m_targetPos = DirectX::SimpleMath::Vector3::Zero;
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);


	//UI�N���X�̃��Z�b�g
	m_pPlaySceneUI->Reset();
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
				ADX2::GetInstance().Stop(m_soundID);
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
