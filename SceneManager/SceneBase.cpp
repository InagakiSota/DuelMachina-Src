///////////////////////////////////
//�V�[���̊��N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N6��19��
///////////////////////////////////
#include "pch.h"
#include "SceneBase.h"
#include "Src/Cgdi.h"

//�R���X�g���N�^
SceneBase::SceneBase()
	:m_pDeviceResources(nullptr),
	m_pStates(nullptr),
	m_pKeyTracker(nullptr),
	m_fadeTimer(1.0f),
	m_sceneState(eSCENE_STATE::FADE_IN),
	m_view{},
	m_proj{},
	m_cameraPos{},
	m_isShake(false)
{
}

//�f�X�g���N�^
SceneBase::~SceneBase()
{
	m_pKeyTracker.reset();
	m_pDeviceResources = nullptr;
	m_pStates = nullptr;

}

///////////////////////////
//������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();
	m_pStates = gdi->GetStates();

	auto device = m_pDeviceResources->GetD3DDevice();
	auto context = m_pDeviceResources->GetD3DDeviceContext();

	//�L�[�g���b�J�[�̍쐬
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	//�V�[���̃X�e�[�g�ݒ�
	m_sceneState = eSCENE_STATE::FADE_IN;

	//�r���[�s����쐬
	m_cameraPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_cameraPos, DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), DirectX::SimpleMath::Vector3::UnitY);

	//��ʃT�C�Y���擾����
	RECT outputSize = m_pDeviceResources->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	//�ˉe�s����쐬
	m_proj = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f, float(backBufferWidth) / float(backBufferHeight), 0.01f, 1000.0f);

}

///////////////////////////
//�X�V
//����:�X�e�b�v�^�C�}�[
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::Update(DX::StepTimer const& timer)
{
	//�L�[�{�[�h�̏�Ԃ̎擾
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

}

///////////////////////////
//�`��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::Render()
{

}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::Finalize()
{

}

///////////////////////////
//�J�����̐U��
//����:�X�e�b�v�^�C�}�[�A�U�����鎞�ԁA�U�����鋭��
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::CameraShake(DX::StepTimer const& timer, float duration, float magnitude)
{
	if (m_isShake == false)
	{
		m_isShake = true;
		//�J�����̐U�������s
		//DoShake(timer, duration, magnitude);
	}
}

///////////////////////////
//�J�����̐U�������s
//����:�X�e�b�v�^�C�}�[�A�U�����鎞�ԁA�U�����鋭��
//�߂�l:
//////////////////////////
void SceneBase::DoShake(DX::StepTimer const& timer, float duration, float magnitude)
{
	if (m_isShake == true)
	{
		//�������W���擾
		DirectX::SimpleMath::Vector3 pos = m_cameraPos;
		//�o�ߎ���
		float elapsed = 0.0f;
		//�I�����ԂɂȂ�܂ŌJ��Ԃ�
		while (elapsed < duration)
		{
			//X��Y�����Ƀ����_���Ɉړ�������
			float x = pos.x + ((rand() % 3) - 1.0f) * magnitude;
			float y = pos.y + ((rand() % 3) - 1.0f) * magnitude;

			m_cameraPos = DirectX::SimpleMath::Vector3(x, y, pos.z);
			//�o�ߎ��Ԃ����Z����
			elapsed += timer.GetElapsedSeconds();

			// yield return null;
		}
		//�������W�ɖ߂�
		m_cameraPos = pos;
		//�J�����̐U���t���O������
		//m_isShake = false;
	}
}


