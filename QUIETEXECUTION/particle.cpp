//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "particle.h"		// �G�t�F�N�g
#include "scene.h"			// Scene
#include "scene3D.h"		// Scene3D
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "debuglog.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CParticle::m_pTexture		[MAX_EFFECTTYPE]		= {};		// �e�N�X�`�����ւ̃|�C���^
D3DXVECTOR3			CParticle::m_pos									= {};		// �ʒu
int					CParticle::m_nEffectType							= {};		// �G�t�F�N�g���
char				CParticle::m_cTextName		[MAX_EFFECTTYPE][128]	= {};		// �e�L�X�g��
char				CParticle::m_cTextureName	[MAX_EFFECTTYPE][128]	= {};		// �e�N�X�`����
int					CParticle::m_nTextureType							= 0;		// �e�N�X�`�����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticle::CParticle() : CScene(3, CScene::OBJTYPE_EFFECT)
{
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{	// �G�t�F�N�g�̐����f�t�H���g��
		m_apScene3D[nCnt] = NULL;
		m_NumMove[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nNumLife[nCnt] = 0;
	}

	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_col			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �J���[
	m_TexUV			= D3DXVECTOR2(0.0f, 0.0f);				// UV
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	m_posRange		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�͈�
	m_nSetPoriMAX	= 0;									// �|���S����
	m_nEffectType	= 0;									// �G�t�F�N�g���
	m_nLife			= 0;									// ����
	m_fGravity		= 0;									// �d��
	m_nBillType		= 0;									// ���Z����
	m_nBindText		= 0;									// �e�N�X�`�����蓖��
	m_nTextureType	= 0;									// �e�N�X�`�����
	m_nAnimCounter	= 0;									// �A�j���[�V�����J�E���^
	m_nPatternCnter = 0;									// �p�^�[���J�E���^
}

//=============================================================================
// �e�N�X�`���ǂݍ���
//=============================================================================
HRESULT CParticle::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	�e�N�X�`������
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Effect/smoke.png", &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CParticle::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_EFFECTTYPE; nCnt++)
	{	//	���g�p�������J��
		if (m_pTexture[nCnt] != NULL)
		{	// �e�N�X�`���̔j��
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
//=============================================================================
//	��������
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, int textype, int texttype)
{
	// �|�C���^����
	CParticle *pEffect = NULL;

	if (pEffect == NULL)
	{	//	�G�t�F�N�g����
		pEffect = new CParticle;
		if (pEffect != NULL)
		{	// �G�t�F�N�g������
			pEffect->m_pos			= pos;			//	�ʒu�ݒ�
			pEffect->m_nEffectType	= textype;		//	�e�N�X�`�����
			pEffect->m_nBindText	= texttype;		//	�G�t�F�N�g���蓖��
			pEffect->Init();
		}
	}
	return pEffect;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticle::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{	// �G�t�F�N�g�̐����񂷁i�������j
		m_apScene3D[nCnt] = NULL;
		m_NumMove[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nNumLife[nCnt] = 0;
	}

	m_nCntAnim = 0;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_TexUV = D3DXVECTOR2(1.0f, 1.0f);			// UV

	if (m_nEffectType == 0)
	{	// �����G�t�F�N�g
		m_col			= D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f);	// �J���[
		m_size			= D3DXVECTOR3(20.0f, 20.0f, 0.0f);		// �T�C�Y
		m_posRange		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�͈�
		m_nSetPoriMAX	= 100;									// �|���S����
		m_nLife			= 100;									// ����
		m_fGravity		= 0.0;									// �d��
		m_nAnimCounter	= 0;									// �A�j���[�V�����J�E���^
		m_nPatternCnter = 0;									// �p�^�[���J�E���^
		m_move			= D3DXVECTOR3(1.5f, 4.0f, 1.5f);		// �ړ���
	}
	else if (m_nEffectType == 1)
	{	// ��
		m_col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �J���[
		m_size			= D3DXVECTOR3(50.0f, 50.0f, 50.0f);		// �T�C�Y
		m_posRange		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�͈�
		m_nSetPoriMAX	= 8;									// �|���S����
		m_nLife			= 20;									// ����
		m_fGravity		= -1.0;									// �d��
		m_nAnimCounter	= 20;									// �A�j���[�V�����J�E���^
		m_nPatternCnter = 0;									// �p�^�[���J�E���^
		m_move			= D3DXVECTOR3(2.0f, 0.00005f, 2.0f);	// �ړ���
	}

	//	�e�L�X�g�f�[�^�ǂݍ���
	for (int nCnt = 0; nCnt < m_nSetPoriMAX; nCnt++)
	{
		m_apScene3D[nCnt] = NULL;

		if (m_apScene3D[nCnt] == NULL)
		{	// Scene3D�擾
			m_apScene3D[nCnt] = new CScene3D(5, CScene::OBJTYPE_EFFECT);

			if (m_apScene3D[nCnt] != NULL)
			{	//	�������m�ې�����

				//	�ړ��ʏ����ݒ�(�X���[�N�ȊO)
				m_NumMove[nCnt] = D3DXVECTOR3(
					(sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_move.x)),		// X
					(cosf((rand() % 628 / 100.0f)) * ((rand() % (int)1) + m_move.y)),		// Y
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_move.z)));		// Z


				//	�ʒu�����ݒ�
				m_pos += D3DXVECTOR3((sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.x)),	// X
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.y)),						// Y
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.z)));						// Z

				//	�����ݒ�
				m_nNumLife[nCnt] = m_nLife;

				m_apScene3D[nCnt]->Init();		// CScene3D�̏�����

				//	�e�N�X�`�����蓖��
				m_apScene3D[nCnt]->BindTexture(m_pTexture[m_nEffectType]);

				if (m_nBillType == 0)
				{	//	���Z�����Ȃ�(���ꂼ��̐ݒ�)
					m_apScene3D[nCnt]->SetPosSize(m_pos, D3DXVECTOR2(m_size.x, m_size.y));					// ���W
					m_apScene3D[nCnt]->SetRotation(m_rot);													// ����
					m_apScene3D[nCnt]->SetColor(&m_col);													// �F
					m_apScene3D[nCnt]->SetTex(0, 0, D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));		// UV
					m_apScene3D[nCnt]->SetTypeNumber(CScene3D::TYPE_EFFECT);								// �^�C�v

				}

				else if (m_nBillType == 1)
				{	//	���Z��������
					m_apScene3D[nCnt]->SetPosSize(m_pos, D3DXVECTOR2(m_size.x, m_size.y));					// ���W
					m_apScene3D[nCnt]->SetRotation(m_rot);													// ����
					m_apScene3D[nCnt]->SetColor(&m_col);													// �F
					m_apScene3D[nCnt]->SetTex(0, 10, D3DXVECTOR2(0.1f, 1.0f), D3DXVECTOR2(0.1f, 0.0f));		// UV
					m_apScene3D[nCnt]->SetTypeNumber(CScene3D::TYPE_BILLBOORD);								// �^�C�v
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (m_apScene3D[nCnt] != NULL)
		{	// �I�u�W�F�N�g�j��
			m_apScene3D[nCnt]->Uninit();
			m_apScene3D[nCnt] = NULL;
		}
	}
	// Scene�J��(���S�t���O)
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticle::Update(void)
{
	D3DXVECTOR3 pos	[MAX_EFFECT];	//	�ʒu�擾
	D3DXVECTOR3 rot	[MAX_EFFECT];	//	�����擾
	D3DXVECTOR2 size[MAX_EFFECT];	//	�T�C�Y�擾
	D3DXCOLOR	col	[MAX_EFFECT];	//	�J���[�擾

	for (int nCnt = 0; nCnt < m_nSetPoriMAX; nCnt++)
	{	//	�|���S��������
		if (m_apScene3D[nCnt] != NULL)
		{//	NULL�ȊO
			pos[nCnt] = m_apScene3D[nCnt]->GetPosition();	//	�ʒu�擾
			rot[nCnt] = m_apScene3D[nCnt]->GetRotation();	//	�����擾
			size[nCnt] = m_apScene3D[nCnt]->GetSize();		//	�T�C�Y�擾
			col[nCnt] = m_apScene3D[nCnt]->GetColor();		//	�J���[�擾

			//	�ʒu�ɓ�������
			pos[nCnt] += m_NumMove[nCnt];

			//	�d�͐ݒ�
			m_NumMove[nCnt].y -= m_fGravity;

			m_apScene3D[nCnt]->SetPosSize(pos[nCnt], size[nCnt]);	//	�ʒu�ƃT�C�Y�ݒ�
			m_apScene3D[nCnt]->SetColor(&col[nCnt]);				//	�J���[�ݒ�
			m_apScene3D[nCnt]->SetRotation(rot[nCnt]);				//	�����ݒ�

			//	�����l����
			m_nNumLife[nCnt]--;

			// 0�ɂȂ�����j��
			if (m_nNumLife[nCnt] < 0) { Uninit(); }
		}
	}

	m_nCntAnim++;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParticle::Draw(void)
{
	for (int nCnt = 0; nCnt < m_nSetPoriMAX; nCnt++)
	{
		if (m_apScene3D[nCnt] != NULL)
		{
			m_apScene3D[nCnt]->Draw();
		}
	}
}