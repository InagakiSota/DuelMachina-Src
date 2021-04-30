///////////////////////////////////
//�L�����N�^�[�P�̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N7��13��
///////////////////////////////////
#include "pch.h"

#include "Character1.h"
#include "GamePad.h"
#include "../../../PlayerID.h"
#include "../../AttackManager/AttackManager.h"
#include "Src\FrameWork\FbxModel\FbxModel.h"
#include "../CharacterState.h"

#include "State\Character1StateManager.h"
#include "Attack\Character1AttackManager.h"
#include "../ChracterAnimationNumber.h"

#include "Adx2.h"

//#include "Adx2.h"
//#include "Test_acf.h"
#include "CueSheet_0.h"
#include "../../../../FrameWork/FbxResourceManager/FbxResourceManager.h"
#include "../CharacterID.h"
#include "Src/Cgdi.h"


//�R���X�g���N�^
Character1::Character1(ePLAYER_ID playerID)
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

	m_pHitEffectManager = nullptr;

	m_pEnemy = nullptr;

	thinkTimer = 0;
	toEnemyRange = 0.0f;
}

//�f�X�g���N�^
Character1::~Character1()
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

	if (m_pHeadBox != nullptr)
	{
		m_pHeadBox.reset();
	}

	if (m_pDeviceResources != nullptr)
	{
		m_pDeviceResources = nullptr;
	}

	if (m_pKeyTracker != nullptr)
	{
		m_pKeyTracker.reset();
	}

	if (m_pFbxModel != nullptr)
	{
		delete m_pFbxModel;
		m_pFbxModel = nullptr;
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
void Character1::Initialize()
{
	m_pDeviceResources = gdi->GetDeviceResources();

	//�ҋ@��Ԃ̃��f���̓ǂݍ���
	//m_pFbxModel = new FbxModel();
	//m_pFbxModel->Load(
	//	window,
	//	pDeviceResources->GetD3DDevice(),
	//	pDeviceResources->GetD3DDeviceContext(),
	//	pDeviceResources->GetRenderTargetView(),
	//	"Resources/Models/robot1/robot1animation3.fbx",	
	//	true,
	//	0
	//);

	m_pFbxModel = FbxResourceManager::GetInstance()->GetModel(eCHARACTER_ID::CHARACTER_1);

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

	//�L�����N�^�[�̃X�e�[�^�X�f�[�^�̓ǂݍ���
	Character1Params::GetInstance()->LoadStatusData();

	//�������W�̐ݒ�	
	m_pos = m_startPos;

	//�����̓����蔻��̔��̓ǂݍ���
	m_legCollBox.size_h = Character1Params::LEG_COLL_SIZE;
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z);
	m_pLegBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_legCollBox.size_h * 2);
	m_legBoxWorld.Identity;

	//�̂̓����蔻��̔��̓ǂݍ���
	m_bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
	m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.2f, m_pos.z);
	m_pBodyBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_bodyCollBox.size_h * 2);
	m_bodyBoxWorld.Identity;

	//���̓����蔻��̔��̓ǂݍ���
	m_headCollBox.size_h = Character1Params::HEAD_COLL_SIZE;
	m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.7f, m_pos.z);
	m_pHeadBox = DirectX::GeometricPrimitive::CreateBox(m_pDeviceResources->GetD3DDeviceContext(), m_headCollBox.size_h * 2);
	m_headBoxWorld.Identity;

	//�L�[�g���b�J�[�̓ǂݍ���
	m_pKeyTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//�O�����̐ݒ�
	m_frontVector = Character1Params::FRONT_VECTOR;

	//�̗͂̐ݒ�
	m_HP = Character1Params::GetInstance()->MAX_HP;
	m_HPBuffer = m_HP;

	//�̂̐F�̐ݒ�
	m_bodyColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.4f);

	//�L�����̃X�e�[�g������
	m_charaState = eCHARACTER_STATE::WAIT;

	if (m_playerID == ePLAYER_ID::PLAYER_1)m_angleY = Character1Params::ANGLE_Y;
	if (m_playerID == ePLAYER_ID::PLAYER_2)m_angleY = -Character1Params::ANGLE_Y;

	//���W���s��ɕϊ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//Y���̊p�x���s��ɕϊ�
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_angleY);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;

	//���f���̍X�V
	//m_pModel->Update(m_world);
	 
	//�U���t���O�̏�����
	for (int i = 0; i < static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM); i++)
	{
		m_isAttackUse[i] = false;
		m_isAttackInput[i] = false;
	}

	//�G�l���M�[�ʂ̏�����
	m_boostCap = Character1Params::GetInstance()->BOOST_CAP_MAX;

	// �T�E���h�̏�����
	//ADX2::GetInstance().Initialize("Test.acf", "CueSheet_0.acb");

	//�U���̓����蔻��̏�����
	for (int i = 0; i < CharacterBase::ATTACK_NUM; i++)
	{
		m_pAttack = nullptr;
	}

	//�X�e�[�g�}�l�[�W���[�̓ǂݍ��݁A������
	m_pStateManager = std::make_unique<Character1StateManager>();
	m_pStateManager->Initialize(this);

	//�U���}�l�[�W���[�̓ǂݍ��݁A������
	m_pCharacterAttackManager = std::make_unique<Character1AttackManager>();
	m_pCharacterAttackManager->Initialize(this,m_pDeviceResources);

	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̓ǂݍ��݁A�쐬
	m_pHitEffectManager = std::make_unique<HitEffectManager>();
	m_pHitEffectManager->Create(m_pDeviceResources, 50);
	
	//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̓ǂݍ��݁A�쐬
	m_pBoostEffectManager = std::make_unique<BoostEffectManager>();
	m_pBoostEffectManager->Initialize(m_pDeviceResources, 1,1,DirectX::SimpleMath::Vector3::Zero);

}

///////////////////////////
//�X�V
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::Update(DX::StepTimer const& timer)
{
	m_stepTimer = timer;
	//�L�[�{�[�h�̏�Ԃ̎擾
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//// �T�E���h�̍X�V
	//ADX2::GetInstance().Update();
	//// ���̍Đ�
	//static int id = ADX2::GetInstance().Play(CRI_CUESHEET_0_ROBOT_FOOTSTEP2__ONLINE_AUDIO_CONVERTER_COM_);
	//ADX2::GetInstance().Stop(id);

	//�X�e�[�g�̊Ǘ�
	if (m_playerID == ePLAYER_ID::PLAYER_1 && m_HP > 0)
	{
		StateManager();
	}

	//�v���C���[�Q�Ȃ�AI����
	if (m_playerID == ePLAYER_ID::PLAYER_2 && m_HP > 0)
	{
		AI();
		//StateManager();
	}

	//�d�͗���
	if (m_landingFlag == false)
	{
		m_vel.y += GRAVITY;

	}
	else if (m_landingFlag == true && m_charaState == eCHARACTER_STATE::BOOST_MOVE)m_vel.y = 0.0f;

	//�ړ�����
	if (m_pos.x > -MOVE_LIMIT_X || m_pos.x < MOVE_LIMIT_X)m_vel.x = 0.0f;
	if (m_pos.x < -MOVE_LIMIT_X) m_pos.x = -MOVE_LIMIT_X;
	if (m_pos.x > MOVE_LIMIT_X) m_pos.x = MOVE_LIMIT_X;

	//�L�����̃X�e�[�g�X�V
	m_pStateManager->Update(m_charaState);

	//�u�[�X�g�ړ��łȂ���΃u�[�X�g�e�ʂ𑝂₷
	if (m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	{
		m_boostCap++;
		//�ő�l�ȏ�ɂȂ�����ő�l����
		if (m_boostCap >= Character1Params::GetInstance()->BOOST_CAP_MAX)
		{
			m_boostCap = Character1Params::GetInstance()->BOOST_CAP_MAX;
		}
	}

	//�v���C���[��Y���̊p�x
	if (m_pos.x < m_pEnemy->GetPos().x && (m_landingFlag == true || m_charaState == eCHARACTER_STATE::BOOST_MOVE)&& m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�����Ă��������ς���
		m_angleY = Character1Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if(m_pos.x > m_enemyPos.x && (m_landingFlag == true || m_charaState == eCHARACTER_STATE::BOOST_MOVE) && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�����Ă��������ς���
		m_angleY = -90.0f;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = -1.0f;

	}
	//��]�s����쐬
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));

	//�U��
	if (m_HP > 0)
	{
		Attack();
	}
	//�U���Ƃ̓����蔻��
	HitAttack();
	//�U���}�l�[�W���[�̍X�V
	m_pCharacterAttackManager->Update();
	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̍X�V
	m_pHitEffectManager->Update(timer);

	//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̍X�V
	if (m_frontVector.x > 0)
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x - Character1Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x + Character1Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_RIGHT);

	}

	//�o�b�t�@�ɍ��W���i�[
	m_posBuf = m_pos;
	//�ړ��ʂ����W�ɉ��Z
	m_pos += m_vel;

	//�̗͂̍X�V
	if (m_HP > m_HPBuffer)m_HP--; 

	//���W���s��ɕϊ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY * trans;

	//�V�[���h�̃T�C�Y�A���W�̍X�V
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	m_shieldWorld = Scale * pos;


	//���f���̍X�V
	//m_pModel->Update(m_world);


	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.85f, m_pos.z);

	if (m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	{
		//�̂̔��̍��W�̍X�V
		m_bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);
		//���̔��̍��W�̍X�V
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.9f, m_pos.z);
	}

	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);


}

///////////////////////////
//�`��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{	
	//���f���̕`��
	//m_pModel->Render(view, proj);

	//m_pFbxModel->Draw(
	//	m_world,
	//	view,
	//	proj
	//);


	//�K�[�h��Ԃ̃��f���̕`��
	if (m_charaState == eCHARACTER_STATE::GUARD)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::GUARD));

	}
	//���Ⴊ�ݏ�Ԃ̃��f���̕`��
	else if (m_charaState == eCHARACTER_STATE::SQUAT 
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::SQUAT));
	}
	//�u�[�X�g�ړ���Ԃ̃��f���̕`��
	else if (m_charaState == eCHARACTER_STATE::BOOST_MOVE)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_BOOST));
		//m_pBoostEffectManager->SetRenderState(view, proj);
		m_pBoostEffectManager->Render(view, proj);
	}

	//�O�ړ���Ԃ̃��f���̕`��
	else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT 
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_FRONT));

	}
	
	//��ړ���Ԃ̃��f���̕`��
	else if (m_charaState == eCHARACTER_STATE::MOVE_BACK)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::MOVE_BACK));
	}

	//�W�����v��Ԃ̃��f���̕`��
	else if (m_charaState == eCHARACTER_STATE::JUMP)
	{
		//m_pFbxModel_Jump->Draw(m_world,view,proj);
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::JUMP));
	}

	//����
	else if (m_charaState == eCHARACTER_STATE::DAMAGE)
	{
		//m_pFbxModel_Damage->Draw(m_world, view, proj);
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::DAMAGE));
	}

	//��j���[�g�����U���̕`��
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

	//���j���[�g�����U���̕`��
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_MIDDLE_NEUTRAL));

	}

	//�����U���̕`��
	else if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)])
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::ATTACK_MIDDLE_BOTTOM));

	}

	//�����U���̕`��
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

	//�ҋ@��Ԃ̃��f���̕`��
	else
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::WAIT));
	}


	if(m_playerID == ePLAYER_ID::PLAYER_1)
		m_pFbxModel->Draw(m_world, view, proj,DirectX::SimpleMath::Vector4(0.7f,0.0f,0.0f,1.0f));
	else if (m_playerID == ePLAYER_ID::PLAYER_2)
		m_pFbxModel->Draw(m_world, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.7f, 1.0f));


	if (m_charaState == eCHARACTER_STATE::GUARD)
	{
		//�V�[���h�̕`��
		if (m_playerID == ePLAYER_ID::PLAYER_1)
			m_pShieldModel->Draw(m_shieldWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f,0.0f,0.0f,0.3f));
		else if (m_playerID == ePLAYER_ID::PLAYER_2)
			m_pShieldModel->Draw(m_shieldWorld, view, proj, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 1.0f, 0.3f));
	}


	//�����̔��̕`��
	//m_pLegBox->Draw(m_legBoxWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 0.4f));

	//�̂̔��̕`��
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//m_pBodyBox->Draw(m_bodyBoxWorld, view, proj, m_bodyColor);
	}
	//���Ⴊ�ݒ��̑̂̔��̕`��
	else
	{
		//m_pBodySquatBox->Draw(m_bodyBoxWorld, view, proj, m_bodyColor);
	}
	//���̔��̕`��
	//m_pHeadBox->Draw(m_headBoxWorld, view, proj, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 0.4f));

	////����U���̒e�̕`��
	//if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] && m_pBulletGP != nullptr)
	//{
	//	m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::Red);
	//}
	////�����U���̒e�̕`��
	//if (m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] && m_pBulletGP != nullptr)
	//{
	//	m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::Black);
	//}
	//�U���}�l�[�W���[�̕`��
	m_pCharacterAttackManager->Render(view, proj);
	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̕`��
	m_pHitEffectManager->SetRenderState(Character1Params::CAMERA_POS, view, proj);
	m_pHitEffectManager->Render();


}

///////////////////////////
//�I��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::Finalize()
{
	//���f���̉��
	//m_pModel->Lost();
	//m_pModel.reset();
	// �T�E���h�̏I������
	//ADX2::GetInstance().Finalize();

	//delete m_pFbxModel;
	//m_pFbxModel = nullptr;

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

	//�U���}�l�[�W���̏I������
	m_pCharacterAttackManager->Finalize();
	m_pCharacterAttackManager.reset();
	m_pCharacterAttackManager = nullptr;

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
void Character1::HitFloor(const Collision::BoxCollision& floorColl)
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
	}

}


///////////////////////////
//�G�̑̂Ƃ̓����蔻��
//����:�G�̑̂̓����蔻��
//�߂�l:�Ȃ�
//////////////////////////
void Character1::HitEnemyBody(const Collision::BoxCollision & enemyBodyColl, const Collision::BoxCollision& enemyHeadColl)
{
	//�̂Ƒ̂̓����蔻��
	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyBodyColl))
	{
		//�O�t���[���̍��W�ɖ߂�
		if (m_landingFlag == true)
		{
			m_pos = m_posBuf;
		}
		else if(m_jumpFlag == true)
		{
			m_pos.x = m_posBuf.x;
		}
	}

	//�̂Ɠ��̓����蔻��(�ォ��������)
	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyHeadColl))
	{
		m_landingFlag = true;
		m_charaState = eCHARACTER_STATE::WAIT;
		m_vel.y = 0.0f;
		if (enemyHeadColl.pos.x > 2.0f || m_bodyCollBox.pos.x > 2.0f)
		{
			m_pos.x -= 0.25f;
		}
		else if (enemyHeadColl.pos.x < -2.0f || m_bodyCollBox.pos.x < -2.0f)
		{
			m_pos.x += 0.25f;
		}

		if (m_bodyCollBox.pos.x < enemyHeadColl.pos.x)
		{
			m_pos.x -= 0.15f;
		}
		else if (m_bodyCollBox.pos.x > enemyHeadColl.pos.x)
		{
			m_pos.x += 0.15f;

		}

	}
	
}

///////////////////////////
//�U���Ƃ̓����蔻��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::HitAttack()
{
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
			//�K�[�h�Ȃ��œ������������
			if (m_charaState != eCHARACTER_STATE::GUARD)
			{
				m_bodyColor = DirectX::SimpleMath::Vector4(0.0f, 1.0f, 1.0f, 0.4f);
				//�̗͂����炷
				//m_HP -= m_pAttackManager->GetAttackStruct(i)->power;
				m_HPBuffer = m_HP - m_pAttackManager->GetAttackStruct(i)->power;

				//�����Ԃɐ؂�ւ���
				m_charaState = eCHARACTER_STATE::DAMAGE;

				//���������t���O�𗧂Ă�
				m_pAttackManager->GetAttackStruct(i)->isHit = true;

				//�q�b�g�G�t�F�N�g�̍Đ�
				m_pHitEffectManager->Play(20.0f, m_pos);

				//�q�b�g���̍Đ�
				static int id;
				id = ADX2::GetInstance().Play(CRI_CUESHEET_0_DAMAGE);

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

				//���������t���O�𗧂Ă�
				m_pAttackManager->GetAttackStruct(i)->isHit = true;
				//�q�b�g�G�t�F�N�g�̍Đ�
				m_pHitEffectManager->Play(10.0f, m_pos);


			}
		}
	}
}


///////////////////////////
//����
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::Ready(DX::StepTimer const& timer)
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

	//�L�����̃X�e�[�g�X�V
	//if(m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	m_pStateManager->Update(m_charaState);
	//else m_pStateManager->Update(eCHARACTER_STATE)



	//�v���C���[��Y���̊p�x
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�����Ă��������ς���
		m_angleY = Character1Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�����Ă��������ς���
		m_angleY = -Character1Params::ANGLE_Y;
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
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;

	//�V�[���h�̃T�C�Y�A���W�̍X�V
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	m_shieldWorld = Scale * pos;



	//���f���̍X�V
	//m_pModel->Update(m_world);

	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.85f, m_pos.z);
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);

	//�̂̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y , m_pos.z);
	}
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);

	//���̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.9f, m_pos.z);
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
				m_pos.x - Character1Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f),Character1Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x + Character1Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character1Params::BOOST_EFFECT_POS_Y,
				0.0f), Character1Params::BOOST_EFFECT_ANGLE_RIGHT);

	}


}

///////////////////////////
//�s�k
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::Lose(DX::StepTimer const & timer)
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
		m_angleY = Character1Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true && m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�����Ă��������ς���
		m_angleY = -Character1Params::ANGLE_Y;
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
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character1Params::BODY_SIZE, Character1Params::BODY_SIZE, Character1Params::BODY_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;
	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.85f, m_pos.z);
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);

	//�̂̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_bodyCollBox.size_h = Character1Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);
	}
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);

	//���̔��̍��W�̍X�V
	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.9f, m_pos.z);
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
void Character1::Reset()
{
	//�������W�̐ݒ�
	m_pos = m_startPos;
	m_posBuf = m_pos;
	//�̗͂̐ݒ�
	m_HP = Character1Params::GetInstance()->MAX_HP;
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
	m_boostCap = Character1Params::GetInstance()->BOOST_CAP_MAX;

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
void Character1::ChangeAnimation(const int & animationStack)
{
	m_pFbxModel->ChangeAnimation(animationStack);
}

///////////////////////////
//AI
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::AI()
{	
	//�G�Ƃ̋���
	float distance = m_pos.x - m_enemyPos.x;
	//�^�C�}�[�̒l�����炷
	m_aiStateTiemer--;
	//�^�C�}�[�̒l��0�ɂȂ����烉���_���ɏ�Ԃ�؂�ւ���
	if (m_aiStateTiemer <= 0.0f)
	{
		m_aiState = rand() % 7;
		m_aiStateTiemer = (rand() % 30) + 30.0f;
	}

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
			case 7:
			{
				m_charaState = eCHARACTER_STATE::BOOST_MOVE;
				break;

			}
			default:
				break;
		}
	}

	//toEnemyRange = fabs(m_pos.x - m_pEnemy->GetPos().x);


	//if (m_pEnemy != nullptr)
	//{
	//	if (thinkTimer <= 0)
	//	{

	//		if (toEnemyRange <= 2.0f && m_isAttacking == false)
	//		{
	//			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;
	//		}
	//		//else if(toEnemyRange > 2.0f && m_isAttacking == false)
	//		//{
	//		//	m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = true;
	//		//}

	//		if (m_pEnemy->GetIsAttacking() == true)
	//		{
	//			m_charaState = eCHARACTER_STATE::GUARD;
	//		}
	//		else
	//		{
	//			m_charaState = eCHARACTER_STATE::WAIT;
	//		}

	//		thinkTimer = 60;
	//	}
	//	
	//}

	//thinkTimer--;
}

///////////////////////////
//�U��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::Attack()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//�v���C���[�P�̏���
	if (m_playerID == ePLAYER_ID::PLAYER_1)
	{
		//��j���[�g�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::WAIT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;
		}
			
		//�㉺�U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::SQUAT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = true;
		}

		//�㉡�U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
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
			m_charaState == eCHARACTER_STATE::SQUAT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = true;
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
	}

	//�v���C���[�Q�̏���
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
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;

				}
				//��
				else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = true;

				}
				//��
				else if (m_charaState == eCHARACTER_STATE::SQUAT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
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
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = true;

				}
				//��
				else if (m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = true;

				}
				//��
				else if (m_charaState == eCHARACTER_STATE::SQUAT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false &&
					m_isAttacking == false)
				{
					m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = true;
				}

			}
			m_aiStateTiemer = (rand() % 30) + 30.0f;
		}


	//��j���[�g�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::O) &&
			keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) == false &&
			keyState.IsKeyDown(DirectX::Keyboard::Keys::Up) == false &&
			keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) == false &&
			keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) == false &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;
		}

	}

 }

///////////////////////////
//�X�e�[�g�̊Ǘ�
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character1::StateManager()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//�v���C���[�P�̏���
	if (m_playerID == ePLAYER_ID::PLAYER_1)
	{
		//�����ԂłȂ���Ίe��Ԃ̏���������
		if (m_charaState != eCHARACTER_STATE::DAMAGE)
		{
			if (m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::WAIT;
			}

			//��Shift�ŃK�[�h
			if ((m_charaState == eCHARACTER_STATE::WAIT ||
				m_charaState == eCHARACTER_STATE::SQUAT ||
				m_charaState == eCHARACTER_STATE::MOVE_BACK ||
				m_charaState == eCHARACTER_STATE::MOVE_FRONT) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::LeftShift))
			{
				m_charaState = eCHARACTER_STATE::GUARD;
			}

			//�X�y�[�X�L�[���͂��u�[�X�g�e�ʂ�����΃u�[�X�g�ړ�
			if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Space) && m_boostCap > 0)
			{
				m_charaState = eCHARACTER_STATE::BOOST_MOVE;
			}


			//�҂���Ԃɉ����͂ł��Ⴊ��
			if (m_charaState == eCHARACTER_STATE::WAIT && keyState.IsKeyDown(DirectX::Keyboard::Keys::Down) && m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::SQUAT;
			}

			//�҂���Ԃō��E���͂ňړ�
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT &&
				m_charaState != eCHARACTER_STATE::GUARD &&
				m_isAttacking == false) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD && 
					m_isAttacking == false))
			{
				//�O�ړ�
				m_charaState = eCHARACTER_STATE::MOVE_FRONT;
			}
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::Left) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT &&
				m_charaState != eCHARACTER_STATE::GUARD && 
				m_isAttacking == false) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::Right) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT &&
					m_charaState != eCHARACTER_STATE::GUARD && 
					m_isAttacking == false))
			{
				//��ړ�
				m_charaState = eCHARACTER_STATE::MOVE_BACK;
			}
			//����͂ŃW�����v
			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up) &&
				m_charaState != eCHARACTER_STATE::SQUAT &&
				m_charaState != eCHARACTER_STATE::BOOST_MOVE)
			{
				m_charaState = eCHARACTER_STATE::JUMP;
			}
		}
	}

	else if (m_playerID == ePLAYER_ID::PLAYER_2)
	{
		//�����ԂłȂ���Ίe��Ԃ̏���������
		if (m_charaState != eCHARACTER_STATE::DAMAGE)
		{
			if (m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::WAIT;
			}


			//�҂���Ԃɉ����͂ł��Ⴊ��
			if (m_charaState == eCHARACTER_STATE::WAIT && keyState.IsKeyDown(DirectX::Keyboard::Keys::S) && m_landingFlag == true)
			{
				m_charaState = eCHARACTER_STATE::SQUAT;
			}

			//�҂���Ԃō��E���͂ňړ�
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::D) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::A) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT && m_charaState != eCHARACTER_STATE::GUARD))
			{
				//�O�ړ�
				m_charaState = eCHARACTER_STATE::MOVE_FRONT;
			}
			if (((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::A) && m_frontVector.x > 0 &&
				m_charaState != eCHARACTER_STATE::SQUAT) ||
				((m_charaState == eCHARACTER_STATE::WAIT || m_charaState == eCHARACTER_STATE::JUMP) &&
					keyState.IsKeyDown(DirectX::Keyboard::Keys::D) && m_frontVector.x < 0 &&
					m_charaState != eCHARACTER_STATE::SQUAT))
			{
				//��ړ�
				m_charaState = eCHARACTER_STATE::MOVE_BACK;

			}

			//����͂ŃW�����v
			if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::W) &&
				m_charaState != eCHARACTER_STATE::SQUAT &&
				m_charaState != eCHARACTER_STATE::BOOST_MOVE)
			{
				m_charaState = eCHARACTER_STATE::JUMP;
			}

			//��Shift�ŃK�[�h
			if ((m_charaState == eCHARACTER_STATE::WAIT ||
				m_charaState == eCHARACTER_STATE::SQUAT ||
				m_charaState == eCHARACTER_STATE::MOVE_BACK ||
				m_charaState == eCHARACTER_STATE::MOVE_FRONT) &&
				keyState.IsKeyDown(DirectX::Keyboard::Keys::Q))
			{
				m_charaState = eCHARACTER_STATE::GUARD;
			}


		}
	}
}

