///////////////////////////////
//�v���C�V�[���̃��j���[�̃N���X
//�쐬���F2021�N5��9��
//�쐬�ҁFGT2_04_��_�D��
///////////////////////////////

#include "pch.h"
#include "PlaySceneMenu.h"

//�R���X�g���N�^
PlaySceneMenu::PlaySceneMenu()
{
	m_menuCursor = static_cast<int>(eMENU_CURSOR::MANUAL);

}

//�f�X�g���N�^
PlaySceneMenu::~PlaySceneMenu()
{
}

////////////////////////
//������
//�����F�Ȃ�
//�߂�l�F�Ȃ�
////////////////////////
void PlaySceneMenu::Initialize()
{
	//���j���[�̉摜�ǂݍ���
	m_pMenuSprite = std::make_unique<Sprite2D>();
	m_pMenuSprite->Create(L"Resources/Textures/Menu.png");
	//�J�[�\���̉摜�̓ǂݍ���
	m_pMenuCursorSprite = std::make_unique<Sprite2D>();
	m_pMenuCursorSprite->Create(L"Resources/Textures/menuCursol.png");


}

////////////////////////
//�X�V
//�����F�Ȃ�
//�߂�l�F�Ȃ�
////////////////////////
void PlaySceneMenu::Update()
{
}

////////////////////////
//�`��
//�����F�Ȃ�
//�߂�l�F�Ȃ�
////////////////////////
void PlaySceneMenu::Render()
{
	m_pMenuSprite->Draw(true);
	m_pMenuCursorSprite->Draw(true);

}

////////////////////////
//�I������
//�����F�Ȃ�
//�߂�l�F�Ȃ�
////////////////////////
void PlaySceneMenu::Finalize()
{
}
