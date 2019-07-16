//=============================================================================
//
// �I�u�W�F�N�g�̏��� [model3D.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "renderer.h"
#include <math.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//===============================================================================
//�@�I�[�o�[���[�h�R���X�g���N�^
//===============================================================================
CModel3D::CModel3D(int nPriority,CScene::OBJTYPE objType) : CScene(nPriority,objType)
{
	m_pMeshObject = NULL;					// ���b�V�����ւ̃|�C���^
	m_pBuffMatObject = NULL;				// �}�e���A���̏��ւ̃|�C���^
	m_nNumMatObject = 0;					// �}�e���A���̏��
	m_mtxWorldObject;						// ���[���h�}�g���b�N�X
	m_VtxMinModel, m_VtxMaxModel;			// ���f���̍ŏ��l�E�ő�l
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_fRot = 0.0f;							// ����
}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CModel3D::~CModel3D()
{
}

//===============================================================================
//�@�N���G�C�g
//===============================================================================
CModel3D * CModel3D::Create(void)
{
	CModel3D *pModel3D = NULL;

	// NULL�`�F�b�N
	if (pModel3D == NULL)
	{// �������̓��I�m��

		pModel3D = new CModel3D;

		if (pModel3D != NULL)
		{
			// �I�u�W�F�N�g�N���X�̐���
			pModel3D->Init();
		}
	}

	return pModel3D;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int		nNumVtx;		// ���_��
	DWORD	sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	m_Pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_Rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����B����
	m_VtxMaxModel	= D3DXVECTOR3(-10000, -10000, -10000);	// �ő�l
	m_VtxMinModel	= D3DXVECTOR3(10000, 10000, 10000);		// �ŏ��l

	// ���_�����擾
	nNumVtx = m_pMeshObject->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMeshObject->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	m_pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// ���_���W�̑��

		// �ő�l
		if (vtx.x > m_VtxMaxModel.x)
		{
			m_VtxMaxModel.x = vtx.x;
		}
		if (vtx.y > m_VtxMaxModel.y)
		{
			m_VtxMaxModel.y = vtx.y;
		}
		if (vtx.z > m_VtxMaxModel.z)
		{
			m_VtxMaxModel.z = vtx.z;
		}
		// �ŏ��l
		if (vtx.x < m_VtxMinModel.x)
		{
			m_VtxMinModel.x = vtx.x;
		}
		if (vtx.y < m_VtxMinModel.y)
		{
			m_VtxMinModel.y = vtx.y;
		}
		if (vtx.z < m_VtxMinModel.z)
		{
			m_VtxMinModel.z = vtx.z;
		}

		// �T�C�Y���̃|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	// ���_�o�b�t�@�̃A�����b�N
	m_pMeshObject->UnlockVertexBuffer();

	m_Rot.y = 0.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel3D::Uninit(void)
{
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel3D::Update(void)
{
	// �N�H�[�^�j�I��
	CModel3D::Quaternion(m_fRot);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX		mtxRot, mtxTrans;		// �v�Z�p
	D3DMATERIAL9	matDef;					// ���݂̃}�e���A����ۑ�
	D3DXMATERIAL	*pMat;					// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldObject);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);

	D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐�3��
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldObject);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMatObject->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMatObject; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// �e�N�X�`��������ꍇ
			pDevice->SetTexture(0, m_pMeshTextures);
		}
		else
		{	// �e�N�X�`�����g���Ă��Ȃ�
			pDevice->SetTexture(0, NULL);
		}
		// �I�u�W�F�N�g(�p�[�c)�̕`��
		m_pMeshObject->DrawSubset(nCntMat);
	}


	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//===============================================================================
//�@�l��n��
//===============================================================================
void CModel3D::BindModel(LPD3DXMESH pMeshObject, LPD3DXBUFFER pBuffMatObject, DWORD nNumMatObject, LPDIRECT3DTEXTURE9 pMeshTextures)
{
	m_pMeshObject		= pMeshObject;
	m_pBuffMatObject	= pBuffMatObject;
	m_nNumMatObject		= nNumMatObject;
	m_pMeshTextures		= pMeshTextures;
}

//===============================================================================
// �N�I�[�^�j�I��
//===============================================================================
void CModel3D::Quaternion(float fRot)
{
	// �N�H�[�^�j�I���錾
	D3DXQUATERNION qouternion, pQou1, pQou2;

	// �}�g���b�N�X�錾
	D3DXMATRIX mtx;

	// ��]�}�g���b�N�X�𐶐�
	D3DXMatrixRotationYawPitchRoll(&mtx, qouternion.x, qouternion.y, qouternion.z);

	// ��]�}�g���N�X���N�H�[�^�j�I���ɕϊ�
	D3DXQuaternionRotationMatrix(&qouternion, &mtx);

	// ���Ԏp���̃N�H�[�^�j�I�����擾
	D3DXQuaternionSlerp(&qouternion, &pQou1, &pQou2, 0.5f);

	// �N�H�[�^�j�I�������]�s��
	D3DXMatrixRotationQuaternion(&mtx, &qouternion);
}