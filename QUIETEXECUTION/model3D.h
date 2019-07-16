//=============================================================================
//
// モデルの処理 [model3D.h]
// Author :		 荒谷由朗
//
//=============================================================================
#ifndef _MODEL3D_H_
#define _MODEL3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CSceneの派生クラス
//=====================
class CModel3D : public CScene
{
public://誰でも扱える
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

private://個人でのみ使う
	LPD3DXMESH				m_pMeshObject;					// メッシュ情報へのポインタ
	LPD3DXBUFFER			m_pBuffMatObject;				// マテリアルの情報へのポインタ
	DWORD					m_nNumMatObject;				// マテリアルの情報数
	LPDIRECT3DTEXTURE9		m_pMeshTextures;
	D3DXMATRIX				m_mtxWorldObject;				// ワールドマトリックス
	D3DXVECTOR3				m_VtxMinModel,m_VtxMaxModel;	// モデルの最小値・最大値
	D3DXVECTOR3				m_Pos;							// 位置
	D3DXVECTOR3				m_Rot;							// 向き
	D3DXVECTOR3				m_Move;							// 動き
	float					m_fRot;							// 向き
	MOVETYPE				m_nMoveType;					// 動きの種類
};
#endif