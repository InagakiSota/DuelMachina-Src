///////////////////////////////////
//�L�����N�^�[�̊��N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N7��13��
///////////////////////////////////
#pragma once

#include <SimpleMath.h>
#include "GeometricPrimitive.h"
#include "Src\FrameWork\ModelObject\ModelObject.h"
#include "Src\FrameWork\Collision\Collision.h"
#include "Keyboard.h"
#include "AttackType.h"
#include "CharacterStateManagerBase.h"
#include "CharacterAttackManagerBase.h"
#include "HitEffectManager.h"
#include "BoostEffectManager.h"
#include "../../../FrameWork/ModelObject/ModelObject.h"

//�O���錾
enum class ePLAYER_ID;
class AttackManager;
struct AttackStruct;
class FbxModel;
enum class eCHARACTER_STATE;


class CharacterBase
{
public:
	//�ړ�����
	static const float MOVE_LIMIT_X;

	//�f�X�g���N�^
	virtual ~CharacterBase();

	//������
	virtual void Initialize() = 0;
	//�X�V
	virtual void Update(DX::StepTimer const& timer) = 0;
	//�`��
	virtual void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;
	//�I��
	virtual void Finalize() = 0;
	//���Ƃ̓����蔻��
	virtual void HitFloor(const Collision::BoxCollision& floorColl) = 0;

	////////////////////////////////
	//�Q�b�^�[�A�Z�b�^�[
	////////////////////////////////
	//�������W�̐ݒ�
	virtual void SetStartPos(const DirectX::SimpleMath::Vector3& startPos) = 0;
	//�����̓����蔻��̔��̎擾
	virtual Collision::BoxCollision GetLegCollBox() = 0;
	//�����̓����蔻��̔��̐ݒ�
	virtual void SetLegCollBox(const Collision::BoxCollision& legCollBox) = 0;
	//�̂̓����蔻��̔��̎擾
	virtual Collision::BoxCollision GetBodyCollBox() = 0;
	//�̂̓����蔻��̔��̐ݒ�
	virtual void SetBodyCollBox(const Collision::BoxCollision& bodyCollBox) = 0;
	//���̓����蔻��̔��̎擾
	virtual Collision::BoxCollision GetHeadCollBox() = 0;
	//���̓����蔻��̔��̐ݒ�
	virtual void SetHeadCollBox(const Collision::BoxCollision& headCollBox) = 0;
	//�G�̑̂Ƃ̓����蔻��
	virtual void HitEnemyBody(const Collision::BoxCollision& enemyBodyColl, const Collision::BoxCollision& enemyHeadColl) = 0;
	//�U���}�l�[�W���[�̎擾
	virtual AttackManager* GetAttackManager() = 0;
	//�U���}�l�[�W���[�̐ݒ�
	virtual void SetAttackManager(AttackManager* pAttackManager) = 0;
	//�U���Ƃ̓����蔻��
	virtual void HitAttack() = 0;
	//�G�̍��W�̐ݒ�
	virtual void SetEnemyPos(const DirectX::SimpleMath::Vector3& enemyPos) = 0;
	//�G�̍��W�̎擾
	virtual DirectX::SimpleMath::Vector3 GetEnemyPos() = 0;
	//�G�̃|�C���^�̎擾
	virtual void SetEnemy(CharacterBase* pEnemy) = 0;
	//���W�̎擾
	virtual DirectX::SimpleMath::Vector3 GetPos() = 0;
	//���W�̐ݒ�
	virtual void SetPos(const DirectX::SimpleMath::Vector3& pos) = 0;
	//Y���̊p�x�̎擾
	virtual float GetAngleY() = 0;
	//Y���̊p�x�̐ݒ�
	virtual void SetAngleY(const float& angleY) = 0;
	//�ړ��ʂ̎擾
	virtual DirectX::SimpleMath::Vector3 GetVel() = 0;
	//�ړ��ʂ̐ݒ�
	virtual void SetVel(const DirectX::SimpleMath::Vector3& vel) = 0;
	//�L�����N�^�[�̃X�e�[�g�̎擾
	virtual eCHARACTER_STATE GetCharaState() = 0;
	//�L�����N�^�[�̃X�e�[�g�̐ݒ�
	virtual void SetCharaState(const eCHARACTER_STATE& state) = 0;
	//�ڒn�t���O�̎擾
	virtual bool GetLandingFlag() = 0;
	//�ڒn�t���O�̐ݒ�
	virtual void SetLandingFlag(const bool& landingFlag) = 0;
	//�W�����v�t���O�̎擾
	virtual bool GetJumpFlag() = 0;
	//�W�����v�t���O�̐ݒ�
	virtual void SetJumpFlag(const bool& jumpFlag) = 0;
	//�_���[�W�^�C�}�[�̎擾
	virtual float GetDamageTimer() = 0;
	//�_���[�W�^�C�}�[�̐ݒ�
	virtual void SetDamageTimer(const float& damageTimer) = 0;
	//�O�����̃x�N�g���̎擾
	virtual DirectX::SimpleMath::Vector3 GetFrontVector() = 0;
	//�U���p�̑O�����̃x�N�g���̎擾
	virtual DirectX::SimpleMath::Vector3 GetAttackFront() = 0;
	//�U���p�̑O�����̃x�N�g���̐ݒ�
	virtual void SetAttackFront(const DirectX::SimpleMath::Vector3& attackFront) = 0;
	//�̗͂̎擾
	virtual int GetHP() = 0;
	//�̗͂̐ݒ�
	virtual void SetHP(const int& hp) = 0;
	//�ő�̗͂̎擾
	virtual int GetMaxHP() = 0;
	//�u�[�X�g�e�ʂ̎擾
	virtual int GetBoostCap() = 0;
	//�u�[�X�g�e�ʂ̐ݒ�
	virtual void SetBoostCap(const int& boostCap) = 0;
	//�ő�u�[�X�g�e�ʂ̎擾
	virtual int GetMaxBoostCap() = 0;
	//�U���\���̂̃|�C���^�̎擾
	virtual AttackStruct* GetAttack() = 0;
	//�U���\���̂̃|�C���^�̐ݒ�
	virtual void SetAttack(AttackStruct* pAttack) = 0;
	//�U���̎g�p���̃t���O�̎擾
	virtual bool GetIsAttackUse(const int& index) = 0;
	//�U���̎g�p���̃t���O�̐ݒ�
	virtual void SetIsAttackUse(const int& index, bool flag) = 0;
	//�U���̓��̓t���O�̎擾
	virtual bool GetIsAttackInput(const int& index) = 0;
	//�U���̓��̓t���O�̐ݒ�
	virtual void SetIsAttackInput(const int& index, bool flag) = 0;
	//�v���C���[ID�̎擾
	virtual ePLAYER_ID GetPlayerID() = 0;
	//�U�����̃t���O�̎擾
	virtual bool GetIsAttacking() = 0;
	//�U�����̃t���O�̐ݒ�
	virtual void SetIsAttacking(const bool& isAttacking) = 0;
	//�X�e�b�v�^�C�}�[�̎擾
	virtual DX::StepTimer GetStepTimer() = 0;

	/////////////////////////////////////

	//����
	virtual void Ready(DX::StepTimer const& timer) = 0;
	//�s�k
	virtual void Lose(DX::StepTimer const& timer) = 0;

	//���Z�b�g
	virtual void Reset() = 0;

	//�A�j���[�V�����̐؂�ւ�
	virtual void ChangeAnimation(const int& animationStack) = 0;


protected:
	static const int ATTACK_NUM = 32;
	//�d��
	static const float GRAVITY;	

	//�f�o�C�X���\�[�X�̃|�C���^
	DX::DeviceResources* m_pDeviceResources;		
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;						
	//���W
	DirectX::SimpleMath::Vector3 m_pos;
	//�������W
	DirectX::SimpleMath::Vector3 m_startPos;
	//�ړ���
	DirectX::SimpleMath::Vector3 m_vel;
	//3D���f��
	std::unique_ptr<ModelObject> m_pModel;
	//�����̔�
	std::unique_ptr<DirectX::GeometricPrimitive> m_pLegBox;		
	//�����̓����蔻��p�̍\����
	Collision::BoxCollision m_legCollBox;						
	//�����̔��̃��[���h�s��
	DirectX::SimpleMath::Matrix m_legBoxWorld;
	//�̂̔�
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBodyBox;
	//�̂̔�(���Ⴊ��)
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBodySquatBox;
	//�̂̓����蔻��p�̍\����
	Collision::BoxCollision m_bodyCollBox;
	//�̂̔��̃��[���h�s��
	DirectX::SimpleMath::Matrix m_bodyBoxWorld;
	//���̔�
	std::unique_ptr<DirectX::GeometricPrimitive> m_pHeadBox;
	//���̔��̓����蔻��\����
	Collision::BoxCollision m_headCollBox;
	//���̔��̃��[���h�s��
	DirectX::SimpleMath::Matrix m_headBoxWorld;
	//�O�����̃x�N�g��
	DirectX::SimpleMath::Vector3 m_frontVector;
	//�U���p�̑O�����̃x�N�g��
	DirectX::SimpleMath::Vector3 m_attackFront;
	//�G�̑̂Ɠ����������̃t���O
	bool m_isHitEnemyBody;
	//���W�̃o�b�t�@
	DirectX::SimpleMath::Vector3 m_posBuf;
	//�U���̃}�l�[�W���[�̃|�C���^
	AttackManager* m_pAttackManager;
	//�U���̍\���̂̃|�C���^
	AttackStruct* m_pAttack;
	//�G�̍��W
	DirectX::SimpleMath::Vector3 m_enemyPos;
	//�G�̃|�C���^
	CharacterBase* m_pEnemy;

	DirectX::SimpleMath::Vector4 m_bodyColor;

	//�e��Ԃ̃��f���̃|�C���^
	//FBX���f��
	FbxModel* m_pFbxModel;

	//�V�[���h�̃��f��
	FbxModel* m_pShieldModel;
	//�V�[���h�̃��f���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_shieldWorld;


	//���n�t���O
	bool m_landingFlag;											
	//�W�����v�t���O
	bool m_jumpFlag;					

	//�v���C���[��ID(�v���C���[�P���Q��)
	ePLAYER_ID m_playerID;

	//�L�[�g���b�J�[�̃|�C���^
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_pKeyTracker;

	//�̗�
	int m_HP;
	//�̗͂̃o�b�t�@
	int m_HPBuffer;

	//�����Ԃ̃^�C�}�[
	float m_damageTimer;

	//Y���̊p�x
	float m_angleY;

	//AI�̃X�e�[�g�^�C�}�[
	float m_aiStateTiemer;

	//AI�̍U���^�C�}�[
	float m_aiAttackTimer;
	//AI�̃X�e�[�g
	int m_aiState;
	//�U�����g�p���Ă���t���O
	bool m_isAttackUse[static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM)];
	//�U���̓��̓t���O
	bool m_isAttackInput[static_cast<int>(eATTACK_TYPE::ATTACK_TYPE_NUM)];
	//�U�����̃t���O
	bool m_isAttacking;


	//�u�[�X�g�e��
	int m_boostCap;

	//�L�����̃X�e�[�g�ϐ�
	eCHARACTER_STATE m_charaState;

	//�L�����̃X�e�[�g�Ǘ�
	virtual void StateManager() = 0;
	//�U��
	virtual void Attack() = 0;

	//�X�e�[�g�}�l�[�W���[�̃|�C���^
	std::unique_ptr<CharacterStateManagerBase> m_pStateManager;
	//�U���}�l�[�W���[�̃|�C���^
	std::unique_ptr<CharacterAttackManagerBase> m_pCharacterAttackManager;
	//�q�b�g�G�t�F�N�g�}�l�[�W���[�̃|�C���^
	std::unique_ptr<HitEffectManager> m_pHitEffectManager;
	//�u�[�X�g�G�t�F�N�g�}�l�[�W���[�̃|�C���^
	std::unique_ptr<BoostEffectManager> m_pBoostEffectManager;

	//�e�̃W�I���g���v���~�e�B�u
	std::unique_ptr<DirectX::GeometricPrimitive> m_pBulletGP;
	//�e�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_bulletWorld;

	//�^�C�}�[
	DX::StepTimer m_stepTimer;
};
