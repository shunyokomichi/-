//=============================================================================
//
// モデル処理 [sceneX.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"

#define MODEL_NAME		"data/MODEL/VF-29_body.x"				// 読み込むモデル

//=============================================================================
// クラスの定義
//=============================================================================
class CSceneX : public CScene  // ←派生クラス
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
	// プレイヤーの構造体
	static LPD3DXMESH	m_pMash;
	static LPD3DXBUFFER	m_pBuffMat;
	static DWORD		m_nNumMat;
	D3DXMATRIX			m_mtxWorld;
	D3DXVECTOR3			m_pos;						// 現在の位置
	D3DXVECTOR3			m_rot;						// 向き
	D3DXVECTOR3			m_vtxMin;					// モデルの最小値
	D3DXVECTOR3			m_vtxMax;					// モデルの最大値
	D3DXCOLOR			m_col;						// 色情報
	int					m_PlayerAnim;				// プレイヤーのアニメーション
	int					m_nIdxModelParent	= -1;	// 親モデルのインデックス
	DWORD				m_nNumMatModel		= 0;	// マテリアル情報の数
	bool				m_bColChange;				// 色を変えるかどうか
};
#endif