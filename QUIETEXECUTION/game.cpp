//=============================================================================
//
// �Q�[������ [game.h]
// Author :		�r�J�R�N
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "MeshField.h"
#include "wall.h"
#include "object.h"
#include "bullet.h"
#include "particle.h"
#include "score.h"
#include "pause.h"
#include "loadtext.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FILENAME	( "data/TEXT/Map.txt" )		// �t�@�C����
#define WORD_NUM		(50)						// ������
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPlayer			*CGame::m_pPlayer	= NULL;
CWall			*CGame::m_pWall		= NULL;
CObject			*CGame::m_pObject	= NULL;
CEnemy			*CGame::m_pEnemy	= NULL;

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_pPlayer = NULL;
	m_bPause = false;
	m_text	= NULL;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//�@����������
//=============================================================================
HRESULT CGame::Init(void)
{
	m_nCntType = 0;
	CLoadText::MAPTEXT Map;
	//***********************************
	// ���[�h
	//***********************************
	// �e�L�X�g�ǂݍ���
	char *cFileName[WORD_NUM] = { MAX_FILENAME };
	if (m_text == NULL)
	{
		m_text = new CLoadText;
		m_text->TextRoad(cFileName[0]);
		Map = m_text->GetMapText();
	}
	// �X�R�A�̃��[�h
	CNumber::Load();
	// �e�N�X�`���̃��[�h
	CTexture::Load();
	// �p�[�e�B�N���̃e�N�X�`���̓ǂݍ���
	CParticle::Load();
	//�ǂ̃e�N�X�`���̓ǂݍ���
	m_pWall->Load();
	//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	m_pObject->Load(m_text->GetMapText().nTypeNum[0]);

	//***********************************
	// ����
	//***********************************
	SetWall();		 // ��
	SetObjet(Map);		 // �I�u�W�F�N�g
	SetTex();		 // UI
	SetFloor(Map);		 // ��

	CScore::AddScore(CLEAR_CRYSTALNUM);		// �X�R�A�̐ݒ�

	if (m_pPlayer == NULL)
	{// �v���C���[�̐���
		m_pPlayer->Create(D3DXVECTOR3(0.0f, PLAYER_DOWN_HEIGHT, 0.0f));
	}

	// �G�l�~�[�̐���
	SetEnemy();

	// �T�E���h�̐���
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//=============================================================================
//�@�I������
//=============================================================================
void CGame::Uninit(void)
{
	CNumber::Unload();		// �X�R�A
	CTexture::UnLoad();		// �e�N�X�`��
	CParticle::Unload();	// �p�[�e�B�N��

	if (m_text != NULL)
	{
		m_text->Reset();
		m_text = NULL;
	}

	//�t�F�[�h�ȊO�폜
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
void CGame::Update(void)
{
	CFade::FADE fade		= CFade::GetFade();					// �t�F�[�h�̎擾
	CInputKeyBoard *pKey	= CManager::GetInputKeyboard();		// ���͏��̎擾

	if (fade == CFade::FADE_IN)
	{// �t�F�[�h�C����Ԃ̂Ƃ�
		if (pKey->GetKeyboardTrigger(DIK_P) == true)
		{// �|�[�Y�؂�ւ�
				m_bPause = m_bPause ? false : true;
				CSound *pSound = CManager::GetSound();		// �T�E���h���̎擾
				if (m_bPause == true)
				{
					pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPENCLOSE);
				}
				else if (m_bPause == false)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPENCLOSE);
				}
		}
	}

	if (pKey->GetKeyboardTrigger(DIK_RETURN))
	{
		CFade::SetFade(CManager::MODE_RESULT);
	}
}

//=============================================================================
//�@�`�揈��
//=============================================================================
void CGame::Draw(void)
{
}

//=============================================================================
//�@�e�N�X�`���̐���
//=============================================================================
void CGame::SetTex(void)
{// CTexture::Create(���W�A�T�C�Y�A���);
	// HP�Q�[�W
	CTexture::Create(D3DXVECTOR3(180.0f, 30.0f, 0.0f), D3DXVECTOR2(170.0f, 20.0f), 4);				// �Q�[�W(�g)
	CTexture::Create(D3DXVECTOR3(33.0f, 30.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f), 5);				// HP�A�C�R��
	CTexture::Create(D3DXVECTOR3(60.0f, 30.0f, 0.0f), D3DXVECTOR2(GAUGE_SIZE, 5.0f), 6);			// HP�Q�[�W

	// �댯�x
	CTexture::Create(D3DXVECTOR3(SCREEN_WIDTH/2, 50.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), 7);		// �댯�x(�w�i)
	CTexture::Create(D3DXVECTOR3(SCREEN_WIDTH/2, 50.0f, 0.0f), D3DXVECTOR2(35.0f, 35.0f), 8);		// �댯�x(�}�[�N)

	// �c��N���X�^��
	CTexture::Create(D3DXVECTOR3(960.0f, 50.0f, 0.0f), D3DXVECTOR2(100.0f, 30.0f), 9);				// �c��N���X�^����
	CTexture::Create(D3DXVECTOR3(1100.0f, 50.0f, 0.0f), D3DXVECTOR2(20.0f, 20.0f), 10);				// �~
	CScore::Create(D3DXVECTOR3(1220.0f, 50.0f, 0.0f));												// ����

	// �����Ԃ�
	CTexture::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH , SCREEN_HEIGHT), 11);

	// ���e�B�N��
	CTexture::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(30.0f, 30.0f), 12);

}

//=============================================================================
//�@���̐���
//=============================================================================
void CGame::SetFloor(CLoadText::MAPTEXT Map)
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
void CGame::SetWall(void)
{// CWall::Create(���W�A�T�C�Y�A�����A�^�C�v)
	CWall::Create(D3DXVECTOR3(1000.0f, 0.0f, 0.0f)		, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �X�e�[�W
	CWall::Create(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f)		, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(750.0f, 0.0f, 750.0f)		, D3DXVECTOR2(250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-750.0f, 0.0f, 750.0f)	, D3DXVECTOR2(250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-0.0f, 0.0f, -750.0f)		, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-500.0f, 0.0f, 1750.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �ʘH
	CWall::Create(D3DXVECTOR3(500.0f, 0.0f, 1750.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-3000.0f, 0.0f, 6500.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �X�e�[�W
	CWall::Create(D3DXVECTOR3(3000.0f, 0.0f, 5750.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(0.0f, 0.0f, 8750.0f)		, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-1750.0f, 0.0f, 2750.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(1250.0f, 0.0f, 2750.0f)	, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-5000.0f, 0.0f, 4250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);	// �X�e�[�W
	CWall::Create(D3DXVECTOR3(-7500.0f, 0.0f, 2250.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-6000.0f, 0.0f, -1750.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-3000.0f, 0.0f, 500.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-9000.0f, 0.0f, 250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-7000.0f, 0.0f, 3250.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-9000.0f, 0.0f, 5750.0f)	, D3DXVECTOR2(3500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �ʘH
	CWall::Create(D3DXVECTOR3(-8000.0f, 0.0f, 3500.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-8000.0f, 0.0f, 8500.0f)	, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-7000.0f, 0.0f, 6250.0f)	, D3DXVECTOR2(1500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-7500.0f, 0.0f, 7750.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-7500.0f, 0.0f, 4750.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-11000.0f, 0.0f, 13250.0f), D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �X�e�[�W
	CWall::Create(D3DXVECTOR3(-3000.0f, 0.0f, 13250.0f)	, D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-10000.0f, 0.0f, 9250.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-5500.0f, 0.0f, 9250.0f)	, D3DXVECTOR2(2500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-9000.0f, 0.0f, 17250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-4500.0f, 0.0f, 17250.0f)	, D3DXVECTOR2(1500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-7000.0f, 0.0f, 19250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �ʘH
	CWall::Create(D3DXVECTOR3(-6000.0f, 0.0f, 18750.0f)	, D3DXVECTOR2(1500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-4250.0f, 0.0f, 21250.0f)	, D3DXVECTOR2(2750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-3750.0f, 0.0f, 20250.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(750.0f, 0.0f, 21250.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);	// �X�e�[�W
	CWall::Create(D3DXVECTOR3(750.0f, 0.0f, 11250.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-1500.0f, 0.0f, 15750.0f)	, D3DXVECTOR2(4500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(3000.0f, 0.0f, 13750.0f)	, D3DXVECTOR2(2500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(3000.0f, 0.0f, 19250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(4250.0f, 0.0f, 16250.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);	// �ʘH
	CWall::Create(D3DXVECTOR3(4250.0f, 0.0f, 17250.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(4500.0f, 0.0f, 12750.0f)	, D3DXVECTOR2(2500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �X�e�[�W
	CWall::Create(D3DXVECTOR3(3500.0f, 0.0f, 20250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(5500.0f, 0.0f, 17750.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(5500.0f, 0.0f, 15750.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(7500.0f, 0.0f, 22250.0f)	, D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(4500.0f, 0.0f, 18250.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(5000.0f, 0.0f, 15250.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(7500.0f, 0.0f, 10250.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(11500.0f, 0.0f, 16250.0f)	, D3DXVECTOR2(6000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(11500.0f, 0.0f, 6250.0f)	, D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �ʘH
	CWall::Create(D3DXVECTOR3(10500.0f, 0.0f, 6250.0f)	, D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(11500.0f, 0.0f, -2750.0f)	, D3DXVECTOR2(5000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �X�e�[�W
	CWall::Create(D3DXVECTOR3(4500.0f, 0.0f, 0.0f)		, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(4500.0f, 0.0f, -5500.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(8000.0f, 0.0f, -7750.0f)	, D3DXVECTOR2(3500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(7500.0f, 0.0f, 2250.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(2000.0f, 0.0f, -250.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// �ʘH
	CWall::Create(D3DXVECTOR3(3000.0f, 0.0f, 250.0f)	, D3DXVECTOR2(2500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(3250.0f, 0.0f, -3250.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(3750.0f, 0.0f, -2250.0f)	, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
}

//=============================================================================
//�@�I�u�W�F�N�g�̐���
//=============================================================================
void CGame::SetObjet(CLoadText::MAPTEXT Map)
{// CObject::Create( ���W , �ړ��� , �g�p����摜�̔ԍ� , ��������I�u�W�F�N�g�̔ԍ� , �I�u�W�F�N�g�̃^�C�v , �����蔻��̗L�� );
	for (int nCntModel = 0; nCntModel < Map.nNum[m_nCntType]; nCntModel++)
	{
		//	// �N���X�^��
		CObject::Create(Map.pos[m_nCntType][nCntModel], 0.0f, Map.nType[m_nCntType][nCntModel], Map.nType[m_nCntType][nCntModel], CObject::MOVETYPE_NOT, 0);
	}

	m_nCntType++;
}
void CGame::SetEnemy(void)
{// CEnemy::Create(���W)
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-1000.0f, 0.0f, 8500.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(2000.0f, 0.0f, 8500.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-7000.0f, 0.0f, 500.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-4000.0f, 0.0f, 500.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-5500.0f, 0.0f, 4000.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-8000.0f, 0.0f, 13000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-5500.0f, 0.0f, 13000.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(2000.0f, 0.0f, 17000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 15000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 21000.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(7300.0f, 0.0f, 13000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(6300.0f, 0.0f, 15000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(8000.0f, 0.0f, 20000.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(9000.0f, 0.0f, 2000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(7000.0f, 0.0f, -4000.0f));
}