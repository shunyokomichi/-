//=============================================================================
//
// 壁処理 [wall.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 読み込むテクスチャ名
#define WALL_TEX_0	"data\\TEXTURE\\wall000.png"			// 壁

#define WALL_PRIOTITY		(3)
#define	MAX_WALL_TEX		(10)
#define	MAX_WALL_HEIGHT		(2000.0f)

//=============================================================================
// クラス定義
//=============================================================================
class CWall : public CScene3D
{
public:
	CWall();
	~CWall();

	static HRESULT	Load	(void);
	static void		UnLoad	(void);
	static CWall	*Create	(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, int nType);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	void	CollisionWallPlayer	(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth);
	void	CollisionWallEnemy	(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth);
	void	CollisionBullet		(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_WALL_TEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// 頂点バッファへのポインタ
	D3DXMATRIX					m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3					m_pos;						// 位置
	D3DXVECTOR2					m_size;						// 大きさ
	int							m_nType;					// 壁の方向
};
#endif
