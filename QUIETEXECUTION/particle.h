//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author :			�r�J�R�N
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "scene3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_EFFECTTYPE	(2)		// �G�t�F�N�g�ő吔
#define MAX_EFFECT		(180)	// �G�t�F�N�g�ő�`�搔

//=============================================================================
// �N���X��`
//=============================================================================
class CParticle : public CScene
{
public:
	CParticle();		// �R���X�g���N�^
	~CParticle() {};	// �f�X�g���N�^(���g����)

	static HRESULT		Load	(void);													// �e�N�X�`���ǂݍ���
	static void			Unload	(void);													// �e�N�X�`���J��
	static CParticle	*Create	(D3DXVECTOR3 pos, int nTextureType, int ntexttype);		// ��������

	HRESULT Init	(void);		// ����������
	void	Uninit	(void);		// �I������
	void	Update	(void);		// �X�V����
	void	Draw	(void);		// �`�揈��(���g����)

private:
	static LPDIRECT3DTEXTURE9	m_pTexture		[MAX_EFFECTTYPE];			// �e�N�X�`�����ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;									// ���[���h�}�g���b�N�X
	CScene3D					*m_apScene3D	[MAX_EFFECT];				// �V�[��3D�̃|�C���^�^�ϐ�(4)
	static int					m_nEffectType;								// �G�t�F�N�g���
	static D3DXVECTOR3			m_pos;										// �ʒu
	D3DXVECTOR3					m_rot, m_size, m_move, m_posRange;			// ����,�T�C�Y,�ړ���,�ʒu�͈�
	D3DXVECTOR2					m_TexUV;									// UV
	D3DXCOLOR					m_col;										// �F
	int							m_nSetPoriMAX;								// �|���S����
	int							m_nLife;									// ����
	float						m_fGravity;									// �d��
	int							m_nBillType;								// ���Z�����L���̎��
	int							m_nAnimCounter, m_nPatternCnter;			// �A�j���[�V�����J�E���^,�p�^�[��
	D3DXVECTOR3					m_NumMove		[MAX_EFFECT];				// �����̊��蓖��
	int							m_nNumLife		[MAX_EFFECT];				// �������蓖�Đݒ�
	static char					m_cTextName		[MAX_EFFECTTYPE][128];		// �e�L�X�g��
	static char					m_cTextureName	[MAX_EFFECTTYPE][128];		// �e�N�X�`����
	int							m_nBindText;								// �e�N�X�`�����蓖�ėp
	static int					m_nTextureType;								// �e�N�X�`�����
	int							m_nCntAnim;									// �A�j���[�V�����J�E���^�[
};
#endif