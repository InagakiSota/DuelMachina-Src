//////////////////////////
//�v���C�V�[����UI�N���X
//�쐬���F2021�N4��30��
//�쐬�ҁFGT2_04_��_�D��
/////////////////////////

#pragma once

class PlaySceneUI
{
public:
	//�R���X�g���N�^
	PlaySceneUI();
	//�f�X�g���N�^
	~PlaySceneUI();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Render();
	//�I������
	void Finalize();

private:
	//�v���C���[�̐�
	static const int PLAYER_NUM = 2;


};
