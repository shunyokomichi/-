//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "debuglog.h"
#include "pause.h"
#include "warning.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager::MODE		CManager::m_mode			= MODE_TITLE;			// ゲームモード
CManager::GAMERESULT CManager::m_GameResult		= GAMERESULT_WIN;		// リザルトタイプ
CRenderer			*CManager::m_pRenderer		= NULL;					// レンダラ
CFade				*CManager::m_pFade			= NULL;					// フェード
CTitle				*CManager::m_pTitle			= NULL;					// タイトル
CTutorial			*CManager::m_pTutorial		= NULL;					// チュートリアル
CGame				*CManager::m_pGame			= NULL;					// ゲーム
CResult				*CManager::m_pResult		= NULL;					// リザルト
CInputKeyBoard		*CManager::m_pInputKeyBoard = NULL;					// キーボード
CInputJoypad		*CManager::m_pInputJoypad	= NULL;					// ジョイパット
CInputMouse			*CManager::m_pInputMouse	= NULL;					// マウス
CCamera				*CManager::m_pCamera		= NULL;					// カメラ
CLight				*CManager::m_pLight			= NULL;					// ライト
CSound				*CManager::m_pSound			= NULL;					// サウンド
CDebuglog			*CManager::m_pDebuglog		= NULL;					// デバックログ
CPlayer				*CManager::m_pPlayer		= NULL;					// プレイヤー
CPause				*CManager::m_pPause			= NULL;					// ポーズ
CWarning			*CManager::m_pWarning		= NULL;					// チュートリアル時の警告

//=============================================================================
//　コンストラクタ
//=============================================================================
CManager::CManager()
{
	m_mode				= MODE_TITLE;		// ゲームモード
	m_GameResult		= GAMERESULT_WIN;	// リザルトタイプ
	m_pRenderer			= NULL;				// レンダラ
	m_pFade				= NULL;				// フェード
	m_pTitle			= NULL;				// タイトル
	m_pTutorial			= NULL;				// セレクト
	m_pGame				= NULL;				// ゲーム
	m_pResult			= NULL;				// リザルト
	m_pInputKeyBoard	= NULL;				// キーボード
	m_pInputJoypad		= NULL;				// ジョイパット
	m_pInputMouse		= NULL;				// マウス
	m_pSound			= NULL;				// カメラ
	m_pCamera			= NULL;				// ライト
	m_pLight			= NULL;				// サウンド
	m_pDebuglog			= NULL;				// デバックログ
	m_pPlayer			= NULL;				// プレイヤー
}

//=============================================================================
//　デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//　初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// レンダリングクラスの生成
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{
			// 初期化処理
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
			{
				return -1;
			}
		}
		else
		{//警告文
			MessageBox(0, "警告：メモリがないです", "警告", MB_OK);
		}
	}
	else
	{//警告文
		MessageBox(0, "警告：何かが入ってます", "警告", MB_OK);
	}

	if (m_pInputKeyBoard == NULL)
	{//キーボード生成
		m_pInputKeyBoard = CInputKeyBoard::Create(hInstance, hWnd);
	}
	if (m_pInputJoypad == NULL)
	{//ジョイパッド生成
		m_pInputJoypad = CInputJoypad::Create(hInstance, hWnd);
	}
	if (m_pInputMouse == NULL)
	{//マウス生成
		m_pInputMouse = CInputMouse::Create(hInstance, hWnd);
	}

	if (m_pSound == NULL)
	{// サウンド
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{// 初期化処理
			m_pSound->Init(hWnd);
		}
	}
	if (m_pCamera == NULL)
	{// カメラ
		m_pCamera = new CCamera;
		if (m_pCamera != NULL)
		{// 初期化処理
			m_pCamera = CCamera::Create();
		}
	}
	if (m_pLight == NULL)
	{// ライト
		m_pLight = new CLight;
		if (m_pLight != NULL)
		{// 初期化処理
			m_pLight->Create();
		}
	}
	if (m_pDebuglog == NULL)
	{// デバックログ
		m_pDebuglog->Init();	// 初期化処理
	}

	if (m_pPause == NULL)
	{// ポーズ
		m_pPause = new CPause;
		if (m_pPause != NULL)
		{
			m_pPause->Init();
		}
	}

	if (m_pWarning == NULL)
	{// チュートリアル時の警告
		m_pWarning = new CWarning;
		if (m_pWarning != NULL)
		{
			m_pWarning->Init();
		}
	}

	SetMode(m_mode);			// ゲームモードの設定

	if (m_pFade == NULL)
	{// フェード
		m_pFade = new CFade;
		if (m_pFade != NULL)
		{// 初期化処理
			m_pFade->Init();
		}
	}
	return S_OK;
}

//=============================================================================
//　終了処理
//=============================================================================
void CManager::Uninit(void)
{
	CScene::ReleseAll();
	// 破棄
	if (m_pRenderer != NULL)
	{// レンダラー
		// 終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;

	}
	if (m_pFade != NULL)
	{// フェード
		// 終了処理
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
	if (m_pInputKeyBoard != NULL)
	{// キーボード
		// 終了処理
		m_pInputKeyBoard->Uninit();
		delete m_pInputKeyBoard;
		m_pInputKeyBoard = NULL;
	}
	if (m_pInputJoypad != NULL)
	{// ジョイパット
		// 終了処理
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}
	if (m_pInputMouse != NULL)
	{// マウス
		// 終了処理
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}
	if (m_pCamera != NULL)
	{// カメラ
		// 終了処理
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	if (m_pLight != NULL)
	{// ライト
		// 終了処理
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	if (m_pPause != NULL)
	{// ポーズ
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}
	if (m_pWarning != NULL)
	{// チュートリアル時の警告
		m_pWarning->Uninit();
		delete m_pWarning;
		m_pWarning = NULL;
	}
	if (m_pSound != NULL)
	{// サウンド
		// 終了処理
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	if (m_pDebuglog != NULL)
	{// デバックログ
		// 終了処理
		m_pDebuglog->Uninit();
		delete m_pDebuglog;
		m_pDebuglog = NULL;
	}
}

//=============================================================================
//　更新処理
//=============================================================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{// レンダラー
		// 更新処理
		m_pRenderer->Update();
	}
	if (m_pInputKeyBoard != NULL)
	{// キーボード
		// 更新処理
		m_pInputKeyBoard->Update();
	}
	if (m_pInputJoypad != NULL)
	{// ジョイパット
		// 更新処理
		m_pInputJoypad->Update();
	}
	if (m_pInputMouse != NULL)
	{// マウス
		// 更新処理
		m_pInputMouse->Update();
	}
	if (m_pCamera != NULL)
	{// カメラ
		// 更新処理
		m_pCamera->Update();
	}
	if (m_pLight != NULL)
	{// ライト
		// 更新処理
		m_pLight->Update();
	}

	switch (m_mode)
	{// ゲームモードの更新処理
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// タイトル
			m_pTitle->Update();
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// セレクト
			m_pTutorial->Update();
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{// ゲーム
			m_pGame->Update();
		}
		break;
	case MODE_RESULT:
		if (m_pResult != NULL)
		{// リザルト
			m_pResult->Update();
		}
		break;
	}
}

//=============================================================================
//　描画処理
//=============================================================================
void CManager::Draw(void)
{
	//～!!入れ替え禁止!!～*******************************
	if (m_pCamera != NULL)
	{// カメラ
		// 更新処理
		m_pCamera->Set();
	}
	if (m_pRenderer != NULL)
	{// レンダラー
		// 描画処理
		m_pRenderer->Draw();
	}
	//***************************************************

	switch (m_mode)
	{// ゲームモードの描画処理
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// タイトル
			m_pTitle->Draw();
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// セレクト
			m_pTutorial->Draw();
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{// ゲーム
			m_pGame->Draw();
		}
		break;
	case MODE_RESULT:
		if (m_pResult != NULL)
		{// リザルト
			m_pResult->Draw();
		}
		break;
	}
}

//=============================================================================
// デバイスの取得(レンダリング)
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================================================
// デバイスの取得(サウンド)
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// 入力情報の取得(キーボード)
//=============================================================================
CInputKeyBoard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyBoard;
}
//=============================================================================
// 入力情報の取得(コントローラー)
//=============================================================================
CInputJoypad * CManager::GetInputJoyPad(void)
{
	return m_pInputJoypad;
}
//=============================================================================
// 入力情報の取得(マウス)
//=============================================================================
CInputMouse * CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}
//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{// ゲームモードの終了処理
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// タイトル
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// セレクト
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{// ゲーム
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
	case MODE_RESULT:
		if (m_pResult != NULL)
		{// リザルト
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;
	}

	m_mode = mode;		// ゲームモードの更新

	switch (mode)
	{// ゲームモードの初期化処理
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{// タイトル
			m_pTitle = new CTitle;
			if (m_pTitle != NULL)
			{// 初期化処理
				m_pTitle->Init();
				// カメラの切り替え
				if (m_pCamera != NULL)
				{
					m_pCamera->SetType(CCamera::CAMTYPE_TITLE);
				}
			}
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// セレクト
			m_pTutorial = new CTutorial;
			if (m_pTutorial != NULL)
			{// 初期化処理
				m_pTutorial->Init();
				// カメラの切り替え
				if (m_pCamera != NULL)
				{
					m_pCamera->SetType(CCamera::CAMTYPE_FPS);
				}
			}
		}
		break;
	case MODE_GAME:
		if (m_pGame == NULL)
		{// ゲーム
			m_pGame = new CGame;
			if (m_pGame != NULL)
			{// 初期化処理
				m_pGame->Init();
				// カメラの切り替え
				if (m_pCamera != NULL)
				{
					m_pCamera->SetType(CCamera::CAMTYPE_FPS);
				}
			}
		}
		break;
	case MODE_RESULT:
		if (m_pResult == NULL)
		{// リザルト
			m_pResult = new CResult;
			if (m_pResult != NULL)
			{// 初期化処理
				m_pResult->Init();
				// カメラの切り替え
				if (m_pCamera != NULL)
				{
					m_pCamera->SetType(CCamera::CAMTYPE_RESULT);
				}
			}
		}
		break;
	}
}