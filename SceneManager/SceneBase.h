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
#include "IScene.h"

class SceneBase : public IScene
{
public:	
	//�R���X�g���N�^
	SceneBase();
	//�f�X�g���N�^
	~SceneBase();

	//������
	void Initialize() override;
	//�X�V
	void Update(DX::StepTimer const& timer) override;
	//�`��
	void Render() override;
	//�I������
	void Finalize() override;

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