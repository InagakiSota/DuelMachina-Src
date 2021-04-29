///////////////////////////////////
//�u�[�X�g�̃G�t�F�N�g�N���X
//�쐬�ҁFGT2_05_��_�D��
//�쐬���F2021�N4��5��
///////////////////////////////////

#include "pch.h"
#include "BoostEffectManager.h"

#include "DeviceResources.h" 
#include "StepTimer.h" 
#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h> 

#include "../../BinaryFile.h"

//�R���X�g���N�^
BoostEffectManager::BoostEffectManager()
	:m_isPlay(false)
{
}

//�f�X�g���N�^
BoostEffectManager::~BoostEffectManager()
{
}

///////////////////////////
//�쐬
//����:�f�o�C�X���\�[�X�A�G�t�F�N�g�̌�
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::Create(DX::DeviceResources * deviceResources, const int & count)
{
	//D3DDevice���擾����
	auto device = deviceResources->GetD3DDevice();

	//�o�b�`�G�t�F�N�g�̍쐬
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	//�v���~�e�B�u�o�b�`�̍쐬
	m_batch =
		std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(deviceResources->GetD3DDeviceContext());

	//�e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/boostEffect_2.png", nullptr,
		m_texture.GetAddressOf());


	//�G�t�F�N�g�����̉����A���X�g�ɓo�^����B
	for (int i = 0; i < count; i++) {
		std::unique_ptr<BoostEffect> effect = std::make_unique<BoostEffect>();
		effect->Create(deviceResources, m_texture.Get(), m_inputLayout.Get());
		m_boostEffectList.push_back(std::move(effect));
	}

}

///////////////////////////
//������
//����:�����A���W�A�G�t�F�N�g�̔��˃^�C�v
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type)
{
	switch (type)
	{
		//�����_��
		case BoostEffectManager::eEFFECT_TYPE::RANDOM:
		{
			InitializeRandom(life, pos);
			break;

		}

		//�ϓ�
		case BoostEffectManager::eEFFECT_TYPE::AVERAGE:
		{
			InitializeAverage(life, pos);

			break;

		}

		//�V�F�[�_
		case BoostEffectManager::eEFFECT_TYPE::CONE:
		{
			InitializeCone(life, pos);

			break;
		}
		default:
			break;
	}

}

///////////////////////////
//������(����)
//����:�����A���W
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::InitializeAverage(float life, DirectX::SimpleMath::Vector3 pos)
{
	const float RAD = DirectX::XM_PI * 2.0f;
	int num = 0;
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		float size = (float)m_boostEffectList.size();
		DirectX::SimpleMath::Vector3 vel =
			DirectX::SimpleMath::Vector3(cos(RAD * num / size + DirectX::XM_PI / 2.0f), sin(RAD * num / size + DirectX::XM_PI / 2.0f), 0);
		vel *= 0.1f;
		(*ite)->Initialize(life, pos, vel);
		num++;
	}

}

///////////////////////////
//������(�����_��)
//����:�����A���W
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::InitializeRandom(float life, DirectX::SimpleMath::Vector3 pos)
{
	int range = 100;

	//life,pos,vel �̒l��m_effect������������
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		DirectX::SimpleMath::Vector3 vel =
			DirectX::SimpleMath::Vector3(
			((rand() % (range * 2)) - range) / (float)range * 0.2f,
				((rand() % (range * 2)) - range) / (float)range * 0.2f,
				((rand() % (range * 2)) - range) / (float)range * 0.2f);

		while (vel.Length() < 0.03f)
		{
			vel =
				DirectX::SimpleMath::Vector3(
				((rand() % (range * 2)) - range) / (float)range * 0.1f,
					((rand() % (range * 2)) - range) / (float)range * 0.1f,
					((rand() % (range * 2)) - range) / (float)range * 0.1f);
		}

		(*ite)->Initialize(life, pos, vel);
		//m_world = DirectX::SimpleMath::Matrix::CreateRotationZ(atan2f(vel.y,vel.x));
	}

}

///////////////////////////
//������(�R�[��)
//����:�����A���W
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::InitializeCone(float life, DirectX::SimpleMath::Vector3 pos)
{
	//�G�t�F�N�g�̒��S��
	DirectX::SimpleMath::Vector3 dir = DirectX::SimpleMath::Vector3(-3, 1, 0);

	//���̒l�𐳋K������
	dir.Normalize();

	//�O�ςɎg�����߂̃J�����ʒu�i�{���͊O����󂯓n����Ɨǂ��j
	DirectX::SimpleMath::Vector3 camera = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 6.0f);


	//life,pos,vel �̒l��m_effect������������
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		//���ƃJ��������O�ς����߂�
		DirectX::SimpleMath::Vector3 d = dir.Cross(camera);
		//�O�ς𐳋K������
		d.Normalize();
		//�O�ς̒l�ƒ��S�������Z����
		d = dir + d * (float)sin(rand())*0.3f;
		//�l���傫���̂�0.1�{
		d *= 0.1f;
		(*ite)->Initialize(life, pos, d);
	}

}

///////////////////////////
//�X�V
//����:�^�C�}�[
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::Update(DX::StepTimer timer)
{
	if (m_isPlay == true)
	{
		for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
		{
			(*ite)->Update(timer);

			//�G�t�F�N�g�̎�����0�ɂȂ�����Đ�����߂�
			if ((*ite)->GetLife() < 0)m_isPlay = false;
		}
	}

}

///////////////////////////
//�`��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::Render()
{
	if (m_isPlay == true)
	{
		for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
		{
			(*ite)->Render();
		}
	}

}

///////////////////////////
//�J�����A�r���[�s��A�ˉe�s��̐ݒ�
//����:�J�������W�A�r���[�s��A�ˉe�s��
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera, view, proj);
	}
	m_cameraPos = camera;
}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::Finalize()
{
}

///////////////////////////
//�d�͂̐ݒ�
//����:�d�̗͂L��
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::SetGravity(bool gravity)
{
	for (std::list<std::unique_ptr<BoostEffect>>::iterator ite = m_boostEffectList.begin(); ite != m_boostEffectList.end(); ite++)
	{
		(*ite)->SetGravity(gravity);
	}

}

///////////////////////////
//�Đ�
//����:�����A���W�A�^�C�v
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::Play(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type)
{
	switch (type)
	{
		//�����_��
		case BoostEffectManager::eEFFECT_TYPE::RANDOM:
		{
			InitializeRandom(life, pos);
			break;

		}

		//�ϓ�
		case BoostEffectManager::eEFFECT_TYPE::AVERAGE:
		{
			InitializeAverage(life, pos);

			break;

		}

		//�V�F�[�_
		case BoostEffectManager::eEFFECT_TYPE::CONE:
		{
			InitializeCone(life, pos);

			break;
		}
		default:
			break;
	}
	//�Đ��t���O�𗧂Ă�
	m_isPlay = true;

}

///////////////////////////
//���Z�b�g
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void BoostEffectManager::Reset()
{
	m_isPlay = false;

}
