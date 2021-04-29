///////////////////////////////////
//�L�����N�^�[�̍U���̊��N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N11��13��
///////////////////////////////////

#pragma once

class CharacterBase;

class CharacterStateBase
{
public:
	~CharacterStateBase() {};
	//������
	virtual void Initialize(CharacterBase* pCharacter) = 0;
	//�X�V
	virtual void Update() = 0;

protected:
	//�L�����N�^�[�̃|�C���^
	CharacterBase* m_pCharacter;
};