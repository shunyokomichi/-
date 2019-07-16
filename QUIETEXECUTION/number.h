//=============================================================================
//
// スコア処理 [number.h]
// Author :		Yoshiro Araya
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "scene2D.h"

#define NUMBER_TEXTURENAME00 "data\\TEXTURE\\UI\\number000.png"

//=============================================================================
// クラスの定義
//=============================================================================
class CNumber
{
public:
	CNumber();
	~CNumber();
	static CNumber *Create	(D3DXVECTOR3 pos, float width, float height);

	static HRESULT	Load	(void);
	static void		Unload	(void);

	HRESULT		Init		(D3DXVECTOR3 pos, float width, float height);
	void		Uninit		(void);
	void		Update		(void);
	void		Draw		(void);
	void		SetNumber	(int number);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3						m_pos;			// ポリゴンの位置
	float							m_fWidth;		// 幅
	float							m_fHeight;		// 高さ
};
#endif