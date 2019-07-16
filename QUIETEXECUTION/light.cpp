//=============================================================================
//
// ���C�g���� [light.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "light.h"

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CLight::CLight()
{

}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
//�@��������
//=============================================================================
CLight *CLight::Create()
{
	CLight *pLight = NULL;

	if (pLight == NULL)
	{
		pLight = new CLight;
		pLight->Init();
	}

	return pLight;
}

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void CLight::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir0;
	D3DXVECTOR3 vecDir1;
	D3DXVECTOR3 vecDir2;

	// ���C�g���N���A����
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	// ���C�g�̎�ނ�ݒ�
	m_light[0].Type = D3DLIGHT_DIRECTIONAL;
	m_light[1].Type = D3DLIGHT_DIRECTIONAL;
	m_light[2].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̊g�U����ݒ�
	m_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir0 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vecDir1 = D3DXVECTOR3(-0.18f, -0.88f, -0.44f);
	vecDir2 = D3DXVECTOR3(0.89f, -0.11f, 0.44f);

	D3DXVec3Normalize(&vecDir0, &vecDir0);				// ���K���ɂ���
	D3DXVec3Normalize(&vecDir1, &vecDir1);				// ���K���ɂ���
	D3DXVec3Normalize(&vecDir2, &vecDir2);				// ���K���ɂ���
	m_light[0].Direction = vecDir0;
	m_light[1].Direction = vecDir1;
	m_light[2].Direction = vecDir2;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void CLight::Update(void)
{
}