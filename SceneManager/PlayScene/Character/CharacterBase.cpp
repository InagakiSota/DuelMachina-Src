#include "pch.h"
#include "CharacterBase.h"

#include "../../PlayerID.h"
#include "FbxModel.h"

//�d��
const float CharacterBase::GRAVITY = -0.007;
//�ړ�����
const float CharacterBase::MOVE_LIMIT_X = 3.0f;


CharacterBase::~CharacterBase()
{
	//���\�[�X�̉��
	if (m_pBodyBox != nullptr)
	{
		m_pBodyBox.reset();
	}

	if (m_pLegBox != nullptr)
	{
		m_pLegBox.reset();
	}

	if (m_pDeviceResources != nullptr)
	{
		m_pDeviceResources = nullptr;
	}

	if (m_pKeyTracker != nullptr)
	{
		m_pKeyTracker.reset();
	}

	if (m_pModel != nullptr)
	{
		m_pModel.reset();
	}

}
