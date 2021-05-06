///////////////////////////////////
//�V�[���̃C���^�[�t�F�[�X�N���X
//�쐬�ҁFGT2_04_��_�D��
//�쐬���F2021�N5��6��
///////////////////////////////////

#pragma once

#include <StepTimer.h>
#include "DeviceResources.h"

class IScene
{
public:
	//�f�X�g���N�^
	virtual ~IScene() {}

	//������
	virtual void Initialize() = 0;
	//�X�V
	virtual void Update(DX::StepTimer const& timer) = 0;
	//�`��
	virtual void Render() = 0;
	//�I������
	virtual void Finalize() = 0;

	///////////////////////////////
	//�Q�b�^�[�A�Z�b�^�[
	//////////////////////////////
	//�f�o�C�X���\�[�X�̎擾
	virtual DX::DeviceResources* GetDeviceResources() = 0;
};
