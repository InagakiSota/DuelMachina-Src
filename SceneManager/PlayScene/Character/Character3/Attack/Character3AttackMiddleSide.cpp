///////////////////////////////////
//�L�����N�^�[�R�̒����U���N���X
//�쐬�ҁFGS305_��_�D��
//�쐬���F2020�N11��23��
///////////////////////////////////
#include "pch.h"
#include "Character3AttackMiddleSide.h"
#include "../../CharacterBase.h"
#include "../../../AttackManager/AttackManager.h"

//�U����
const int Character3AttackMiddleSide::POWER = 5;
//�����蔻��̃T�C�Y
const DirectX::SimpleMath::Vector3 Character3AttackMiddleSide::COLL_SIZE = DirectX::SimpleMath::Vector3(0.15f, 0.15f, 0.15f);
//������܂ł̎���
const float Character3AttackMiddleSide::VANISH_TIME = 60.0f;
//�W�I���g���v���~�e�B�u�̃T�C�Y
const float Character3AttackMiddleSide::GP_SIZE = 2.0f;

//�R���X�g���N�^
Character3AttackMiddleSide::Character3AttackMiddleSide()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
	m_world = DirectX::SimpleMath::Matrix::Identity;
}

//�f�X�g���N�^
Character3AttackMiddleSide::~Character3AttackMiddleSide()
{
	m_pCharacter = nullptr;
	m_pAttack = nullptr;
	m_pBulletGP = nullptr;
	m_pDeviceResources = nullptr;
}

///////////////////////////
//������
//����:�L�����N�^�[�̃|�C���^�A�f�o�C�X���\�[�X
//�߂�l:
//////////////////////////
void Character3AttackMiddleSide::Initialize(CharacterBase * pCharacter, DX::DeviceResources * pDeviceResources)
{
	m_pCharacter = pCharacter;
	m_pDeviceResources = pDeviceResources;
}

///////////////////////////
//�X�V
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3AttackMiddleSide::Update()
{
	//�L�[���͂��󂯎��
	if (m_pCharacter->GetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)) == true)
	{
		for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
		{
			if (m_pCharacter->GetAttackManager()->GetAttackStruct(i)->isUsed == false)
			{
				if (m_pAttack == nullptr)
				{
					m_pAttack = m_pCharacter->GetAttackManager()->GetAttackStruct(i);
					break;
				}
			}

		}
		//�v���C���[�̑O�����̃x�N�g����ۑ�
		m_pCharacter->SetAttackFront(m_pCharacter->GetFrontVector());
		//�g�p����Ă����Ԃɂ���
		m_pAttack->isUsed = true;
		//�З͂�ݒ�
		m_pAttack->power = Character3AttackMiddleSide::POWER;
		//�������W��ݒ�
		m_pAttack->pos = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetPos().x + 0.5f * m_pCharacter->GetAttackFront().x,
			m_pCharacter->GetPos().y - 0.2f,
			m_pCharacter->GetPos().z);
		//�����蔻��̃T�C�Y��ݒ�
		m_pAttack->attackColl.size_h = DirectX::SimpleMath::Vector3(0.15f, 0.15f, 0.15f);
		//�v���C���[��ID��ݒ�(�g�p�҂̃v���C���[ID�Ɠ���)
		m_pAttack->playerID = m_pCharacter->GetPlayerID();
		//�ړ��ʂ�ݒ�
		m_pAttack->vel = DirectX::SimpleMath::Vector3(
			m_pCharacter->GetAttackFront().x * 0.2f, 
			0.2f, 
			0.0f);

		//�e�̃W�I���g���v���~�e�B�u����
		m_pBulletGP = DirectX::GeometricPrimitive::CreateSphere(m_pDeviceResources->GetD3DDeviceContext(), m_pAttack->attackColl.size_h.x * Character3AttackMiddleSide::GP_SIZE);

		//�U���g�p���̃t���O�̐ݒ�
		m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), true);
		//�U�����͂̃t���O�̐ݒ�
		m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
		//�U�����̃t���O�̐ݒ�
		m_pCharacter->SetIsAttacking(true);
	}

	if (m_pAttack != nullptr &&m_pCharacter->GetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)) == true)
	{
		DirectX::SimpleMath::Vector3 toEnemyVec =
			DirectX::SimpleMath::Vector3(
				m_pCharacter->GetEnemyPos().x - m_pAttack->pos.x, 
				(m_pCharacter->GetEnemyPos().y - 1.5f) - m_pAttack->pos.y,
				m_pCharacter->GetEnemyPos().z - m_pAttack->pos.z);
		DirectX::SimpleMath::Vector3 addVec = m_pAttack->vel;
		//addVec.Normalize();
		//�^�C�}�[�����Z
		m_pAttack->timer++;

		if (m_pAttack->timer >= 10.0f)
		{
			addVec.y += (toEnemyVec.y * 0.2f);
		}

		if (m_pAttack->timer > 0.0f)
		{
			//�ړ��ʂ����W�ɉ��Z
			m_pAttack->pos += addVec * 0.6f;
			//���W��ݒ�
			m_pAttack->attackColl.pos = m_pAttack->pos;
			m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pAttack->attackColl.pos);
		}

		//�^�C�}�[����莞�Ԓ����������
		if (m_pAttack->timer >= Character3AttackMiddleSide::VANISH_TIME)
		{
			m_pAttack->isUsed = false;

			m_pAttack->timer = 0;

			//m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = false;

		}
		//����������e������
		if (m_pAttack->isHit == true)
		{
			m_pAttack->isUsed = false;
		}

		//���g�p��ԂɂȂ��������
		if (m_pAttack->isUsed == false)
		{
			m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
			m_pCharacter->SetIsAttacking(false);
			m_pAttack->timer = 0;
			m_pAttack->pos.Zero;
			m_pAttack->vel.Zero;
			m_pAttack = nullptr;
			m_pCharacter->SetAttackFront(DirectX::SimpleMath::Vector3::Zero);
			m_pBulletGP.reset();
			m_world = DirectX::SimpleMath::Matrix::Identity;

		}

	}

}

///////////////////////////
//�`��
//����:�r���[�s��A�ˉe�s��
//�߂�l:�Ȃ�
//////////////////////////
void Character3AttackMiddleSide::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	if (m_pBulletGP != nullptr)
	{
		if (m_pCharacter->GetPlayerID() == ePLAYER_ID::PLAYER_1)
			m_pBulletGP->Draw(m_world, view, proj, DirectX::Colors::Red);
		else if (m_pCharacter->GetPlayerID() == ePLAYER_ID::PLAYER_2)
			m_pBulletGP->Draw(m_world, view, proj, DirectX::Colors::Blue);
	}
}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3AttackMiddleSide::Finalize()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}
	m_pDeviceResources = nullptr;
}

///////////////////////////
//���Z�b�g
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3AttackMiddleSide::Reset()
{
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}

	m_pCharacter->SetIsAttackUse(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
	m_pCharacter->SetIsAttackInput(static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE), false);
	m_pCharacter->SetIsAttacking(false);
}
