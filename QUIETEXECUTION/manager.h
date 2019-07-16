//=============================================================================
//
// マネージャ処理 [manager.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "debuglog.h"

//=============================================================================
// クラスの定義
//=============================================================================
class CManager
{
public:
	typedef enum
	{// モードタイプ
		MODE_TITLE = 0,		// タイトル
		MODE_TUTORIAL,		// チュートリアル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_MAX
	}MODE;

	typedef enum
	{// リザルトタイプ
		GAMERESULT_WIN = 0,	// クリア
		GAMERESULT_LOSE,	// オーバー
		GAMERESULT_MAX
	}GAMERESULT;

	CManager();
	~CManager();

	HRESULT Init	(HINSTANCE hInstance, HWND hWnd, bool bWindow);		// 初期化処理
	void	Uninit	(void);												// 終了処理
	void	Update	(void);												// 更新処理
	void	Draw	(void);												// 描画処理

	static CRenderer		*GetRenderer		(void);
	static CSound			*GetSound			(void);
	static CInputKeyBoard	*GetInputKeyboard	(void);
	static CInputJoypad		*GetInputJoyPad		(void);
	static CInputMouse		*GetInputMouse		(void);

	static void				SetMode				(MODE mode);
	static void				SetResult			(GAMERESULT result) { m_GameResult = result; }
	static CCamera			*GetCamera			(void)				{ return m_pCamera; }
	static CPlayer			*&GetPlayer			(void)				{ return m_pPlayer; }
	static CGame			*&GetGame			(void)				{ return m_pGame; }
	static CTutorial		*&GetTutorial		(void)				{ return m_pTutorial; }
	static MODE				GetMode				(void)				{ return m_mode; }
	static GAMERESULT		GetResult			(void)				{ return m_GameResult; }

private:
	static GAMERESULT		m_GameResult;
	static CRenderer		*m_pRenderer;
	static CFade			*m_pFade;
	static CInputJoypad		*m_pInputJoypad;
	static CInputKeyBoard	*m_pInputKeyBoard;
	static CInputMouse		*m_pInputMouse;
	static CSound			*m_pSound;
	static CTitle			*m_pTitle;
	static CTutorial		*m_pTutorial;
	static CGame			*m_pGame;
	static CResult			*m_pResult;
	static CPlayer			*m_pPlayer;
	static MODE				m_mode;
	static CCamera			*m_pCamera;
	static CLight			*m_pLight;
	static CPause			*m_pPause;
	static CWarning			*m_pWarning;
public:
	static CDebuglog		*m_pDebuglog;
};
#endif