//=============================================================================
//
// �r���{�[�h�̏��� [scene2D.cpp]
// Author : �r�J�R�N
//
//=============================================================================
#include "billboord.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "scene3D.h"
#include "camera.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBillBoord::m_pTexture[MAX_BILLBOORD_TEX] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CBillBoord::CBillBoord() : CScene3D(4, CScene::OBJTYPE_BILLBOORD)
{
	m_pVtxBuff	= NULL;
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size		= D3DXVECTOR2(0.0f, 0.0f);
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CBillBoord::~CBillBoord()
{

}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CBillBoord * CBillBoord::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTexType)
{
	CBillBoord *pBillBoord = NULL;

	//NULL�`�F�b�N
	if (pBillBoord == NULL)
	{//�������̓��I�m��

		pBillBoord = new CBillBoord;

		if (pBillBoord != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pBillBoord->Init();
			//�e�N�X�`���̊��蓖��
			pBillBoord->BindTexture(m_pTexture[nTexType]);
			//�ʒu�̊��蓖��
			pBillBoord->SetPosSize(pos, size);
		}
	}

	return pBillBoord;
}
//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CBillBoord::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BILL_TEXTURE_NAME_0,&m_pTexture[0]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CBillBoord::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BILLBOORD_TEX; nCount++)
	{
		//�e�N�X�`���[�̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CBillBoord::Init(void)
{
	CScene3D::Init();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBillBoord::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBillBoord::Update(void)
{
	CScene3D::Update();

	// �ʒu�̏�����
	D3DXVECTOR3 pos = CScene3D::GetPosition();

	// �r���{�[�h
	CScene3D::SetPosition(pos);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CBillBoord::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ʒu�̏�����
	D3DXVECTOR3 BillPos = CScene3D::GetPosition();
	// �v���C���[�̈ʒu���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	// �J�����̈ʒu
	D3DXVECTOR3 CameraPosV = CCamera::GetPosV();
	// �Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	// �߂��̃r���{�[�h�ȊO������
	if (BillPos.x + 1500.0f >= PlayerPos.x && BillPos.x - 1300.0f <= PlayerPos.x)
	{
		CScene3D::Draw();
	}
}