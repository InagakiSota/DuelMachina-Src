///////////////////////////////////
//�^�C�g���V�[���̃N���X
//�쐬�ҁFGS3_05_��_�D��
//�쐬���F2020�N6��19��
///////////////////////////////////

#pragma once
#include "../SceneBase.h"
#include "../../FrameWork/ModelObject/ModelObject.h"

class Sprite2D;

class TitleScene :
	public SceneBase
{
public:
	TitleScene();
	~TitleScene();

	void Initialize() override;		//������
	void Update(DX::StepTimer const& timer)override;			//�X�V
	void Render()override;			//�`��
	void Finalize()override;		//�I������

private:

	//���S�̉摜
	std::unique_ptr<Sprite2D> m_pLogoSprite;
	//pushSpace�̉摜
	std::unique_ptr<Sprite2D> m_pSpaceSprite;

	//pushSpace�̉摜�̍��W
	static const DirectX::SimpleMath::Vector2 PUSH_SPACE_POS;
	//pushSpace�̉摜�̓_�Ń^�C�}�[
	float m_pushSpaceBlindTimer;
	//pushSpace�̉摜�̓_�Ńt���O
	bool m_pushSpaceBlindFlag;
	//pushSpace�̉摜�̓_�ŃC���^�[�o��
	static const float PUSH_SPACE_BLIND_INTERBVAL;

	//�V���̃��f��
	std::unique_ptr<ModelObject> m_pSkyBox;
	//�V���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_skyBoxWorld;
	
	//�V����Y���̊p�x
	float m_skyBoxAngleY;
	//�V���̍��W
	static const DirectX::SimpleMath::Vector3 SKYBOX_POS;

};

