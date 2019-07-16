//=============================================================================
//
// バレット処理 [bullet.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "scene3D.h"
#include "enemy.h"

#define BULLET_PRIOTITY (3)
#define BULLET_TEXTURENAME00 "data\\TEXTURE\\bullet000.png"

class CWall;
class CObject;
class CEnemy;
//=============================================================================
// クラスの定義
//=============================================================================
class CBullet : public CScene3D  // ←派生クラス
{
public:
	CBullet();
	~CBullet();
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, CEnemy *pEnemy);

	static HRESULT	Load	(void);
	static void		UnLoad	(void);
	HRESULT			Init	(void);
	void			Uninit	(void);
	void			Update	(void);
	void			Draw	(void);

	bool CollisionBulletWall	(CWall *pWall);
	bool CollisionBulletObje	(CObject *pObj);
	bool CollisionBulletSearch	(void);

	CEnemy *&GetEnemy(void) { return m_pEnemy; };

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	D3DXVECTOR3					m_pos;			// ポリゴンの位置
	D3DXVECTOR3					m_move;			// 移動量
	D3DXVECTOR3					m_rot;			// 向き
	D3DXVECTOR2					m_size;			// サイズ
	int							m_nLife;		// ライフ
	int							m_nCntLife;		// ライフカウンター
	CEnemy						*m_pEnemy;		// エネミーのポインタ
};
#endif