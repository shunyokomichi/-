//=============================================================================
//
// ポーズ処理 [pause.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

#define PAUSE_TEXTURENAME00 "data\\TEXTURE\\pause\\board.png"
#define PAUSE_TEXTURENAME01 "data\\TEXTURE\\pause\\pause000.png"
#define PAUSE_TEXTURENAME02 "data\\TEXTURE\\pause\\CONTINUE.png"
#define PAUSE_TEXTURENAME03 "data\\TEXTURE\\pause\\RETRY.png"
#define PAUSE_TEXTURENAME04 "data\\TEXTURE\\pause\\QUIT.png"

#define PAUSE_MAX_TEXTURE	(5)					// テクスチャの最大数
#define PAUSE_MAX_POLIGON	(5)					// ポリゴンの最大数
#define PAUSE_WIDTH			(SCREEN_WIDTH)		// ポーズの幅
#define PAUSE_HEIGHT		(SCREEN_HEIGHT)		// ポーズの高さ

//*****************************************************************************
//	クラス定義
//*****************************************************************************
class CPause
{
public:
	typedef enum
	{
		TYPE_CONTINUE,		// CONTINUE
		TYPE_RETRY,			// RETRY
		TYPE_QUIT, 			// QUIT
		TYPE_MAX
	}TYPE;

	CPause();
	~CPause();

	// 静的メンバ変数
	static HRESULT	Load(void);		// 読み込む関数(テクスチャ)
	static void		Unload(void);	// 開放する関数(テクスチャ)

	HRESULT		Init	(void);
	void		Uninit	(void);
	static void Update	(void);
	static void Draw	(void);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[PAUSE_MAX_TEXTURE];	// テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;						// 頂点バッファへのポインタ
	D3DXVECTOR3						m_pos;							// ポーズの位置
	static int						m_SelectNum;					// 選択番号
	static float					m_SelectColor;					// 選択色
};
#endif