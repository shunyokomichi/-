//=============================================================================
//
// �Ǐ��� [wall.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CWall::m_pTexture[MAX_WALL_TEX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CWall::CWall() : CScene3D(WALL_PRIOTITY, CScene::OBJTYPE_WALL)
{
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size	= D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWall::~CWall()
{
}

//=============================================================================
// �ǂݍ��ݏ���
//=============================================================================
HRESULT CWall::Load(void)
{
	// �f�o�C�X�擾
	CRenderer			*pRenderer	= CManager::GetRenderer();		// �����_���[�̎擾
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();		// �f�o�C�X���擾

	// �e�N�X�`������
	D3DXCreateTextureFromFile(pDevice, WALL_TEX_0, &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CWall::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	{
		// �e�N�X�`���j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CWall * CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, int nTexType)
{
	CWall *pWall = NULL;

	// NULL�`�F�b�N
	if (pWall == NULL)
	{// �������̓��I�m��

		pWall = new CWall;

		if (pWall != NULL)
		{
			// ��ނ̐ݒ�
			pWall->m_nType = nTexType;
			// ����������
			pWall->Init();
			// �ʒu�̐ݒ�
			pWall->m_pos = pos;
			// �����蔻��p
			pWall->m_size = size;
			// �e�N�X�`���̐ݒ�
			pWall->BindTexture(m_pTexture[nTexType]);
			// �ʒu�Ɖ�]�ƃT�C�Y
			pWall->SetWall(pos, rot, size);
		}
	}

	return pWall;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWall::Init()
{
	// ����������
	CScene3D::Init();

	// ��ނ̐ݒ�
	SetTypeNumber(TYPE_WALL);

	if (m_nType == 9)
	{
		CScene3D::SetNor(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWall::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}

	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWall::Update(void)
{
	CollisionBullet();		// �e�Ƃ̓����蔻��
	CScene3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWall::Draw(void)
{
	D3DXVECTOR3		PlayerPos	= CGame::GetPlayer()->GetPos();			// �v���C���[���W�̎擾
	CManager::MODE	pMode		= CManager::GetMode();					// �Q�[�����[�h�̎擾

	// �߂��̕ǈȊO������
	if (m_pos.x + 6000.0f >= PlayerPos.x && m_pos.x - 6000.0f <= PlayerPos.x)
	{
		// 3D���f����Draw
		CScene3D::Draw();
	}
	else if (pMode == CManager::MODE_TITLE || pMode == CManager::MODE_RESULT)
	{
		CScene3D::Draw();
	}
}
//=============================================================================
// �v���C���[�Ƃ̓����蔻��
//=============================================================================
void CWall::CollisionWallPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth)
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
		{// �^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{// pos��rot���擾
				D3DXVECTOR3 WallPos		= ((CScene3D*)pScene)->GetPosition();
				D3DXVECTOR3 WallRot		= ((CScene3D*)pScene)->GetRotation();
				D3DXVECTOR2 WallSize	= ((CScene3D*)pScene)->GetSize();

				if (WallRot.y == (0.0f * D3DX_PI))
				{// Z�����̕�
					if (pPos->x <= WallPos.x + WallSize.x + 18.5f
						&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
					{// �ǂƓ���z���ɑ��݂��Ă���
						if (pPos->z <= WallPos.z + WallSize.x + 18.5f
							&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
						{// �ǂƓ���x���ɑ��݂��Ă���
							if (pPos->y - PLAYER_DOWN_HEIGHT < WallPos.y + WallSize.y || pPos->y - PLAYER_DOWN_HEIGHT >= WallPos.y)
							{// �ǂ����Ⴂ
								if (pPos->z >= WallPos.z - fDepth
									&& pPosOld->z <= WallPos.z - fDepth)
								{// ��O����ǂ��ђʂ���
									pPos->z = WallPos.z - fDepth;
								}
								else if (pPos->z <= WallPos.z + fDepth
									&& pPosOld->z >= WallPos.z + fDepth)
								{// ������ǂ��ђʂ���
									pPos->z = WallPos.z + fDepth;
								}
							}
						}
					}
				}
				else if (WallRot.y == (0.5f * D3DX_PI))
				{// X�����̕�
					if (pPos->x <= WallPos.x + WallSize.x + 18.5f
						&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
					{// �ǂƓ���z���ɑ��݂��Ă���
						if (pPos->z <= WallPos.z + WallSize.x + 18.5f
							&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
						{// �ǂƓ���x���ɑ��݂��Ă���
							if (pPos->y - PLAYER_DOWN_HEIGHT < WallPos.y + WallSize.y || pPos->y - PLAYER_DOWN_HEIGHT >= WallPos.y)
							{// �ǂ����Ⴂ
								if (pPos->x >= WallPos.x - fDepth
									&& pPosOld->x <= WallPos.x - fDepth)
								{// ��O����ǂ��ђʂ���
									pPos->x = WallPos.x - fDepth;
								}
								else if (pPos->x <= WallPos.x + fDepth
									&& pPosOld->x >= WallPos.x + fDepth)
								{// ������ǂ��ђʂ���
									pPos->x = WallPos.x + fDepth;
								}
							}
						}
					}
				}
			}
		}
		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �G�Ƃ̓����蔻��
//=============================================================================
void CWall::CollisionWallEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth)
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
		{// �^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{// pos��rot���擾
				D3DXVECTOR3 WallPos		= ((CScene3D*)pScene)->GetPosition();
				D3DXVECTOR3 WallRot		= ((CScene3D*)pScene)->GetRotation();
				D3DXVECTOR2 WallSize	= ((CScene3D*)pScene)->GetSize();

				if (WallRot.y == (0.0f * D3DX_PI))
				{// Z�����̕�
					if (pPos->x <= WallPos.x + WallSize.x + 18.5f
						&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
					{// �ǂƓ���z���ɑ��݂��Ă���
						if (pPos->z <= WallPos.z + WallSize.x + 18.5f
							&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
						{// �ǂƓ���x���ɑ��݂��Ă���
							if (pPos->y < WallPos.y + WallSize.y && pPos->y + ENEMY_HEIGHT >= WallPos.y)
							{// �ǂ����Ⴂ
								if (pPos->z >= WallPos.z - fDepth
									&& pPosOld->z <= WallPos.z - fDepth)
								{// ��O����ǂ��ђʂ���
									pPos->z = WallPos.z - fDepth;
								}
								else if (pPos->z <= WallPos.z + fDepth
									&& pPosOld->z >= WallPos.z + fDepth)
								{// ������ǂ��ђʂ���
									pPos->z = WallPos.z + fDepth;
								}
							}
						}
					}
				}
				else if (WallRot.y == (0.5f * D3DX_PI))
				{// X�����̕�
					if (pPos->x <= WallPos.x + WallSize.x + 18.5f
						&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
					{// �ǂƓ���z���ɑ��݂��Ă���
						if (pPos->z <= WallPos.z + WallSize.x + 18.5f
							&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
						{// �ǂƓ���x���ɑ��݂��Ă���
							if (pPos->y < WallPos.y + WallSize.y && pPos->y + ENEMY_HEIGHT >= WallPos.y)
							{// �ǂ����Ⴂ
								if (pPos->x >= WallPos.x - fDepth
									&& pPosOld->x <= WallPos.x - fDepth)
								{// ��O����ǂ��ђʂ���
									pPos->x = WallPos.x - fDepth;
								}
								else if (pPos->x <= WallPos.x + fDepth
									&& pPosOld->x >= WallPos.x + fDepth)
								{// ������ǂ��ђʂ���
									pPos->x = WallPos.x + fDepth;
								}
							}
						}
					}
				}
			}
		}
		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �e�̔���
//=============================================================================
void CWall::CollisionBullet()
{
	CScene *pScene;
	// �v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(BULLET_PRIOTITY);

	bool bCollision = false;

	// NULL�`�F�b�N
	while (pScene != NULL)
	{
		// Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();
		if (pScene->GetDeath() == false)
		{
			if (pScene->GetObjType() == OBJTYPE_BULLET)
			{// �^�C�v���e��������
				bCollision = ((CBullet*)pScene)->CollisionBulletWall(this);
			}
		}
		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}