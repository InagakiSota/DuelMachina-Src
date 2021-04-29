///////////////////////////////////
//�v���C���[�̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N6��25��
///////////////////////////////////

#pragma once

#include <SimpleMath.h>
#include "GeometricPrimitive.h"
#include "Src\FrameWork\ModelObject\ModelObject.h"
#include "Src\FrameWork\Collision\Collision.h"
//#include "../Character/CharacterBase.h"

//class ModelObject;
class CharacterBase;


class Player
{
public:
	//�R���X�g���N�^
	Player();
	//�f�X�g���N�^
	~Player();	

	//������
	void Initialize(DX::DeviceResources* pDeviceResources);
	//�X�V
	void Update();
	//�`��
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	//�I������
	void Finalize();
	//�����蔻��p�̑����̔��̎擾
	Collision::BoxCollision GetLegCollBox();								
	//���Ƃ̓����蔻��
	void CheckFloorHit(const Collision::BoxCollision& floorBox);			


private:
	//static const float WALK_FORCE;								//�����̈ړ���
	//static const float JUMP_FORCE;								//�W�����v�̈ړ���
	//static const float GRAVITY;									//�d��

	//DX::DeviceResources* m_pDeviceResources;					//�f�o�C�X���\�[�X�̃|�C���^
	//DirectX::SimpleMath::Matrix m_world;						//���[���h�s��
	//DirectX::SimpleMath::Vector3 m_pos;							//���W
	//DirectX::SimpleMath::Vector3 m_vel;							//�ړ���
	//std::unique_ptr<ModelObject> m_pModel;						//3D���f��
	//std::unique_ptr<DirectX::GeometricPrimitive> m_pLegBox;		//�����̔�
	//Collision::BoxCollision m_legCollBox;						//�����蔻��p�̑����̔�
	//DirectX::SimpleMath::Matrix m_legBoxWorld;					//�����̔��̃��[���h�s��

	//bool m_landingFlag;											//���n�t���O
	//bool m_jumpFlag;											//�W�����v�t���O

	//�L�����N�^�[�̃|�C���^
	CharacterBase* m_pCharacter;
};