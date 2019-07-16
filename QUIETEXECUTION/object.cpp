//=============================================================================
//
// �I�u�W�F�N�g�̏��� [object.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "camera.h"
#include "renderer.h"
#include "game.h"
#include "bullet.h"
#include "enemy.h"
#include "debuglog.h"
#include "particle.h"
#include "score.h"
#include "tutorial.h"
#include "warning.h"
#include "loadtext.h"
//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH			CObject::m_pMeshModel	[MAX_OBJECT]			= {};		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		CObject::m_pBuffMatModel[MAX_OBJECT]			= {};		// �}�e���A���̏��ւ̃|�C���^
DWORD				CObject::m_nNumMatModel	[MAX_OBJECT]			= {};		// �}�e���A���̏��
LPDIRECT3DTEXTURE9	CObject::m_pMeshTextures[MAX_OBJECT_TEXTURE]	= {};
int					CObject::m_nCrystalCounter						= NULL;		// �N���X�^���̊l����

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CObject::CObject() : CModel3D(OBJECT_PRIOTITY,CScene::OBJTYPE_OBJECT)
{
	m_pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ModelMove			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bCrystalMove		= false;
	m_fCrystalMoveFleam	= 0.0f;
	m_nCrystalCounter	= 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CObject::~CObject()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 3D���f����Init
	CModel3D::Init();

	// �ʒu�̏�����
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �ʒu�̑��
	CModel3D::SetPosition(pos);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject::Uninit(void)
{
	// 3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject::Update(void)
{
	switch (m_nType)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		// �N���X�^���̏㉺�ړ�
		m_fCrystalMoveFleam++;
		if (m_bCrystalMove == false)
		{// �㏸��
			m_pos.y += 0.2f;
		}
		else
		{// ���~��
			m_pos.y -= 0.2f;
		}
		CModel3D::SetPosition(m_pos);
		if (m_fCrystalMoveFleam >= CRYSTAL_MOVETIME)
		{// �㉺�̓���ւ�
			m_bCrystalMove = m_bCrystalMove ? false : true;
			m_fCrystalMoveFleam = 0.0f;
		}

		// �N���X�^���̉�]
		m_rot = D3DXVECTOR3(m_rot.x, m_rot.y + CRYSTAL_MOVE, m_rot.z);
		CModel3D::SetRot(m_rot);
		break;
	}

	// �����蔻��
	CollisionBullet();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	// �v���C���[�̈ʒu���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	// �߂��̃��f���ȊO������
	if (m_pos.x + 6000.0f >= PlayerPos.x && m_pos.x - 6000.0f <= PlayerPos.x)
	{
		// 3D���f����Draw
		CModel3D::Draw();
	}
}

//===============================================================================
//�@�N���G�C�g
//===============================================================================
CObject * CObject::Create(D3DXVECTOR3 pos, float move, int nTexType,int nObjectType, MOVETYPE nMovetype, int nCollision)
{
	CObject *pObject = NULL;

	// NULL�`�F�b�N
	if (pObject == NULL)
	{// �������̓��I�m��

		pObject = new CObject;

		if (pObject != NULL)
		{
			// ��ނ̐ݒ�
			pObject->BindModel(m_pMeshModel[nObjectType], m_pBuffMatModel[nObjectType], m_nNumMatModel[nObjectType], m_pMeshTextures[nTexType]);
			// �I�u�W�F�N�g�N���X�̐���
			pObject->Init();
			// �I�u�W�F�N�g���Ƃ̐ݒ�p�^�C�v
			pObject->m_nType = nObjectType;
			// �ʒu����
			pObject->SetPosition(pos);
			pObject->m_pos = pos;
			// �����̎�ނƈړ��ʂ�ݒ�
			pObject->SetMoveType(nMovetype);
			pObject->m_move = D3DXVECTOR3(move, move, move);
			// �R���W������ONOFF
			pObject->m_nCollision = nCollision;
		}
	}

	return pObject;
}

//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CObject::Load(int nMaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;			// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < nMaxModel; nCntModel++)
	{
		char *cMoedlName = {};
		cMoedlName = CLoadText::GetName(nCntModel, 0);
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(cMoedlName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[nCntModel], NULL, &m_nNumMatModel[nCntModel], &m_pMeshModel[nCntModel]);		// �M
	}

	for (int nCount = 0; nCount < nMaxModel; nCount++)
	{
		// �}�e���A����񂩂�e�N�X�`���̎擾
		pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCount]->GetBufferPointer();
	}

	// �g���Ă���e�N�X�`��
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\UV\\object\\barrel.jpg",&m_pMeshTextures[0]);			// �M
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\UV\\object\\stone_UV.png", &m_pMeshTextures[1]);		// ��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\UV\\object\\crystal_UV.jpg", &m_pMeshTextures[2]);	// �N���X�^��

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CObject::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		// ���b�V���̊J��
		if (m_pMeshModel[nCount] != NULL)
		{
			m_pMeshModel[nCount]->Release();
			m_pMeshModel[nCount] = NULL;
		}
		// �}�e���A���̊J��
		if (m_pBuffMatModel[nCount] != NULL)
		{
			m_pBuffMatModel[nCount]->Release();
			m_pBuffMatModel[nCount] = NULL;
		}
	}
	//�e�N�X�`��
	for (int nCntTex = 0; nCntTex < MAX_OBJECT_TEXTURE; nCntTex++)
	{
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
}

//===============================================================================
// �����蔻��
//===============================================================================
bool CObject::CollisionObject(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove, bool bTouchOld)
{
	// �������Ă��邩�������ĂȂ���
	bool bTouch		= false;
	int nCollision	= m_nCollision;

	// �e����̎擾(�ǂ̃��f���ł��g�p����)
	D3DXVECTOR3 ModelPos	= CModel3D::GetPosition();		// �ʒu
	D3DXVECTOR3 ModelMove	= CModel3D::GetMove();			// �ړ���
	D3DXVECTOR3 VtxMax		= CModel3D::VtxMax();			// ���f���̍ő�l
	D3DXVECTOR3 VtxMin		= CModel3D::VtxMin();			// ���f���̍ŏ��l

	D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::VtxMax();	// �ʒu���݂̍ő�l
	D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::VtxMin();	// �ʒu���݂̍ŏ��l

	switch (m_nCollision)
	{
	case 1: // �⁕�M

		// �ړ��ʂ̕ێ�
		if (ModelMove.x == 0.0f)
		{
			ModelMove.x = m_ModelMove.x;
		}
		if (ModelMove.y == 0.0f)
		{
			ModelMove.y = m_ModelMove.y;
		}
		if (ModelMove.z == 0.0f)
		{
			ModelMove.z = m_ModelMove.z;
		}

		if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
		{// Z�͈͓̔��ɂ���
			if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
			{// X�͈͓̔��ɂ���
				if (pPosOld->y - PLAYER_DOWN_HEIGHT >= ModelMax.y && pPos->y - PLAYER_DOWN_HEIGHT <= ModelMax.y)
				{// �I�u�W�F�N�g�̏ォ�瓖����ꍇ
					pPos->y = ModelMax.y + abs(ModelMove.y) + PLAYER_DOWN_HEIGHT;
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X�ړ�����
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z�ړ�����
						pPos->z += ModelMove.z;
					}

					bTouch = true;
				}
				else if (pPosOld->y + PLAYER_UP_HEIGHT <= ModelMin.y && pPos->y + PLAYER_UP_HEIGHT >= ModelMin.y)
				{// �I�u�W�F�N�g�̉����瓖����ꍇ
					pPos->y = ModelMin.y - PLAYER_UP_HEIGHT;
					pMove->y = 0.0f;
				}

				if (!(pPos->y - PLAYER_DOWN_HEIGHT >= ModelMax.y) && !(pPos->y + PLAYER_UP_HEIGHT <= ModelMin.y))
				{// �I�u�W�F�N�g���Ƃ̓����蔻��
					if (pPosOld->x - PLAYER_DEPTH >= ModelMax.x && pPos->x - PLAYER_DEPTH < ModelMax.x)
					{// �E���甲�����ꍇ
						pPos->x = ModelMax.x + PLAYER_DEPTH;
					}
					else if (pPosOld->x + PLAYER_DEPTH <= ModelMin.x && pPos->x + PLAYER_DEPTH > ModelMin.x)
					{// �����甲�����ꍇ
						pPos->x = ModelMin.x - PLAYER_DEPTH;
					}
					if (pPosOld->z - PLAYER_DEPTH >= ModelMax.z && pPos->z - PLAYER_DEPTH < ModelMax.z)
					{// �����甲�����ꍇ
						pPos->z = ModelMax.z + PLAYER_DEPTH;
					}
					else if (pPosOld->z + PLAYER_DEPTH <= ModelMin.z && pPos->z + PLAYER_DEPTH > ModelMin.z)
					{// ��O���甲�����ꍇ
						pPos->z = ModelMin.z - PLAYER_DEPTH;
					}
				}
			}
		}

		// �ʒu�̑��
		CModel3D::SetPosition(ModelPos);
		// �ړ��ʂ̑��
		m_ModelMove = ModelMove;

		break;

	case 2:// �N���X�^��
		// �ړ��ʂ̕ێ�
		if (ModelMove.x == 0.0f)
		{
			ModelMove.x = m_ModelMove.x;
		}

		if (ModelMove.y == 0.0f)
		{
			ModelMove.y = m_ModelMove.y;
		}

		if (ModelMove.z == 0.0f)
		{
			ModelMove.z = m_ModelMove.z;
		}

		if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
		{// Z�͈͓̔��ɂ���
			if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
			{// X�͈͓̔��ɂ���
				if (pPosOld->y - PLAYER_DOWN_HEIGHT >= ModelMax.y && pPos->y - PLAYER_DOWN_HEIGHT <= ModelMax.y)
				{// �I�u�W�F�N�g�̏ォ�瓖����ꍇ
					pPos->y = ModelMax.y + abs(ModelMove.y) + PLAYER_DOWN_HEIGHT;
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X�ړ�����
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z�ړ�����
						pPos->z += ModelMove.z;
					}
					bTouch = true;
				}
				else if (pPosOld->y + PLAYER_UP_HEIGHT <= ModelMin.y && pPos->y + PLAYER_UP_HEIGHT >= ModelMin.y)
				{// �I�u�W�F�N�g�̉����瓖����ꍇ
					pPos->y = ModelMin.y - PLAYER_UP_HEIGHT;
					pMove->y = 0.0f;
					bTouch = true;
				}

				if (!(pPos->y - PLAYER_DOWN_HEIGHT >= ModelMax.y) && !(pPos->y + PLAYER_UP_HEIGHT <= ModelMin.y))
				{// �I�u�W�F�N�g���Ƃ̓����蔻��
					if (pPosOld->x - PLAYER_DEPTH >= ModelMax.x && pPos->x - PLAYER_DEPTH < ModelMax.x)
					{// �E���甲�����ꍇ
						pPos->x = ModelMax.x + PLAYER_DEPTH;
						bTouch = true;
					}
					else if (pPosOld->x + PLAYER_DEPTH <= ModelMin.x && pPos->x + PLAYER_DEPTH > ModelMin.x)
					{// �����甲�����ꍇ
						pPos->x = ModelMin.x - PLAYER_DEPTH;
						bTouch = true;
					}
					if (pPosOld->z - PLAYER_DEPTH >= ModelMax.z && pPos->z - PLAYER_DEPTH < ModelMax.z)
					{// �����甲�����ꍇ
						pPos->z = ModelMax.z + PLAYER_DEPTH;
						bTouch = true;
					}
					else if (pPosOld->z + PLAYER_DEPTH <= ModelMin.z && pPos->z + PLAYER_DEPTH > ModelMin.z)
					{// ��O���甲�����ꍇ
						pPos->z = ModelMin.z - PLAYER_DEPTH;
						bTouch = true;
					}
				}
			}
		}
		// �ʒu�̑��
		CModel3D::SetPosition(ModelPos);
		// �ړ��ʂ̑��
		m_ModelMove = ModelMove;

		if (bTouch == true)
		{// �G��Ă��������
			CFade::FADE fade = CFade::GetFade();
			CManager::MODE m_mode = CManager::GetMode();
			m_nCrystalCounter++;
			if (m_mode == CManager::MODE_GAME)
			{
				CParticle::Create(m_pos, 0, 0);
				Uninit();
				if (m_nCrystalCounter >= CLEAR_CRYSTALNUM)
				{
					CManager::SetResult(CManager::GAMERESULT_WIN);

					if (fade != CFade::FADE_OUT)
					{
						//���[�h�ݒ�
						CFade::SetFade(CManager::MODE_RESULT);
					}
				}
				CScore::AddScore(-1);
			}
			else if(m_mode == CManager::MODE_TUTORIAL && bTouchOld == false)
			{
				if (fade != CFade::FADE_OUT)
				{
					CTutorial *pGame = CManager::GetTutorial();
					CTutorial *pWarning = ((CTutorial*)pGame);
					bool bWarning = pWarning->GetWarning();
					pWarning->SetWarning(true);
				}
			}
		}
		break;
	}
	return bTouch;
}

//===============================================================================
// �G�l�~�[�Ƃ̓����蔻��
//===============================================================================
bool CObject::CollisionObjectEnemy(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove)
{
	//�������Ă��邩�������ĂȂ���
	bool bLand		= false;
	int nCollision	= m_nCollision;

	switch (m_nCollision)
	{
	case 1:

		// �e����̎擾
		D3DXVECTOR3 ModelPos	= CModel3D::GetPosition();		// �ʒu
		D3DXVECTOR3 ModelMove	= CModel3D::GetMove();			// �ړ���
		D3DXVECTOR3 VtxMax		= CModel3D::VtxMax();			// ���f���̍ő�l
		D3DXVECTOR3 VtxMin		= CModel3D::VtxMin();			// ���f���̍ŏ��l

		D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::VtxMax();	// �ʒu���݂̍ő�l
		D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::VtxMin();	// �ʒu���݂̍ŏ��l

		// �ړ��ʂ̕ێ�
		if (ModelMove.x == 0.0f)
		{
			ModelMove.x = m_ModelMove.x;
		}

		if (ModelMove.y == 0.0f)
		{
			ModelMove.y = m_ModelMove.y;
		}

		if (ModelMove.z == 0.0f)
		{
			ModelMove.z = m_ModelMove.z;
		}

		if (pPos->x + ENEMY_DEPTH >= ModelMin.x && pPos->x - ENEMY_DEPTH <= ModelMax.x)
		{// Z�͈͓̔��ɂ���
			if (pPos->z + ENEMY_DEPTH >= ModelMin.z && pPos->z - ENEMY_DEPTH <= ModelMax.z)
			{// X�͈͓̔��ɂ���
				if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
				{// �I�u�W�F�N�g�̏ォ�瓖����ꍇ
					pPos->y = ModelMax.y + abs(ModelMove.y);
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X�ړ�����
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z�ړ�����
						pPos->z += ModelMove.z;
					}

					//bLand = true;
				}
				else if (pPosOld->y + ENEMY_HEIGHT <= ModelMin.y && pPos->y + ENEMY_HEIGHT >= ModelMin.y)
				{// �I�u�W�F�N�g�̉����瓖����ꍇ
					pPos->y = ModelMin.y - ENEMY_HEIGHT;
					pMove->y = 0.0f;
				}

				if (!(pPos->y >= ModelMax.y) && !(pPos->y + ENEMY_HEIGHT <= ModelMin.y))
				{// �I�u�W�F�N�g���Ƃ̓����蔻��
					if (pPosOld->x - ENEMY_DEPTH >= ModelMax.x && pPos->x - ENEMY_DEPTH < ModelMax.x)
					{// �E���甲�����ꍇ
						pPos->x = ModelMax.x + ENEMY_DEPTH;
						bLand = true;

						CManager::m_pDebuglog->Print(1, "�E\n");
					}
					else if (pPosOld->x + ENEMY_DEPTH <= ModelMin.x && pPos->x + ENEMY_DEPTH > ModelMin.x)
					{// �����甲�����ꍇ
						pPos->x = ModelMin.x - ENEMY_DEPTH;
						bLand = true;
						CManager::m_pDebuglog->Print(1, "��\n");
					}
					if (pPosOld->z - ENEMY_DEPTH >= ModelMax.z && pPos->z - ENEMY_DEPTH < ModelMax.z)
					{// �����甲�����ꍇ
						pPos->z = ModelMax.z + ENEMY_DEPTH;
						bLand = true;
						CManager::m_pDebuglog->Print(1, "��\n");
					}
					else if (pPosOld->z + ENEMY_DEPTH <= ModelMin.z && pPos->z + ENEMY_DEPTH > ModelMin.z)
					{// ��O���甲�����ꍇ
						pPos->z = ModelMin.z - ENEMY_DEPTH;
						bLand = true;
						CManager::m_pDebuglog->Print(1, "��O\n");
					}
				}
			}
		}

		// �ʒu�̑��
		CModel3D::SetPosition(ModelPos);
		// �ړ��ʂ̑��
		m_ModelMove = ModelMove;

		break;
	}

	return bLand;
}

//=============================================================================
// �e�̔���
//=============================================================================
void CObject::CollisionBullet()
{
	CScene *pScene;
	bool bCollision = false;

	// �v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(BULLET_PRIOTITY);

	// NULL�`�F�b�N
	while (pScene != NULL)
	{
		// Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();
		if (pScene->GetDeath() == false)
		{
			if (pScene->GetObjType() == OBJTYPE_BULLET)
			{// �^�C�v���e��������
				bCollision = ((CBullet*)pScene)->CollisionBulletObje(this);
			}
		}
		// Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}