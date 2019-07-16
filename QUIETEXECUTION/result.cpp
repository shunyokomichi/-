//=============================================================================
//
// ���U���g���� [result.h]
// Author :		�r�J�R�N
//
//=============================================================================
#include "result.h"
#include "player.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "MeshField.h"
#include "wall.h"
#include "texture.h"
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
CPlayer			*CResult::m_pPlayer = NULL;
CWall			*CResult::m_pWall	= NULL;
CObject			*CResult::m_pObject = NULL;
CEnemy			*CResult::m_pEnemy	= NULL;
int				CResult::m_nSelect	= NULL;
//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	m_pPlayer = NULL;
	m_nSelect = 0;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//�@����������
//=============================================================================
HRESULT CResult::Init(void)
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

	CTexture::Load();

	//�ǂ̃e�N�X�`���̓ǂݍ���
	m_pWall->Load();

	//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	m_pObject->Load(m_text->GetMapText().nTypeNum[0]);

	//===================================
	// ����
	//===================================
	SetWall();		 // ��
	SetObjet(Map);	 // �I�u�W�F�N�g
	SetTex();		 // UI
	SetFloor(Map);	 // ��

	// �T�E���h�̐���
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//=============================================================================
//�@�I������
//=============================================================================
void CResult::Uninit(void)
{
	// �e�N�X�`���̔j��
	CTexture::UnLoad();

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
void CResult::Update(void)
{
	CFade::FADE		fade	= CFade::GetFade();					// �t�F�[�h�����擾
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// ���͏����擾
	CInputMouse		*pMouse = CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)
	CSound			*pSound = CManager::GetSound();				// �T�E���h�̎擾

	// �I������
	if (pKey->GetKeyboardTrigger(DIK_UP) == true || pKey->GetKeyboardTrigger(DIK_W) == true)
	{// �I��؂�ւ�(��)
		m_nSelect = (m_nSelect + 1) % 2;
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SWITCH);
	}
	if (pKey->GetKeyboardTrigger(DIK_DOWN) == true || pKey->GetKeyboardTrigger(DIK_S) == true)
	{// �I��؂�ւ�(��)
		m_nSelect = (m_nSelect + 1) % 2;
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SWITCH);
	}

	if (pKey->GetKeyboardTrigger(DIK_RETURN) == true || pMouse->GetMouseTrigger(0) == true)
	{// ���莞
		if (m_nSelect == 0)
		{// RETRY�̎�
			if (fade != CFade::FADE_OUT)
			{
				CFade::SetFade(CManager::MODE_GAME);
			}
		}
		else if (m_nSelect == 1)
		{// QUIT�̎�
			if (fade != CFade::FADE_OUT)
			{
				CFade::SetFade(CManager::MODE_TITLE);
			}
		}
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_DECISION);
	}
	// �f�o�b�N���O
	CManager::m_pDebuglog->Print(1, "�I��ԍ� �` %d �`\n", m_nSelect);
}

//=============================================================================
//�@�`�揈��
//=============================================================================
void CResult::Draw(void)
{
}

//=============================================================================
//�@�e�N�X�`���̐���
//=============================================================================
void CResult::SetTex(void)
{
	CManager::GAMERESULT gameresult = CManager::GetResult();		// ���U���g�̎擾
	switch (gameresult)
	{
	case CManager::GAMERESULT_WIN:
		CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 150.0f, 0.0f), D3DXVECTOR2(350.0f, 130.0f), 24);	// GAME CREAR
		break;
	case CManager::GAMERESULT_LOSE:
		CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 180.0f, 0.0f), D3DXVECTOR2(350.0f, 150.0f), 25);	// GAME OVER
		break;
	}
	CTexture::Create(D3DXVECTOR3(1000.0f, 500.0f, 0.0f), D3DXVECTOR2(250.0f, 200.0f), 26);	// ���j���[�w�i
	CTexture::Create(D3DXVECTOR3(1000.0f, 425.0f, 0.0f), D3DXVECTOR2(150.0f, 50.0f), 27);	// RETRY
	CTexture::Create(D3DXVECTOR3(1000.0f, 575.0f, 0.0f), D3DXVECTOR2(150.0f, 50.0f), 28);	// QUIT
}

//=============================================================================
//�@���̐���
//=============================================================================
void CResult::SetFloor(CLoadText::MAPTEXT Map)
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
void CResult::SetWall(void)
{
}

//=============================================================================
//�@�I�u�W�F�N�g�̐���
//=============================================================================
void CResult::SetObjet(CLoadText::MAPTEXT Map)
{// CObject::Create( ���W , �ړ��� , �g�p����摜�̔ԍ� , ��������I�u�W�F�N�g�̔ԍ� , �I�u�W�F�N�g�̃^�C�v , �����蔻��̗L�� );
	CManager::GAMERESULT gameresult = CManager::GetResult();
	switch (gameresult)
	{
	case CManager::GAMERESULT_WIN:
		if (m_pEnemy == NULL)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		break;
	case CManager::GAMERESULT_LOSE:
		if (m_pPlayer == NULL)
		{
			m_pPlayer->Create(D3DXVECTOR3(-50.0f, 50.0f, 0.0f));
		}
		break;
	}

	for (int nCntModel = 0; nCntModel < Map.nNum[m_nCntType]; nCntModel++)
	{
		//	// �N���X�^��
		CObject::Create(Map.pos[m_nCntType][nCntModel], 0.0f, Map.nType[m_nCntType][nCntModel], Map.nType[m_nCntType][nCntModel], CObject::MOVETYPE_NOT, 0);
	}

	m_nCntType++;
}