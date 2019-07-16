//=============================================================================
//
// ���b�V���t�B�[���h���� [MeshField.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "MeshField.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debuglog.h"
#include "player.h"
#include "loadtext.h"

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CMeshField::CMeshField(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture	= NULL;
	m_pVtxBuff	= NULL;
	m_pldxBuff	= NULL;
	m_mtxWorld	= {};
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth	= 0.0f;
	m_fHeight	= 0.0f;
	m_meshX		= 0;
	m_meshZ		= 0;
	m_posMAX	= 0;
	m_meshMAX	= 0;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
//�@��������
//=============================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, float width, float height, int meshX, int meshY, int nTexType)
{
	CMeshField *pMeshField = NULL;

	if (pMeshField == NULL)
	{
		pMeshField				= new CMeshField;
		pMeshField->m_pos		= pos;
		pMeshField->m_fWidth	= width;
		pMeshField->m_fHeight	= height;
		pMeshField->m_meshX		= meshX;
		pMeshField->m_meshZ		= meshY;
		pMeshField->m_nTexType	= nTexType;
		pMeshField->Init();
	}

	return pMeshField;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshField::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	m_posMAX	= ((m_meshX + 1) * (m_meshZ + 1));
	m_meshMAX	= ((m_meshX * m_meshZ * 2) + (4 * (m_meshZ - 1)));

	char *cTexName = {};
	cTexName = CLoadText::GetName(m_nTexType, 1);

	// �e�N�X�`���̓ǂݍ���(1�̕�)
	D3DXCreateTextureFromFile(pDevice, cTexName, &m_pTexture);

	// ���_�o�b�t�@�̓ǂݍ���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_posMAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_meshMAX + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pldxBuff,NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���E�e�N�X�`�����W
	for (int nCntZ = 0; nCntZ < m_meshZ + 1; nCntZ++)
	{// �c��
		for (int nCntX = 0; nCntX < m_meshX + 1; nCntX++)
		{// ����
			pVtx[(nCntZ * (m_meshX + 1)) + nCntX].pos = D3DXVECTOR3((m_fWidth / m_meshX) * nCntX, 0.0f, -(m_fHeight / m_meshZ) * nCntZ);
			pVtx[(nCntZ * (m_meshX + 1)) + nCntX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[(nCntZ * (m_meshX + 1)) + nCntX].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntZ * (m_meshX + 1)) + nCntX].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);
		}
	}

	// �@���̐ݒ�
	D3DXVECTOR3 VecA, VecC; // �v�Z�p
	D3DXVECTOR3 nor[2];

	for (int nCntMeshZ = 0; nCntMeshZ < m_meshZ; nCntMeshZ++)
	{	// �c��
		for (int nCntMeshX = 0; nCntMeshX < m_meshX; nCntMeshX++)
		{	// ����
			VecA = pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos
				- pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos
				- pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
			D3DXVec3Cross(&nor[0], &VecA, &VecC);
			D3DXVec3Normalize(&nor[0], &nor[0]);

			VecA = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos
				- pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1)) + 1].pos
				- pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos;
			D3DXVec3Cross(&nor[1], &VecA, &VecC);
			D3DXVec3Normalize(&nor[1], &nor[1]);

			pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].nor = nor[0];
			pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].nor = (nor[0] + nor[1]) / 2; // ���Ԓl������^����
			pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].nor = (nor[0] + nor[1]) / 2;
			pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].nor = nor[1];

		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	WORD*pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pldxBuff->Lock(0, 0, (void**)&pIdx, 0);
	// �C���f�b�N�X�̐ݒ�
	for (int nCount_yz = 0; nCount_yz < m_meshZ; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < m_meshX + 1; nCount_x++)
		{// x���J�E���g
		 // �C���f�b�N�X���̐ݒ�
			pIdx[0] = (WORD)(((nCount_yz + 1) * (m_meshX + 1)) + nCount_x);
			pIdx[1] = (WORD)(((nCount_yz) * (m_meshX + 1)) + nCount_x);

			pIdx += 2;   // �C���f�b�N�X�o�b�t�@��i�߂�

			if (m_meshZ > 1 && nCount_yz != m_meshZ - 1 && nCount_x == m_meshX)
			{// ���[�ɓ��B������
			 // �C���f�b�N�X���̐ݒ�i�k�ރ|���S���j
				pIdx[0] = (WORD)(((nCount_yz) * (m_meshX + 1)) + nCount_x);
				pIdx[1] = (WORD)(((nCount_yz + 2) * (m_meshX + 1)));

				pIdx += 2;  // �C���f�b�N�X�o�b�t�@��i�߂�
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pldxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshField::Uninit(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pldxBuff != NULL)
	{
		m_pldxBuff->Release();
		m_pldxBuff = NULL;
	}
	// �I�u�W�F�N�g(�������g)�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshField::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshField::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX				mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pldxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_posMAX, 0, m_meshMAX);
}

//=============================================================================
// �N���̔���
//=============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 posold)
{
	D3DXVECTOR3 VecA, VecC; // �v�Z�p
	float fGaiseki[5];		// �O��

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshZ = 0; nCntMeshZ < m_meshZ; nCntMeshZ++)
	{	// �c��
		for (int nCntMeshX = 0; nCntMeshX < m_meshX; nCntMeshX++)
		{	// ����
			// ��
			VecA = pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pos - pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
			fGaiseki[0] = (VecA.z * VecC.x) - (VecA.x * VecC.z);
			// ��
			VecA = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pos - pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos;
			fGaiseki[2] = (VecA.z * VecC.x) - (VecA.x * VecC.z);
			// �E
			VecA = pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pos - pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos;
			fGaiseki[1] = (VecA.z * VecC.x) - (VecA.x * VecC.z);
			// ��
			VecA = pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pos - pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos;
			fGaiseki[3] = (VecA.z * VecC.x) - (VecA.x * VecC.z);

			if (fGaiseki[0] >= 0 && fGaiseki[1] >= 0 &&
				fGaiseki[2] >= 0 && fGaiseki[3] >= 0)
			{// �|���S�����ɂ���Ƃ�
			 // ���ォ�E�����ǂ���
				VecA = pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
				VecC = pos - pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
				fGaiseki[4] = (VecA.z * VecC.x) - (VecA.x * VecC.z);
				if (fGaiseki[4] > 0)
				{// ����
					D3DXVECTOR3 Vec0;
					D3DXVECTOR3 Nor0;
					D3DXVECTOR3 Pos0, Pos2, Pos3;	// �v�Z�p
					float PosStockY;				// Y���̉��u��
					PosStockY = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;
					Pos0 = pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos; Pos0.y -= pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// pVtx[0].pos.y��0�ɂ�������
					Pos3 = pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos; Pos3.y -= pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// pVtx[0].pos.y��0�ɂ�������
					Pos2 = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos; Pos2.y -= pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// Nor0�����߂�Ƃ��ApVtx[0].pos.y��0�ɂ���

					VecA = Pos3 - Pos2;						// Nor0�����߂�Ƃ���VecA
					VecC = Pos0 - Pos2;						// Nor0�����߂�Ƃ���VecC
					D3DXVec3Cross(&Nor0, &VecA, &VecC);		// D3DXVec3Cross��Nor0��XYZ���v�Z���Ă���
					D3DXVec3Normalize(&Nor0, &Nor0);		// ���K��

					Vec0 = pos - Pos2;

					Vec0.y = ((Nor0.x * Vec0.x) + (Nor0.z * Vec0.z)) / -Nor0.y;	//  VecY = ((norX * VecX) + (norZ * VecZ)) / -norY + �v���C���[��Y��

					//Vec0.y = (Vec0.x * Nor0.x) + (Vec0.y * Nor0.y) + (Vec0.z * Nor0.z);		// �@�������������m�F

					pos.y = Vec0.y + PosStockY;		// ���炵��Y����߂�
					CManager::m_pDebuglog->Print(1, "PosStockY �`%.1f�`\n", PosStockY);
				}
				else
				{// �E��
					D3DXVECTOR3 Vec0;
					D3DXVECTOR3 Nor0;
					D3DXVECTOR3 Pos0, Pos1, Pos3;	// �v�Z�p
					float PosStockY;				// Y���̉��u��
					PosStockY = pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;
					Pos0 = pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos; Pos0.y -= pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// pVtx[3].pos.y��0�ɂ�������
					Pos3 = pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos; Pos3.y -= pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// pVtx[3].pos.y��0�ɂ�������
					Pos1 = pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos; Pos1.y -= pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// Nor0�����߂�Ƃ��ApVtx[3].pos.y��0�ɂ���

					VecA = Pos0 - Pos1;						// Nor0�����߂�Ƃ���VecA
					VecC = Pos3 - Pos1;						// Nor0�����߂�Ƃ���VecC
					D3DXVec3Cross(&Nor0, &VecA, &VecC);		// D3DXVec3Cross��Nor0��XYZ���v�Z���Ă���
					D3DXVec3Normalize(&Nor0, &Nor0);

					Vec0 = pos - Pos1;

					Vec0.y = ((Nor0.x * Vec0.x) + (Nor0.z * Vec0.z)) / -Nor0.y;	//  VecY = (norX * VecX) + (norZ * VecZ) - norY + �v���C���[��Y��

					//Vec0.y = (Vec0.x * Nor0.x) + (Vec0.y * Nor0.y) + (Vec0.z * Nor0.z);		// �@�������������m�F

					pos.y = Vec0.y + PosStockY;		// ���炵��Y����߂�
					CManager::m_pDebuglog->Print(1, "PosStockY �`%.1f�`\n", PosStockY);
				}
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return pos.y;
}