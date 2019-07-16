//=============================================================================
//
// �G�̏��� [enemy.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"
#include "fade.h"
#include "billboord.h"
#include "bullet.h"
#include "wall.h"
#include "object.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH				CEnemy::m_pMesh		[MAX_ENEMYPARTS]	= {};
LPD3DXBUFFER			CEnemy::m_pBuffMat	[MAX_ENEMYPARTS]	= {};
DWORD					CEnemy::m_nNumMat	[MAX_ENEMYPARTS]	= {};
LPDIRECT3DTEXTURE9		*CEnemy::m_pTexture	[MAX_ENEMYPARTS]	= {};
CBillBoord				*CEnemy::m_pBillBoord					= NULL;
CBullet					*CEnemy::m_pBullet						= NULL;

//*****************************************************************************
//�O���[�o���ϐ�
//*****************************************************************************
int		g_nNumEnemyModel;
char	g_aFileNameEnemyModel[MAX_ENEMYPARTS][256];

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	//�l�̏�����
	m_mtxWorld		= {};
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCountModel = 0; nCountModel < MAX_ENEMYPARTS; nCountModel++)
	{
		m_pModel[nCountModel] = NULL;
	}
	m_nAnimnow		= ENEMYANIM_NEUTRAL;
	m_nStatse		= ENEMYSTATSE_NONE;
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bAttack		= true;
	m_nAttackDelay	= 0;
	m_bDamage		= true;
	m_nDamageDelay	= 0;
	m_nLife			= 0;
	m_nCntSearch	= 0;
	m_nCntStatse	= 0;
	m_fAngle		= 0.0f;
	m_fWalkFrame	= 0;
	m_bMove			= true;
	m_fMoveFlame	= (rand() % 400 + 150);
	m_VigilancePos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posAngle		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntAttack	= 0;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//�@��������
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;
		pEnemy->Init();
		pEnemy->m_pos = pos;
	}
	return pEnemy;
}

//=============================================================================
//�@���[�h����
//=============================================================================
HRESULT CEnemy::Load(void)
{
	return S_OK;
}

//=============================================================================
//�@�A�����[�h����
//=============================================================================
void CEnemy::Unload(void)
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
HRESULT CEnemy::Init(void)
{
	srand((unsigned int)time(0));	// �����_��
	//�r���{�[�h�e�N�X�`���̓ǂݍ���
	m_pBillBoord->Load();
	m_pBullet->Load();

	// �ʒu�E�����̏����ݒ�
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bMotionEnd	= true;
	m_nAnimnow		= ENEMYANIM_NEUTRAL;
	m_nStatse		= ENEMYSTATSE_NONE;
	m_nAttackDelay	= 0;
	m_nDamageDelay	= 0;
	m_fMoveFlame	= (rand() % 400 + 150);
	m_nLife			= 4;

	FileLoad();// �t�@�C�����̓ǂݍ���

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	//�r���{�[�h�e�N�X�`���̔j��
	m_pBillBoord->UnLoad();
	m_pBullet->UnLoad();

	// ���f���̔j��
	for (int nCntModel = 0; nCntModel < MAX_ENEMYPARTS; nCntModel++)
	{
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Uninit();
			//delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}
	}
	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	CManager::MODE	m_mode		= CManager::GetMode();				// �Q�[�����[�h�̎擾
	CInputKeyBoard	*pKey		= CManager::GetInputKeyboard();		// ���͏��̎擾
	CSound			*pSound		= CManager::GetSound();				// �T�E���h���̎擾
	D3DXVECTOR3		PlayerPos	= CGame::GetPlayer()->GetPos();		//�v���C���[�̈ʒu���

	switch (m_mode)
	{// �J�����̃^�C�v�ʏ���
	case CManager::MODE_TITLE:
		break;
	case CManager::MODE_TUTORIAL:
		break;
	case CManager::MODE_GAME:
		if (m_pos.x + 6000.0f >= PlayerPos.x && m_pos.x - 6000.0f <= PlayerPos.x)
		{// �v���C���[�ɋ߂��Ƃ�
			m_posOld = m_pos;		// �ʒu�X�V(�ߋ��̈ʒu����ۑ�)

			if (m_nCntSearch % 20 == 0 && m_nStatse != ENEMYSTATSE_DEDA)
			{// �{���e�̐���
				D3DXVECTOR3 rot = (m_rot - D3DXVECTOR3(0.0f, (ENEMY_SEARCH_INTERVAL * (ENEMY_SEARCH_NUM / 2)), 0.0f));	// �{���͈͂̒��S��ݒ�
				for (int nCntSearch = 0; nCntSearch < ENEMY_SEARCH_NUM; nCntSearch++)
				{
					D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					move.x = sinf((D3DX_PI)+(rot.y + nCntSearch * ENEMY_SEARCH_INTERVAL)) * 35.0f;		// �����ɍ��킹�Ĉړ��ʂ�ݒ�(X)
					move.z = cosf((D3DX_PI)+(rot.y + nCntSearch * ENEMY_SEARCH_INTERVAL)) * 35.0f;		// �����ɍ��킹�Ĉړ��ʂ�ݒ�(Z)
					m_pBullet->Create(D3DXVECTOR3(m_pos.x, (m_pos.y + ENEMY_EYE), m_pos.z), move, D3DXVECTOR2(30.0f, 30.0f), this);
				}
			}

			// ��ԏ���
			switch (m_nStatse)
			{
			case ENEMYSTATSE_NONE:
				if (m_bDamage == true)
				{// �_���[�W���󂯂Ă��Ȃ��Ƃ�
					MoveNone();
				}
				CManager::m_pDebuglog->Print(1, "�G�̏�� �` �ʏ� �`\n");
				break;
			case ENEMYSTATSE_VIGILANCE:
				if (m_bDamage == true)
				{// �_���[�W���󂯂Ă��Ȃ��Ƃ�
					MoveVigilance();
				}
				CManager::m_pDebuglog->Print(1, "�G�̏�� �` �x�� �`\n");
				break;
			case ENEMYSTATSE_PURSUIT:
				if (m_bDamage == true)
				{// �_���[�W���󂯂Ă��Ȃ��Ƃ�
					if (m_bAttack == true)
					{// �U�����Ă��Ȃ��Ƃ�
						MovePursuit();
					}
					AttackMove();
				}
				CManager::m_pDebuglog->Print(1, "�G�̏�� �` �ǐ� �`\n");
				break;
			case ENEMYSTATSE_DAMAGE:
				CManager::m_pDebuglog->Print(1, "�G�̏�� �` �_���[�W �`\n");
				break;
			case ENEMYSTATSE_DEDA:
				MoveDeda();
				CManager::m_pDebuglog->Print(1, "�G�̏�� �` ���S �`\n");
				break;
			}

			// �����蔻��
			if (m_nStatse != ENEMYSTATSE_DEDA)
			{
				CollisionPlayer();									// �v���C���[�Ƃ̓����蔻��
				CollisionSearch();									// �{���̔���
				CollisitionWall();									// �ǂ̓����蔻��
				CollisitionObject3D(&m_pos, &m_posOld, &m_move);	// �I�u�W�F�N�g�̓����蔻��
			}

			if (m_nLife <= 0)
			{// ���C�t��0�ɂȂ����Ƃ�
				m_nStatse = ENEMYSTATSE_DEDA;		// �X�e�[�^�X�̕ύX
				m_nAnimnow = ENEMYANIM_DEDA;		// ���[�V�����̕ύX
				pSound->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DEAD);
			}

			// ���[�V�����̍X�V
			m_nMotionType = m_nAnimnow;
			UpdateMotion();

			m_nCntSearch++;		// �J�E���^�[�̉��Z
		}
		break;
	case CManager::MODE_RESULT:
		m_nAnimnow = ENEMYANIM_DEDA;		// ���[�V�����̕ύX
		// ���[�V�����̍X�V
		m_nMotionType = m_nAnimnow;
		UpdateMotion();
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
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

	//�v���C���[�̈ʒu���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//�߂��̓G�ȊO������
	if (m_pos.x + 5000.0f >= PlayerPos.x && m_pos.x - 5000.0f <= PlayerPos.x)
	{
		for (int nCntParts = 0; nCntParts < MAX_ENEMYPARTS; nCntParts++)
		{
			if (m_pModel[nCntParts] != NULL)
			{
				m_pModel[nCntParts]->Draw();
			}
		}
	}
}

//*************************************************************************************************************
// ���ꂼ��̏�ԓ���
//=============================================================================
// �ʏ�̏���
//=============================================================================
void CEnemy::MoveNone(void)
{
	m_fWalkFrame++;		// �J�E���^�[�̉��Z
	if (m_bMove == true)
	{// �ړ����Ă���Ƃ�
		if (m_fWalkFrame % m_fMoveFlame == 0)
		{// �����̐؂�ւ�
			m_fMoveFlame	= (rand() % 400 + 150);
			m_bMove			= false;
		}

		// �ړ��ʂ̍X�V
		m_move.x += sinf((D3DX_PI)+m_rot.y) * (ENEMY_SPEED);
		m_move.z += cosf((D3DX_PI)+m_rot.y) * (ENEMY_SPEED);

		// ����
		m_move.x += (0.0f - m_move.x) * 0.3f;
		m_move.y += (0.0f - m_move.y) * 0.3f;
		m_move.z += (0.0f - m_move.z) * 0.3f;

		// �ʒu�X�V�i�ړ��j
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
		m_pos.z += m_move.z;

		m_nAnimnow = ENEMYANIM_WALK;		// ���[�V�����̕ύX
	}
	else if (m_bMove == false)
	{// �ړ����Ă��Ȃ��Ƃ�
		if (m_fWalkFrame % m_fMoveFlame == 0)
		{// �����̐؂�ւ�
			m_fMoveFlame	= (rand() % 400 + 150);
			m_rot			= D3DXVECTOR3(0.0f, (float)(rand() % 10), 0.0f);
			m_bMove			= true;
		}
		m_nAnimnow = ENEMYANIM_OVERLOOKING;		// ���[�V�����̕ύX
	}
}

//=============================================================================
// �x���̏���
//=============================================================================
void CEnemy::MoveVigilance(void)
{
	// �����̐���
	float fAngle = atan2f(m_VigilancePos.x - m_pos.x, m_VigilancePos.z - m_pos.z) + D3DX_PI;

	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2;
	}

	m_rot.y = fAngle;	// �����ɑ��

	// �e�L�X�g�f�[�^������W�����擾����
	m_PosDest = m_VigilancePos;

	// �������v�Z����
	m_posAngle.x = m_PosDest.x - m_pos.x;		// ���� = �ړI - ����
	m_posAngle.z = m_PosDest.z - m_pos.z;		// ���� = �ړI - ����

	// ���W�̍X�V
	m_pos.x += m_posAngle.x * ENEMY_VIGILANCESPEED;
	m_pos.z += m_posAngle.z * ENEMY_VIGILANCESPEED;

	m_nAnimnow = ENEMYANIM_WALK;		// ���[�V�����̕ύX

	CManager::m_pDebuglog->Print(1, "�����@�` %.0f , %.0f �`\n", m_posAngle.x, m_posAngle.z);
}
//=============================================================================
// �ǐՂ̏���
//=============================================================================
void CEnemy::MovePursuit(void)
{
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();		// �v���C���[���W�̎擾

	// �����̐���
	float fAngle = atan2f(PlayerPos.x - m_pos.x, PlayerPos.z - m_pos.z) + D3DX_PI;

	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2;
	}

	m_rot.y = fAngle;	// �����ɑ��

	// �e�L�X�g�f�[�^������W�����擾����
	m_PosDest = PlayerPos;

	// �������v�Z����
	m_posAngle.x = m_PosDest.x - m_pos.x;		// ���� = �ړI - ����
	m_posAngle.z = m_PosDest.z - m_pos.z;		// ���� = �ړI - ����

	// ���W�̍X�V
	m_pos.x += m_posAngle.x * ENEMY_PURSUITSPEED;
	m_pos.z += m_posAngle.z * ENEMY_PURSUITSPEED;

	m_nAnimnow = ENEMYANIM_RUN;		// ���[�V�����̕ύX
}
//=============================================================================
// ���S�̏���
//=============================================================================
void CEnemy::MoveDeda(void)
{
	m_nStatse = ENEMYSTATSE_NONE;	// �X�e�[�^�X�̕ύX
}
//*************************************************************************************************************

//=============================================================================
// �U���̏���
//=============================================================================
void CEnemy::AttackMove(void)
{
	CPlayer						*pPlayer	= CGame::GetPlayer();		// �v���C���[�̏����擾
	D3DXVECTOR3					PlayerPos	= pPlayer->GetPos();		// �v���C���[�̈ʒu���擾
	CPlayer::PlayerStageStatus	PlayerState	= pPlayer->GetStatus();		// �v���C���[�̏�Ԃ��擾
	CSound						*pSound		= CManager::GetSound();		// �T�E���h���̎擾

	if (m_pModel[15] != NULL)
	{// ����̃|�C���^�̂Ƃ�
		// ����̏�Ԃ��擾
		D3DXMATRIX apWorldmtx = m_pModel[15]->GetMtxWorld();	// ����

		// �G�Ǝ����̋����𑪂�
		float fLength = (((PlayerPos.x - m_pos.x) * (PlayerPos.x - m_pos.x)) + ((PlayerPos.z - m_pos.z) * (PlayerPos.z - m_pos.z)));
		// �ڕW�Ƃ̍������o��
		float fWeapon = (apWorldmtx._41 - PlayerPos.x) * (apWorldmtx._41 - PlayerPos.x) + (apWorldmtx._43 - PlayerPos.z) * (apWorldmtx._43 - PlayerPos.z);	// �v���C���[

		if (!(PlayerPos.y - PLAYER_DOWN_HEIGHT >= m_vtxMax.y) && !(PlayerPos.y + PLAYER_UP_HEIGHT <= m_vtxMin.y))
		{// Y���̔���
			if (ENEMY_ATTACK * ENEMY_ATTACK > fLength && m_bAttack == true && m_bDamage == true && m_nCntAttack > 180)
			{// �U���͈͓̔���������
				m_bAttack		= false;				// �U����Ԃ�
				m_nAnimnow		= ENEMYANIM_ATTACK;		// ���[�V�����̕ύX
				m_nCntAttack	= 0;					// �J�E���^�[�̃��Z�b�g
			}

			if (fWeapon <= 500.0f * 500.0f && PlayerState == CPlayer::PLAYERSTAGESTATUS_NONE && m_nAnimnow == ENEMYANIM_ATTACK)
			{// �����蔻��
				int &pPlayerLife = CPlayer::GetLife();		// �v���C���[�̗̑͂̎擾
				if (pPlayerLife > 0)
				{// �v���[���[�̗̑͂����鎞
					pPlayerLife -= 1;										// �̗͂̌��Z
					pPlayer->SetLife(pPlayerLife);							// �̗͂̐ݒ�
					pPlayer->SetStatus(CPlayer::PLAYERSTAGESTATUS_DAMAGE);	// �v���C���[���_���[�W��Ԃ�
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_DAMEG);
				}
			}
		}
		m_nCntAttack++;		// �J�E���^�[�̉��Z
	}
}

//=============================================================================
// ���[�V�����̍X�V
//=============================================================================
void CEnemy::UpdateMotion(void)
{
	//���[�V����
	KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;

	//�L�[���ő吔������Ȃ��悤��
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	//���[�V�����X�V
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			//���݂̃L�[���擾
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			//���̃L�[���擾
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			//���݂̃L�[���玟�̃L�[�ւ̍Đ��t���[�����ɂ����郂�[�V�����J�E���^�[�̑��Βl���Z�o
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			//ROT
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);
			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);

			//ROT
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);

			//ROT
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);

			//�p�[�c�𓮂���
			m_pModel[nCntParts]->Setrot(rotmotion);

			//POS
			m_pModel[nCntParts]->SetPos(D3DXVECTOR3(m_OffSetPos[nCntParts].x + posmotion.x,
				m_OffSetPos[nCntParts].y + posmotion.y,
				m_OffSetPos[nCntParts].z + posmotion.z));

		}
	}

	//���[�v�̔���
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:

		//���[�v����
		//�t���[����i�߂�
		m_nCountFlame++;
		//�L�[�̍X�V
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{// m_nCountFlame �� �t���[�����ɒB������
			if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
			{// �Ō�̃L�[��������0�ɖ߂�
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
		//���[�v���Ȃ�
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{//�t���[����i�߂�
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			if (m_nAnimnow == ENEMYANIM_ATTACK)
			{//�U�����[�V����
				m_nAttackDelay++;
				if (m_nAttackDelay > 40)
				{
					m_bAttack = true;
					m_nAttackDelay = 0;
				}
			}
			if (m_nAnimnow == ENEMYANIM_DAMAGE)
			{//�_���[�W���[�V����
				m_nDamageDelay++;
				if (m_nDamageDelay > 20)
				{
					m_bDamage = true;
					m_nDamageDelay = 0;
					m_nAnimnow = ENEMYANIM_OVERLOOKING;
				}
			}
			m_bMotionEnd = true;
		}
		//�L�[�̍X�V
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
}

//=============================================================================
// �G�̓����蔻��
//=============================================================================
void CEnemy::Collision(D3DXVECTOR3 posPlayer)
{
	// �G�Ǝ����̋����𑪂�
	float fLength = (((m_pos.x - posPlayer.x) * (m_pos.x - posPlayer.x)) + ((m_pos.z - posPlayer.z) * (m_pos.z - posPlayer.z)));

	// �����ƓG�̊p�x�𑪂�
	float fAngle = atan2f(m_pos.x - posPlayer.x, m_pos.z - posPlayer.z);

	if (ENEMY_RADIUS * ENEMY_RADIUS > fLength)
	{// �͈͓���������
		m_pos.x = posPlayer.x + sinf(fAngle) * ENEMY_RADIUS;
		m_pos.z = posPlayer.z + cosf(fAngle) * ENEMY_RADIUS;
	}
}

//=============================================================================
// �v���C���[�Ƃ̓����蔻��
//=============================================================================
void CEnemy::CollisionPlayer(void)
{
	for (int nCntPriotity = 0; nCntPriotity < NUM_PRIORITY; nCntPriotity++)
	{
		CScene *pScene;
		//�v���C�I���e�B�[�`�F�b�N
		pScene = CScene::GetTop(nCntPriotity);

		//NULL�`�F�b�N
		while (pScene != NULL)
		{
			//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetDeath() == false)
			{
				if (pScene->GetObjType() == OBJTYPE_PLAYER)
				{
					//�I�u�W�F�N�g�̂����锻��
					((CPlayer*)pScene)->Collision(m_pos);
				}
			}
			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �G�̃_���[�W����
//=============================================================================
void CEnemy::CollisionDamege(D3DXVECTOR3 posHand, CPlayer::PlayerAnim playerState)
{
	CSound *pSound = CManager::GetSound();
	if (playerState == CPlayer::PLAYERANIM_ATTACK)
	{
		int nNumBill = 0;
		for (int nCntModel = 0; nCntModel < MAX_ENEMYPARTS; nCntModel++)
		{
			if (m_pModel[nCntModel] != NULL)
			{
				D3DXMATRIX mtxRot, mtxTrans, mtxWorld;				// �v�Z�p�}�g���b�N�X
				int nCollisionNum;
				float fRadius;
				D3DXVECTOR3 CollisionMove;
				D3DXMATRIX ModelWorld = m_pModel[nCntModel]->GetMtxWorld();

				switch (nCntModel)
				{
				case 0:// ��
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 60.0f;		// ���a
					break;
				case 1:// ��
					nCollisionNum = 2;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(30.0f, 30.0f, 0.0f);	// �c�q�̂���
					fRadius = 80.0f;		// ���a
					break;
				case 2:// ��
					nCollisionNum = 3;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(20.0f, 10.0f, -10.0f);	// �c�q�̂���
					fRadius = 40.0f;		// ���a
					break;
				case 3:// ����r
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 4:// ���O�r
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 5:// ����
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 6:// �E��r
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 7:// �E�O�r
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 8:// �E��
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 9:// ����
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 10:// ����
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 11:// ����
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 12:// �E��
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 13:// �E��
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 14:// �E��
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				case 15:// ��
					nCollisionNum = 1;		// �c�q�̐�
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �c�q�̂���
					fRadius = 10.0f;		// ���a
					break;
				}
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorld);
				// ��]
				D3DXMatrixRotationYawPitchRoll(&mtxRot, ModelWorld._31, ModelWorld._32, ModelWorld._33);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
				// �ʒu
				D3DXMatrixTranslation(&mtxTrans, ModelWorld._41, ModelWorld._42, ModelWorld._43);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				for (int nCntCollision = 0; nCntCollision < nCollisionNum; nCntCollision++)
				{
					// �ʒu
					D3DXMatrixTranslation(&mtxTrans, (CollisionMove.x * nCntCollision), (CollisionMove.y * nCntCollision), (CollisionMove.z * nCntCollision));
					D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxWorld); // mtxWorld�̊p�x�𗘗p���Đ���

					// �G�Ǝ����̍����𑪂�
					float fRadiusEnemy = powf(fRadius * 3.0f, 2);

					float fLengthX = posHand.x - mtxTrans._41;
					float fLengthY = posHand.y - mtxTrans._42;
					float fLengthZ = posHand.z - mtxTrans._43;

					float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);

					if (fRadiusEnemy > fLengthTotal && m_bDamage == true)
					{
						m_nLife--;
						m_bDamage	= false;				// �_���[�W��Ԃ�
						m_bAttack	= true;					// �U�����Ă��Ȃ���Ԃ�
						m_nStatse	= ENEMYSTATSE_DAMAGE;	// �X�e�[�^�X�̕ύX
						m_nAnimnow	= ENEMYANIM_DAMAGE;		// ���[�V�����̕ύX
						CManager::m_pDebuglog->Print(1, "[%d]HIT!!\n", nCntModel);
						pSound->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DAMEG);
					}
					nNumBill++;
				}
			}
		}
	}
}

//=============================================================================
// �{���̔���
//=============================================================================
void CEnemy::CollisionSearch()
{
	CScene *pScene;
	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(BULLET_PRIOTITY);

	bool bCollision = false;
	bool bDistance	= false;

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//�^�C�v���G��������
			if (pScene->GetObjType() == OBJTYPE_BULLET)
			{//	�o���b�g�������Ƃ�
				bCollision = ((CBullet*)pScene)->CollisionBulletSearch();
				if (bCollision == true)
				{
					bDistance = CollisionDistance((CBullet*)pScene);
					break;
				}
				else
				{
					CBullet *pBullet	= (CBullet*)pScene;
					CEnemy	*pEnemy		= pBullet->GetEnemy();

					if (pEnemy->m_nStatse != ENEMYSTATSE_NONE)
					{// �ʏ��Ԃ̂Ƃ�
						if (pEnemy->m_nCntStatse >= 30000)
						{
							pEnemy->m_nStatse		= ENEMYSTATSE_NONE;		// �X�e�[�^�X�̕ύX
							pEnemy->m_nCntStatse	= 0;
						}
						pEnemy->m_nCntStatse++;		// �J�E���^�[�̉��Z
					}
				}
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �G�ƃv���C���[�̋���
//=============================================================================
bool CEnemy::CollisionDistance(CBullet *pBullet)
{
	D3DXVECTOR3 PlayerPos	= CGame::GetPlayer()->GetPos();		// �v���C���[���W�̎擾
	CEnemy		*pEnemy		= pBullet->GetEnemy();				// �G�l�~�[�̎擾
	bool bCollision			= false;

	// �G�Ǝ����̋����𑪂�
	float fLength = (((PlayerPos.x - pEnemy->m_pos.x) * (PlayerPos.x - pEnemy->m_pos.x)) + ((PlayerPos.z - pEnemy->m_pos.z) * (PlayerPos.z - pEnemy->m_pos.z)));

	if (ENEMY_SEARCH_VIGILANCE * ENEMY_SEARCH_VIGILANCE > fLength && pEnemy->m_bAttack == true && pEnemy->m_bDamage == true)
	{// �x���͈͓̔���������
		if (pEnemy->m_nStatse == ENEMYSTATSE_NONE)
		{// �ʏ��Ԃ������Ƃ�
			pEnemy->m_nStatse		= ENEMYSTATSE_VIGILANCE;		// �X�e�[�^�X�̕ύX
			pEnemy->m_VigilancePos	= PlayerPos;					// �ړI�n�̍X�V
			pEnemy->m_nCntStatse	= 0;
		}
		else if (ENEMY_SEARCH_PURSUIT * ENEMY_SEARCH_PURSUIT > fLength && pEnemy->m_bAttack == true && pEnemy->m_bDamage == true)
		{// �ǐՂ͈͓̔���������
			pEnemy->m_nStatse		= ENEMYSTATSE_PURSUIT;			// �X�e�[�^�X�̕ύX
			pEnemy->m_nCntStatse	= 0;
		}
		bCollision = true;
	}
	return bCollision;
}

//=============================================================================
// �ǂ̓����蔻��
//=============================================================================
void CEnemy::CollisitionWall(void)
{
	// �v���C�I���e�B�[�`�F�b�N
	CScene *pScene = CScene::GetTop(WALL_PRIOTITY);

	// NULL�`�F�b�N
	while (pScene != NULL)
	{
		// Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			// �^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{// �ǂ������Ƃ�
				// �ǂ̓����蔻��
				((CWall*)pScene)->CollisionWallEnemy(&m_pos, &m_posOld, &m_move, 20.0f);
			}
		}

		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
//	�I�u�W�F�N�g�̓����蔻��
//=============================================================================
void CEnemy::CollisitionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	// �v���C�I���e�B�[�`�F�b�N
	CScene *pScene = CScene::GetTop(OBJECT_PRIOTITY);

	// NULL�`�F�b�N
	while (pScene != NULL)
	{
		// Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			// �^�C�v�����b�V����������
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// �I�u�W�F�N�g�������Ƃ�
				// �I�u�W�F�N�g�̓����蔻��
				((CObject*)pScene)->CollisionObjectEnemy(pPos, pPosOld, pMove);
			}
		}

		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �t�@�C���ǂݍ���
//=============================================================================
void CEnemy::FileLoad(void)
{
	// �f�o�C�X���擾
	CRenderer			*pRenderer	= CManager::GetRenderer();
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();

	// �t�@�C���ǂݍ��ݗp�ϐ�
	FILE *pFile;		// �t�@�C���|�C���^
	char *pStrcur;		// ���݂̐擪�̕�����
	char aLine[256];	// ������
	char aStr[256];		// �ꎞ�ۑ�������
	int nIndex = 0;		// ���݂̃C���f�b�N�X
	int nWord = 0;		// �|�b�v�ŕԂ��ꂽ�l��ێ�

	D3DXVECTOR3 ParentPos;	// �e�̈ʒu�����擾
#if 1
	// �t�@�C�����J�� �ǂݍ���
	pFile = fopen(ENEMY_FILE, "r");
	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_ENEMYMOTION; )
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
				g_nNumEnemyModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nNumEnemyModel; nCntModel++)
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
						strcpy(&g_aFileNameEnemyModel[nCntModel][0], aStr);

						if (m_pMesh[nCntModel] == NULL)
						{
							// X�t�@�C���̓ǂݍ���
							D3DXLoadMeshFromX(&g_aFileNameEnemyModel[nCntModel][0],
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&m_pBuffMat[nCntModel],
								NULL,
								&m_nNumMat[nCntModel],
								&m_pMesh[nCntModel]);

							D3DXMATERIAL *pmat;			// �}�e���A���f�[�^�ւ̃|�C���^
							D3DMATERIAL9 *matDef;		// ���݂̃}�e���A���ۑ��p
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
char *CEnemy::ReadLine(FILE *pFile, char *pDst)
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
char * CEnemy::GetLineTop(char * pStr)
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
int CEnemy::PopString(char * pStr, char * pDest)
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
			// ���o��
			nWord += strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	// ������̐���Ԃ�
	return nWord;
}