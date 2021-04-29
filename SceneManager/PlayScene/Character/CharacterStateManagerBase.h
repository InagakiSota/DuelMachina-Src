///////////////////////////////////
//�L�����N�^�[�̃X�e�[�g�Ǘ��N���X�̊��
//�쐬�ҁFGS3_05_��_
//�쐬���F2020�N10��17��
///////////////////////////////////

#pragma once
#include "CharacterState.h"
#include "CharacterStateBase.h"

class CharacterBase;

class CharacterStateManagerBase
{
public:
	~CharacterStateManagerBase() {};

	//������
	virtual void Initialize(CharacterBase* pCharacterBase) = 0;
	//�X�V
	virtual void Update(const eCHARACTER_STATE& charaState) = 0;
	//�I������
	virtual void Finalize() = 0;

protected:
	//�L�����N�^�[�̏�Ԃ̃|�C���^�̔z��
	std::unique_ptr<CharacterStateBase> m_pCharacterStateArray[static_cast<int>(eCHARACTER_STATE::MAX_NUM)];

};
