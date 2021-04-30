///////////////////////////////////
//�^�C�g���V�[���̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N6��19��
///////////////////////////////////

#include "pch.h"
#include "TitleScene.h"

#include "../SceneManager.h"
#include "Keyboard.h"
#include "Src\FrameWork\DebugFont\DebugFont.h"

#include "Src\FrameWork\Sprite2D\Sprite2D.h"
#include "Src\FrameWork\FadeManager\FadeManager.h"
#include "Game.h"

#include "Adx2.h"
#include "CueSheet_0.h"
#include "Src/Cgdi.h"


//pushSpace�̉摜�̍��W
const DirectX::SimpleMath::Vector2 TitleScene::PUSH_SPACE_POS = DirectX::SimpleMath::Vector2(700.0f,700.0f);
//pushSpace�̉摜�̓_�ŃC���^�[�o��
const float TitleScene::PUSH_SPACE_BLIND_INTERBVAL = 1.0f;
//�V���̍��W
const DirectX::SimpleMath::Vector3 TitleScene::SKYBOX_POS = DirectX::SimpleMath::Vector3(0.0f, 20.0f, 0.0f);



TitleScene::TitleScene()
{
	m_pDeviceResources = nullptr;
	m_pKeyTracker = nullptr;

	m_sceneState = eSCENE_STATE::FADE_IN;
	m_fadeTimer = 1.0f;

	m_pLogoSprite = nullptr;
	m_pSpaceSprite = nullptr;

	m_pushSpaceBlindFlag = true;
	m_pushSpaceBlindTimer = 0.0f;

	m_skyBoxAngleY = 0.0f;
}


TitleScene::~TitleScene()
{
	m_pKeyTracker.reset();
	m_pDeviceResources = nullptr;

	m_pLogoSprite.reset();
	m_pSpaceSprite.reset();

	m_pStates = nullptr;
}


///////////////////////////
//������
//����:�f�o�C�X���\�[�X�̃|�C���^
//�߂�l:�Ȃ�
//////////////////////////
void TitleScene::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();

	m_pStates = gdi->GetStates();

	auto device = m_pDeviceResources->GetD3DDevice();
	auto context = m_pDeviceResources->GetD3DDeviceContext();


	DebugFont::GetInstance()->Create(device, context);

	//�L�[�g���b�J�[�̍쐬
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	m_sceneState = eSCENE_STATE::FADE_IN;

	//�r���[�s����쐬
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

	//��ʃT�C�Y���擾����
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//�ˉe�s����쐬
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);

	//���S�摜�̓ǂݍ���
	m_pLogoSprite = std::make_unique<Sprite2D>();
	m_pLogoSprite->Create(L"Resources/Textures/logo.png");
	//pushSpace�̉摜�̓ǂݍ���
	m_pSpaceSprite = std::make_unique<Sprite2D>();
	m_pSpaceSprite->Create(L"Resources/Textures/pushSpace.png");

	//�V���̃��f���ǂݍ���
	m_pSkyBox = std::make_unique<ModelObject>();
	m_pSkyBox->Create(m_pDeviceResources, L"Resources/Models/Space.cmo");

	m_skyBoxWorld = DirectX::SimpleMath::Matrix::Identity;

	//pushSpace�̉摜�̓_�Ńt���O������
	m_pushSpaceBlindFlag = true;
}

///////////////////////////
//�X�V
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void TitleScene::Update(DX::StepTimer const& timer)
{
	//�L�[�{�[�h�̏�Ԃ̎擾
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	m_pSpaceSprite->Update(PUSH_SPACE_POS);

	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(SKYBOX_POS);
	m_skyBoxAngleY += 0.001f;
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_skyBoxAngleY);

//	m_skyBoxWorld *= rotY;// *pos;
	//m_skyBoxWorld *= rotY;// *pos;
	//m_skyBoxWorld += pos;
	//m_skyBoxWorld *= rotY; 
	//m_skyBoxWorld += pos;
	m_skyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero) * rotY * pos;

	//m_skyBoxWorld *= rotY;

	//�V���̃��f���̍X�V
	m_pSkyBox->Update(m_skyBoxWorld);

	////pushSpace�̉摜�̓_�Ń^�C�}�[�B�t���O�̍X�V
	m_pushSpaceBlindTimer += static_cast<float>(timer.GetElapsedSeconds());
	if (m_pushSpaceBlindTimer >= PUSH_SPACE_BLIND_INTERBVAL)
	{
		m_pushSpaceBlindFlag = !m_pushSpaceBlindFlag;
		m_pushSpaceBlindTimer = 0.0f;
	}
	
	//�V�[���̃X�e�[�g
	switch (m_sceneState)
	{
		//�t�F�[�h�C��
		case SceneBase::eSCENE_STATE::FADE_IN:
		{
			m_fadeTimer -= static_cast<float>(timer.GetElapsedSeconds())* 2.0f;
			if (m_fadeTimer <= 0.0f)m_sceneState = eSCENE_STATE::MAIN;

			break;
		}
		//���C��
		case SceneBase::eSCENE_STATE::MAIN:
		{
			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
			{
				m_sceneState = eSCENE_STATE::FADE_OUT;

				soundID = ADX2::GetInstance().Play(CRI_CUESHEET_0_SUBMIT);
			}

			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Escape))
			{
				PostQuitMessage(0);
			}


			break;
		}
		//�t�F�[�h�A�E�g
		case SceneBase::eSCENE_STATE::FADE_OUT:
		{
			m_fadeTimer += static_cast<float>(timer.GetElapsedSeconds())* 2.0f;
			if(m_fadeTimer >= 1.0f)SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);

			break;
		}
		default:
			break;
	}
	
	//�t�F�[�h�}�l�[�W���[�̍X�V
	FadeManager::GetInstance()->Update(timer, m_fadeTimer);
}

///////////////////////////
//�`��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void TitleScene::Render()
{
	//�V���̕`��
	m_pSkyBox->Render(m_view, m_proj);

	//���S�摜�̕`��
	m_pLogoSprite->Draw(true);
	//pushSpace�̉摜�̕`��
	if(m_pushSpaceBlindFlag)m_pSpaceSprite->Draw(true);
}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void TitleScene::Finalize()
{
	DebugFont::Dispose();

	m_pSkyBox->Lost();

	m_pLogoSprite->Reset();
	m_pSpaceSprite->Reset();
}






