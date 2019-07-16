//=============================================================================
//
// �}�l�[�W������ [manager.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "debuglog.h"
#include "pause.h"
#include "warning.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CManager::MODE		CManager::m_mode			= MODE_TITLE;			// �Q�[�����[�h
CManager::GAMERESULT CManager::m_GameResult		= GAMERESULT_WIN;		// ���U���g�^�C�v
CRenderer			*CManager::m_pRenderer		= NULL;					// �����_��
CFade				*CManager::m_pFade			= NULL;					// �t�F�[�h
CTitle				*CManager::m_pTitle			= NULL;					// �^�C�g��
CTutorial			*CManager::m_pTutorial		= NULL;					// �`���[�g���A��
CGame				*CManager::m_pGame			= NULL;					// �Q�[��
CResult				*CManager::m_pResult		= NULL;					// ���U���g
CInputKeyBoard		*CManager::m_pInputKeyBoard = NULL;					// �L�[�{�[�h
CInputJoypad		*CManager::m_pInputJoypad	= NULL;					// �W���C�p�b�g
CInputMouse			*CManager::m_pInputMouse	= NULL;					// �}�E�X
CCamera				*CManager::m_pCamera		= NULL;					// �J����
CLight				*CManager::m_pLight			= NULL;					// ���C�g
CSound				*CManager::m_pSound			= NULL;					// �T�E���h
CDebuglog			*CManager::m_pDebuglog		= NULL;					// �f�o�b�N���O
CPlayer				*CManager::m_pPlayer		= NULL;					// �v���C���[
CPause				*CManager::m_pPause			= NULL;					// �|�[�Y
CWarning			*CManager::m_pWarning		= NULL;					// �`���[�g���A�����̌x��

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	m_mode				= MODE_TITLE;		// �Q�[�����[�h
	m_GameResult		= GAMERESULT_WIN;	// ���U���g�^�C�v
	m_pRenderer			= NULL;				// �����_��
	m_pFade				= NULL;				// �t�F�[�h
	m_pTitle			= NULL;				// �^�C�g��
	m_pTutorial			= NULL;				// �Z���N�g
	m_pGame				= NULL;				// �Q�[��
	m_pResult			= NULL;				// ���U���g
	m_pInputKeyBoard	= NULL;				// �L�[�{�[�h
	m_pInputJoypad		= NULL;				// �W���C�p�b�g
	m_pInputMouse		= NULL;				// �}�E�X
	m_pSound			= NULL;				// �J����
	m_pCamera			= NULL;				// ���C�g
	m_pLight			= NULL;				// �T�E���h
	m_pDebuglog			= NULL;				// �f�o�b�N���O
	m_pPlayer			= NULL;				// �v���C���[
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//�@����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �����_�����O�N���X�̐���
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{
			// ����������
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
			{
				return -1;
			}
		}
		else
		{//�x����
			MessageBox(0, "�x���F���������Ȃ��ł�", "�x��", MB_OK);
		}
	}
	else
	{//�x����
		MessageBox(0, "�x���F�����������Ă܂�", "�x��", MB_OK);
	}

	if (m_pInputKeyBoard == NULL)
	{//�L�[�{�[�h����
		m_pInputKeyBoard = CInputKeyBoard::Create(hInstance, hWnd);
	}
	if (m_pInputJoypad == NULL)
	{//�W���C�p�b�h����
		m_pInputJoypad = CInputJoypad::Create(hInstance, hWnd);
	}
	if (m_pInputMouse == NULL)
	{//�}�E�X����
		m_pInputMouse = CInputMouse::Create(hInstance, hWnd);
	}

	if (m_pSound == NULL)
	{// �T�E���h
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{// ����������
			m_pSound->Init(hWnd);
		}
	}
	if (m_pCamera == NULL)
	{// �J����
		m_pCamera = new CCamera;
		if (m_pCamera != NULL)
		{// ����������
			m_pCamera = CCamera::Create();
		}
	}
	if (m_pLight == NULL)
	{// ���C�g
		m_pLight = new CLight;
		if (m_pLight != NULL)
		{// ����������
			m_pLight->Create();
		}
	}
	if (m_pDebuglog == NULL)
	{// �f�o�b�N���O
		m_pDebuglog->Init();	// ����������
	}

	if (m_pPause == NULL)
	{// �|�[�Y
		m_pPause = new CPause;
		if (m_pPause != NULL)
		{
			m_pPause->Init();
		}
	}

	if (m_pWarning == NULL)
	{// �`���[�g���A�����̌x��
		m_pWarning = new CWarning;
		if (m_pWarning != NULL)
		{
			m_pWarning->Init();
		}
	}

	SetMode(m_mode);			// �Q�[�����[�h�̐ݒ�

	if (m_pFade == NULL)
	{// �t�F�[�h
		m_pFade = new CFade;
		if (m_pFade != NULL)
		{// ����������
			m_pFade->Init();
		}
	}
	return S_OK;
}

//=============================================================================
//�@�I������
//=============================================================================
void CManager::Uninit(void)
{
	CScene::ReleseAll();
	// �j��
	if (m_pRenderer != NULL)
	{// �����_���[
		// �I������
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;

	}
	if (m_pFade != NULL)
	{// �t�F�[�h
		// �I������
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
	if (m_pInputKeyBoard != NULL)
	{// �L�[�{�[�h
		// �I������
		m_pInputKeyBoard->Uninit();
		delete m_pInputKeyBoard;
		m_pInputKeyBoard = NULL;
	}
	if (m_pInputJoypad != NULL)
	{// �W���C�p�b�g
		// �I������
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}
	if (m_pInputMouse != NULL)
	{// �}�E�X
		// �I������
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}
	if (m_pCamera != NULL)
	{// �J����
		// �I������
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	if (m_pLight != NULL)
	{// ���C�g
		// �I������
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	if (m_pPause != NULL)
	{// �|�[�Y
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}
	if (m_pWarning != NULL)
	{// �`���[�g���A�����̌x��
		m_pWarning->Uninit();
		delete m_pWarning;
		m_pWarning = NULL;
	}
	if (m_pSound != NULL)
	{// �T�E���h
		// �I������
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	if (m_pDebuglog != NULL)
	{// �f�o�b�N���O
		// �I������
		m_pDebuglog->Uninit();
		delete m_pDebuglog;
		m_pDebuglog = NULL;
	}
}

//=============================================================================
//�@�X�V����
//=============================================================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{// �����_���[
		// �X�V����
		m_pRenderer->Update();
	}
	if (m_pInputKeyBoard != NULL)
	{// �L�[�{�[�h
		// �X�V����
		m_pInputKeyBoard->Update();
	}
	if (m_pInputJoypad != NULL)
	{// �W���C�p�b�g
		// �X�V����
		m_pInputJoypad->Update();
	}
	if (m_pInputMouse != NULL)
	{// �}�E�X
		// �X�V����
		m_pInputMouse->Update();
	}
	if (m_pCamera != NULL)
	{// �J����
		// �X�V����
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ���C�g
		// �X�V����
		m_pLight->Update();
	}

	switch (m_mode)
	{// �Q�[�����[�h�̍X�V����
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// �^�C�g��
			m_pTitle->Update();
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �Z���N�g
			m_pTutorial->Update();
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{// �Q�[��
			m_pGame->Update();
		}
		break;
	case MODE_RESULT:
		if (m_pResult != NULL)
		{// ���U���g
			m_pResult->Update();
		}
		break;
	}
}

//=============================================================================
//�@�`�揈��
//=============================================================================
void CManager::Draw(void)
{
	//�`!!����ւ��֎~!!�`*******************************
	if (m_pCamera != NULL)
	{// �J����
		// �X�V����
		m_pCamera->Set();
	}
	if (m_pRenderer != NULL)
	{// �����_���[
		// �`�揈��
		m_pRenderer->Draw();
	}
	//***************************************************

	switch (m_mode)
	{// �Q�[�����[�h�̕`�揈��
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// �^�C�g��
			m_pTitle->Draw();
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �Z���N�g
			m_pTutorial->Draw();
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{// �Q�[��
			m_pGame->Draw();
		}
		break;
	case MODE_RESULT:
		if (m_pResult != NULL)
		{// ���U���g
			m_pResult->Draw();
		}
		break;
	}
}

//=============================================================================
// �f�o�C�X�̎擾(�����_�����O)
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================================================
// �f�o�C�X�̎擾(�T�E���h)
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// ���͏��̎擾(�L�[�{�[�h)
//=============================================================================
CInputKeyBoard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyBoard;
}
//=============================================================================
// ���͏��̎擾(�R���g���[���[)
//=============================================================================
CInputJoypad * CManager::GetInputJoyPad(void)
{
	return m_pInputJoypad;
}
//=============================================================================
// ���͏��̎擾(�}�E�X)
//=============================================================================
CInputMouse * CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}
//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{// �Q�[�����[�h�̏I������
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// �^�C�g��
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �Z���N�g
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{// �Q�[��
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
	case MODE_RESULT:
		if (m_pResult != NULL)
		{// ���U���g
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;
	}

	m_mode = mode;		// �Q�[�����[�h�̍X�V

	switch (mode)
	{// �Q�[�����[�h�̏���������
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{// �^�C�g��
			m_pTitle = new CTitle;
			if (m_pTitle != NULL)
			{// ����������
				m_pTitle->Init();
				// �J�����̐؂�ւ�
				if (m_pCamera != NULL)
				{
					m_pCamera->SetType(CCamera::CAMTYPE_TITLE);
				}
			}
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// �Z���N�g
			m_pTutorial = new CTutorial;
			if (m_pTutorial != NULL)
			{// ����������
				m_pTutorial->Init();
				// �J�����̐؂�ւ�
				if (m_pCamera != NULL)
				{
					m_pCamera->SetType(CCamera::CAMTYPE_FPS);
				}
			}
		}
		break;
	case MODE_GAME:
		if (m_pGame == NULL)
		{// �Q�[��
			m_pGame = new CGame;
			if (m_pGame != NULL)
			{// ����������
				m_pGame->Init();
				// �J�����̐؂�ւ�
				if (m_pCamera != NULL)
				{
					m_pCamera->SetType(CCamera::CAMTYPE_FPS);
				}
			}
		}
		break;
	case MODE_RESULT:
		if (m_pResult == NULL)
		{// ���U���g
			m_pResult = new CResult;
			if (m_pResult != NULL)
			{// ����������
				m_pResult->Init();
				// �J�����̐؂�ւ�
				if (m_pCamera != NULL)
				{
					m_pCamera->SetType(CCamera::CAMTYPE_RESULT);
				}
			}
		}
		break;
	}
}