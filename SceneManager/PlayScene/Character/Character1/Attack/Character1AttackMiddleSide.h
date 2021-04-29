///////////////////////////////////
//�����U��
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N11��17��
///////////////////////////////////
#pragma once

#include "../../CharacterAttackBase.h"

class Character1AttackMiddleSide : public CharacterAttackBase
{
public:
	//�R���X�g���N�^
	Character1AttackMiddleSide();
	//�f�X�g���N�^
	~Character1AttackMiddleSide();

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
	//X���W
	static const float POS_X;
	//Y���W
	static const float POS_Y;
	//X�����̈ړ���
	static const float VEL_X;
	//Y�����̈ړ���
	static const float VEL_Y;
	//������܂ł̎���
	static const float VANISH_TIME;
	//�W�I���g���v���~�e�B�u�̃T�C�Y
	static const int GP_SIZE;
	//�����蔻��̃T�C�Y
	static const DirectX::SimpleMath::Vector3 COLL_SIZE;

};