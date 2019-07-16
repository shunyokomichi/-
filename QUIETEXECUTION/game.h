//=============================================================================
//
// ゲーム処理 [game.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "manager.h"
#include "loadtext.h"

#define GAUGE_SIZE (140.0f)		// ゲージのサイズ

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
class CGame
{
public:
	CGame();
	~CGame();

	HRESULT Init	(void);		// 初期化処理
	void	Update	(void);		// 更新処理
	void	Uninit	(void);		// 終了処理
	void	Draw	(void);		// 描画処理

	void	SetTex(void);		// テクスチャの生成
	void	SetFloor(CLoadText::MAPTEXT Map);		// 床の生成
	void	SetWall(void);		// 壁の生成
	void	SetObjet(CLoadText::MAPTEXT Map);		// オブジェクトの生成
	void	SetEnemy(void);		// 敵の生成

	static CPlayer *GetPlayer(void)		{ return m_pPlayer; };
	static CObject *Get3DObject(void)	{ return m_pObject; }

	bool GetPause(void)			{ return m_bPause; }
	void SetPause(bool bPause)	{ m_bPause = bPause; }

private:
	static CPlayer		*m_pPlayer;			// プレイヤーのポインタ
	static CWall		*m_pWall;			// 壁のポインタ
	static CObject		*m_pObject;			// オブジェクトのポインタ
	static CEnemy		*m_pEnemy;			// 敵のポインタ
	bool				m_bPause;			// ポーズ
	CLoadText			*m_text;			// テキストへのポインタ
	int					m_nCntType;		// テキストの回数
};
#endif