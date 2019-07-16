//=============================================================================
//
// フェード処理 [fade.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

//=============================================================================
// クラスの定義
//=============================================================================
class CFade
{
public:
	//*************************************
	// フェードの状態
	//*************************************
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;

	CFade();
	~CFade();
	static CFade	*Create(void);

	static HRESULT	Init	(void);
	static void		Uninit	(void);
	static void		Update	(void);
	static void		Draw	(void);

	static void		SetFade(CManager::MODE modeNext);
	static FADE		GetFade(void) { return m_fadeType; }

private:
	static LPDIRECT3DTEXTURE9			m_pTexture;		// テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3							m_pos;			// ポリゴンの位置
	static CManager::MODE				m_modeNext;		// 次のゲームの状態
	static D3DXCOLOR					m_fadeColor;	// フェードの色
	static FADE							m_fadeType;		// フェードの状態
};
#endif