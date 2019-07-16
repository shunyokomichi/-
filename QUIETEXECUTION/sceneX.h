//=============================================================================
//
// ���f������ [sceneX.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"

#define MODEL_NAME		"data/MODEL/VF-29_body.x"				// �ǂݍ��ރ��f��

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CSceneX : public CScene  // ���h���N���X
{
public:
	CSceneX(int nPriority = 3);
	~CSceneX();
	static CSceneX *Create(D3DXVECTOR3 pos);

	HRESULT		Init		(void);
	void		Uninit		(void);
	void		Update		(void);
	void		Draw		(void);

	void		BindModel	(LPD3DXMESH Mash, LPD3DXBUFFER BuffMat, DWORD NumMat);
	void		Set			(D3DXVECTOR3 pos);
	void		SetCol		(D3DXCOLOR col) { m_col = col; m_bColChange = true; }
	D3DXVECTOR3 &GetPos		(void)			{ return m_pos; }
	D3DXVECTOR3 &GetRot		(void)			{ return m_rot; }

private:
	// �v���C���[�̍\����
	static LPD3DXMESH	m_pMash;
	static LPD3DXBUFFER	m_pBuffMat;
	static DWORD		m_nNumMat;
	D3DXMATRIX			m_mtxWorld;
	D3DXVECTOR3			m_pos;						// ���݂̈ʒu
	D3DXVECTOR3			m_rot;						// ����
	D3DXVECTOR3			m_vtxMin;					// ���f���̍ŏ��l
	D3DXVECTOR3			m_vtxMax;					// ���f���̍ő�l
	D3DXCOLOR			m_col;						// �F���
	int					m_PlayerAnim;				// �v���C���[�̃A�j���[�V����
	int					m_nIdxModelParent	= -1;	// �e���f���̃C���f�b�N�X
	DWORD				m_nNumMatModel		= 0;	// �}�e���A�����̐�
	bool				m_bColChange;				// �F��ς��邩�ǂ���
};
#endif