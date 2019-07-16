//=============================================================================
//
// �`���[�g���A�����̌x������ [warning.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "warning.h"
#include "scene2D.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "tutorial.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9				CWarning::m_pTexture[WARNING_MAX_TEXTURE]	= {};
LPDIRECT3DVERTEXBUFFER9			CWarning::m_pVtxBuff						= NULL;
int								CWarning::m_SelectNum						= NULL;		// �I��ԍ�
float							CWarning::m_SelectColor						= NULL;		// �I��F

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CWarning::CWarning()
{
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SelectNum		= NULL;
	m_SelectColor	= 0.0f;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CWarning::~CWarning()
{
}

//=============================================================================
//�@�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CWarning::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, WARNING_TEXTURENAME00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, WARNING_TEXTURENAME01, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, WARNING_TEXTURENAME02, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, WARNING_TEXTURENAME03, &m_pTexture[3]);

	return S_OK;
}

//=============================================================================
//�@�A�����[�h
//=============================================================================
void CWarning::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < WARNING_MAX_TEXTURE; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWarning::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer			*pRenderer	= CManager::GetRenderer();		// �����_���[�̎擾
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();		// �f�o�C�X�̎擾

	m_SelectNum		= 0;						// �I��ԍ�
	m_SelectColor	= 0.0f;						// �I���J���[

	int nCntWarning;							// �|�[�Y�J�E���^
	float fPosWarning = -170;					// �|�[�Y�ʒu

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * WARNING_MAX_POLIGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	Load();

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntWarning = 0; nCntWarning < WARNING_MAX_POLIGON; nCntWarning++)
	{// �|�[�Y���J�E���g
		if (nCntWarning == 0)
		{// BG
		 // ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		}
		else if (nCntWarning == 1)
		{// �w�i
		 // ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - 300, (SCREEN_HEIGHT / 2) - 250, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + 300, (SCREEN_HEIGHT / 2) - 250, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - 300, (SCREEN_HEIGHT / 2) + 250, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + 300, (SCREEN_HEIGHT / 2) + 250, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (nCntWarning == 2)
		{// YES
		 // ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3((500) - 100, 400, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((500) + 100, 400, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((500) - 100, 400 + 100, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((500) + 100, 400 + 100, 0.0f);

			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			fPosWarning += 100;
		}
		else if (nCntWarning == 3)
		{// NO
		 // ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3((800) - 100, 400, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((800) + 100, 400, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((800) - 100, 400 + 100, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((800) + 100, 400 + 100, 0.0f);

			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			fPosWarning += 100;
		}

		// 1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWarning::Uninit(void)
{
	Unload();

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CWarning::Update(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	CFade::FADE		fade	= CFade::GetFade();					// �t�F�[�h�̎擾
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// ���͏��̎擾
	CInputMouse		*pMouse = CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)
	CSound			*pSound = CManager::GetSound();				// �T�E���h�̎擾

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);
	if (pKey->GetKeyboardTrigger(DIK_UP) == true || pKey->GetKeyboardTrigger(DIK_W) == true)
	{// �I��؂�ւ�(��)
		// ���_�J���[�ݒ�
		pVtx[m_SelectNum * 4 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		m_SelectNum		= (m_SelectNum + 1) % 2;
		m_SelectColor	= 1.0f;

		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SWITCH);
	}
	if (pKey->GetKeyboardTrigger(DIK_DOWN) == true || pKey->GetKeyboardTrigger(DIK_S) == true)
	{// �I��؂�ւ�(��)
		// ���_�J���[�ݒ�
		pVtx[m_SelectNum * 4 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		m_SelectNum		= (m_SelectNum + 1) % 2;
		m_SelectColor	= 1.0f;

		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SWITCH);
	}

	m_SelectColor -= 0.004f;

	if (m_SelectColor < 0.65f)
	{
		m_SelectColor = 1.0f;
	}

	// ���_�J���[�ݒ�
	pVtx[m_SelectNum * 4 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor);
	pVtx[m_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor);
	pVtx[m_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor);
	pVtx[m_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	if (pKey->GetKeyboardTrigger(DIK_RETURN) == true || pMouse->GetMouseTrigger(0) == true)
	{// ����
		CTutorial *pGame	= CManager::GetTutorial();
		CTutorial *pWarning = ((CTutorial*)pGame);
		bool bWarning		= pWarning->GetWarning();

		if (m_SelectNum == 0)
		{
			if (fade != CFade::FADE_OUT)
			{
				CFade::SetFade(CManager::MODE_GAME);
				pWarning->SetWarning(false);

				// ���_�J���[�ݒ�
				pVtx[m_SelectNum * 4 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[m_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[m_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[m_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

				m_SelectNum = 0;

				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_DECISION);
			}
		}
		else if (m_SelectNum == 1)
		{
			pWarning->SetWarning(false);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPENCLOSE);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWarning::Draw(void)
{
	int nCntWarning;			// �|�[�Y�J�E���^

	// �f�o�C�X�̎擾
	CRenderer			*pRenderer	= CManager::GetRenderer();
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWarning = 0; nCntWarning < WARNING_MAX_POLIGON; nCntWarning++)
	{// �|�[�Y���J�E���g
	 // �e�N�X�`���ݒ�
		pDevice->SetTexture(0, m_pTexture[nCntWarning]);

		// �|�[�Y�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWarning * 4, 2);
	}
}