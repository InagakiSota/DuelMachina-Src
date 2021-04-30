///////////////////////////////////
//�v���C�V�[���̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N6��19��
///////////////////////////////////

#pragma once
#include "../SceneBase.h"
#include <SimpleMath.h>
#include "Src\FrameWork\Collision\Collision.h"

#include "GeometricPrimitive.h"

//�N���X�̑O���錾
class EffectManager;
class FbxModel;
class Sprite2D;
class DebugFont;
class ModelObject;
class Fbx;
class CharacterBase;
class DebugCamera;
class AttackManager;

class FbxModel;
class ShadowManager;
class HitEffectManager;

class PlayScene :
	public SceneBase
{
////////////////////////////////////
//�֐�
public:

	//�R���X�g���N�^
	PlayScene();
	//�f�X�g���N�^
	~PlayScene();			
	//������
	void Initialize() override;	
	//�X�V
	void Update(DX::StepTimer const& timer) override;			
	//�`��
	void Render() override;		
	//�I������
	void Finalize() override;

private:
	//���Z�b�g
	void Reset();
	//�������
	void Manual();
	//���j���[
	void Menu();
	//���U���g
	void Result(DX::StepTimer const& timer);

////////////////////////////////////
//�ϐ��E�萔
private:
	//�v���C�V�[���̃X�e�[�g
	enum class ePLAY_SCENE_STATE
	{
		NONE = -1,

		COUNT_DOWN,		//�J�E���g�_�E��
		MAIN,			//���C��
		TIME_UP,		//�^�C���A�b�v
		RESULT,			//���U���g

		OVER_ID
	};

	//���E���h
	enum class eROUND
	{
		NONE = -1,

		ROUND_1,		//���E���h�P
		ROUND_2,		//���E���h�Q
		ROUND_3,		//���E���h�R

		ROUND_NUM,		//�S���E���h��
	};

	//��������̉摜�̎��
	enum class eMANUAL_SPRITE_TYPE
	{
		NONE = -1,

		KEYBOARD,		//�L�[�{�[�h
		COMMAND,		//�R�}���h

		SPRITE_NUM,		//�摜����
	};

	//�v���C�V�[���̃X�e�[�g�ϐ�
	ePLAY_SCENE_STATE m_playSceneState;

	//�v���C���[�̐�
	static const int PLAYER_NUM = 2;		
	//�̗̓o�[�̉���
	static const float HP_BAR_WIDTH;			
	//�̗̓o�[�̍���
	static const float HP_BAR_HEIGHT;
	//�̗̓o�[�̃}�[�W��
	static const float HP_BAR_MARGIN;

	//�u�[�X�g�e�ʃo�[�̉���
	static const float BOOST_BAR_WIDTH;
	//�u�[�X�g�e�ʃo�[�̍���
	static const float BOOST_BAR_HEIGHT;


	//�J�E���g�_�E���̎���
	static const float COUNT_DOWN_TIME;			
	//�J�E���g�_�E���摜�̍��W
	static const DirectX::SimpleMath::Vector2 COUNT_DOWN_SPRITE_POS;
	//�������邽�߂̎擾�{��
	static const int WIN_NUM = 2;
	//�������Ԃ̍ő�l
	static const float TIME_MAX;
	//�^�C���A�b�v�V�[���̎���
	static const float TIME_UP_TIME;
	//���E���h�؂�ւ��̎���
	static const float ROUND_CHANGE_TIME;
	//�����{���̉摜�̉���
	static const float WIN_NUM_SPRITE_WIDTH;
	//�����{���̉摜�̍���
	static const float WIN_NUM_SPRITE_HEIGHT;
	//�������Ԃ̉摜�̉���
	static const float TIME_SPRITE_WIDTH;
	//�������Ԃ̉摜�̍���
	static const float TIME_SPRITE_HEIGHT;
	//�������Ԃ̉摜��X���W
	static const float TIME_SPRITE_POS_X;
	//�̗̓o�[�̏������W
	static const DirectX::SimpleMath::Vector2 HP_BAR_POS[PLAYER_NUM];
	//�u�[�X�g�e�ʃo�[�̏������W
	static const DirectX::SimpleMath::Vector2 BOOST_BAR_POS[PLAYER_NUM];

	DirectX::SimpleMath::Matrix m_spaceWorld;	//�V���̃��[���h�s��

	//��������̉摜�̉���
	static const float MANUAL_SPRITE_WIDTH;
	//��������̉摜�̍���
	static const float MANUAL_SPRITE_HEIGHT;

	//std::unique_ptr<DirectX::CommonStates>m_states;
	//std::unique_ptr<DirectX::IEffectFactory>m_fxFactory;
	//std::unique_ptr<DirectX::Model>m_model;
	
	//3d���f��
	//std::unique_ptr<ModelObject>m_modelObject;//�v���C���[(���{)

	//�V��
	std::unique_ptr<ModelObject>m_space;		
	//�X�v���C�g								
	std::unique_ptr<Sprite2D> m_sprite2D;		

	//�̗̓o�[
	std::unique_ptr<Sprite2D> m_pHPBar[PLAYER_NUM];
	//�̗̓o�[(�̗͒�)	
	std::unique_ptr <Sprite2D> m_pHPBarDanger[PLAYER_NUM];
	//�̗̓o�[�̔w�i
	std::unique_ptr<Sprite2D> m_pHPBarBack[PLAYER_NUM];

	//�̗̓o�[�̍��W
	DirectX::SimpleMath::Vector2 m_hpBarPos[PLAYER_NUM];
	//�̗̓o�[�̐؂���ʒu
	RECT m_hpBarRect[PLAYER_NUM];

	//�u�[�X�g�e�ʃo�[
	std::unique_ptr<Sprite2D> m_pBoostBar[PLAYER_NUM];
	//�u�[�X�g�e�ʃo�[(�I�[�o�[�q�[�g)
	std::unique_ptr<Sprite2D> m_pBoostOverHeatBar[PLAYER_NUM];
	//�u�[�X�g�e�ʃo�[�̔w�i
	std::unique_ptr<Sprite2D> m_pBoostBack[PLAYER_NUM];
	//�u�[�X�g�e�ʃo�[�̍��W
	DirectX::SimpleMath::Vector2 m_boostBarPos[PLAYER_NUM];
	//�u�[�X�g�e�ʃo�[�̐؂���ʒu
	RECT m_boostBarRect[PLAYER_NUM];

	//�����摜
	std::unique_ptr<Sprite2D> m_pWinSprite[PLAYER_NUM];
	//�����摜�̍��W
	DirectX::SimpleMath::Vector2 m_winSpritePos;

	//���E���h���̉摜
	std::unique_ptr<Sprite2D> m_pRoundSprite[static_cast<int>(eROUND::ROUND_NUM)];
	//FIGHT�̉摜
	std::unique_ptr<Sprite2D> m_pFightSprite;
	//�^�C���A�b�v�̉摜
	std::unique_ptr<Sprite2D> m_pTimeUpSprite;
	//�����������̉摜
	std::unique_ptr<Sprite2D> m_pDrawSprite;
	//�������Ԃ̉摜(��̈�)
	std::unique_ptr<Sprite2D> m_pTimeSpriteOne;
	//�������Ԃ̉摜(�\�̈�)
	std::unique_ptr<Sprite2D> m_pTimeSpriteTen;

	//�����{���̉摜
	std::unique_ptr<Sprite2D> m_pWinNumSprtie[PLAYER_NUM][WIN_NUM];
	//�����{���̉摜�̐؂���ʒu
	RECT m_winNumSpriteRect[PLAYER_NUM][WIN_NUM];
	//�����{���̉摜�̍��W
	DirectX::SimpleMath::Vector2 m_winNumSpritePos[PLAYER_NUM][WIN_NUM];
	//�v���C���[�̓���̃J�[�\��
	std::unique_ptr<Sprite2D> m_pOverHeadSprite[PLAYER_NUM];
	//�v���C���[�̓���̃J�[�\���̍��W
	DirectX::SimpleMath::Vector2 m_overHeadSpritePos[PLAYER_NUM];
	//�������Ԃ̉摜�̐؂���ʒu�@��̈�
	RECT m_timeSpriteOneRect;
	//�������Ԃ̉摜�̐؂���ʒu�@�\�̈�
	RECT m_timeSpriteTenRect;

	//fbx
	//std::unique_ptr<Fbx> m_pFbx;

	//��
	std::unique_ptr<DirectX::GeometricPrimitive> m_pFloor;
	//���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_floorWorld;
	//���̓����蔻��p�̔�
	Collision::BoxCollision m_floorBox;

	//�v���C���[�̃|�C���^
	CharacterBase* m_pPlayer[PLAYER_NUM];

	//�f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_pDebugCamera;

	//�J�����̒����_
	DirectX::SimpleMath::Vector3 m_targetPos;

	//�����̉e�̃G�t�F�N�g
	std::unique_ptr<ShadowManager> m_pShadowManager[PLAYER_NUM];
	
	//�U���̃}�l�[�W���[
	//std::unique_ptr<AttackManager> m_pAttackManager;

	FbxModel* m_pFbxModel;
	DirectX::SimpleMath::Matrix m_fbxModelWorld;

	//���݂̃��E���h
	eROUND m_nowRound;
	//�e�v���C���[�̏�����(���E���h�擾�{��)
	int m_playerWinNum[PLAYER_NUM];

	//��������
	float m_time;

	//�J�E���g�_�E���^�C�}�[
	float m_countdownTimer;

	//�̗͂̃o�b�t�@
	int m_playerHpBuffer[PLAYER_NUM];

	//��������摜
	std::unique_ptr<Sprite2D> m_pManualSprite[static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM)];
	//��������̉摜�̍��W
	DirectX::SimpleMath::Vector2 m_ManualSpritePos[static_cast<int>(eMANUAL_SPRITE_TYPE::SPRITE_NUM)];
	//�Q�[����~�t���O
	bool m_isStop;
	//��������㏸�t���O
	bool m_isManualUp;
	//����������~�t���O
	bool m_isManualDown;
	//��������E�ړ��t���O
	bool m_isManualRight;
	//����������ړ��t���O
	bool m_isManualLeft;
	//��������\���t���O
	bool m_isManualDisplay;
	//��������̃J�[�\���̉摜(�E)
	std::unique_ptr<Sprite2D> m_pManualCursorSpriteRight;
	//��������̃J�[�\���̉摜(��)
	std::unique_ptr<Sprite2D> m_pManualCursorSpriteLeft;

	
	//���j���[�̉摜
	std::unique_ptr<Sprite2D> m_pMenuSprite;
	//���j���[�̃J�[�\���摜
	std::unique_ptr<Sprite2D> m_pMenuCursorSprite;
	//���j���[�\���t���O
	bool m_isMenuDisplay;

	//���j���[�̃J�[�\���̏ꏊ
	enum class eMENU_CURSOR
	{
		NONE = -1,

		MANUAL,			//�������
		CHARA_SELECT,	//�L�����N�^�[�Z���N�g
		EXIT,			//�Q�[���I��

		OVER_ID
	};
	int m_menuCursor;
	//�J�[�\���̍��W
	static const DirectX::SimpleMath::Vector2 MENU_CURSOR_POS[static_cast<int>(eMENU_CURSOR::OVER_ID)];
	//����
	float m_totalSeconds;
	//���U���g�ɑJ�ڂ����t���O
	bool m_isResult;
	//���U���g��pushSpace�̉摜
	std::unique_ptr<Sprite2D> m_pPushSpaceResult;
	//���U���g��pushSpace�̉摜�̍��W
	static const DirectX::SimpleMath::Vector2 PUSH_SPACE_RESULT_POS;
};

