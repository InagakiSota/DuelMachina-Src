///////////////////////////////////
//�L�����N�^�[2�̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N7��13��
///////////////////////////////////
#include "pch.h"
#include "Character2.h"
#include "GamePad.h"
#include "../../../PlayerID.h"
#include "../../AttackManager/AttackManager.h"
#include "Src\FrameWork\FbxModel\FbxModel.h"
#include "../CharacterState.h"
#include "State\Character2StateManager.h"
#include "Attack\Character2AttackManager.h"
#include "Character2Params.h"
#include "../ChracterAnimationNumber.h"

#include "Adx2.h"
#include "CueSheet_0.h"
#include "../../../../FrameWork/FbxResourceManager/FbxResourceManager.h"
#include "../CharacterID.h"



//�R���X�g���N�^
Character2::Character2(ePLAYER_ID playerID)
{
	m_jumpFlag = false;
	m_landingFlag = false;
	m_legBoxWorld.Identity;
	m_legCollBox = {};
	m_pDeviceResources = nullptr;
	m_pLegBox = nullptr;
	m_pModel = nullptr;
	m_pos.Zero;
	m_vel.Zero;
	m_frontVector.Zero;
	m_pBodyBox = nullptr;
	m_bodyBoxWorld.Identity;
	m_bodyCollBox = {};
	m_startPos.Zero;
	m_pKeyTracker = nullptr;
	m_playerID = playerID;
	m_isHitEnemyBody = false;
	m_posBuf.Zero;

	m_pAttack = nullptr;

	m_pAttackManager = nullptr;
	m_enemyPos.Zero;
	m_pHeadBox = nullptr;
	m_headCollBox = {};
	m_headBoxWorld.Identity;
	m_HP = 0;
	m_HPBuffer = 0;
	m_charaState = eCHARACTER_STATE::NONE;
	m_damageTimer = 0.0f;
	m_angleY = 0.0f;
	m_aiAttackTimer = 0.0f;
	m_aiStateTiemer = 0.0f;
	m_aiState = 0;
	m_boostCap = 0;

	m_pFbxModel = nullptr;

	m_pStateManager = nullptr;
	m_pCharacterAttackManager = nullptr;
	m_attackFront = DirectX::SimpleMath::Vector3::Zero;
	m_isAttacking = false;

	m_pEnemy = nullptr;
}

//�f�X�g���N�^
Character2::~Character2()
{
	//���\�[�X�̉��
	if (m_pBodyBox != nullptr)
	{
		m_pBodyBox.reset();
	}

	if (m_pLegBox != nullptr)
	{
		m_pLegBox.reset();
	}

	if (m_pDeviceResources != nullptr)
	{
		m_pDeviceResources = nullptr;
	}

	if (m_pKeyTracker != nullptr)
	{
		m_pKeyTracker.reset();
	}

	if (m_pModel != nullptr)
	{
		m_pModel.reset();
	}
}

///////////////////////////
//������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::Initialize(DX::DeviceResources* pDeviceResources, HWND window)
{
	m_pDeviceResources = pDeviceResources;

	////CMO��ǂݍ���Ń��f�����쐬����
	//m_pModel = std::make_unique<ModelObject>();
	//m_pModel->Create(m_pDeviceResources, L"Resources/Models/robot2maya7.cmo");

	//FBX��ǂݍ���Ń��f�����쐬
	//�ҋ@
	//m_pFbxModel = new FbxModel();
	//m_pFbxModel->Load(
	//	window,
	//	pDeviceResources->GetD3DDevice(),
	//	pDeviceResources->GetD3DDeviceContext(),
	//	pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/robot2/robot2animation.fbx",
	//	true
	//);
	m_pFbxModel = FbxResourceManager::GetInstance()->GetModel(eCHARACTER_ID::CHARACTER_2);

	//m_pShieldModel = new FbxModel();
	//m_pShieldModel->Load(
	//	window,
	//	pDeviceResources->GetD3DDevice(),
	//	pDeviceResources->GetD3DDeviceContext(),
	//	pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/Shield.fbx",
	//	false
	//);

	m_pShieldModel = FbxResourceManager::GetInstance()->GetShieldModel();

	m_shieldWorld = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3::Zero);
	m_shieldWorld = pos;


	//�X�e�[�^�X�̓ǂݍ���
	Character2Params::GetInstance()->LoadStatusData();

	//�������W�̐ݒ�	
	m_pos = m_startPos;

	//�����̓����蔻��̔��̓ǂݍ���
	m_legCollBox.size_h = Character2Params::LEG_COLL_SIZE;
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.7f, m_pos.z);
	m_pLegBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_legCollBox.size_h * 2);
	m_legBoxWorld.Identity;

	//�̂̓����蔻��̔��̓ǂݍ���
	m_bodyCollBox.size_h = Character2Params::BODY_COLL_SIZE_NORMAL;
	m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.9f, m_pos.z);
	m_pBodyBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_bodyCollBox.size_h * 2);
	m_pBodySquatBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), Character2Params::BODY_COLL_SIZE_SQUAT * 2);
	m_bodyBoxWorld.Identity;

	//���̓����蔻��̔��̓ǂݍ���
	m_headCollBox.size_h = Character2Params::HEAD_COLL_SIZE;
	m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);
	m_pHeadBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_headCollBox.size_h * 2);
	m_headBoxWorld.Identity;

	//�L�[�g���b�J�[�̓ǂݍ���
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	////�����̈ړ��ʂ̐ݒ�
	//m_walkForce = 0.05f;
	////�W�����v�̈ړ��ʂ̐ݒ�
	//m_jumpForce = 0.14f;

	//�O�����̐ݒ�
	m_frontVector = Character2Params::FRONT_VECTOR;

	//�̗͂̐ݒ�
	m_HP = Character2Params::GetInstance()->MAX_HP;
	m_HPBuffer = m_HP;

	//�̂̐F�̐ݒ�
	m_bodyColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.4f);


	//�L�����̃X�e�[�g������
	m_charaState = eCHARACTER_STATE::WAIT;

	if (m_playerID == ePLAYER_ID::PLAYER_1)m_angleY = Character2Params::ANGLE_Y;
	if (m_playerID == ePLAYER_ID::PLAYER_2)m_angleY = -180.0f + Character2Params::ANGLE_Y;

	//���W���s��ɕϊ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(Character2Params::MODEL_SIZE);
	//Y���̊p�x���s��ɕϊ�
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_angleY);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;


	//�u�[�X�g�e�ʂ̏�����
	m_boostCap = Character2Params::GetInstance()->BOOST_CAP_MAX;

	//���f���̍X�V
	//m_pModel->Update(m_world);

	//�U���t���O�̏�����
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_isAttackInput[i] = false;
		m_isAttackUse[i] = false;
	}

	//�X�e�[�g�}�l�[�W���[�̓ǂݍ��݁A������
	m_pStateManager = std::make_unique<Character2StateManager>();
	m_pStateManager->Initialize(this);
	
	//�L�����N�^�[�̍U���}�l�[�W���[�̓ǂݍ��݁A������
	m_pCharacterAttackManager = std::make_unique<Character2AttackManager>();
	m_pCharacterAttackManager->Initialize(this,m_pDeviceResources);

	//�q�b�g�G�t�F�N�g�̓ǂݍ��݁A�쐬
	m_pHitEffectManager = std::make_unique<HitEffectManager>();
	m_pHitEffectManager->Create(m_pDeviceResources, 50);

	//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̓ǂݍ��݁A�쐬
	m_pBoostEffectManager = std::make_unique<BoostEffectManager>();
	m_pBoostEffectManager->Initialize(m_pDeviceResources, 1, 1, DirectX::SimpleMath::Vector3::Zero);

}

///////////////////////////
//�X�V
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::Update(DX::StepTimer const& timer)
{
	m_stepTimer = timer;
	//�L�[�{�[�h�̏�Ԃ̎擾
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//�X�e�[�g�̊Ǘ�
	if (m_playerID == ePLAYER_ID::PLAYER_1 && m_HP > 0)
	{
		StateManager();
	}

	//�v���C���[�Q�Ȃ�AI����
	if (m_playerID == ePLAYER_ID::PLAYER_2 && m_HP > 0)
	{
		AI();
	}

	////�d�͗���
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;

	}
	else if (m_landingFlag == true)m_vel.y = 0.0f;

	//�ړ�����
	if (m_pos.x > -MOVE_LIMIT_X || m_pos.x < MOVE_LIMIT_X)m_vel.x = 0.0f;
	if (m_pos.x < -MOVE_LIMIT_X) m_pos.x = -MOVE_LIMIT_X;
	if (m_pos.x > MOVE_LIMIT_X) m_pos.x = MOVE_LIMIT_X;

	//�v���C���[��Y���̊p�x
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = Character2Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = -180.0f + Character2Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = -1.0f;

	}
	//��]�s����쐬
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));

	//�L�����̃X�e�[�g�X�V
	m_pStateManager->Update(m_charaState);

	//�U��
	if (m_HP > 0)
	{
		Attack();
	}
	//�U���Ƃ̓����蔻��
	HitAttack();

	//�L�����N�^�[�̍U���}�l�[�W���[�̍X�V
	m_pCharacterAttackManager->Update();

	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̍X�V
	m_pHitEffectManager->Update(timer);

	//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̍X�V
	if (m_frontVector.x > 0)
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x - Character2Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character2Params::BOOST_EFFECT_POS_Y,
				0.0f), Character2Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x + Character2Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character2Params::BOOST_EFFECT_POS_Y,
				0.0f), Character2Params::BOOST_EFFECT_ANGLE_RIGHT);

	}

	//�o�b�t�@�ɍ��W���i�[
	m_posBuf = m_pos;

	//�ړ��ʂ����W�ɉ��Z
	m_pos += m_vel;

	//�̗͂̍X�V
	if (m_HP > m_HPBuffer)m_HP--;


	//Matrix rotZ = Matrix::CreateRotationZ(XMConvertToRadians(30));

	////m_world = rotX;
	//m_world = rotY *  m_world * rotX;
	//���W���s��ɕϊ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(m_pos.x,m_pos.y -1.0f,m_pos.z));
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(Character2Params::MODEL_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;

	//�V�[���h�̃T�C�Y�A���W�̍X�V
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(
		DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - Character2Params::SHIELD_POS_Y, m_pos.z));
	m_shieldWorld = Scale * pos;


	//���f���̍X�V
	//m_pModel->Update(m_world);

	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.7f, m_pos.z);
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);

	//�̂̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	{
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.9f, m_pos.z);
		m_bodyCollBox.size_h = Character2Params::BODY_COLL_SIZE_NORMAL;
	}
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);

	//���̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);
	}
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);

}

///////////////////////////
//�`��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{	
	//���f���̕`��
	//m_pModel->Render(view, proj);

	//�L�����N�^�[�̏�Ԃɉ����ĕ`�悷�郂�f����؂�ւ���
	//���Ⴊ��
	if (m_charaState == eCHARACTER_STATE::SQUAT
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false)
	{
		//m_pFbxModel_Squat->Draw(m_world,view,proj);
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::SQUAT));
	}
	//�W�����v
	else if (m_charaState == eCHARACTER_STATE::JUMP)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::JUMP));
	}
	//�K�[�h
	else if (m_charaState == eCHARACTER_STATE::GUARD)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::GUARD));
	}
	//�O�ړ�
	else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_FRONT));
	}
	//��ړ�
	else if (m_charaState == eCHARACTER_STATE::MOVE_BACK)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_BACK));
	}

	//�u�[�X�g�ړ�
	else if (m_charaState == eCHARACTER_STATE::BOOST_MOVE)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_BOOST));
		//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̕`��
		m_pBoostEffectManager->Render(view,proj);

	}
	//����
	else if (m_charaState == eCHARACTER_STATE::DAMAGE)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::DAMAGE));
	}

	//��j���[�g�����U��
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_WEAK_NEUTRAL));

	}
	//�㉺�U���̕`��
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_WEAK_BOTTOM));

	}
	//�㉡�U���̕`��
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_WEKA_SIDE));

	}
	//���j���[�g�����U��
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_MIDDLE_NEUTRAL));

	}
	//�����U��
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_MIDDLE_BOTTOM));

	}
	//�����U��
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_MIDDLE_SIDE));

	}

	//����
	else if (m_charaState == eCHARACTER_STATE::WIN)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::WIN_POSE));
	}
	//�s�k
	else if (m_charaState == eCHARACTER_STATE::LOSE)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::LOSE_POSE));

	}
	//�ҋ@
	else
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::WAIT));
	}
	
	if (m_playerID == ePLAYER_ID::PLAYER_1)
		m_pFbxModel->Draw(m_world, view, proj, DirectX::SimpleMath::Vector4(0.7f, 0.0f, 0.0f, 1.0f));
	else if (m_playerID == ePLAYER_ID::PLAYER_2)
		m_pFbxModel->Draw(m_world, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.7f, 1.0f));


	if (m_charaState == eCHARACTER_STATE::GUARD)
	{
		//�V�[���h�̕`��
		if (m_playerID == ePLAYER_ID::PLAYER_1)
			m_pShieldModel->Draw(m_shieldWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.3f));
		else if (m_playerID == ePLAYER_ID::PLAYER_2)
			m_pShieldModel->Draw(m_shieldWorld, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.3f));
	}
	//�����̔��̕`��
	//m_pLegBox->Draw(m_legBoxWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.4f));

	//�̂̔��̕`��
	//m_pBodyBox->Draw(m_bodyBoxWorld, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.4f));
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//m_pBodyBox->Draw(m_bodyBoxWorld, view, proj, m_bodyColor);
	}
	else
	{
		//m_pBodySquatBox->Draw(m_bodyBoxWorld, view, proj, m_bodyColor);
	}

	//���̔��̕`��
	//m_pHeadBox->Draw(m_headBoxWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.4f));

	//�㉡�U���̒e�̕`��
	if (m_pBulletGP != nullptr && m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)])
	{
		m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::Red);
	}
	//�L�����N�^�[�̍U���}�l�[�W���[�̕`��
	m_pCharacterAttackManager->Render(view,proj);

	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̕`��
	m_pHitEffectManager->SetRenderState(Character2Params::CAMERA_POS, view, proj);
	m_pHitEffectManager->Render();

}

///////////////////////////
//�I��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::Finalize()
{
	//���f���̉��
	//m_pModel->Lost();
	//m_pModel.reset();

	//���f���̊J��
	//delete m_pFbxModel;
	m_pFbxModel = nullptr;

	//delete m_pShieldModel;
	m_pShieldModel = nullptr;
	
	//�W�I���g���v���~�e�B�u�̉��
	m_pLegBox.reset();

	//delete m_pDeviceResources;
	m_pDeviceResources = nullptr;

	//�X�e�[�g�}�l�[�W���[�̏I������
	m_pStateManager->Finalize();
	m_pStateManager.reset();
	m_pStateManager = nullptr;

	//�L�����N�^�[�̍U���}�l�[�W���[�̏I������
	m_pCharacterAttackManager->Finalize();
	m_pCharacterAttackManager.reset();

	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̏I������
	m_pHitEffectManager->Finalize();
	m_pHitEffectManager.reset();
	m_pHitEffectManager = nullptr;

	//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̏I������
	m_pBoostEffectManager->Finalize();
	m_pBoostEffectManager.reset();
	m_pBoostEffectManager = nullptr;


	m_pEnemy = nullptr;

}

///////////////////////////
//���Ƃ̓����蔻��
//����:���̓����蔻��\����
//�߂�l:�Ȃ�
//////////////////////////
void Character2::HitFloor(const Collision::BoxCollision& floorColl)
{
	if (Collision::CheckHitBox2Box(m_legCollBox, floorColl))
	{
		m_vel.y = 0.0f;
		//���n�t���O�𗧂Ă�
		m_landingFlag = true;
		//m_pos.y += 0.0001f;
		m_jumpFlag = false;
		//�W�����v��ԂȂ�҂���Ԃɂ���
		if (m_charaState == eCHARACTER_STATE::JUMP)
		{
			m_charaState = eCHARACTER_STATE::WAIT;
		}
	}
	else
	{
		m_landingFlag = false;
		//m_jumpFlag = true;
	}

}


///////////////////////////
//�G�̑̂Ƃ̓����蔻��
//����:�G�̑̂̓����蔻��
//�߂�l:�Ȃ�
//////////////////////////
void Character2::HitEnemyBody(const Collision::BoxCollision & enemyBodyColl, const Collision::BoxCollision& enemyHeadColl)
{
	//if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyBodyColl))
	//{
	//	m_isHitEnemyBody = true;
	//}
	//else m_isHitEnemyBody = false;

	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyBodyColl))
	{
		if (m_landingFlag == true)
		{
			//�o�b�t�@�Ɋi�[�������W�ɖ߂�
			m_pos = m_posBuf;
		}
		else if (m_jumpFlag == true)
		{
			m_pos.x = m_posBuf.x;
		}

	}
	//�̂Ɠ��̓����蔻��(�ォ��������)
	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyHeadColl))
	{
		//m_landingFlag = true;
		m_charaState = eCHARACTER_STATE::WAIT;
		m_vel.y = 0.0f;

		if (m_bodyCollBox.pos.x < enemyHeadColl.pos.x)
		{
			m_pos.x -= 0.15f;
		}
		else if (m_bodyCollBox.pos.x > enemyHeadColl.pos.x)
		{
			m_pos.x += 0.15f;

		}
		if (enemyHeadColl.pos.x > 2.0f || m_bodyCollBox.pos.x > 2.0f)
		{
			m_pos.x -= 0.25f;
		}
		else if (enemyHeadColl.pos.x < -2.0f || m_bodyCollBox.pos.x < -2.0f)
		{
			m_pos.x += 0.25f;
		}
	}

}

///////////////////////////
//�U���Ƃ̓����蔻��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::HitAttack()
{
	//�̂̐F�̐ݒ�
	m_bodyColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.4f);
	//�L�[�̏�Ԏ擾
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	for (int i = 0; i < AttackManager::ATTACK_NUM; i++)
	{
		//�g�p����Ă��Ȃ���΃X���[
		if (m_pAttackManager->GetAttackStruct(i)->isUsed == false)continue;
		//�U���̃v���C���[ID��������ID�ƈ�v���Ă���΃X���[
		if (m_pAttackManager->GetAttackStruct(i)->playerID == m_playerID)continue;

		if (Collision::CheckHitBox2Box(m_bodyCollBox, m_pAttackManager->GetAttackStruct(i)->attackColl) && m_pAttackManager->GetAttackStruct(i)->isHit == false)
		{
			//����͂Ȃ��œ������������
			if (m_charaState != eCHARACTER_STATE::GUARD)
			{
				//�̗͂����炷
				//m_HP -= m_pAttackManager->GetAttackStruct(i)->power;
				m_HPBuffer = m_HP - m_pAttackManager->GetAttackStruct(i)->power;

				//�����Ԃɐ؂�ւ���
				m_charaState = eCHARACTER_STATE::DAMAGE;
				//�q�b�g�G�t�F�N�g�̍Đ�
				m_pHitEffectManager->Play(20.0f,
					DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.5f, m_pos.z));

				////�m�b�N�o�b�N
				//if (m_frontVector.x > 0 && m_pos.x > -MOVE_LIMIT_X)
				//{
				//	m_pos.x -= 0.1f;
				//}
				//else if (m_frontVector.x < 0 && m_pos.x < MOVE_LIMIT_X)
				//{
				//	m_pos.x += 0.1f;
				//}

				//�q�b�g���̍Đ�
				static int id;
				id = ADX2::GetInstance().Play(CRI_CUESHEET_0_DAMAGE);

				//m_pAttackManager->GetAttackStruct(i)->isUsed = false;
				//���������t���O�𗧂Ă�
				m_pAttackManager->GetAttackStruct(i)->isHit = true;

				//�U���͂�5���傫����΃q�b�g�X�g�b�v
				if (m_pAttackManager->GetAttackStruct(i)->power > 5)
				{
					Sleep(100);
				}

			}
			
			//�K�[�h���ɍU��������������_���[�W�ʂ����炷
			else if (m_charaState == eCHARACTER_STATE::GUARD)
			{
				//�̗͂����炷
				//m_HP -= m_pAttackManager->GetAttackStruct(i)->power / m_pAttackManager->GetAttackStruct(i)->power;
				m_HPBuffer = m_HP - (m_pAttackManager->GetAttackStruct(i)->power / m_pAttackManager->GetAttackStruct(i)->power);
				//�K�[�h��Ԃɐ؂�ւ���
				//m_charaState = eCHARACTER_STATE::GUARD;
				//���������t���O�𗧂Ă�
				m_pAttackManager->GetAttackStruct(i)->isHit = true;
				//�q�b�g�G�t�F�N�g�̍Đ�
				m_pHitEffectManager->Play(20.0f,
					DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.5f, m_pos.z));



			}

		}
	}

}

///////////////////////////
//����
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::Ready(DX::StepTimer const& timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);
	//�X�e�[�g�̊Ǘ�
	if (m_HP > 0)StateManager();

	//�d�͗���
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;

	}
	else if (m_landingFlag == true)m_vel.y = 0.0f;

	//�ړ�����
	if (m_pos.x > -MOVE_LIMIT_X || m_pos.x < MOVE_LIMIT_X)m_vel.x = 0.0f;
	if (m_pos.x < -MOVE_LIMIT_X) m_pos.x = -MOVE_LIMIT_X;
	if (m_pos.x > MOVE_LIMIT_X) m_pos.x = MOVE_LIMIT_X;

	//m_pStateManager->Update(m_charaState);

	//�v���C���[��Y���̊p�x
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = Character2Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = -180.0f + Character2Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = -1.0f;

	}
	//��]�s����쐬
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));

	//�L�����̃X�e�[�g
	//if (m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	m_pStateManager->Update(m_charaState);

	//�o�b�t�@�ɍ��W���i�[
	m_posBuf = m_pos;
	//�ړ��ʂ����W�ɉ��Z
	m_pos += m_vel;


	//���W���s��ɕϊ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z));
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(Character2Params::MODEL_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;

	//�V�[���h�̃T�C�Y�A���W�̍X�V
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(
		DirectX::SimpleMath::Vector3(m_pos.x,m_pos.y - Character2Params::SHIELD_POS_Y, m_pos.z));
	m_shieldWorld = Scale * pos;


	//���f���̍X�V
	//m_pModel->Update(m_world);

	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.7f, m_pos.z);
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);

	//�̂̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_bodyCollBox.size_h = Character2Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.9f, m_pos.z);
	}
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);

	//���̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);
	}
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);

	//�U���}�l�[�W���[�̍X�V
	m_pCharacterAttackManager->Update();
	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̍X�V
	m_pHitEffectManager->Update(timer);

	//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̍X�V
	if (m_frontVector.x > 0)
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x - Character2Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character2Params::BOOST_EFFECT_POS_Y,
				0.0f), Character2Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x + Character2Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character2Params::BOOST_EFFECT_POS_Y,
				0.0f), Character2Params::BOOST_EFFECT_ANGLE_RIGHT);

	}
}

///////////////////////////
//�s�k
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::Lose(DX::StepTimer const & timer)
{
	m_vel.x = 0.0f;
	m_charaState = eCHARACTER_STATE::LOSE;
	//�d�͗���
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;

	}
	else if (m_landingFlag == true)m_vel.y = 0.0f;
	//�v���C���[��Y���̊p�x
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�����Ă��������ς���
		m_angleY = Character2Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�����Ă��������ς���
		m_angleY = -Character2Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = -1.0f;

	}
	//��]�s����쐬
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));


	//�o�b�t�@�ɍ��W���i�[
	m_posBuf = m_pos;
	//�ړ��ʂ����W�ɉ��Z
	m_pos += m_vel;


	//���W���s��ɕϊ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z));
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(Character2Params::MODEL_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;

	//�V�[���h�̃T�C�Y�A���W�̍X�V
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(
		DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - Character2Params::SHIELD_POS_Y, m_pos.z));
	m_shieldWorld = Scale * pos;


	//���f���̍X�V
	//m_pModel->Update(m_world);

	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.7f, m_pos.z);
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);

	//�̂̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_bodyCollBox.size_h = Character2Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.9f, m_pos.z);
	}
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);

	//���̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);
	}
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);

	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̍X�V
	m_pHitEffectManager->Update(timer);

}

///////////////////////////
//���Z�b�g
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::Reset()
{	
	//�������W�̐ݒ�
	m_pos = m_startPos;
	m_posBuf = m_pos;
	//�̗͂̐ݒ�
	m_HP = Character2Params::GetInstance()->MAX_HP;
	m_HPBuffer = m_HP;
	//�X�e�[�g�̐ݒ�
	m_charaState = eCHARACTER_STATE::WAIT;

	m_pAttack = nullptr;
	//�U���̓����蔻��̏�����
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_isAttackInput[i] = false;
		m_isAttackUse[i] = false;
	}
	//�G�l���M�[�ʂ̏�����
	m_boostCap = Character2Params::GetInstance()->BOOST_CAP_MAX;

	//�e�̃W�I���g���v���~�e�B�u�̃��Z�b�g
	if (m_pBulletGP != nullptr)
	{
		m_pBulletGP.reset();
	}
	//�U���}�l�[�W���[�̃��Z�b�g
	m_pCharacterAttackManager->Reset();

	//�q�b�g�G�t�F�N�g�̃��Z�b�g
	m_pHitEffectManager->Reset();

}

///////////////////////////
//�A�j���[�V�����̐؂�ւ�
//����:�؂�ւ���A�j���[�V�����̔ԍ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::ChangeAnimation(const int & animationStack)
{
	m_pFbxModel->ChangeAnimation(animationStack);
}


///////////////////////////
//�U��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::Attack()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();


	if (m_playerID == ePLAYER_ID::PLAYER_1)
	{
		//��j���[�g�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::WAIT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;
		}

		//�㉺�U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::SQUAT &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = true;
		}


		//�㉡�U��
		if(m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = true;
		}

		//���j���[�g�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::WAIT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = true;
		}

		//�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false &&
			m_isAttacking == false) 
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = true;
		}

		//�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::SQUAT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = true;
		}

	}

	//CPU
	else if (m_playerID == ePLAYER_ID::PLAYER_2)
	{
		m_aiStateTiemer--;
		if (m_aiStateTiemer <= 0.0f)
		{
			int random = rand() % 2;
			//��U��
			if (random == 0)
			{
				//�j���[�g����
				if (m_charaState == eCHARACTER_STATE::WAIT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_charaState != eCHARACTER_STATE::DAMAGE &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;

				}
				//��
				else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_charaState != eCHARACTER_STATE::DAMAGE &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = true;

				}
				//��
				else if (m_charaState == eCHARACTER_STATE::SQUAT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_charaState != eCHARACTER_STATE::DAMAGE &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = true;
				}
			}
			//���U��
			else if (random == 1)
			{
				//�j���[�g����
				if (m_charaState == eCHARACTER_STATE::WAIT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_charaState != eCHARACTER_STATE::DAMAGE &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = true;

				}
				//��
				else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_charaState != eCHARACTER_STATE::DAMAGE &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = true;
				}

			}
			m_aiStateTiemer = (rand() % 30) + 30.0f;
		}

	}
}

///////////////////////////
//�X�e�[�g�̊Ǘ�
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::StateManager()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//�u�[�X�g�ړ��łȂ���΃u�[�X�g�e�ʂ𑝂₷
	if (m_charaState != eCHARACTER_STATE::BOOST_MOVE && keyState.IsKeyUp(DirectX::Keyboard::Keys::Space) && m_charaState != eCHARACTER_STATE::DAMAGE)
	{
		m_boostCap++;
		//�ő�l�ȏ�ɂȂ�����ő�l����
		if (m_boostCap >= Character2Params::GetInstance()->BOOST_CAP_MAX)
		{
			m_boostCap = Character2Params::GetInstance()->BOOST_CAP_MAX;
		}
	}

	//�v���C���[�P�̏���
	if (m_playerID == ePLAYER_ID::PLAYER_1)
	{
		if (m_charaState != eCHARACTER_STATE::DAMAGE)
		{
			if (m_landingFlag == true)m_charaState = eCHARACTER_STATE::WAIT;


			//��Shift�ŃK�[�h
			if ((m_charaState == eCHARACTER_STATE::WAIT ||
				m_charaState == eCHARACTER_STATE::SQUAT ||
				m_charaState == eCHARACTER_STATE::MOVE_BACK ||
				m_charaState == eCHARACTER_STATE::MOVE_FRONT) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::LeftShift))
			{
				m_charaState = eCHARACTER_STATE::GUARD;
			}

			//�҂���Ԃɉ����͂ł��Ⴊ��
			if (m_charaState == eCHARACTER_STATE::WAIT && keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) && m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::SQUAT;
			}
			//�X�y�[�X�L�[���͂Ńu�[�X�g�ړ�
			if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Space) && m_boostCap > 0)
			{
				m_charaState = eCHARACTER_STATE::BOOST_MOVE;
			}


			//�҂���Ԃō��E���͂ňړ�
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD &&
				m_isAttacking == false) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) && m_frontVector.x < 0 &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false &&
					m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttacking == false))
			{
				//�O�ړ�
				m_charaState = eCHARACTER_STATE::MOVE_FRONT;
			}
			//else m_charaState = eCHARACTER_STATE::WAIT;
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT&& m_charaState != eCHARACTER_STATE::GUARD) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD))
			{
				//��ړ�
				m_charaState = eCHARACTER_STATE::MOVE_BACK;

				//if (keyState.IsKeyUp(DirectX::Keyboard::Keys::Left) || keyState.IsKeyUp(DirectX::Keyboard::Keys::Right))
				//{
				//	m_charaState = eCHARACTER_STATE::WAIT;
				//}

			}
			//����͂ŃW�����v
			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up) && m_charaState != eCHARACTER_STATE::SQUAT)
			{
				m_charaState = eCHARACTER_STATE::JUMP;
			}
		}

		//if ((m_charaState == eCHARACTER_STATE::MOVE_FRONT && keyState.IsKeyUp(DirectX::Keyboard::Keys::Left))||
		//	(m_charaState == eCHARACTER_STATE::MOVE_FRONT && keyState.IsKeyUp(DirectX::Keyboard::Keys::Right)))
		//{
		//	m_charaState = eCHARACTER_STATE::WAIT;
		//}

		//else m_charaState = eCHARACTER_STATE::WAIT;

		//if (m_charaState == eCHARACTER_STATE::SQUAT && keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) == false && m_landingFlag == false)
		//{
		//	m_charaState = eCHARACTER_STATE::WAIT;
		//}


	}

}

///////////////////////////
//AI�̏���
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character2::AI()
{
	//�u�[�X�g�ړ��łȂ���΃u�[�X�g�e�ʂ𑝂₷
	if (m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	{
		m_boostCap++;
		//�ő�l�ȏ�ɂȂ�����ő�l����
		if (m_boostCap >= Character2Params::GetInstance()->BOOST_CAP_MAX)
		{
			m_boostCap = Character2Params::GetInstance()->BOOST_CAP_MAX;
		}
	}

	//�^�C�}�[�̒l�����炷
	m_aiStateTiemer--;
	//�^�C�}�[�̒l��0�ɂȂ����烉���_���ɏ�Ԃ�؂�ւ���
	if (m_aiStateTiemer <= 0.0f)
	{
		m_aiState = rand() % 7;
		m_aiStateTiemer = (rand() % 30) + 30.0f;
	}

	if (m_charaState != eCHARACTER_STATE::DAMAGE)
	{
		switch (m_aiState)
		{
			if (m_charaState != eCHARACTER_STATE::DAMAGE)
			{
				//�ҋ@
				case 0:
				{
					m_charaState = eCHARACTER_STATE::WAIT;
					break;
				}
				//�O�i
				case 1:
				{
					m_charaState = eCHARACTER_STATE::MOVE_FRONT;
					break;
				}
				//���
				case 2:
				{
					m_charaState = eCHARACTER_STATE::MOVE_BACK;
					break;
				}
				//�W�����v
				case 3:
				{
					m_charaState = eCHARACTER_STATE::JUMP;
					break;
				}
				//���Ⴊ��
				case 4:
				{
					m_charaState = eCHARACTER_STATE::SQUAT;
					break;
				}
				//�K�[�h
				case 5:
				{
					m_charaState = eCHARACTER_STATE::GUARD;
					break;
				}
				//�u�[�X�g�ړ�
				case 6:
				{
					m_charaState = eCHARACTER_STATE::BOOST_MOVE;
					break;
				}

				default:
					break;
			}
		}
	}

}
