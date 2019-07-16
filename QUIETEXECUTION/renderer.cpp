//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "renderer.h"
#include "fade.h"
#include "scene.h"
#include "manager.h"
#include "debuglog.h"
#include "input.h"
#include "game.h"
#include "pause.h"
#include "tutorial.h"
#include "warning.h"

//=============================================================================
//　コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	m_pD3D			= NULL;
	m_pD3DDevice	= NULL;
#ifdef _DEBUG
	m_pFont			= NULL;
#endif
}

//=============================================================================
//　デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DDISPLAYMODE			d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));										// ワークをゼロクリア
	d3dpp.BackBufferCount				= 1;								// バックバッファの数
	d3dpp.BackBufferWidth				= SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;					// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat				= d3ddm.Format;						// カラーモードの指定
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil		= TRUE;								// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed						= bWindow;							// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	//入力情報
	bool			pPause				= false;
	bool			pWarning			= false;
	CInputKeyBoard	*pCInputKeyBoard	= CManager::GetInputKeyboard();
	CGame			*pGame				= CManager::GetGame();
	CTutorial		*pTutorial			= CManager::GetTutorial();

	CManager::m_pDebuglog->Print(0,"FPS %d\n",GetFPS());
#ifdef _DEBUG
	if (pCInputKeyBoard != NULL)
	{
		if (pCInputKeyBoard->GetKeyboardPress(DIK_F1) == true)
		{//矢印の上
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 2);			// ワイヤーフレーム指定
		}
		if (pCInputKeyBoard->GetKeyboardPress(DIK_F2) == true)
		{//矢印の上
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 0);			// ワイヤーフレーム指定
		}
	}
#endif
	if (pGame != NULL)
	{
		pPause = pGame->GetPause();
	}
	else
	{
		pPause = false;
	}
	if (pTutorial != NULL)
	{
		pWarning = pTutorial->GetWarning();
	}
	else
	{
		pWarning = false;
	}

	if (pPause == false && pWarning == false)
	{
		// ポリゴンの更新処理
		CScene::UpdateAll();
	}
	else if(pPause == true)
	{
		CPause::Update();
	}
	else if (pWarning == true)
	{
		CWarning::Update();
	}

	CFade::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	bool pPause		= false;
	bool pWarning	= false;
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear
	(	0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(100, 100, 255, 255),
		1.0f,
		0
	);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CGame		*pGame		= CManager::GetGame();			// ゲームの取得
		CTutorial	*pTutorial	= CManager::GetTutorial();		// チュートリアルの取得
		if (pGame != NULL)
		{
			pPause = pGame->GetPause();
		}
		else
		{
			pPause = false;
		}
		if (pTutorial != NULL)
		{
			pWarning = pTutorial->GetWarning();
		}
		else
		{
			pWarning = false;
		}

		// ポリゴンの描画処理
		CScene::DrawAll();

		// フェードの描画処理
		CFade::Draw();

		if (pPause == true)
		{// ポーズの描画処理
			CPause::Draw();
		}
		else if (pWarning == true)
		{// 警告の描画処理
			CWarning::Draw();
		}
#ifdef _DEBUG
		// FPS表示
		CManager::m_pDebuglog->Draw();
#endif

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}
	else
	{
		MessageBox(0, "Drawの処理でNULL", "警告", MB_OK);
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}