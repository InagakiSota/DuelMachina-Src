///////////////////////////////////
//�L�����N�^�[�̍U���̊��N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N11��13��
///////////////////////////////////

#pragma once

#include <SimpleMath.h>
#include "GeometricPrimitive.h"
#include "../../DeviceResources.h"

class CharacterBase;
struct AttackStruct;

class CharacterAttackBase
{
public:
	~CharacterAttackBase() {};
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
	//�L�����N�^�[�̃|�C���^
	CharacterBase* m_pCharacter;
	//�U���̃|�C���^
	AttackStruct* m_pAttack;
	//�e�̃W�I���g���v���~�e�B�u
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBulletGP;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�f�o�C�X���\�[�X�̃|�C���^
	DX::DeviceResources* m_pDeviceResources;
};