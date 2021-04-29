///////////////////////////////////
//�V�[���̊��N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N6��19��
///////////////////////////////////

#pragma once
#include "../../DeviceResources.h"
#include "../../StepTimer.h"
#include "Keyboard.h"
#include <SimpleMath.h>
#include <CommonStates.h>

class SceneBase
{
public:	
	//�f�X�g���N�^
	virtual ~SceneBase()
	{

	}

	//������
	virtual void Initialize(HWND window, DX::DeviceResources* pDeviceResources, DirectX::CommonStates* pStates) = 0;
	//�X�V
	virtual void Update(DX::StepTimer const& timer) = 0;
	//�`��
	virtual void Render() = 0;
	//�I������
	virtual void Finalize() = 0;

protected:
	//�V�[���̃X�e�[�g
	enum class eSCENE_STATE
	{
		NONE = -1,

		FADE_IN,
		MAIN,
		FADE_OUT,

		OVER,
	};

	//�f�o�C�X
	DX::DeviceResources* m_pDeviceResources;

	DirectX::CommonStates* m_pStates;

	//�L�[�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_pKeyTracker;
	//�V�[���̃X�e�[�g�ϐ�
	eSCENE_STATE m_sceneState;
	//�t�F�[�h�̃^�C�}�[
	float m_fadeTimer;
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;
	//�J�����̍��W
	DirectX::SimpleMath::Vector3 m_cameraPos;
	//����ID
	int soundID;

};	 