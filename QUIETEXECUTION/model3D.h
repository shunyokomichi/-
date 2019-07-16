//=============================================================================
//
// ���f���̏��� [model3D.h]
// Author :		 �r�J�R�N
//
//=============================================================================
#ifndef _MODEL3D_H_
#define _MODEL3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene�̔h���N���X
//=====================
class CModel3D : public CScene
{
public://�N�ł�������
	typedef enum
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_NOT,
		MOVETYPE_MAX
	}MOVETYPE;

	CModel3D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_3DMODEL);
	~CModel3D();
	static CModel3D *Create(void);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	D3DXVECTOR3 GetPosition	(void)					{ return m_Pos; }
	D3DXVECTOR3 GetRot		(void)					{ return m_Rot; }
	MOVETYPE	GetMoveType	(void)					{ return m_nMoveType; }
	D3DXVECTOR3 GetMove		(void)					{ return m_Move; }
	void		BindModel	(LPD3DXMESH pMeshObject, LPD3DXBUFFER	pBuffMatObject, DWORD nNumMatObject, LPDIRECT3DTEXTURE9	pMeshTextures);
	D3DXVECTOR3 VtxMin		(void)					{ return m_VtxMinModel; }
	D3DXVECTOR3 VtxMax		(void)					{ return m_VtxMaxModel; }
	void SetMove			(D3DXVECTOR3 m_move)	{ m_Move = m_move; }
	void SetPosition		(D3DXVECTOR3 pos)		{ m_Pos = pos; }
	void SetRot				(D3DXVECTOR3 rot)		{ m_Rot = rot; }
	void SetMoveType		(MOVETYPE MoveType)		{ m_nMoveType = MoveType; }
	void Quaternion			(float fRot);

private://�l�ł̂ݎg��
	LPD3DXMESH				m_pMeshObject;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			m_pBuffMatObject;				// �}�e���A���̏��ւ̃|�C���^
	DWORD					m_nNumMatObject;				// �}�e���A���̏��
	LPDIRECT3DTEXTURE9		m_pMeshTextures;
	D3DXMATRIX				m_mtxWorldObject;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_VtxMinModel,m_VtxMaxModel;	// ���f���̍ŏ��l�E�ő�l
	D3DXVECTOR3				m_Pos;							// �ʒu
	D3DXVECTOR3				m_Rot;							// ����
	D3DXVECTOR3				m_Move;							// ����
	float					m_fRot;							// ����
	MOVETYPE				m_nMoveType;					// �����̎��
};
#endif