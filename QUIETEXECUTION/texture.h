//=============================================================================
//
// テキストの処理 [texture.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"
#include "enemy.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// タイトル
#define TEXTURENAME_TITLE_0		"data\\TEXTURE\\title\\title000.png"			// タイトル(左)
#define TEXTURENAME_TITLE_1		"data\\TEXTURE\\title\\title001.png"			// タイトル(右)
#define TEXTURENAME_TITLE_2		"data\\TEXTURE\\title\\title002.png"			// PRESS背景
#define TEXTURENAME_TITLE_3		"data\\TEXTURE\\title\\puressanybotton.png"		// PRESS

// UI
#define TEXTURENAME_UI_0		"data\\TEXTURE\\UI\\gauge000.png"				// ゲージ(枠)
#define TEXTURENAME_UI_1		"data\\TEXTURE\\UI\\lifegauge001.png"			// HPマーク
#define TEXTURENAME_UI_2		"data\\TEXTURE\\UI\\lifegauge002.png"			// HPゲージ
#define TEXTURENAME_UI_3		"data\\TEXTURE\\UI\\Degree000.png"				// 危険度(背景)
#define TEXTURENAME_UI_4		"data\\TEXTURE\\UI\\Degree001.png"				// 危険度(マーク)
#define TEXTURENAME_UI_5		"data\\TEXTURE\\UI\\crystal000.png"				// 残りクリスタル数
#define TEXTURENAME_UI_6		"data\\TEXTURE\\UI\\cross000.png"				// ×
#define TEXTURENAME_UI_7		"data\\TEXTURE\\UI\\damage000.png"				// 血しぶき
#define TEXTURENAME_UI_8		"data\\TEXTURE\\UI\\pointer000.png"				// レティクル

// チュートリアル
#define TEXTURENAME_TUTORIAL_0	"data\\TEXTURE\\tutorial\\tutorial_board.png"	// 表示背景
#define TEXTURENAME_TUTORIAL_1	"data\\TEXTURE\\tutorial\\tutorial000.png"		// カメラ操作
#define TEXTURENAME_TUTORIAL_2	"data\\TEXTURE\\tutorial\\tutorial001.png"		// 移動操作
#define TEXTURENAME_TUTORIAL_3	"data\\TEXTURE\\tutorial\\tutorial002.png"		// ジャンプ操作
#define TEXTURENAME_TUTORIAL_4	"data\\TEXTURE\\tutorial\\tutorial003.png"		// 攻撃操作
#define TEXTURENAME_TUTORIAL_5	"data\\TEXTURE\\tutorial\\tutorial004.png"		// ライフ説明
#define TEXTURENAME_TUTORIAL_6	"data\\TEXTURE\\tutorial\\tutorial005.png"		// 危険度説明
#define TEXTURENAME_TUTORIAL_7	"data\\TEXTURE\\tutorial\\tutorial006.png"		// 終了説明
#define TEXTURENAME_TUTORIAL_8	"data\\TEXTURE\\tutorial\\fsde_tutorial.png"	// 確認画面(背景)
#define TEXTURENAME_TUTORIAL_9	"data\\TEXTURE\\tutorial\\YES000.png"			// 確認画面(YES)
#define TEXTURENAME_TUTORIAL_10	"data\\TEXTURE\\tutorial\\NO000.png"			// 確認画面(NO)

// リザルト
#define TEXTURENAME_RESULT_0	"data\\TEXTURE\\result\\GAMECLEAR.png"			// GAMECLEAR
#define TEXTURENAME_RESULT_1	"data\\TEXTURE\\result\\GAMEOVER.png"			// GAMEOVER
#define TEXTURENAME_RESULT_2	"data\\TEXTURE\\pause\\pause000.png"			// メニュー背景
#define TEXTURENAME_RESULT_3	"data\\TEXTURE\\pause\\RETRY.png"				// RETRY
#define TEXTURENAME_RESULT_4	"data\\TEXTURE\\pause\\QUIT.png"				// QUIT


class CEnemy;
//=====================
//  CScene2Dの派生クラス
//=====================
class CTexture : public CScene2D
{
public://誰でも扱える
	CTexture();
	~CTexture();
	static HRESULT	Load	(void);
	static void		UnLoad	(void);
	static CTexture *Create	(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	void	TitleMove	(void);		// タイトルの動き
	void	TutorialMove(void);		// チュートリアルの動き
	void	GameMove	(void);		// ゲームの動き
	void	ResultMove	(void);		// リザルトの動き

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_TEXTURE];	// 共有テクスチャのポインタ
	int							m_nType;					// 種類
	D3DXVECTOR2					m_size;						// サイズ(現在)
	D3DXVECTOR2					m_sizeDest;					// サイズ(目的)
	float						m_fLength;					// 距離
	float						m_fAngle;					// 角度
	D3DXVECTOR3					m_rot;						// 向き
	D3DXVECTOR3					m_posDest;					// 座標(目的)
	int							m_nCounterAnim;				// アニメーションのカウンター
	int							m_nPatternAnim;				// アニメーションパターンのカウンター
	int							m_nCntflame;				// カウンター
	int							m_nCntColor;				// カウンター
	static int					m_nCntTutorial;				// 操作説明カウンター
	bool						m_nBlink;					// 点滅
	CEnemy::EnemyStatse			m_EnemyStatse;				// エネミーのステータス
};
#endif