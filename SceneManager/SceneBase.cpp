///////////////////////////////////
//�V�[���̊��N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N6��19��
///////////////////////////////////
#include "pch.h"
#include "SceneBase.h"

//�R���X�g���N�^
SceneBase::SceneBase()
	:m_pDeviceResources(nullptr),
	m_pStates(nullptr),
	m_pKeyTracker(nullptr),
	m_fadeTimer(1.0f),
	m_sceneState(eSCENE_STATE::FADE_IN),
	m_view{},
	m_proj{},
	m_cameraPos{}
{
}

//�f�X�g���N�^
SceneBase::~SceneBase()
{
}

///////////////////////////
//������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::Initialize()
{
}

///////////////////////////
//�X�V
//����:�X�e�b�v�^�C�}�[
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::Update(DX::StepTimer const& timer)
{
}

///////////////////////////
//�`��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::Render()
{
}

///////////////////////////
//�I������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void SceneBase::Finalize()
{
}
