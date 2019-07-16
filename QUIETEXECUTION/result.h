//=============================================================================
//
// リザルト処理 [result.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "manager.h"
#include "loadtext.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CWall;
class CObject;
class CEnemy;
class CLoadText;

//=============================================================================
// クラスの定義
//=============================================================================
class CResult
{
public:
	CResult();
	~CResult();

	HRESULT Init	(void);				// 初期化処理
	void	Update	(void);				// 更新処理
	void	Uninit	(void);				// 終了処理
	void	Draw	(void);				// 描画処理

	void	SetTex(void);		// テクスチャの生成
	void	SetFloor(CLoadText::MAPTEXT Map);		// 床の生成
	void	SetWall(void);		// 壁の生成
	void	SetObjet(CLoadText::MAPTEXT Map);		// オブジェクトの生成

	static int GetSelect(void) { return m_nSelect; }

private:
	static	CPlayer		*m_pPlayer;		// プレイヤーのポインタ
	static	CWall		*m_pWall;		// 壁のポインタ
	static	CObject		*m_pObject;		// オブジェクトのポインタ
	static CEnemy		*m_pEnemy;		// 敵のポインタ
	static int			m_nSelect;		// 選択番号
	CLoadText			*m_text;			// テキストへのポインタ
	int					m_nCntType;		// テキストの回数
};
#endif