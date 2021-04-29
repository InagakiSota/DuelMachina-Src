///////////////////////////////////
//�L�����N�^�[�R�̒����U���N���X
//�쐬�ҁFGS305_��_�D��
//�쐬���F2020�N11��23��
///////////////////////////////////

#pragma once

#include "../../CharacterAttackBase.h"

class Character3AttackMiddleSide : public CharacterAttackBase
{
public:
	//�R���X�g���N�^
	Character3AttackMiddleSide();
	//�f�X�g���N�^
	~Character3AttackMiddleSide();

	//������
	void Initialize(CharacterBase* pCharacter, DX::DeviceResources* pDeviceResources) override;
	//�X�V
	void Update() override;
	//�`��
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	//�I������
	void Finalize() override;
	//���Z�b�g
	void Reset() override;

private:
	//�U����
	static const int POWER;
	//�����蔻��̃T�C�Y
	static const DirectX::SimpleMath::Vector3 COLL_SIZE;
	//������܂ł̎���
	static const float VANISH_TIME;
	//�W�I���g���v���~�e�B�u�̃T�C�Y
	static const float GP_SIZE;
};