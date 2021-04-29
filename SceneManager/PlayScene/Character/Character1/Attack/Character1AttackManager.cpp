///////////////////////////////////
//�L�����N�^�[�P�̍U���}�l�[�W���[�N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N11��13��
///////////////////////////////////

#include "pch.h"
#include "Character1AttackManager.h"
#include "../../CharacterBase.h"
#include "../../CharacterAttackBase.h"

#include "Character1AttackWeakNeutral.h"
#include "Character1AttackWeakSide.h"
#include "Character1AttackWeakBottom.h"
#include "Character1AttackMiddleNeutral.h"
#include "Character1AttackMiddleBottom.h"
#include "Character1AttackMiddleSide.h"


//�R���X�g���N�^
Character1AttackManager::Character1AttackManager()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_pCharacterAttackArray[i] = nullptr;
	}
}

//�f�X�g���N�^
Character1AttackManager::~Character1AttackManager()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if(m_pCharacterAttackArray[i] != nullptr)delete m_pCharacterAttackArray[i];
		m_pCharacterAttackArray[i] = nullptr;
	}
}

///////////////////////////
//������
//����:�L�����N�^�[�̃|�C���^
//�߂�l:�Ȃ�
//////////////////////////
void Character1AttackManager::Initialize(CharacterBase * pCharacter, DX::DeviceResources* pDeviceResources)
{
	//��j���[�g����
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = new Character1AttackWeakNeutral();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)]->Initialize(pCharacter, pDeviceResources);
	//�㉡
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = new Character1AttackWeakSide();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)]->Initialize(pCharacter, pDeviceResources);
	//�㉺
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = new Character1AttackWeakBottom();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)]->Initialize(pCharacter, pDeviceResources);
	//���j���[�g�����U��
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = new Character1AttackMiddleNeutral();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)]->Initialize(pCharacter, pDeviceResources);
	//�����U��
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = new Character1AttackMiddleBottom();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)]->Initialize(pCharacter, pDeviceResources);
	//�����U��
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = new Character1AttackMiddleSide();
	m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)]->Initialize(pCharacter, pDeviceResources);

	PI;
}

///////////////////////////
//�X�V
//����:�U���̓��̓t���O
//�߂�l:�Ȃ�
//////////////////////////
void Character1AttackManager::Update()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{	
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			m_pCharacterAttackArray[i]->Update();
		}
	}
}

///////////////////////////
//�`��
//����:�r���[�s��A�ˉe�s��
//�߂�l:�Ȃ�
//////////////////////////
void Character1AttackManager::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			m_pCharacterAttackArray[i]->Render(view,proj);
		}
	}
}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1AttackManager::Finalize()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			m_pCharacterAttackArray[i]->Finalize();
		}

		delete m_pCharacterAttackArray[i];
		m_pCharacterAttackArray[i] = nullptr;
	}
}

///////////////////////////
//���Z�b�g
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1AttackManager::Reset()
{
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		if (m_pCharacterAttackArray[i] != nullptr)
		{
			m_pCharacterAttackArray[i]->Reset();
		}
	}

}
