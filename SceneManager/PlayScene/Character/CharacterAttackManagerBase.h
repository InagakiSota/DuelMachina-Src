///////////////////////////////////
//�L�����N�^�[�̍U���Ǘ��N���X�̊��
//�쐬�ҁFGS3_05_��_
//�쐬���F2020�N10��23��
///////////////////////////////////

#pragma once

#include "AttackType.h"
#include <SimpleMath.h>
#include "../../DeviceResources.h"

class CharacterBase;
class CharacterAttackBase;

class CharacterAttackManagerBase
{
public:
	//�f�X�g���N�^
	~CharacterAttackManagerBase() {}

	//������
	virtual void Initialize(CharacterBase* pCharacter, DX::DeviceResources* pDeviceResources) = 0;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	//�I������
	virtual void Finalize() = 0;
	//���Z�b�g
	virtual void Reset() = 0;

protected:
	CharacterAttackBase* m_pCharacterAttackArray[static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM)];
};
