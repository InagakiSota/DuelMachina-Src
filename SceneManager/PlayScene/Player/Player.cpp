#include "pch.h"
#include "Player.h"
#include "Keyboard.h"
#include "GamePad.h"

#include "../Character/CharacterBase.h"
#include "../Character/Character1/Character1.h"

//�R���X�g���N
Player::Player()
{
}

//�f�X�g���N�^
Player::~Player()
{
	if (m_pCharacter != nullptr)
	{
		delete m_pCharacter;
		m_pCharacter = nullptr;
	}
}

///////////////////////////
//������
//����:�f�o�C�X���\�[�X�̃|�C���^
//�߂�l:�Ȃ�
//////////////////////////
void Player::Initialize(DX::DeviceResources* pDeviceResources)
{
	//�L�����N�^�[�P�̓ǂݍ���
	m_pCharacter = new Character1();
	//�L�����N�^�[�̏�����
	m_pCharacter->Initialize(pDeviceResources);
}

///////////////////////////
//�X�V
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Player::Update()
{
	////�ړ�
	//Move();

	////�ړ��ʂ����W�ɉ��Z
	//m_pos += m_vel;
	//////Y����]
	////Matrix rotX = Matrix::CreateRotationX(time);
	//DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));
	////Matrix rotZ = Matrix::CreateRotationZ(XMConvertToRadians(30));

	//////m_world = rotX;
	////m_world = rotY *  m_world * rotX;
	////���W���s��ɕϊ�
	//DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	////�T�C�Y���s��ɕϊ�
	//DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(0.0015f, 0.0015f, 0.0015f);
	////���[���h�s��ɉ��Z
	//m_world = scale * rotY* trans;

	////���f���̍X�V
	//m_pModel->Update(m_world);

	////�����̔��̍��W�̍X�V
	//m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x-0.1f, m_pos.y - 1.4f, m_pos.z);
	//m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);
	
	//�L�����N�^�[�̍X�V
	m_pCharacter->Update();
}

///////////////////////////
//�`��
//����:�r���[�s��A�ˉe�s��
//�߂�l:�Ȃ�
//////////////////////////
void Player::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	////���f���̕`��
	//m_pModel->Render(view, proj);

	////�����̔��̕`��
	//m_pLegBox->Draw(m_legBoxWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f,0.0f,0.0f,0.4f));

	//�L�����N�^�[�̕`��
	m_pCharacter->Render(view, proj);
}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Player::Finalize()
{
	////���f���̉��
	//m_pModel->Lost();

	////�W�I���g���v���~�e�B�u�̉��
	//m_pLegBox.reset();

	////delete m_pDeviceResources;
	//m_pDeviceResources = nullptr;

	delete m_pCharacter;
	m_pCharacter = nullptr;
}

///////////////////////////
//�����蔻��p�̑����̔��̎擾
//����:�Ȃ�
//�߂�l:�����蔻��p�̑����̔�
//////////////////////////
Collision::BoxCollision Player::GetLegCollBox()
{
	return m_pCharacter->GetLegCollBox();
}

///////////////////////////
//���Ƃ̓����蔻��
//����:���̓����蔻��̔�
//�߂�l:�����������ǂ���
//////////////////////////
void Player::CheckFloorHit(const Collision::BoxCollision & floorBox)
{
	m_pCharacter->HitFloor(floorBox);
}

