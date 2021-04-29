///////////////////////////////////
//�u�[�X�g�̃G�t�F�N�g�N���X
//�쐬�ҁFGT2_05_��_�D��
//�쐬���F2021�N4��5��
///////////////////////////////////

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include "Effect.h"
#include <list>
#include "BoostEffect.h"


class BoostEffectManager
{
public:
	enum class eEFFECT_TYPE
	{
		NONE = -1,
		RANDOM,			//�����_��
		AVERAGE,		//�ϓ�
		CONE,			//�R�[��

		OVER_ID,
	};

	//�R���X�g���N�^
	BoostEffectManager();
	//�f�X�g���N�^
	~BoostEffectManager();

	//Create()�őS�������͒����̂ŕ���
	void Create(DX::DeviceResources* deviceResources, const int& count);

	//������
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type = eEFFECT_TYPE::RANDOM);
	//���Ԋu�ŃG�t�F�N�g���΂������̏�����
	void InitializeAverage(float life, DirectX::SimpleMath::Vector3 pos);
	//�����_���ɃG�t�F�N�g���΂������̏�����
	void InitializeRandom(float life, DirectX::SimpleMath::Vector3 pos);
	//�R�[����ɃG�t�F�N�g���΂������̏�����
	void InitializeCone(float life, DirectX::SimpleMath::Vector3 pos);

	//�X�V
	void Update(DX::StepTimer timer);
	//�`��
	void Render();
	//�J�����A�ˉe�A�r���[�̔��f
	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//�I������
	void Finalize();
	//�d�͂�ݒ肷��
	void SetGravity(bool gravity);

	//�Đ�
	void Play(float life, DirectX::SimpleMath::Vector3 pos, eEFFECT_TYPE type = eEFFECT_TYPE::CONE);
	//���Z�b�g
	void Reset();

private:
	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//�G�t�F�N�g�̃|�C���^
	std::list<std::unique_ptr<BoostEffect>>	m_boostEffectList;
	//�e�N�X�`���f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;

	//�Đ��t���O
	bool m_isPlay;
	//�J�������W
	DirectX::SimpleMath::Vector3 m_cameraPos;
};