//=============================================================================
//
// �e�L�X�g�̏��� [texture.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "result.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CTexture::m_pTexture[MAX_TEXTURE]	= {};
int					CTexture::m_nCntTutorial			= NULL;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CTexture::CTexture() : CScene2D(4, CScene::OBJTYPE_2DPOLYGON)//�D��D�ʃC�j�V�����C�Y
{
	m_nType			= 0;
	m_size			= D3DXVECTOR2(0.0f, 0.0f);
	m_sizeDest		= D3DXVECTOR2(0.0f, 0.0f);
	m_fLength		= 0.0f;
	m_fAngle		= 0.0f;
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCounterAnim	= 0;
	m_nPatternAnim	= 0;
	m_nCntflame		= 0;
	m_nCntColor		= 0;
	m_nBlink		= false;
	m_EnemyStatse	= CEnemy::ENEMYSTATSE_NONE;
}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CTexture::~CTexture()
{
}

//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CTexture::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//// �e�N�X�`���̐���
	// �^�C�g��
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TITLE_0, &m_pTexture[0]);		// �^�C�g��(��)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TITLE_1, &m_pTexture[1]);		// �^�C�g��(�E)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TITLE_2, &m_pTexture[2]);		// PRESS�w�i
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TITLE_3, &m_pTexture[3]);		// PRESS

	// UI
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_0, &m_pTexture[4]);			// �Q�[�W(�g)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_1, &m_pTexture[5]);			// HP�}�[�N
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_2, &m_pTexture[6]);			// HP�Q�[�W
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_3, &m_pTexture[7]);			// �댯�x(�w�i)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_4, &m_pTexture[8]);			// �댯�x(�}�[�N)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_5, &m_pTexture[9]);			// �c��N���X�^����
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_6, &m_pTexture[10]);			// �~
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_7, &m_pTexture[11]);			// �����Ԃ�
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_8, &m_pTexture[12]);			// ���e�B�N��

	// �`���[�g���A��
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_0, &m_pTexture[13]);	// �\���w�i
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_1, &m_pTexture[14]);	// �J��������
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_2, &m_pTexture[15]);	// �ړ�����
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_3, &m_pTexture[16]);	// �W�����v����
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_4, &m_pTexture[17]);	// �U������
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_5, &m_pTexture[18]);	// ���C�t����
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_6, &m_pTexture[19]);	// �댯�x����
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_7, &m_pTexture[20]);	// �I������
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_8, &m_pTexture[21]);	// �m�F���(�w�i)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_9, &m_pTexture[22]);	// �m�F���(YES)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_10, &m_pTexture[23]);	// �m�F���(NO)

	// ���U���g
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_0, &m_pTexture[24]);		// GAMECLEAR
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_1, &m_pTexture[25]);		// GAMEOVER
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_2, &m_pTexture[26]);		// ���j���[�w�i
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_3, &m_pTexture[27]);		// RETRY
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_4, &m_pTexture[28]);		// QUIT

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CTexture::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
	{
		//�e�N�X�`���[�̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//===============================================================================
//�@��������
//===============================================================================
CTexture * CTexture::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType)
{
	CTexture *pTitleTEX = NULL;

	// NULL�`�F�b�N
	if (pTitleTEX == NULL)
	{// �������̓��I�m��

		pTitleTEX = new CTexture;

		if (pTitleTEX != NULL)
		{
			// ��ނ����蓖�Ă�
			pTitleTEX->m_nType = nType;
			// �I�u�W�F�N�g�N���X�̐���
			pTitleTEX->Init();
			// �e�N�X�`�������蓖�Ă�
			pTitleTEX->BindTexture(m_pTexture[nType]);
			// �ʒu�����蓖�Ă�
			pTitleTEX->SetPosSize(pos, size);
			// �T�C�Y�̊��蓖��
			pTitleTEX->m_size = size;
			pTitleTEX->m_sizeDest = size;
			if (nType == 13 )
			{// �\���w�i��������
				D3DXCOLOR col[4];
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				pTitleTEX->SetColor(&col[0]);
			}
			else if (nType >= 14 && nType <= 23)
			{
				D3DXCOLOR col[4];
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pTitleTEX->SetColor(&col[0]);
			}
		}
	}

	return pTitleTEX;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTexture::Init(void)
{
	srand((unsigned int)time(0));		// �����_��
	CScene2D::Init();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTexture::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTexture::Update(void)
{
	CManager::MODE m_mode = CManager::GetMode();
	switch (m_mode)
	{// �J�����̃^�C�v�ʏ���
	case CManager::MODE_TITLE:
		TitleMove();
		break;
	case CManager::MODE_TUTORIAL:
		TutorialMove();
		break;
	case CManager::MODE_GAME:
		GameMove();
		break;
	case CManager::MODE_RESULT:
		ResultMove();
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTexture::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �^�C�g���̓���
//=============================================================================
void CTexture::TitleMove(void)
{
	CFade::FADE		fade		= CFade::GetFade();		// �t�F�[�h�̎擾
	CTitle::TYPE	pTitleType	= CTitle::GetType();	// �^�C�g���̃^�C�v�̎擾
	D3DXVECTOR3		pos			= GetPosition();		// �ʒu�̎擾
	D3DXCOLOR	col[4];

	if (m_nType == 0 || m_nType == 1)
	{// �^�C�g�����S
		if (pTitleType == CTitle::TYPE_PRESS)
		{
			// �ړ� ------------------------------
			if (m_nType == 0)
			{
				pos.x += -7.5f;		// �ړ���(X)
				pos.y += 4.4f;		// �ړ���(Y)
			}
			else if (m_nType == 1)
			{
				pos.x += 7.5f;		// �ړ���(X)
				pos.y += -4.4f;		// �ړ���(Y)
			}
			// ���� ------------------------------
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.05f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.05f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.05f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.05f));
			if (col[0].a <= 0.0f)
			{
				if (fade != CFade::FADE_OUT)
				{
					// ���[�h�ݒ�
					CFade::SetFade(CManager::MODE_TUTORIAL);
				}
			}

			CScene2D::SetPosSize(pos, m_size);
			CScene2D::SetColor(&col[0]);
		}
	}
	else if (m_nType == 3)
	{//	PRESS
		if (pTitleType == CTitle::TYPE_NONE)
		{
			if (m_nBlink == false)
			{//	�J���[�𔖂�����
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nBlink = true;
				}
			}
			else if (m_nBlink == true)
			{//	�J���[��Z������
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
				if (col[0].a >= 1.0f)
				{
					m_nCntColor = 0;
					m_nBlink = false;
				}
			}
		}
		else if (pTitleType == CTitle::TYPE_PRESS)
		{
			if (m_nBlink == false)
			{//	�J���[�𔖂�����
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.1f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.1f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.1f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.1f));
				if (col[0].a <= 0.25f)
				{
					m_nCntColor = 0;
					m_nBlink = true;
				}
			}
			else if (m_nBlink == true)
			{//	�J���[��Z������
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.2f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.2f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.2f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.2f));
				if (col[0].a >= 1.0f)
				{
					m_nCntColor = 0;
					m_nBlink = false;
				}
			}
		}
		CScene2D::SetColor(&col[0]);
	}
	m_nCntflame++;		// �J�E���^�[�̍X�V
}

//=============================================================================
// �`���[�g���A���̓���
//=============================================================================
void CTexture::TutorialMove(void)
{
	CFade::FADE fade		= CFade::GetFade();						// �t�F�[�h�̎擾
	D3DXVECTOR3 pos			= GetPosition();						// �ʒu�̎擾
	int			PlayerLife	= CGame::GetPlayer()->GetLife();		// �v���C���[���C�t�̎擾
	D3DXCOLOR	col[4];

	if (m_nType == 6)
	{// HP�Q�[�W
		CScene2D::SetInitGauge(pos, D3DXVECTOR2((GAUGE_SIZE / PLAYER_LIFE)* PlayerLife, m_size.y));
	}
	else if (m_nType == 8)
	{//	�댯�x
		int nPatternAnim = 0;

		for (int nCntPriotity = 0; nCntPriotity < NUM_PRIORITY; nCntPriotity++)
		{
			CScene *pScene;
			// �v���C�I���e�B�[�`�F�b�N
			pScene = CScene::GetTop(nCntPriotity);
			while (pScene != NULL)
			{// NULL�`�F�b�N
			 // Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
				CScene *pSceneNext = pScene->GetNext();
				if (pScene->GetDeath() == false)
				{
					if (pScene->GetObjType() == OBJTYPE_ENEMY)
					{// �G�l�~�[��������
						// �I�u�W�F�N�g�̓����蔻��
						CEnemy::EnemyStatse statse = ((CEnemy*)pScene)->GetStatse();
						switch (statse)
						{// �J�����̃^�C�v�ʏ���
						case CEnemy::ENEMYSTATSE_NONE:		// �ʏ�
							if (nPatternAnim != 1 && nPatternAnim != 2)
							{// �x�����ǐՂ���Ȃ��Ƃ������ʂ�
								nPatternAnim = 0;
							}
							break;
						case CEnemy::ENEMYSTATSE_VIGILANCE:	// �x��
							if (nPatternAnim != 2)
							{// �ǐՂ���Ȃ��Ƃ������ʂ�
								nPatternAnim = 1;
							}
							break;
						case CEnemy::ENEMYSTATSE_PURSUIT:	// �ǐ�
							// �ǐՂ�������K���ʂ�
							nPatternAnim = 2;
							break;
						}
					}
				}
				// Next�Ɏ���Scene������
				pScene = pSceneNext;
			}
		}
		CScene2D::SetTexture(nPatternAnim, 3, 1);
	}

	if (m_nType == 14 && m_nCntTutorial == 0)
	{// �J��������
		if (m_nBlink == false)
		{//	�J���[��Z������
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
		}
		else if (m_nBlink == true)
		{//	�J���[�𔖂�����
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
			}
		}
	}
	else if (m_nType == 15 && m_nCntTutorial == 1)
	{// �ړ�����
		if (m_nBlink == false)
		{//	�J���[��Z������
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
		}
		else if (m_nBlink == true)
		{//	�J���[�𔖂�����
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
			}
		}
	}
	else if (m_nType == 16 && m_nCntTutorial == 2)
	{// �W�����v����
		if (m_nBlink == false)
		{//	�J���[��Z������
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
		}
		else if (m_nBlink == true)
		{//	�J���[�𔖂�����
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
			}
		}
	}
	else if (m_nType == 17 && m_nCntTutorial == 3)
	{// �U������
		if (m_nBlink == false)
		{//	�J���[��Z������
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
		}
		else if (m_nBlink == true)
		{//	�J���[�𔖂�����
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
			}
		}
	}
	else if (m_nType == 18 && m_nCntTutorial == 4)
	{// ���C�t����
		if (m_nBlink == false)
		{//	�J���[��Z������
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
		}
		else if (m_nBlink == true)
		{//	�J���[�𔖂�����
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
			}
		}
	}
	else if (m_nType == 19 && m_nCntTutorial == 5)
	{// �댯�x����
		if (m_nBlink == false)
		{//	�J���[��Z������
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
		}
		else if (m_nBlink == true)
		{//	�J���[�𔖂�����
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
			}
		}
	}
	else if (m_nType == 20 && m_nCntTutorial == 6)
	{// �I������
		if (m_nBlink == false)
		{//	�J���[��Z������
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nCntTutorial++;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// �J���[�̍X�V
		}
	}
}

//=============================================================================
// �Q�[���̓���
//=============================================================================
void CTexture::GameMove(void)
{
	D3DXVECTOR3 pos = GetPosition();					// �v���C���[���W�̎擾
	int PlayerLife	= CGame::GetPlayer()->GetLife();	// �v���C���[���C�t�̎擾

	// �G�̏�Ԃ��擾
	for (int nCntPriotity = 0; nCntPriotity < NUM_PRIORITY; nCntPriotity++)
	{
		CScene *pScene;
		// �v���C�I���e�B�[�`�F�b�N
		pScene = CScene::GetTop(nCntPriotity);

		// NULL�`�F�b�N
		while (pScene != NULL)
		{
			// Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetDeath() == false)
			{
				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{
					if (m_EnemyStatse != ((CEnemy*)pScene)->GetStatse())
					{// �X�e�[�^�X���Ⴄ�Ƃ�
						m_EnemyStatse = ((CEnemy*)pScene)->GetStatse();
						if (((CEnemy*)pScene)->GetStatse() != CEnemy::ENEMYSTATSE_NONE)
						{// �X�e�[�^�X���ʏ킶��Ȃ��Ƃ�
							m_EnemyStatse = ((CEnemy*)pScene)->GetStatse();
							break;
						}
					}
				}
			}
			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}

	if (m_nType == 6)
	{// HP�Q�[�W
		CScene2D::SetInitGauge(pos, D3DXVECTOR2((GAUGE_SIZE / PLAYER_LIFE)* PlayerLife, m_size.y));
	}
	else if (m_nType == 8)
	{//	�댯�x
		int nPatternAnim = 0;

		for (int nCntPriotity = 0; nCntPriotity < NUM_PRIORITY; nCntPriotity++)
		{
			CScene *pScene;
			// �v���C�I���e�B�[�`�F�b�N
			pScene = CScene::GetTop(nCntPriotity);
			while (pScene != NULL)
			{// NULL�`�F�b�N
				// Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
				CScene *pSceneNext = pScene->GetNext();
				if (pScene->GetDeath() == false)
				{
					if (pScene->GetObjType() == OBJTYPE_ENEMY)
					{
						// �I�u�W�F�N�g�̂����锻��
						CEnemy::EnemyStatse statse = ((CEnemy*)pScene)->GetStatse();
						switch (statse)
						{// �J�����̃^�C�v�ʏ���
						case CEnemy::ENEMYSTATSE_NONE:		// �ʏ�
							if (nPatternAnim != 1 && nPatternAnim != 2)
							{// �x�����ǐՂ���Ȃ��Ƃ������ʂ�
								nPatternAnim = 0;
							}
							break;
						case CEnemy::ENEMYSTATSE_VIGILANCE:	// �x��
							if (nPatternAnim != 2)
							{// �ǐՂ���Ȃ��Ƃ������ʂ�
								nPatternAnim = 1;
							}
							break;
						case CEnemy::ENEMYSTATSE_PURSUIT:	// �ǐ�
							// �ǐՂ�������K���ʂ�
							nPatternAnim = 2;
							break;
						}
					}
				}
				// Next�Ɏ���Scene������
				pScene = pSceneNext;
			}
		}
		CScene2D::SetTexture(nPatternAnim, 3, 1);
	}

	if (m_nType == 11)
	{// �����Ԃ�
		CPlayer::PlayerStageStatus PlayerState = CGame::GetPlayer()->GetStatus();		// �v���C���[�̏�Ԃ��擾
		D3DXCOLOR col[4];

		switch (PlayerState)
		{// �J�����̃^�C�v�ʏ���
		case CPlayer::PLAYERSTAGESTATUS_NONE:
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			break;
		case CPlayer::PLAYERSTAGESTATUS_DAMAGE:
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.02f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.02f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.02f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.02f));
			if (col[0].a <= 0.0f)
			{
				m_nCntColor = 0;
				CGame::GetPlayer()->SetStatus(CPlayer::PLAYERSTAGESTATUS_NONE);
			}
			break;
		}
		CScene2D::SetColor(&col[0]);
	}
	m_nCounterAnim++;
	m_nCntflame++;		// �J�E���^�[�̍X�V
}

//=============================================================================
// ���U���g�̓���
//=============================================================================
void CTexture::ResultMove(void)
{
	int nSelect = CResult::GetSelect();		// �I��ԍ��̎擾
	D3DXCOLOR col[4];

	if (m_nType == 27)
	{// RETRY
		if (nSelect == 0)
		{
			if (m_nBlink == false)
			{//	�J���[�𔖂�����
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.5f)
				{
					m_nCntColor = 0;
					m_nBlink = true;
				}
			}
			else if (m_nBlink == true)
			{//	�J���[��Z������
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				if (col[0].a >= 1.0f)
				{
					m_nCntColor = 0;
					m_nBlink = false;
				}
			}
		}
		else
		{
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		CScene2D::SetColor(&col[0]);
	}
	else if (m_nType == 28)
	{// QUIT
		if (nSelect == 1)
		{
			if (m_nBlink == false)
			{//	�J���[�𔖂�����
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.5f)
				{
					m_nCntColor = 0;
					m_nBlink = true;
				}
			}
			else if (m_nBlink == true)
			{//	�J���[��Z������
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				if (col[0].a >= 1.0f)
				{
					m_nCntColor = 0;
					m_nBlink = false;
				}
			}
		}
		else
		{
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		CScene2D::SetColor(&col[0]);
	}
}