///////////////////////////////////
//�L�����N�^�[3�̃N���X
//�쐬�ҁFGS305_��_�D��
//�쐬���F2020�N7��13��
///////////////////////////////////
#include "pch.h"
#include "Character3.h"
#include "GamePad.h"
#include "../../../PlayerID.h"
#include "../../AttackManager/AttackManager.h"
#include "Src\FrameWork\FbxModel\FbxModel.h"
#include "../CharacterState.h"
#include "State/Character3StateManager.h"
#include "Attack\/Character3AttackManager.h"
#include "../ChracterAnimationNumber.h"

#include "Adx2.h"
#include "CueSheet_0.h"
#include "../../../../FrameWork/FbxResourceManager/FbxResourceManager.h"
#include "../CharacterID.h"
#include "Src/Cgdi.h"


//�R���X�g���N�^
Character3::Character3(ePLAYER_ID playerID)
	:	//���N���X�̃R���X�g���N�^
	CharacterBase::CharacterBase(playerID)
{
	//���g�̃L�����N�^�[��ID��ݒ�
	SetCharacterID(eCHARACTER_ID::CHARACTER_3);
}

//�f�X�g���N�^
Character3::~Character3()
{	
	//���N���X�̃f�X�g���N�^
	CharacterBase::~CharacterBase();
}

///////////////////////////
//������
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3::Initialize()
{	
	//���N���X�̏�����
	CharacterBase::Initialize();

	//FBX��ǂݍ���Ń��f�����쐬
	m_pFbxModel = FbxResourceManager::GetInstance()->GetModel(eCHARACTER_ID::CHARACTER_3);


	Character3Params::GetInstance()->LoadStatusData();


	//�����̓����蔻��̔��̓ǂݍ���
	m_legCollBox.size_h = Character3Params::LEG_COLL_SIZE;
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.4f, m_pos.z);
	m_pLegBox = DirectX::GeometricPrimitive::CreateBox(gdi->GetDeviceResources()->GetD3DDeviceContext(), m_legCollBox.size_h * 2);
	m_legBoxWorld.Identity;

	//�̂̓����蔻��̔��̓ǂݍ���
	m_bodyCollBox.size_h = Character3Params::BODY_COLL_SIZE_NORMAL;
	m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.3f, m_pos.z);
	m_pBodyBox = DirectX::GeometricPrimitive::CreateBox(gdi->GetDeviceResources()->GetD3DDeviceContext(), m_bodyCollBox.size_h * 2);
	m_pBodySquatBox = DirectX::GeometricPrimitive::CreateBox(gdi->GetDeviceResources()->GetD3DDeviceContext(), Character3Params::BODY_COLL_SIZE_SQUAT * 2);
	m_bodyBoxWorld.Identity;

	//���̓����蔻��̔��̓ǂݍ���
	m_headCollBox.size_h = Character3Params::HEAD_COLL_SIZE;
	m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y + 0.3f, m_pos.z);
	m_pHeadBox = DirectX::GeometricPrimitive::CreateBox(gdi->GetDeviceResources()->GetD3DDeviceContext(), m_headCollBox.size_h * 2);
	m_headBoxWorld.Identity;


	//�O�����̐ݒ�
	m_frontVector = Character3Params::FRONT_VECTOR;

	//�̗͂̐ݒ�
	m_hp = m_hpMax = Character3Params::GetInstance()->MAX_HP;
	m_hpBuffer = m_hp;

	//�u�[�X�g�e�ʂ̏�����
	m_boostCap = Character3Params::GetInstance()->BOOST_CAP_MAX;


	//Y���̊p�x�̐ݒ�
	if (m_playerID == ePLAYER_ID::PLAYER_1)m_angleY = Character3Params::ANGLE_Y;
	if (m_playerID == ePLAYER_ID::PLAYER_2)m_angleY = 180.0f + Character3Params::ANGLE_Y;

	//���W���s��ɕϊ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character3Params::BODY_SIZE, Character3Params::BODY_SIZE, Character3Params::BODY_SIZE);
	//Y���̊p�x���s��ɕϊ�
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_angleY);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;


}

///////////////////////////
//�X�V
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3::Update(DX::StepTimer const& timer)
{
	m_stepTimer = timer;
	//�L�[�{�[�h�̏�Ԃ̎擾
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);

	//�X�e�[�g�̊Ǘ�
	if (m_playerID == ePLAYER_ID::PLAYER_1 && m_hp > 0)
	{
		StateManager();
	}

	//�v���C���[�Q�Ȃ�AI����
	if (m_playerID == ePLAYER_ID::PLAYER_2 && m_hp > 0)
	{
		AI();
	}

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

	//�v���C���[��Y���̊p�x
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = Character3Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = -180.0f + Character3Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = -1.0f;

	}
	//��]�s����쐬
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angleY));


	//�L�����̃X�e�[�g
	m_pStateManager->Update(m_charaState);

	//�ړ�
	//Move();
	//�U��
	if (m_hp > 0)
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
				m_pos.x - Character3Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character3Params::BOOST_EFFECT_POS_Y,
				0.0f), Character3Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x + Character3Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character3Params::BOOST_EFFECT_POS_Y,
				0.0f), Character3Params::BOOST_EFFECT_ANGLE_RIGHT);

	}

	//�o�b�t�@�ɍ��W���i�[
	m_posBuf = m_pos;
	//�ړ��ʂ����W�ɉ��Z
	m_pos += m_vel;

	//�̗͂̍X�V
	if (m_hp > m_hpBuffer)m_hp--;


	////Y����]
	//Matrix rotX = Matrix::CreateRotationX(time);

	//Matrix rotZ = Matrix::CreateRotationZ(XMConvertToRadians(30));

	////m_world = rotX;
	//m_world = rotY *  m_world * rotX;

	//���W���s��ɕϊ�
	DirectX::SimpleMath::Matrix trans = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z));
	//�T�C�Y���s��ɕϊ�
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character3Params::BODY_SIZE, Character3Params::BODY_SIZE, Character3Params::BODY_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;

	//�V�[���h�̃T�C�Y�A���W�̍X�V
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(
		DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - Character3Params::SHIELD_POS_Y, m_pos.z));
	m_shieldWorld = Scale * pos;


	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 2.2f, m_pos.z);

	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�̂̔��̍��W�̍X�V
		m_bodyCollBox.size_h = Character3Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.4f, m_pos.z);
		//���̔��̍��W�̍X�V
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.5f, m_pos.z);
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
void Character3::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{	

	//���f���̕`��
	//m_pModel->Render(view, proj);
	//���Ⴊ��
	if (m_charaState == eCHARACTER_STATE::SQUAT
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false
		&& m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::SQUAT));

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
		m_pBoostEffectManager->Render(view,proj);
	}
	//�K�[�h
	else if (m_charaState == eCHARACTER_STATE::GUARD)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::GUARD));
	}
	//�W�����v
	else if (m_charaState == eCHARACTER_STATE::JUMP)
	{
		m_pFbxModel->ChangeAnimation(static_cast<int>(eCHARACTER_ANIMATION_NUMBER::JUMP));
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

	//��j���[�g�����U���̒e�̕`��
	if (m_pBulletGP != nullptr && m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)])
	{
		m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::SkyBlue);
	}
	//�㉡�U���̒e�̕`��
	if (m_pBulletGP != nullptr && m_isAttackUse[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)])
	{
		m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::SkyBlue);
	}
	//���j���[�g�����U���̒e�̕`��
	if ( m_pBulletGP != nullptr && m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)])
	{
		m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::Silver);
	}
	//�����U���̒e�̕`��
	if (m_pBulletGP != nullptr && m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)])
	{
		m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::Silver);
	}
	//�����U���̒e�̕`��
	if (m_pBulletGP != nullptr && m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)])
	{
		m_pBulletGP->Draw(m_bulletWorld, view, proj, DirectX::Colors::Magenta);
	}
	
	//�U���}�l�[�W���[�̕`��
	m_pCharacterAttackManager->Render(view, proj);

	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̕`��
	m_pHitEffectManager->SetRenderState(Character3Params::CAMERA_POS, view, proj);
	m_pHitEffectManager->Render();

}

///////////////////////////
//�I��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3::Finalize()
{
	//�W�I���g���v���~�e�B�u�̉��
	m_pLegBox.reset();

	//�X�e�[�g�}�l�[�W���[�̏I������
	m_pStateManager->Finalize();
	m_pStateManager.reset();

	//�U���}�l�[�W���[�̏I������
	m_pCharacterAttackManager->Finalize();
	m_pCharacterAttackManager.reset();

	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̏I������
	m_pHitEffectManager->Finalize();
	m_pHitEffectManager.reset();
	m_pHitEffectManager = nullptr;

	//���f���̃|�C���^�̍폜
	//delete m_pFbxModel;
	m_pFbxModel = nullptr;

	//delete m_pShieldModel;
	m_pShieldModel = nullptr;
	
	

	m_pEnemy = nullptr;

	//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̏I������
	m_pBoostEffectManager->Finalize();
	m_pBoostEffectManager.reset();
	m_pBoostEffectManager = nullptr;


}

///////////////////////////
//���Ƃ̓����蔻��
//����:���̓����蔻��\����
//�߂�l:�Ȃ�
//////////////////////////
void Character3::HitFloor(const Collision::BoxCollision& floorColl)
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

		//////����͉������ςȂ��ɂ��A���W�����v�h�~
		//if (keyTracker.IsKeyPressed(static_cast<DirectX::Keyboard::Keys>(VK_UP)) == false)
		//{
		//	
		//}
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
void Character3::HitEnemyBody(const Collision::BoxCollision & enemyBodyColl, const Collision::BoxCollision& enemyHeadColl)
{
	//if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyBodyColl))
	//{
	//	m_isHitEnemyBody = true;
	//}
	//else m_isHitEnemyBody = false;

	//�̂Ƒ̂̓����蔻��
	if (Collision::CheckHitBox2Box(m_bodyCollBox, enemyBodyColl))
	{
		//�O�t���[���̍��W�ɖ߂�
		if (m_landingFlag == true)
		{
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
void Character3::HitAttack()
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
			//�K�[�h�Ȃ��œ������������
			if (m_charaState != eCHARACTER_STATE::GUARD)
			{
				m_bodyColor = DirectX::SimpleMath::Vector4(0.0f, 1.0f, 1.0f, 0.4f);
				//�̗͂����炷
				//m_hp -= m_pAttackManager->GetAttackStruct(i)->power;
				m_hpBuffer = m_hp - m_pAttackManager->GetAttackStruct(i)->power;

				//�����Ԃɐ؂�ւ���
				m_charaState = eCHARACTER_STATE::DAMAGE;
				//�q�b�g�G�t�F�N�g�̍Đ�
				m_pHitEffectManager->Play(20.0f,
					DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z));


				////�m�b�N�o�b�N
				//if (m_frontVector.x > 0 && m_pos.x > -MOVE_LIMIT_X)
				//{
				//	m_pos.x -= 0.1f;
				//}
				//else if (m_frontVector.x < 0 && m_pos.x < MOVE_LIMIT_X)
				//{
				//	m_pos.x += 0.1f;
				//}

				//m_pAttackManager->GetAttackStruct(i)->isUsed = false;
				//���������t���O�𗧂Ă�
				m_pAttackManager->GetAttackStruct(i)->isHit = true;

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
				//m_hp -= m_pAttackManager->GetAttackStruct(i)->power / m_pAttackManager->GetAttackStruct(i)->power;
				m_hpBuffer = m_hp - m_pAttackManager->GetAttackStruct(i)->power / m_pAttackManager->GetAttackStruct(i)->power;
				//�K�[�h��Ԃɐ؂�ւ���
				//m_charaState = eCHARACTER_STATE::GUARD;
				//���������t���O�𗧂Ă�
				m_pAttackManager->GetAttackStruct(i)->isHit = true;
				//�q�b�g�G�t�F�N�g�̍Đ�
				m_pHitEffectManager->Play(10.0f,
					DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.0f, m_pos.z));


			}

		}
	}

}

///////////////////////////
//����
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3::Ready(DX::StepTimer const& timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_pKeyTracker->Update(keyState);
	//�X�e�[�g�̊Ǘ�
	if(m_hp > 0)StateManager();

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

	//�v���C���[��Y���̊p�x
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = Character3Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = -180.0f + Character3Params::ANGLE_Y;
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
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(
		Character3Params::BODY_SIZE, Character3Params::BODY_SIZE, Character3Params::BODY_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;

	//�V�[���h�̃T�C�Y�A���W�̍X�V
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(
		DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - Character3Params::SHIELD_POS_Y, m_pos.z));
	m_shieldWorld = Scale * pos;

	//���f���̍X�V
	//m_pModel->Update(m_world);

	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 2.2f, m_pos.z);

	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�̂̔��̍��W�̍X�V
		m_bodyCollBox.size_h = Character3Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.4f, m_pos.z);
		//���̔��̍��W�̍X�V
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.4f, m_pos.z);
	}
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);
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
				m_pos.x - Character3Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character3Params::BOOST_EFFECT_POS_Y,
				0.0f), Character3Params::BOOST_EFFECT_ANGLE_LEFT);
	}
	else
	{
		m_pBoostEffectManager->Update(timer,
			DirectX::SimpleMath::Vector3(
				m_pos.x + Character3Params::BOOST_EFFECT_POS_X,
				m_pos.y + Character3Params::BOOST_EFFECT_POS_Y,
				0.0f), Character3Params::BOOST_EFFECT_ANGLE_RIGHT);

	}
}

///////////////////////////
//�s�k
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3::Lose(DX::StepTimer const & timer)
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
	if (m_pos.x < m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = Character3Params::ANGLE_Y;
		//�O�����̃x�N�g����ς���
		m_frontVector.x = 1.0f;
	}
	else if (m_pos.x > m_enemyPos.x && m_landingFlag == true)
	{
		//�����Ă��������ς���
		m_angleY = -180.0f + Character3Params::ANGLE_Y;
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
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(Character3Params::BODY_SIZE);
	//���[���h�s��ɉ��Z
	m_world = scale * rotY* trans;

	//�V�[���h�̃T�C�Y�A���W�̍X�V
	DirectX::SimpleMath::Matrix Scale = DirectX::SimpleMath::Matrix::CreateScale(1);
	DirectX::SimpleMath::Matrix pos = DirectX::SimpleMath::Matrix::CreateTranslation(
		DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - Character3Params::SHIELD_POS_Y, m_pos.z));
	m_shieldWorld = Scale * pos;

	//���f���̍X�V
	//m_pModel->Update(m_world);

	//�����̔��̍��W�̍X�V
	m_legCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 2.2f, m_pos.z);

	if (m_charaState != eCHARACTER_STATE::SQUAT)
	{
		//�̂̔��̍��W�̍X�V
		m_bodyCollBox.size_h = Character3Params::BODY_COLL_SIZE_NORMAL;
		m_bodyCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 1.4f, m_pos.z);
		//���̔��̍��W�̍X�V
		m_headCollBox.pos = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y - 0.4f, m_pos.z);
	}
	m_legBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_legCollBox.pos);
	m_bodyBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_bodyCollBox.pos);
	m_headBoxWorld = DirectX::SimpleMath::Matrix::CreateTranslation(m_headCollBox.pos);

	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̍X�V
	m_pHitEffectManager->Update(timer);

}

///////////////////////////
//���Z�b�g
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3::Reset()
{
	//�������W�̐ݒ�
	m_pos = m_startPos;
	m_posBuf = m_pos;
	//�̗͂̐ݒ�
	m_hp = m_hpMax;
	m_hpBuffer = m_hp;
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
	m_boostCap = Character3Params::GetInstance()->BOOST_CAP_MAX;

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
void Character3::ChangeAnimation(const int & animationStack)
{
	m_pFbxModel->ChangeAnimation(animationStack);
}


///////////////////////////
//�U��
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3::Attack()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	if (m_playerID == ePLAYER_ID::PLAYER_1)
	{
		//��j���[�g�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::WAIT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] == false && 
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_NEUTRAL)] = true;
		}

		//�㉺�U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::SQUAT &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_BOTTOM)] = true;
		}

		//�㉡�U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) &&
			m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::WEAK_SIDE)] = true;
		}

		//���j���[�g�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::WAIT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_NEUTRAL)] = true;
		}

		//�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::MOVE_FRONT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_SIDE)] = true;
		}



		//�����U��
		if (m_pKeyTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) &&
			m_charaState == eCHARACTER_STATE::SQUAT &&
			m_charaState != eCHARACTER_STATE::GUARD &&
			m_charaState != eCHARACTER_STATE::DAMAGE &&
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false &&
			m_isAttacking == false)
		{
			m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = true;
		}
	}
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
				//��
				else if (m_charaState == eCHARACTER_STATE::SQUAT &&
					m_charaState != eCHARACTER_STATE::GUARD &&
					m_charaState != eCHARACTER_STATE::DAMAGE &&
					m_isAttackUse[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] == false &&
					m_isAttacking == false)
				{
					//m_isAttackInput[static_cast<int>(eATTACK_TYPE::MIDDLE_BOTTOM)] = true;
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
void Character3::StateManager()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//�u�[�X�g�ړ��łȂ���΃u�[�X�g�e�ʂ𑝂₷
	if (m_charaState != eCHARACTER_STATE::BOOST_MOVE && keyState.IsKeyUp(DirectX::Keyboard::Keys::Space) && m_charaState != eCHARACTER_STATE::DAMAGE)
	{
		m_boostCap++;
		//�ő�l�ȏ�ɂȂ�����ő�l����
		if (m_boostCap >= Character3Params::GetInstance()->BOOST_CAP_MAX)
		{
			m_boostCap = Character3Params::GetInstance()->BOOST_CAP_MAX;
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
				//m_charaState == eCHARACTER_STATE::SQUAT ||
				m_charaState == eCHARACTER_STATE::MOVE_BACK ||
				m_charaState == eCHARACTER_STATE::MOVE_FRONT
				) &&
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
	}
}

///////////////////////////
//CPU����
//����:�Ȃ�
//�߂�l:�Ȃ�
//////////////////////////
void Character3::AI()
{
	//�u�[�X�g�ړ��łȂ���΃u�[�X�g�e�ʂ𑝂₷
	if (m_charaState != eCHARACTER_STATE::BOOST_MOVE)
	{
		m_boostCap++;
		//�ő�l�ȏ�ɂȂ�����ő�l����
		if (m_boostCap >= Character3Params::GetInstance()->BOOST_CAP_MAX)
		{
			m_boostCap = Character3Params::GetInstance()->BOOST_CAP_MAX;
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
