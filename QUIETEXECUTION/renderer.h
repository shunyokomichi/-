//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//=============================================================================
// クラスの定義
//=============================================================================
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init	(HWND hWnd, bool bWindow);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	LPDIRECT3DDEVICE9 GetDevice(void);

private:
	LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)
	bool					m_bOnOff;
#ifdef _DEBUG
	LPD3DXFONT				m_pFont;		// フォントへのポインタ
#endif
};
#endif