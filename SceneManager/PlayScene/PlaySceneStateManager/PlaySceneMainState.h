///////////////////////////////////
//�v���C�V�[���̃��C���X�e�[�g
//�쐬�ҁF2021�N5��10��
//�쐬���FGT2_04_��_�D��
///////////////////////////////////

#pragma once
#include "../PlayScene.h"
class PlaySceneMainState :public PlayScene
{	
public:
	//�R���X�g���N�^
	PlaySceneMainState();
	//�f�X�g���N�^
	~PlaySceneMainState();

	//�X�V
	void Update(DX::StepTimer const& timer);

	//�v���C�V�[���̃|�C���^�擾
	void SetPlayScene(PlayScene* pPlayScene)
	{
		m_pPlayScene = pPlayScene;
	}


private:
	//�������Ԃ̍ő�l
	static const float TIME_MAX;
	//�^�C���A�b�v�V�[���̎���
	static const float TIME_UP_TIME;
	//���E���h�؂�ւ��̎���
	static const float ROUND_CHANGE_TIME;
	//�J�E���g�_�E���̎���
	static const float COUNT_DOWN_TIME;


	//�v���C�V�[���̃|�C���^
	PlayScene* m_pPlayScene;
};

