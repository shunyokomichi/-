//=============================================================================
//
// �����_�����O���� [renderer.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "renderer.h"
#include "fade.h"
#include "scene.h"
#include "manager.h"
#include "debuglog.h"
#include "input.h"
#include "game.h"
#include "pause.h"
#include "tutorial.h"
#include "warning.h"

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	m_pD3D			= NULL;
	m_pD3DDevice	= NULL;
#ifdef _DEBUG
	m_pFont			= NULL;
#endif
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DDISPLAYMODE			d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));										// ���[�N���[���N���A
	d3dpp.BackBufferCount				= 1;								// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth				= SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;					// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat				= d3ddm.Format;						// �J���[���[�h�̎w��
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;			// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil		= TRUE;								// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;						// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed						= bWindow;							// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	//���͏��
	bool			pPause				= false;
	bool			pWarning			= false;
	CInputKeyBoard	*pCInputKeyBoard	= CManager::GetInputKeyboard();
	CGame			*pGame				= CManager::GetGame();
	CTutorial		*pTutorial			= CManager::GetTutorial();

	CManager::m_pDebuglog->Print(0,"FPS %d\n",GetFPS());
#ifdef _DEBUG
	if (pCInputKeyBoard != NULL)
	{
		if (pCInputKeyBoard->GetKeyboardPress(DIK_F1) == true)
		{//���̏�
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 2);			// ���C���[�t���[���w��
		}
		if (pCInputKeyBoard->GetKeyboardPress(DIK_F2) == true)
		{//���̏�
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 0);			// ���C���[�t���[���w��
		}
	}
#endif
	if (pGame != NULL)
	{
		pPause = pGame->GetPause();
	}
	else
	{
		pPause = false;
	}
	if (pTutorial != NULL)
	{
		pWarning = pTutorial->GetWarning();
	}
	else
	{
		pWarning = false;
	}

	if (pPause == false && pWarning == false)
	{
		// �|���S���̍X�V����
		CScene::UpdateAll();
	}
	else if(pPause == true)
	{
		CPause::Update();
	}
	else if (pWarning == true)
	{
		CWarning::Update();
	}

	CFade::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	bool pPause		= false;
	bool pWarning	= false;
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear
	(	0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(100, 100, 255, 255),
		1.0f,
		0
	);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CGame		*pGame		= CManager::GetGame();			// �Q�[���̎擾
		CTutorial	*pTutorial	= CManager::GetTutorial();		// �`���[�g���A���̎擾
		if (pGame != NULL)
		{
			pPause = pGame->GetPause();
		}
		else
		{
			pPause = false;
		}
		if (pTutorial != NULL)
		{
			pWarning = pTutorial->GetWarning();
		}
		else
		{
			pWarning = false;
		}

		// �|���S���̕`�揈��
		CScene::DrawAll();

		// �t�F�[�h�̕`�揈��
		CFade::Draw();

		if (pPause == true)
		{// �|�[�Y�̕`�揈��
			CPause::Draw();
		}
		else if (pWarning == true)
		{// �x���̕`�揈��
			CWarning::Draw();
		}
#ifdef _DEBUG
		// FPS�\��
		CManager::m_pDebuglog->Draw();
#endif

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}
	else
	{
		MessageBox(0, "Draw�̏�����NULL", "�x��", MB_OK);
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}