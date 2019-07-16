//=============================================================================
//
// チュートリアル時の警告処理 [warning.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _WARNING_H_
#define _WARNING_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

#define WARNING_TEXTURENAME00	"data\\TEXTURE\\tutorial\\tutorial_board.png"		// BG
#define WARNING_TEXTURENAME01	"data\\TEXTURE\\tutorial\\fsde_tutorial.png"		// 背景
#define WARNING_TEXTURENAME02	"data\\TEXTURE\\tutorial\\YES000.png"				// YES
#define WARNING_TEXTURENAME03	"data\\TEXTURE\\tutorial\\NO000.png"				// NO

#define WARNING_MAX_TEXTURE		(4)
#define WARNING_MAX_POLIGON		(4)
#define WARNING_WIDTH			(SCREEN_WIDTH)					// ポーズの幅
#define WARNING_HEIGHT			(SCREEN_HEIGHT)					// ポーズの高さ

//*****************************************************************************
//	クラス定義
//*****************************************************************************
class CWarning
{
public:
	typedef enum
	{
		TYPE_YES,		// YES
		TYPE_NO,		// NO
		TYPE_MAX
	}TYPE;

	CWarning();
	~CWarning();

	HRESULT			Init	(void);
	void			Uninit	(void);
	static void		Update	(void);
	static void		Draw	(void);

	// 静的メンバ変数
	static HRESULT	Load	(void);		// 読み込む関数(テクスチャ)
	static void		Unload	(void);		// 開放する関数(テクスチャ)

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[WARNING_MAX_TEXTURE];	// テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// 頂点バッファへのポインタ
	D3DXVECTOR3						m_pos;								// ポーズの位置
	static int						m_SelectNum;						// 選択番号
	static float					m_SelectColor;						// 選択色

};
#endif