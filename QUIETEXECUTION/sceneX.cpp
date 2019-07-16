//=============================================================================
//
// ���f������ [sceneX.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "sceneX.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH		CSceneX::m_pMash	= NULL;
LPD3DXBUFFER	CSceneX::m_pBuffMat = NULL;
DWORD			CSceneX::m_nNumMat	= NULL;

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(int nPriority) :CScene(nPriority)
{
	m_mtxWorld		= {};
	m_bColChange	= false;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{
}

//=============================================================================
//	��������
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos)
{
	CSceneX *pSceneX = NULL;

	pSceneX = new CSceneX;
	pSceneX->Init();

	return pSceneX;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneX::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	// �ʒu�E�����E�F�̏����ݒ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	int		nNumVtx;		// ���_��
	DWORD	sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	// ���_���̎擾
	nNumVtx = m_pMash->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMash->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMash->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			// ���_���W�̑��

		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}
		if (vtx.x > m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		pVtxBuff += sizeFVF;			// �T�C�Y���̃|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMash->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSceneX::Uninit(void)
{
	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneX::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneX::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX		mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9	matDef;					// ���݂̃}�e���A��
	D3DXMATERIAL	*pMat;					// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX		mtxParent;				// �y�A�����g�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_nIdxModelParent == -1)
	{
		mtxParent = m_mtxWorld;
	}
	else
	{
		mtxParent = m_mtxWorld;
	}

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

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	float fCol_R = 0.0f;		// �F(��)

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (m_bColChange == true)
		{
			m_col.r = pMat[nCntMat].MatD3D.Diffuse.r;
			pMat[nCntMat].MatD3D.Diffuse.r = fCol_R;
		}

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		pDevice->SetTexture(0, NULL);
		// ���f��(�p�[�c)�̕`��
		m_pMash->DrawSubset(nCntMat);

		if (m_bColChange == true)
		{	// �J���[�ύX
			pMat[nCntMat].MatD3D.Diffuse.r = fCol_R;
		}
	}
	// �}�e���A����Ԃ�
	pDevice->SetMaterial(&matDef);

	m_bColChange = false;		// �ύX�I��
}

//=============================================================================
// �}�g���b�N�X���̐ݒ�
//=============================================================================
void CSceneX::BindModel(LPD3DXMESH Mash, LPD3DXBUFFER BuffMat, DWORD NumMat)
{
	m_pMash = Mash;
	m_pBuffMat = BuffMat;
	m_nNumMat = NumMat;
}

//=============================================================================
// ���W�̐ݒ�
//=============================================================================
void CSceneX::Set(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
