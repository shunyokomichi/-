//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "fade.h"		// ��"texture.h"�Ŏg���Ă���̂ŏ����Ȃ�
#include "title.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "texture.h"
#include "MeshField.h"
#include "object.h"
#include "wall.h"
#include "object.h"
#include "loadtext.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FILENAME	( "data/TEXT/Map.txt" )		// �t�@�C����
#define WORD_NUM		(50)						// ������
//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CWall			*CTitle::m_pWall	= NULL;
CObject			*CTitle::m_pObject	= NULL;
CTitle::TYPE	CTitle::m_pType		= TYPE_NONE;

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	m_pType = TYPE_NONE;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//�@����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	m_nCntType = 0;
	CLoadText::MAPTEXT Map;
	//===================================
	// ���[�h
	//===================================
	// �e�L�X�g�ǂݍ���
	char *cFileName[WORD_NUM] = { MAX_FILENAME };
	if (m_text == NULL)
	{
		m_text = new CLoadText;
		m_text->TextRoad(cFileName[0]);
		Map = m_text->GetMapText();
	}

	// UI�̓ǂݍ���
	CTexture::Load();

	// �ǂ̃e�N�X�`���̓ǂݍ���
	m_pWall->Load();

	//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	m_pObject->Load(Map.nTypeNum[m_nCntType]);

	//===================================
	// ����
	//===================================
	SetWall();		 // ��
	SetObjet(Map);		 // �I�u�W�F�N�g
	SetTex();		 // UI
	SetFloor(Map);		 // ��

	// �T�E���h�̐���
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//=============================================================================
//�@�I������
//=============================================================================
void CTitle::Uninit(void)
{
	CTexture::UnLoad();		// �e�N�X�`��

	if (m_text != NULL)
	{
		m_text->Reset();
		m_text = NULL;
	}

	// �t�F�[�h�ȊO�폜
	CScene::NotFadeReleseAll();

	// �T�E���h�̍폜
	CSound *pSound = CManager::GetSound();
	if (pSound != NULL)
	{
		pSound->AllStopSound();
	}
}

//=============================================================================
//�@�X�V����
//=============================================================================
void CTitle::Update(void)
{
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// ���͏��̎擾
	CInputMouse		*pMouse = CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)

	if (pKey != NULL)
	{
		if (pKey->GetKeyboardPress(DIK_RETURN) == true || pMouse->GetMouseTrigger(0) == true)
		{// �}�E�X�N���b�N�����Ƃ�
			m_pType = TYPE_PRESS;	// PRESS��Ԃɂ���
			CSound *pSound = CManager::GetSound();
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_DECISION);
		}
	}
}

//=============================================================================
//�@�`�揈��
//=============================================================================
void CTitle::Draw(void)
{
}

//=============================================================================
//�@�e�N�X�`���̐���
//=============================================================================
void CTitle::SetTex(void)
{
	CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 180.0f, 0.0f), D3DXVECTOR2(350.0f, 150.0f), 0);	// �^�C�g�����S(��)
	CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 180.0f, 0.0f), D3DXVECTOR2(350.0f, 150.0f), 1);	// �^�C�g�����S(�E)
	CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 650.0f, 0.0f), D3DXVECTOR2(170.0f, 40.0f), 2);		// PRESS�w�i
	CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 650.0f, 0.0f), D3DXVECTOR2(150.0f, 20.0f), 3);		// PRESS
}

//=============================================================================
//�@���̐���
//=============================================================================
void CTitle::SetFloor(CLoadText::MAPTEXT Map)
{// CMeshField::Create(���W�A�T�C�Y�A������)
	for (int nCntFloor = 0; nCntFloor < Map.nNum[m_nCntType]; nCntFloor++)
	{
		CMeshField::Create(Map.pos[m_nCntType][m_nCntType], Map.size[m_nCntType].x, Map.size[m_nCntType].z, 100, 100, Map.nType[m_nCntType][m_nCntType]);	// Create(���W�A�T�C�Y�A������)
	}
	m_nCntType++;
}

//=============================================================================
//�@�ǂ̐���
//=============================================================================
void CTitle::SetWall(void)
{
}

//=============================================================================
//�@�I�u�W�F�N�g�̐���
//=============================================================================
void CTitle::SetObjet(CLoadText::MAPTEXT Map)
{// CObject::Create( ���W , �ړ��� , �g�p����摜�̔ԍ� , ��������I�u�W�F�N�g�̔ԍ� , �I�u�W�F�N�g�̃^�C�v , �����蔻��̗L�� );
	for (int nCntModel = 0; nCntModel < Map.nNum[m_nCntType]; nCntModel++)
	{
	//	// �N���X�^��
		CObject::Create(Map.pos[m_nCntType][nCntModel], 0.0f, Map.nType[m_nCntType][nCntModel], Map.nType[m_nCntType][nCntModel], CObject::MOVETYPE_NOT, 0);
	}

	m_nCntType++;
}