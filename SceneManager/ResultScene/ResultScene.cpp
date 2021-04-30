///////////////////////////////////
//���U���g�V�[���̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N7��28��
///////////////////////////////////

#include "pch.h"
#include "ResultScene.h"

#include "Keyboard.h"
#include "../SceneManager.h"
#include "Src\FrameWork\Sprite2D\Sprite2D.h"
#include "Src\FrameWork\FadeManager\FadeManager.h"
#include "Src/Cgdi.h"

ePLAYER_ID ResultScene::m_winPlayerID = ePLAYER_ID::PLAYER_1;

//�R���X�g���N�^
ResultScene::ResultScene()
{
}

//�f�X�g���N�^
ResultScene::~ResultScene()
{
	m_pStates = nullptr;
}

///////////////////////////
//������
//����:�E�B���h�E�A�f�o�C�X�̃|�C���^
//�߂�l:�Ȃ�
//////////////////////////
void ResultScene::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();

	m_pStates = gdi->GetStates();

	auto device = m_pDeviceResources->GetD3DDevice();

	// TODO: Initialize device dependent objects here (independent of window size).
	device;

	auto context = m_pDeviceResources->GetD3DDeviceContext();

	//�r���[�s����쐬����
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.f, 5.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);

	//��ʃT�C�Y���擾����
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//�ˉe�s������
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);

	//m_pDebugCamera = std::make_unique<DebugCamera>();

	m_fadeTimer = 1.0f;
	m_sceneState = eSCENE_STATE::FADE_IN;

	//�L�[�g���b�J�[�̍쐬
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	m_winPlayer1Sprite = std::make_unique<Sprite2D>();
	m_winPlayer1Sprite->Create(L"Resources/Textures/p1win_result.png");
	m_winPlayer2Sprite = std::make_unique<Sprite2D>();
	m_winPlayer2Sprite->Create(L"Resources/Textures/p2win_result.png");
	m_winPlayer1Sprite->Update(640.0f, 300.0f);
	m_winPlayer2Sprite->Update(640.0f, 300.0f);

}

///////////////////////////
//�X�V
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void ResultScene::Update(DX::StepTimer const & timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//�r���[�s��̍X�V
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::UnitY);
	//m_view = m_pDebugCamera->getViewMatrix();

	float time = float(timer.GetTotalSeconds());

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
			////���̃��[���h�s��̍X�V
			//Matrix boxScale = Matrix::CreateScale(Vector3(1, 1, 1));
			//Matrix boxTrans = Matrix::CreateTranslation(Vector3(1.5f, 0, 0));
			//Matrix boxRotY = Matrix::CreateRotationY(time);

			//cupWorld = boxScale  * boxRotY* boxTrans;

			//static float rot = 0;
			//rot += 0.01f;
			//Matrix effectRotY = Matrix::CreateRotationY(rot);
			//Matrix effectTrans = Matrix::CreateTranslation(0.0f, 1.0f, -1.0f);
			//Matrix effectScale = Matrix::CreateScale(2.0f, 2.0f, 0.0f);

			//Matrix effectWorld = effectScale * effectRotY * effectTrans;

	


			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
			{
				m_sceneState = eSCENE_STATE::FADE_OUT;
			}

			break;
		}


		//�t�F�[�h�A�E�g
		case SceneBase::eSCENE_STATE::FADE_OUT:
		{
			m_fadeTimer += static_cast<float>(timer.GetElapsedSeconds())* 2.0f;

			if (m_fadeTimer >= 1.0f)SceneManager::GetInstance()->SetScene(eSCENE_ID::CHARA_SELECT_SCENE);

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
void ResultScene::Render()
{
	if (m_winPlayerID == ePLAYER_ID::PLAYER_1)
	{
		m_winPlayer1Sprite->Draw(true, false);
	}
	else if (m_winPlayerID == ePLAYER_ID::PLAYER_2)
	{
		m_winPlayer2Sprite->Draw(true, false);
	}

}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void ResultScene::Finalize()
{
	m_winPlayer1Sprite->Reset();
	m_winPlayer2Sprite->Reset();

}
