//=============================================================================
//
// タイトル処理 [title.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "manager.h"
#include "loadtext.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CWall;
class CObject;
class CLoadText;

//=============================================================================
// クラスの定義
//=============================================================================
class CTitle
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// 何もしていない
		TYPE_PRESS,		// PRESSされたとき
		TYPE_MAX
	}TYPE;

	CTitle();
	~CTitle();

	HRESULT Init	(void);		// 初期化処理
	void	Update	(void);		// 更新処理
	void	Uninit	(void);		// 終了処理
	void	Draw	(void);		// 描画処理

	void	SetTex	(void);		// テクスチャの生成
	void	SetFloor(CLoadText::MAPTEXT Map);		// 床の生成
	void	SetWall	(void);		// 壁の生成
	void	SetObjet(CLoadText::MAPTEXT Map);		// オブジェクトの生成

	static TYPE GetType(void) { return m_pType; };	// タイプの取得

private:
	static	CWall		*m_pWall;		// 壁のポインタ
	static	CObject		*m_pObject;		// オブジェクトのポインタ
	static	TYPE		m_pType;		// 状態
	CLoadText			*m_text;		// テキストへのポインタ
	int					m_nCntType;		// テキストの回数
};
#endif