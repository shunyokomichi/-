//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"
#include "camera.h"
#include "enemy.h"
#include "wall.h"
#include "object.h"
#include "fade.h"
#include "texture.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH					CPlayer::m_pMesh	[MAX_PLAYERPARTS]	= {};
LPD3DXBUFFER				CPlayer::m_pBuffMat	[MAX_PLAYERPARTS]	= {};
DWORD						CPlayer::m_nNumMat	[MAX_PLAYERPARTS]	= {};
LPDIRECT3DTEXTURE9			*CPlayer::m_pTexture[MAX_PLAYERPARTS]	= {};
CPlayer::PlayerStageStatus	CPlayer::m_Status						= PLAYERSTAGESTATUS_NONE;
D3DXVECTOR3					CPlayer::m_pos							= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3					CPlayer::m_posOld						= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3					CPlayer::m_rot							= D3DXVECTOR3(0.0f, 0.5f, 0.0f);
bool						CPlayer::m_bJump						= NULL;
bool						CPlayer::m_bLand						= NULL;
int							CPlayer::m_nLife						= NULL;
int							CPlayer::m_nMp							= NULL;

//*****************************************************************************
//�O���[�o���ϐ�
//*****************************************************************************
int		g_nNumPlayerModel;
char	g_aFileNamePlayerModel[MAX_PLAYERPARTS][256];

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	//�l�̏�����
	for (int nCount = 0; nCount < MAX_PLAYERPARTS; nCount++)
	{
		m_pModel[nCount] = NULL;
	}
	m_mtxWorld		= {};
	m_Status		= PLAYERSTAGESTATUS_NONE;
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bJump			= true;
	m_bLand			= true;
	m_bAttack		= true;
	m_bCatch		= true;
	m_bRun			= true;
	m_bTouchOld		= true;
	m_nLife			= 0;
	m_nMp			= 0;
	m_fGravityFrame = 0.0f;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
//�@��������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;
		pPlayer->Init();
		GetPos() = pos;
	}

	return pPlayer;
}

//=============================================================================
//�@�e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CPlayer::Load(void)
{
	return S_OK;
}

//=============================================================================
//�@�A�����[�h
//=============================================================================
void CPlayer::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < MAX_ENEMYPARTS; nCntParts++)
	{
		if (m_pBuffMat[nCntParts] != NULL)
		{
			m_pBuffMat[nCntParts]->Release();
			m_pBuffMat[nCntParts] = NULL;
		}
		if (m_pMesh[nCntParts] != NULL)
		{
			m_pMesh[nCntParts]->Release();
			m_pMesh[nCntParts] = NULL;
		}

		for (DWORD nCntTex = 0; nCntTex < m_nNumMat[nCntParts]; nCntTex++)
		{
			if (m_pTexture[nCntParts][nCntTex] != NULL)
			{
				m_pTexture[nCntParts][nCntTex]->Release();
				m_pTexture[nCntParts][nCntTex] = NULL;
			}
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();
	// �ʒu�E�����̏����ݒ�
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bInvincible	= false;
	m_bMotionEnd	= true;
	m_nAnimnow		= PLAYERANIM_NEUTRAL;
	m_nAttackDelay	= 0;
	m_nLife			= PLAYER_LIFE;
	m_nMp			= PLAYER_MP;
	m_bUse			= false;

	//�v���C���[���̓ǂݍ���
	FileLoad();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// ���f���̔j��
	for (int nCntModel = 0; nCntModel < MAX_PLAYERPARTS; nCntModel++)
	{
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Uninit();
			delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}
	}

	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	CManager::MODE	m_mode	= CManager::GetMode();				// �Q�[�����[�h�̎擾
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// ���͏��̎擾
	CSound			*pSound = CManager::GetSound();				// �T�E���h�̎擾

	if (m_mode == CManager::MODE_GAME || m_mode == CManager::MODE_TUTORIAL)
	{// �Q�[���ƃ`���[�g���A���̂Ƃ�
		if (m_bInvincible == false)
		{
			PlayerMove();		// �v���C���[�̈ړ�����
		}
		else
		{
			InvincibleMove();

		}
		if (m_bJump == false && m_pos.y - PLAYER_DOWN_HEIGHT < 0.0f)
		{
			m_bJump = true;
		}

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
					{// �^�C�v�����b�V����������
						// �I�u�W�F�N�g�̂����锻��
						((CEnemy*)pScene)->Collision(m_pos);
					}
				}
				// Next�Ɏ���Scene������
				pScene = pSceneNext;
			}
		}

		// ���[�V�����̍X�V
		m_nMotionType = m_nAnimnow;
		UpdateMotion();

#ifdef _DEBUG	// �f�o�b�N�p
		if (pKey != NULL)
		{// ���͏���
		 // ���ꂼ��̏����œ���������ς���i�����j
			if (pKey->GetKeyboardPress(DIK_UP) == true)
			{// ���C�t�̉��Z
				if (m_nLife < 100)
				{
					m_nLife++;
				}
			}
			else if (pKey->GetKeyboardPress(DIK_DOWN) == true)
			{// ���C�t�̌��Z
				if (m_nLife > 0)
				{
					m_nLife--;
				}
			}
		}
		if (pKey->GetKeyboardPress(DIK_Z) == true && pKey->GetKeyboardTrigger(DIK_X) == true)
		{// �f�o�b�N���[�h�؂�ւ�
			m_bInvincible = m_bInvincible ? false : true;
		}
		if (m_bInvincible == false)
		{
			CManager::m_pDebuglog->Print(1, "�����蔻�� �`�L�`\n");
		}
		else
		{
			CManager::m_pDebuglog->Print(1, "�����蔻�� �`���`\n");
		}
#endif

		if (m_bInvincible == false)
		{
			// �ǂ̓����蔻��
			CollisitionWall();
			// �I�u�W�F�N�g�̓����蔻��
			CollisitionObject3D(&m_pos, &m_posOld, &m_move, m_bTouchOld);
		}

		// �̗̓`�F�b�N
		if (m_nLife <= 0)
		{
			CManager::SetResult(CManager::GAMERESULT_LOSE);

			CFade::FADE fade = CFade::GetFade();

			if (fade != CFade::FADE_OUT)
			{
				//���[�h�ݒ�
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}


		// �\������
		CManager::m_pDebuglog->Print(1, "�v���C���[ �`pos.x %.1f  pos.y %.1f  pos.z %.1f�`\n", m_pos.x, m_pos.y, m_pos.z);
		CManager::m_pDebuglog->Print(1, "�v���C���[ �`HP:%d , MP:%d�`\n", m_nLife, m_nMp);
	}
	else if(m_mode == CManager::MODE_RESULT)
	{
		m_nAnimnow = PLAYERANIM_RESULT;
		// ���[�V�����̍X�V
		m_nMotionType = m_nAnimnow;
		UpdateMotion();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	mtxParent = m_mtxWorld;

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			m_pModel[nCntParts]->Draw();
		}
	}
}

//=============================================================================
// �v���C���[�̈ړ�����
//=============================================================================
void CPlayer::PlayerMove(void)
{
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// ���͏��̎擾
	CInputMouse		*pMouse = CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)
	CSound			*pSound = CManager::GetSound();				// �T�E���h�̎擾

	D3DXVECTOR3		&m_pos	= CManager::GetPlayer()->GetPos();
	m_posOld				= CManager::GetPlayer()->GetPos();

	if (pKey != NULL)
	{// ���͏���
	 // ���ꂼ��̏����œ���������ς���i�����j
		if (pKey->GetKeyboardPress(DIK_LSHIFT) == true && m_bAttack == true && m_bCatch == true)
		{// �_�b�V����
			if (pMouse->GetMouseTrigger(0) == true && m_bAttack == true && m_bCatch == true)
			{// �U��
			 // �U�����[�V����
				m_nKey		= 0;
				m_bRun		= true;						// �����ĂȂ���Ԃ�
				m_bAttack	= false;					// �U����Ԃ�
				m_nAnimnow	= PLAYERANIM_ATTACK;		// ���[�V�����̕ύX
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_ATTAK);
			}
			else if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
			{// �U����A�j���[�g�����ɖ߂�
				m_nAnimnow = PLAYERANIM_NEUTRAL;	// ���[�V�����̕ύX
			}

			if (pKey->GetKeyboardPress(DIK_D) == true)
			{
				if (pKey->GetKeyboardPress(DIK_S) == true)
				{	//�����ړ�
					m_move.x += sinf((D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf((D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
				}
				else if (pKey->GetKeyboardPress(DIK_W) == true)
				{	//����ړ�
					m_move.x += sinf((D3DX_PI * 0.25f) + m_rot.y) * RUN_SPEED;
					m_move.z += cosf((D3DX_PI * 0.25f) + m_rot.y) * RUN_SPEED;
				}
				else
				{	//���ړ�
					m_move.x += sinf((D3DX_PI * 0.5f) + m_rot.y) * (RUN_SPEED + 2.0f);
					m_move.z += cosf((D3DX_PI * 0.5f) + m_rot.y) * (RUN_SPEED + 2.0f);
				}
				if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
				{// �U�����ɑ����Ă��邩�ǂ���
					// �ړ����[�V����
					m_bRun		= false;				// �����Ă��Ԃ�
					m_nAnimnow	= PLAYERANIM_MOVE;		// ���[�V�����̕ύX
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
				}
			}
			else if (pKey->GetKeyboardPress(DIK_A) == true)
			{
				if (pKey->GetKeyboardPress(DIK_S) == true)
				{	// �E���ړ�
					m_move.x += sinf(-(D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf(-(D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
				}
				else if (pKey->GetKeyboardPress(DIK_W) == true)
				{	// �E��ړ�
					m_move.x += sinf(-(D3DX_PI * 0.25f) + m_rot.y)* RUN_SPEED;
					m_move.z += cosf(-(D3DX_PI * 0.25f) + m_rot.y) * RUN_SPEED;
				}
				else
				{	// �E�ړ�
					m_move.x += sinf(-(D3DX_PI * 0.5f) + m_rot.y)* (RUN_SPEED + 2.0f);
					m_move.z += cosf(-(D3DX_PI * 0.5f) + m_rot.y)* (RUN_SPEED + 2.0f);
				}
				if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
				{// �U�����ɑ����Ă��邩�ǂ���
					// �ړ����[�V����
					m_bRun		= false;				// �����Ă��Ԃ�
					m_nAnimnow	= PLAYERANIM_MOVE;		// ���[�V�����̕ύX
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
				}
			}
			else if (pKey->GetKeyboardPress(DIK_W) == true)
			{	// �O�Ɉړ�
				m_move.x += sinf((0) + m_rot.y) * (RUN_SPEED + 2.0f);
				m_move.z += cosf((0) + m_rot.y) * (RUN_SPEED + 2.0f);
				if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
				{// �U�����ɑ����Ă��邩�ǂ���
					// �ړ����[�V����
					m_bRun		= false;				// �����Ă��Ԃ�
					m_nAnimnow	= PLAYERANIM_MOVE;		// ���[�V�����̕ύX
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
				}
			}
			else if (pKey->GetKeyboardPress(DIK_S) == true)
			{	// ���Ɉړ�
				m_move.x += sinf((D3DX_PI)+m_rot.y) * (WALK_SPEED + 2.0f);
				m_move.z += cosf((D3DX_PI)+m_rot.y) * (WALK_SPEED + 2.0f);
				if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
				{// �U�����ɑ����Ă��邩�ǂ���
					// �ړ����[�V����
					m_bRun		= false;					// �����Ă��Ԃ�
					m_nAnimnow	= PLAYERANIM_NEUTRAL;		// ���[�V�����̕ύX
				}
			}
			else if ((m_bJump || m_bLand) && m_bRun == false && m_bAttack == true && m_bCatch == true)
			{// �V�t�g�������Ȃ���ړ����Ă��Ȃ���
				// �ҋ@���[�V����
				m_bRun		= true;						// �����ĂȂ���Ԃ�
				m_nAnimnow	= PLAYERANIM_NEUTRAL;		// ���[�V�����̕ύX
				pSound->StopSound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
			}
		}
		else if (m_bRun == false)
		{// �����Ă���󋵂ŃV�t�g���������
			m_bRun = true;		// �����ĂȂ���Ԃ�
			pSound->StopSound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
		}
		else
		{// ����
			if (pMouse->GetMouseTrigger(0) == true && m_bAttack == true && m_bCatch == true)
			{// �U��
			 // �U�����[�V����
				m_nKey		= 0;
				m_bAttack	= false;					// �U����Ԃ�
				m_nAnimnow	= PLAYERANIM_ATTACK;		// ���[�V�����̕ύX
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_ATTAK);
			}
			if (pKey->GetKeyboardPress(DIK_D) == true)
			{
				if (pKey->GetKeyboardPress(DIK_S) == true)
				{	// �����ړ�
					m_move.x += sinf((D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf((D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
				}
				else if (pKey->GetKeyboardPress(DIK_W) == true)
				{	// ����ړ�
					m_move.x += sinf((D3DX_PI * 0.25f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf((D3DX_PI * 0.25f) + m_rot.y) * WALK_SPEED;
				}
				else
				{	// ���ړ�
					m_move.x += sinf((D3DX_PI * 0.5f) + m_rot.y) * (WALK_SPEED + 2.0f);
					m_move.z += cosf((D3DX_PI * 0.5f) + m_rot.y) * (WALK_SPEED + 2.0f);
				}
				if ((m_bJump || m_bLand) && m_bAttack == true && m_bCatch == true)
				{// �U�������ǂ���
					// �ړ����[�V����
					m_nAnimnow = PLAYERANIM_NEUTRAL;	// ���[�V�����̕ύX
				}
			}
			else if (pKey->GetKeyboardPress(DIK_A) == true)
			{
				if (pKey->GetKeyboardPress(DIK_S) == true)
				{	// �E���ړ�
					m_move.x += sinf(-(D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf(-(D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
				}
				else if (pKey->GetKeyboardPress(DIK_W) == true)
				{	// �E��ړ�
					m_move.x += sinf(-(D3DX_PI * 0.25f) + m_rot.y)* WALK_SPEED;
					m_move.z += cosf(-(D3DX_PI * 0.25f) + m_rot.y) * WALK_SPEED;
				}
				else
				{	// �E�ړ�
					m_move.x += sinf(-(D3DX_PI * 0.5f) + m_rot.y)* (WALK_SPEED + 2.0f);
					m_move.z += cosf(-(D3DX_PI * 0.5f) + m_rot.y)* (WALK_SPEED + 2.0f);
				}
				if ((m_bJump || m_bLand) && m_bAttack == true && m_bCatch == true)
				{// �U�������ǂ���
					// �ړ����[�V����
					m_nAnimnow = PLAYERANIM_NEUTRAL;	// ���[�V�����̕ύX
				}
			}
			else if (pKey->GetKeyboardPress(DIK_W) == true)
			{	// ���Ɉړ�
				m_move.x += sinf((0) + m_rot.y) * (WALK_SPEED + 2.0f);
				m_move.z += cosf((0) + m_rot.y) * (WALK_SPEED + 2.0f);
				if ((m_bJump || m_bLand) && m_bAttack == true && m_bCatch == true)
				{// �U�������ǂ���
					// �ړ����[�V����
					m_nAnimnow = PLAYERANIM_NEUTRAL;	// ���[�V�����̕ύX
				}
			}
			else if (pKey->GetKeyboardPress(DIK_S) == true)
			{	// ��Ɉړ�
				m_move.x += sinf((D3DX_PI)+m_rot.y) * (WALK_SPEED + 2.0f);
				m_move.z += cosf((D3DX_PI)+m_rot.y) * (WALK_SPEED + 2.0f);
				if ((m_bJump || m_bLand) && m_bAttack == true && m_bCatch == true)
				{// �U�������ǂ���
					// �ړ����[�V����
					m_nAnimnow = PLAYERANIM_NEUTRAL;	// ���[�V�����̕ύX
				}
			}
			else if ((m_bJump || m_bLand) && m_bAttack == true && m_bRun == true && m_bCatch == true)
			{
				// �ҋ@���[�V����
				m_nAnimnow = PLAYERANIM_NEUTRAL;		// ���[�V�����̕ύX
			}
		}

		if (m_pos.y - PLAYER_DOWN_HEIGHT > 0.0f)
		{
			// �d��
			m_move.y = GRAVITY_REAL * 10.0f;
		}
		else if (m_bLand == true)
		{
			m_bJump = true;
		}

		if (pKey->GetKeyboardTrigger(DIK_SPACE) == true && (m_bJump == true || m_bLand == true))
		{
			m_bJump		= false;
			m_move.y	= cosf(D3DX_PI) * -JUMP_POWER;
		}

		//����
		m_move.x += (0.0f - m_move.x) * 0.6f;
		m_move.y += (0.0f - m_move.y) * 0.6f;
		m_move.z += (0.0f - m_move.z) * 0.6f;

		//�ʒu�X�V�i�ړ��j
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
		m_pos.z += m_move.z;
	}
}

//=============================================================================
// �f�o�b�N���[�h�̈ړ�����
//=============================================================================
void CPlayer::InvincibleMove(void)
{
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// ���͏��̎擾
	D3DXVECTOR3		&m_pos	= CManager::GetPlayer()->GetPos();
	m_posOld				= CManager::GetPlayer()->GetPos();

	if (pKey->GetKeyboardPress(DIK_D) == true)
	{
		if (pKey->GetKeyboardPress(DIK_S) == true)
		{	// �����ړ�
			m_move.x += sinf((D3DX_PI * 0.75f) + m_rot.y) * 100.0f;
			m_move.z += cosf((D3DX_PI * 0.75f) + m_rot.y) * 100.0f;
		}
		else if (pKey->GetKeyboardPress(DIK_W) == true)
		{	// ����ړ�
			m_move.x += sinf((D3DX_PI * 0.25f) + m_rot.y) * 100.0f;
			m_move.z += cosf((D3DX_PI * 0.25f) + m_rot.y) * 100.0f;
		}
		else
		{	// ���ړ�
			m_move.x += sinf((D3DX_PI * 0.5f) + m_rot.y) * (100.0f + 2.0f);
			m_move.z += cosf((D3DX_PI * 0.5f) + m_rot.y) * (100.0f + 2.0f);
		}
	}
	else if (pKey->GetKeyboardPress(DIK_A) == true)
	{
		if (pKey->GetKeyboardPress(DIK_S) == true)
		{	// �E���ړ�
			m_move.x += sinf(-(D3DX_PI * 0.75f) + m_rot.y) * 100.0f;
			m_move.z += cosf(-(D3DX_PI * 0.75f) + m_rot.y) * 100.0f;
		}
		else if (pKey->GetKeyboardPress(DIK_W) == true)
		{	// �E��ړ�
			m_move.x += sinf(-(D3DX_PI * 0.25f) + m_rot.y)* 100.0f;
			m_move.z += cosf(-(D3DX_PI * 0.25f) + m_rot.y) * 100.0f;
		}
		else
		{	// �E�ړ�
			m_move.x += sinf(-(D3DX_PI * 0.5f) + m_rot.y)* (100.0f + 2.0f);
			m_move.z += cosf(-(D3DX_PI * 0.5f) + m_rot.y)* (100.0f + 2.0f);
		}
	}
	else if (pKey->GetKeyboardPress(DIK_W) == true)
	{	// ���Ɉړ�
		m_move.x += sinf((0) + m_rot.y) * (100.0f + 2.0f);
		m_move.z += cosf((0) + m_rot.y) * (100.0f + 2.0f);
	}
	else if (pKey->GetKeyboardPress(DIK_S) == true)
	{	// ��Ɉړ�
		m_move.x += sinf((D3DX_PI)+m_rot.y) * (100.0f + 2.0f);
		m_move.z += cosf((D3DX_PI)+m_rot.y) * (100.0f + 2.0f);
	}

	// ����
	m_move.x += (0.0f - m_move.x) * 0.6f;
	m_move.y += (0.0f - m_move.y) * 0.6f;
	m_move.z += (0.0f - m_move.z) * 0.6f;

	// �ʒu�X�V�i�ړ��j
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

}

//=============================================================================
//	�ǂ̓����蔻��
//=============================================================================
void CPlayer::CollisitionWall(void)
{
	CScene *pScene;
	// �v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(WALL_PRIOTITY);

	// NULL�`�F�b�N
	while (pScene != NULL)
	{
		// Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{// �^�C�v���ǂ�������
				((CWall*)pScene)->CollisionWallPlayer(&m_pos, &m_posOld, &m_move, 20.0f);
			}
		}

		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �v���C���[�̃��[�V����
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	// ���[�V����
	KEY			*pKey, *pNextKey;
	float		fRateMotion;
	float		fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;

	// �L�[���ő吔������Ȃ��悤��
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	// ���[�V�����X�V
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			// ���݂̃L�[���擾
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			// ���̃L�[���擾
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			// ���݂̃L�[���玟�̃L�[�ւ̍Đ��t���[�����ɂ����郂�[�V�����J�E���^�[�̑��Βl���Z�o
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			// ROT
			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			// ���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);

			// POS
			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			// ���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);


			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			// ���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			// POS
			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			// ���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);


			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			// ���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			// POS
			// ���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			// ���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);


			// �p�[�c�𓮂���
			m_pModel[nCntParts]->Setrot(rotmotion);

			// POS
			m_pModel[nCntParts]->SetPos(D3DXVECTOR3(m_OffSetPos[nCntParts].x + posmotion.x,
				m_OffSetPos[nCntParts].y + posmotion.y,
				m_OffSetPos[nCntParts].z + posmotion.z));

		}
	}

	// ���[�v�̔���
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:
		// ���[�v����
		// �t���[����i�߂�
		m_nCountFlame++;
		// �L�[�̍X�V
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
			{
				m_nKey = 0;
			}
			else
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}
		break;
	case false:
		// ���[�v���Ȃ�
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{// �t���[����i�߂�
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			if (m_nAnimnow == PLAYERANIM_ATTACK)
			{// �U�����[�V����
				m_nAttackDelay++;
				if (m_nAttackDelay > 10)
				{
					m_bAttack = true;
					m_nAttackDelay = 0;
				}
			}
			if (m_nAnimnow == PLAYERANIM_CATCH)
			{// �U�����[�V����
				m_nAttackDelay++;
				if (m_nAttackDelay > 10)
				{
					m_bCatch = true;
					m_nAttackDelay = 0;
				}
			}
			m_bMotionEnd = true;
		}
		// �L�[�̍X�V
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey > m_nKey)
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}
		break;
	}

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
					D3DXVECTOR3 HandPos = m_pos + m_pModel[19]->GetPos();
					// �I�u�W�F�N�g�̂����锻��
					((CEnemy*)pScene)->CollisionDamege(HandPos, m_nAnimnow);
				}
			}
			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �G�̓����蔻��
//=============================================================================
void CPlayer::Collision(D3DXVECTOR3 posEnemy)
{
	// �G�Ǝ����̋����𑪂�
	float fLength = (((m_pos.x - posEnemy.x) * (m_pos.x - posEnemy.x)) + ((m_pos.z - posEnemy.z) * (m_pos.z - posEnemy.z)));

	// �����ƓG�̊p�x�𑪂�
	float fAngle = atan2f(m_pos.x - posEnemy.x, m_pos.z - posEnemy.z);

	if (PLAYER_RADIUS * PLAYER_RADIUS > fLength)
	{// �͈͓���������
		m_pos.x = posEnemy.x + sinf(fAngle) * PLAYER_RADIUS;
		m_pos.z = posEnemy.z + cosf(fAngle) * PLAYER_RADIUS;
	}
}
//=============================================================================
// �U���̓����蔻��
//=============================================================================
void CPlayer::CollisionAttack(D3DXVECTOR3 posEnemy, D3DXMATRIX mtxWorld, float Radius, int NumModel)
{
	if (m_pModel[19] != NULL)
	{// ���������Ƃ�
		D3DXMATRIX mtxModelWorld = m_pModel[19]->GetMtxWorld();		// ���}�g���b�N�X

		// �G�Ǝ����̍����𑪂�
		float fRadiusEnemy = powf(Radius * 2.0f, 2);

		float fLengthX = posEnemy.x - mtxModelWorld._41;
		float fLengthY = posEnemy.y - mtxModelWorld._42;
		float fLengthZ = posEnemy.z - mtxModelWorld._43;

		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);

		if (fRadiusEnemy > fLengthTotal)
		{// �����蔻��ɓ����Ă����Ƃ�
			CManager::m_pDebuglog->Print(1, "[%d]HIT!!\n", NumModel);
		}
	}
}

//=============================================================================
//	�I�u�W�F�N�g�̓����蔻��
//=============================================================================
void CPlayer::CollisitionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, bool bTouchOld)
{
	CScene *pScene;

	// �v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(OBJECT_PRIOTITY);

	// NULL�`�F�b�N
	while (pScene != NULL)
	{
		// Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			// �^�C�v�����b�V����������
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{
				// �I�u�W�F�N�g�̂����锻��
				m_bLand		= ((CObject*)pScene)->CollisionObject(pPos, pPosOld, pMove, bTouchOld);
				m_bTouchOld = m_bLand;
				if (m_bLand == true)
				{
					m_bJump = true;
				}
			}
		}

		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �t�@�C���ǂݍ���
//=============================================================================
void CPlayer::FileLoad(void)
{
	// �f�o�C�X���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �t�@�C���ǂݍ��ݗp�ϐ�
	FILE	*pFile;				// �t�@�C���|�C���^
	char	*pStrcur;			// ���݂̐擪�̕�����
	char	aLine[256];			// ������
	char	aStr[256];			// �ꎞ�ۑ�������
	int		nIndex = 0;			// ���݂̃C���f�b�N�X
	int		nWord = 0;			// �|�b�v�ŕԂ��ꂽ�l��ێ�

	D3DXVECTOR3 ParentPos;		// �e�̈ʒu�����擾
#if 1
	// �t�@�C�����J�� �ǂݍ���
	pFile = fopen(PLAYER_FILE, "r");
	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_PLAYERMOTION; )
		{
			// ������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			// ����������o��
			strcpy(aStr, pStrcur);

			// ������̃f�[�^ ��r���镶���� ��r���镶����
			if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				// ���o��
				pStrcur += strlen("NUM_MODEL = ");
				// ������̐擪��ݒ�
				strcpy(aStr, pStrcur);
				// �����񔲂��o��
				g_nNumPlayerModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nNumPlayerModel; nCntModel++)
				{
					// ������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					// ����������߂�
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
						// ���o��
						pStrcur += strlen("MODEL_FILENAME = ");

						// �������ݒ�
						strcpy(aStr, pStrcur);

						// �K�v�ȕ�����̍Ō�̕����܂ł̕������𐔂���
						int nNullNum = PopString(pStrcur, &aStr[0]);

						// ����������߂�
						strcpy(aStr, pStrcur);

						// �Ō�̕�����NULL����������
						aStr[nNullNum - 1] = '\0';

						// �Ώۂ̕����񂩂甲���o��
						strcpy(&g_aFileNamePlayerModel[nCntModel][0], aStr);

						if (m_pMesh[nCntModel] == NULL)
						{
							// X�t�@�C���̓ǂݍ���
							D3DXLoadMeshFromX(&g_aFileNamePlayerModel[nCntModel][0],
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&m_pBuffMat[nCntModel],
								NULL,
								&m_nNumMat[nCntModel],
								&m_pMesh[nCntModel]);

							D3DXMATERIAL *pmat;        // �}�e���A���f�[�^�ւ̃|�C���^
							D3DMATERIAL9 *matDef;      // ���݂̃}�e���A���ۑ��p
							pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
							matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
							m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

							for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
							{// �J�E���g
								matDef[tex] = pmat[tex].MatD3D;
								matDef[tex].Ambient = matDef[tex].Diffuse;
								m_pTexture[nCntModel][tex] = NULL;
								if (pmat[tex].pTextureFilename != NULL &&
									lstrlen(pmat[tex].pTextureFilename) > 0)
								{// �e�N�X�`�����g�p���Ă���
									if (FAILED(D3DXCreateTextureFromFile(pDevice,
										pmat[tex].pTextureFilename,
										&m_pTexture[nCntModel][tex])))
									{
										MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
									}
								}
							}

							delete[] matDef;
							matDef = NULL;
						}
					}
				}
				// ������̐擪��ݒ�
				pStrcur = ReadLine(pFile, &aLine[0]);
				// ����������o��
				strcpy(aStr, pStrcur);
			}


			// ������̃f�[�^ ��r���镶���� ��r���镶����
			if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
			{
				while (1)
				{
					// ������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					// ����������o��
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{
						// ���o��
						pStrcur += strlen("NUM_PARTS = ");
						// ������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						// �����񔲂��o��
						m_nNumParts = atoi(pStrcur);
					}
					if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							// ������̐擪��ݒ�
							pStrcur = ReadLine(pFile, &aLine[0]);

							// INDEX��ǂݍ���
							if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								// ���o��
								pStrcur += strlen("INDEX = ");
								// ������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								// �����񔲂��o��
								nIndex = atoi(pStrcur);
							}
							// PARENT��ǂݍ���
							if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								// ���o��
								pStrcur += strlen("PARENT = ");
								// ������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								// �����񔲂��o��
								m_aIndexParent[nIndex] = atoi(pStrcur);
							}
							// POS��ǂݍ���
							if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
							{
								// ���o��
								pStrcur += strlen("POS = ");
								// ������̐擪��ݒ�
								strcpy(aStr, pStrcur);

								// ��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								// ������ϊ�
								m_aKayOffset[nIndex].fposX = (float)atof(pStrcur);
								// ���������i�߂�
								pStrcur += nWord;

								// ��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								// ������ϊ�
								m_aKayOffset[nIndex].fposY = (float)atof(pStrcur);
								// ���������i�߂�
								pStrcur += nWord;

								// ��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								// ������ϊ�
								m_aKayOffset[nIndex].fposZ = (float)atof(pStrcur);

							}
							// ROT��ǂݍ���
							if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
							{
								// ���o��
								pStrcur += strlen("ROT = ");
								// ������̐擪��ݒ�
								strcpy(aStr, pStrcur);

								// ��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								// ������ϊ�
								m_aKayOffset[nIndex].frotX = (float)atof(pStrcur);

								// ���������i�߂�
								pStrcur += nWord;
								// ��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								// ������ϊ�
								m_aKayOffset[nIndex].frotY = (float)atof(pStrcur);

								// ���������i�߂�
								pStrcur += nWord;
								// ��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								// ������ϊ�
								m_aKayOffset[nIndex].frotZ = (float)atof(pStrcur);
							}
							// �p�[�c�Z�b�g�I��
							else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{
								// NULL�`�F�b�N
								if (m_pModel[nIndex] == NULL)
								{// ���I�m��
									m_pModel[nIndex] = new CModel;
									// NULL�`�F�b�N
									if (m_pModel[nIndex] != NULL)
									{
										// ���f���̐���
										m_pModel[nIndex]->BindModel(m_pMesh[nIndex], m_pBuffMat[nIndex], m_nNumMat[nIndex], m_pTexture[nIndex]);
										m_pModel[nIndex]->Init();
									}
								}

								// ���f���𐶐�	�I�t�Z�b�g�ݒ�
								m_pModel[nIndex]->SetPos(D3DXVECTOR3(m_pos.x + m_aKayOffset[nIndex].fposX,
									m_pos.y + m_aKayOffset[nIndex].fposY,
									m_pos.z + m_aKayOffset[nIndex].fposZ));

								// pos����
								ParentPos = m_pModel[nIndex]->GetPos();
								m_OffSetPos[nIndex] = m_pModel[nIndex]->GetPos();

								// ���f�������蓖��
								if (m_aIndexParent[nIndex] == -1)
								{
									// ���f���̐e���w��
									m_pModel[nIndex]->SetParent(NULL);
									ParentPos = m_pModel[nIndex]->GetPos();
								}
								else
								{
									// ���f���̐e���w��
									m_pModel[nIndex]->SetParent(m_pModel[m_aIndexParent[nIndex]]);
								}

								break;
							}
						}
					}
					// �L�����N�^�[�Z�b�g�I��
					else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{
						break;
					}
				}
				// ������̐擪��ݒ�
				pStrcur = ReadLine(pFile, &aLine[0]);
				// ����������o��
				strcpy(aStr, pStrcur);
			}

			// ���[�V�����ǂݍ���
			if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				// ���o��
				pStrcur += strlen("MOTIONSET");

				while (1)
				{
					// ������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					// ����������o��
					strcpy(aStr, pStrcur);

					if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
					{
						// ���o��
						pStrcur += strlen("LOOP = ");
						// ������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						switch (atoi(pStrcur))
						{
						case 0:
							// �����񔲂��o��
							m_aMotionInfo[nCntMotion].bLoop = false;
							break;
						case 1:
							// �����񔲂��o��
							m_aMotionInfo[nCntMotion].bLoop = true;
							break;
						}
						// ������̐擪��ݒ�
						pStrcur = ReadLine(pFile, &aLine[0]);
					}
					if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{
						// ���o��
						pStrcur += strlen("NUM_KEY = ");
						// ������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						// �����񔲂��o��
						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrcur);

						// ������̐擪��ݒ�
						pStrcur = ReadLine(pFile, &aLine[0]);
						// ����������o��
						strcpy(aStr, pStrcur);
					}

					// �L�[�̐ݒ�
					for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey;)
					{
						if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
						{
							// ���o��
							pStrcur += strlen("KEYSET");
							// ������̐擪��ݒ�
							strcpy(aStr, pStrcur);
							// ������̐擪��ݒ�
							pStrcur = ReadLine(pFile, &aLine[0]);

							if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								// ���o��
								pStrcur += strlen("FRAME = ");

								m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

								// ������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								// ������̐擪��ݒ�
								pStrcur = ReadLine(pFile, &aLine[0]);
							}

							// �p�[�c����
							for (int nCntParts = 0; nCntParts < m_nNumParts;)
							{
								if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
								{
									// ������̐擪��ݒ�
									pStrcur = ReadLine(pFile, &aLine[0]);

									if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
									{
										// ���o��
										pStrcur += strlen("POS = ");
										// ������̐擪��ݒ�
										strcpy(aStr, pStrcur);

										// ��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										// POS.X���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
										// ���������i�߂�
										pStrcur += nWord;

										// ��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										// POS.Y���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
										// ���������i�߂�
										pStrcur += nWord;

										// ��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										// POS.Z���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
										// ������̐擪��ݒ�
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									// ROT��ǂݍ���
									if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
									{
										// ���o��
										pStrcur += strlen("ROT = ");
										// ������̐擪��ݒ�
										strcpy(aStr, pStrcur);

										// ��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										// RotX
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
										// ���������i�߂�
										pStrcur += nWord;

										// ��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										// RotY
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
										// ���������i�߂�
										pStrcur += nWord;

										// ��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										// RotZ
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

										// ������̐擪��ݒ�
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
									{
										// ���o��
										pStrcur += strlen("END_KEY");
										// ������̐擪��ݒ�
										strcpy(aStr, pStrcur);
										// ������̐擪��ݒ�
										pStrcur = ReadLine(pFile, &aLine[0]);
										// �p�[�c�̃J�E���g��i�߂�
										nCntParts++;
									}
								}
								else
								{
									// ������̐擪��ݒ�
									pStrcur = ReadLine(pFile, &aLine[0]);
								}
							}
							if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								// ������̐擪��ݒ�
								pStrcur = ReadLine(pFile, &aLine[0]);
								// �J�E���g��i�߂�
								nCntKey++;
							}
						}
						else
						{
							// ������̐擪��ݒ�
							pStrcur = ReadLine(pFile, &aLine[0]);
						}

					}
					if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						// ���[�V�����̏����Z�b�g
						m_pKeyInfo[nCntMotion] = &m_aMotionInfo[nCntMotion].aKayInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			// �X�N���v�g�̏I���
			if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
	}
	else
	{	// �t�@�C�����J���Ȃ�����
		printf("�t�@�C�����J���܂���ł���\n");
	}
	// �t�@�C�������
	fclose(pFile);
#endif
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݖ�������r��
//=============================================================================
char *CPlayer::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		// 1�s���ǂݍ���
		fgets(&pDst[0], 256, pFile);

		// ������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		// ������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		// ������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		// ������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݐ擪��r��
//=============================================================================
char * CPlayer::GetLineTop(char * pStr)
{
	while (1)
	{
		// ������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		// ������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//�@��������Ԃ�
//=============================================================================
int CPlayer::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	// ���o��
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	// ���o��
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	// ���o��
			pStr = "\0";
			nWord += strlen("\t");
			break;
		}
		// ������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//���o��
			nWord += strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	// ������̐���Ԃ�
	return nWord;
}