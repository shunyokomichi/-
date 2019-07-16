//=============================================================================
//
// �v���C���[���� [model.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CModel �P�ƃN���X
//=====================
class CModel
{
public://�N�ł�������
	CModel();
	~CModel();

	HRESULT Init		(void);
	void	Uninit		(void);
	void	Update		(void);
	void	Draw		(void);

	void		SetPos		(D3DXVECTOR3 pos)	{ m_Pos = pos; }
	void		Setrot		(D3DXVECTOR3 rot)	{ m_Rot = rot; }
	void		SetParent	(CModel *pModel)	{ m_pParent = pModel; }
	void		BindModel	(LPD3DXMESH mesh, LPD3DXBUFFER buff, DWORD nummat, LPDIRECT3DTEXTURE9 *texture) { m_pMesh = mesh; m_pBuffMat = buff; m_nNumMat = nummat; m_pTexture = texture; }
	D3DXMATRIX	GetMtxWorld	(void)				{ return m_mtxWorld; }
	D3DXVECTOR3 GetPos		(void)				{ return m_Pos; }
	void		SetCol		(D3DXCOLOR col)		{ m_col = col; m_bColChange = true; }

private://�l�ł̂ݎg��
	LPD3DXMESH					m_pMesh;						// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				m_pBuffMat;						// �}�e���A���̏��ւ̃|�C���^
	DWORD						m_nNumMat;						// �}�e���A���̏��
	LPDIRECT3DTEXTURE9			*m_pTexture;
	D3DXMATRIX					m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3					m_VtxMin, m_VtxMax;				// ���f���̍ŏ��l�E�ő�l
	D3DXVECTOR3					m_Pos;							// �ʒu
	D3DXVECTOR3					m_Rot;							// ����
	CModel						*m_pParent;						// �e���f���ւ̃|�C���^
	D3DXCOLOR					m_col;							// �F
	bool						m_bColChange;					// �F�̕ω��t���O
};
#endif