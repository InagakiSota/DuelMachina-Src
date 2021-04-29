///////////////////////////////////
//�L�����N�^�[2�̏�ԊǗ��N���X
//�쐬�ҁFGS305_��_
//�쐬���F2020�N10��17��
///////////////////////////////////

#pragma once

#include "../../CharacterStateManagerBase.h"
#include "../../CharacterState.h"

class CharacterBase;
class CharacterStateBase;

class Character3StateManager : public CharacterStateManagerBase
{
public:
	//�R���X�g���N�^
	Character3StateManager();
	//�f�X�g���N�^
	~Character3StateManager();

	//������
	void Initialize(CharacterBase* pCharacterBase) override;

	//�X�V
	void Update(const eCHARACTER_STATE& charaState) override;

	//�I������
	void Finalize() override;

};