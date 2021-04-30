///////////////////////////////////
//�L�����N�^�[2�̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N7��13��
///////////////////////////////////
#pragma once

#include "../CharacterBase.h"
#include "Keyboard.h"
#include "Character2Params.h"

class CharacterStateManagerBase;

class Character2 : public CharacterBase
{
public:
	//�R���X�g���N�^
	Character2(ePLAYER_ID playerID);
	//�f�X�g���N�^
	~Character2();

	//������
	void Initialize() override;
	//�X�V
	void Update(DX::StepTimer const& timer) override;
	//�`��
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	//�I��
	void Finalize() override;
	//���Ƃ̓����蔻��
	void HitFloor(const Collision::BoxCollision& floorColl) override;
	//�������W�̐ݒ�
	void SetStartPos(const DirectX::SimpleMath::Vector3& startPos) override
	{
		m_startPos = startPos;
		m_pos = m_startPos;
	}

	//�����̓����蔻��̔��̎擾
	Collision::BoxCollision GetLegCollBox() override
	{
		return m_legCollBox;
	}

	//�����̓����蔻��̔��̐ݒ�
	void SetLegCollBox(const Collision::BoxCollision& legCollBox) override
	{
		m_legCollBox = legCollBox;
	}

	//�̂̓����蔻��̔��̎擾
	Collision::BoxCollision GetBodyCollBox() override
	{
		return m_bodyCollBox;
	}

	//�̂̓����蔻��̔��̐ݒ�
	void SetBodyCollBox(const Collision::BoxCollision& bodyCollBox) override
	{
		m_bodyCollBox = bodyCollBox;
	}

	//���̓����蔻��̔��̎擾
	Collision::BoxCollision GetHeadCollBox() override
	{
		return m_headCollBox;
	}

	//���̓����蔻��̔��̐ݒ�
	void SetHeadCollBox(const Collision::BoxCollision& headCollBox) override
	{
		m_headCollBox = headCollBox;
	}

	//�G�̑̂Ƃ̓����蔻��
	void HitEnemyBody(const Collision::BoxCollision& enemyBodyColl, const Collision::BoxCollision& enemyHeadColl) override;
	//�U���}�l�[�W���[�̎擾
	AttackManager* GetAttackManager()
	{
		return m_pAttackManager;
	}
	//�U���}�l�[�W���[�̐ݒ�
	void SetAttackManager(AttackManager* pAttackManager) override
	{
		m_pAttackManager = pAttackManager;
	}

	//�U���Ƃ̓����蔻��
	void HitAttack()override;
	//�G�̍��W�̐ݒ�
	void SetEnemyPos(const DirectX::SimpleMath::Vector3& enemyPos) override
	{
		m_enemyPos = enemyPos;
	}
	//�G�̍��W�̎擾
	DirectX::SimpleMath::Vector3 GetEnemyPos() override
	{
		return m_enemyPos;
	}

	//�G�̃|�C���^�̎擾
	void SetEnemy(CharacterBase* pEnemy) override
	{
		m_pEnemy = pEnemy;
	}


	//���W�̎擾
	DirectX::SimpleMath::Vector3 GetPos() override
	{
		return m_pos;
	}

	//���W�̐ݒ�
	void SetPos(const DirectX::SimpleMath::Vector3& pos) override
	{
		m_pos = pos;
	}

	//Y���̊p�x�̎擾
	float GetAngleY() override
	{
		return m_angleY;
	}

	//Y���̊p�x�̐ݒ�
	void SetAngleY(const float& angleY) override
	{
		m_angleY = angleY;
	}

	//�ړ��ʂ̎擾
	DirectX::SimpleMath::Vector3 GetVel() override
	{
		return m_vel;
	}

	//�ړ��ʂ̐ݒ�
	void SetVel(const DirectX::SimpleMath::Vector3& vel) override
	{
		m_vel = vel;
	}

	//�L�����N�^�[�̃X�e�[�g�̎擾
	eCHARACTER_STATE GetCharaState() override
	{
		return m_charaState;
	}

	//�L�����N�^�[�̃X�e�[�g�̐ݒ�
	void SetCharaState(const eCHARACTER_STATE& charaState) override
	{
		m_charaState = charaState;
	}

	//�ڒn�t���O�̎擾
	bool GetLandingFlag() override
	{
		return m_landingFlag;
	}

	//�ڒn�t���O�̐ݒ�
	void SetLandingFlag(const bool& landingFlag) override
	{
		m_landingFlag = landingFlag;
	}

	//�W�����v�t���O�̎擾
	bool GetJumpFlag() override
	{
		return m_jumpFlag;
	}

	//�W�����v�t���O�̐ݒ�
	void SetJumpFlag(const bool& jumpFlag) override
	{
		m_jumpFlag = jumpFlag;
	}

	//�_���[�W�^�C�}�[�̎擾
	float GetDamageTimer() override
	{
		return m_damageTimer;
	}

	//�_���[�W�^�C�}�[�̐ݒ�
	void SetDamageTimer(const float& damageTimer) override
	{
		m_damageTimer = damageTimer;
	}

	//�O�����̃x�N�g���̎擾
	DirectX::SimpleMath::Vector3 GetFrontVector() override
	{
		return m_frontVector;
	}

	//�U���p�̑O�����̃x�N�g���̎擾
	DirectX::SimpleMath::Vector3 GetAttackFront() override
	{
		return m_attackFront;
	}

	//�U���p�̑O�����̃x�N�g���̐ݒ�
	void SetAttackFront(const DirectX::SimpleMath::Vector3& attackFront) override
	{
		m_attackFront = attackFront;
	}

	//�̗͂̎擾
	int GetHP() override
	{
		return m_HP;
	}
	
	//�̗͂̐ݒ�
	void SetHP(const int& hp) override
	{
		m_HP = hp;
	}

	//�ő�̗͂̎擾
	int GetMaxHP()  override
	{
		return  Character2Params::GetInstance()->MAX_HP;
	}

	//�u�[�X�g�e�ʂ̎擾
	int GetBoostCap() override
	{
		return m_boostCap;
	}

	//�u�[�X�g�e�ʂ̐ݒ�
	void SetBoostCap(const int& boostCap) override
	{
		m_boostCap = boostCap;
	}

	//�ő�u�[�X�g�e�ʂ̎擾
	int GetMaxBoostCap() override
	{
		return  Character2Params::GetInstance()->BOOST_CAP_MAX;
	}

	//�U���\���̂̃|�C���^�̎擾
	AttackStruct* GetAttack() override
	{
		return m_pAttack;
	}
	//�U���\���̂̃|�C���^�̐ݒ�
	void SetAttack(AttackStruct* pAttack) override
	{
		m_pAttack = pAttack;
	}

	//�U���̎g�p���̃t���O�̎擾
	bool GetIsAttackUse(const int& index) override
	{
		return m_isAttackUse[index];
	}

	//�U���̎g�p���̃t���O�̐ݒ�
	void SetIsAttackUse(const int& index, bool flag) override
	{
		m_isAttackUse[index] = flag;
	}

	//�U���̓��̓t���O�̎擾
	bool GetIsAttackInput(const int& index) override
	{
		return m_isAttackInput[index];
	}

	//�U���̓��̓t���O�̐ݒ�
	void SetIsAttackInput(const int& index, bool flag) override
	{
		m_isAttackInput[index] = flag;
	}

	//�v���C���[ID�̎擾
	ePLAYER_ID GetPlayerID()
	{
		return m_playerID;
	}


	//�U�����̃t���O�̎擾
	bool GetIsAttacking() override
	{
		return m_isAttacking;
	}
	//�U�����̃t���O�̐ݒ�
	void SetIsAttacking(const bool& isAttacking) override
	{
		m_isAttacking = isAttacking;
	}

	//�X�e�b�v�^�C�}�[�̎擾
	DX::StepTimer GetStepTimer()
	{
		return m_stepTimer;
	}


	//����
	void Ready(DX::StepTimer const& timer) override;
	//�s�k
	void Lose(DX::StepTimer const& timer) override;
	//���Z�b�g
	void Reset() override;

	//�A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(const int& animationStack) override;

private:

	//�U��
	void Attack()override;
	//�L�����̃X�e�[�g�Ǘ�
	void StateManager()override;

	//AI
	void AI();

};