//=============================================================================
//
// チュートリアル時の警告処理 [warning.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "warning.h"
#include "scene2D.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "tutorial.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9				CWarning::m_pTexture[WARNING_MAX_TEXTURE]	= {};
LPDIRECT3DVERTEXBUFFER9			CWarning::m_pVtxBuff						= NULL;
int								CWarning::m_SelectNum						= NULL;		// 選択番号
float							CWarning::m_SelectColor						= NULL;		// 選択色

//=============================================================================
//	コンストラクタ
//=============================================================================
CWarning::CWarning()
{
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_SelectNum		= NULL;
	m_SelectColor	= 0.0f;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CWarning::~CWarning()
{
}

//=============================================================================
//　テクスチャの読み込み
//=============================================================================
HRESULT CWarning::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, WARNING_TEXTURENAME00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, WARNING_TEXTURENAME01, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, WARNING_TEXTURENAME02, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, WARNING_TEXTURENAME03, &m_pTexture[3]);

	return S_OK;
}

//=============================================================================
//　アンロード
//=============================================================================
void CWarning::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < WARNING_MAX_TEXTURE; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWarning::Init(void)
{
	// デバイスの取得
	CRenderer			*pRenderer	= CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();		// デバイスの取得

	m_SelectNum		= 0;						// 選択番号
	m_SelectColor	= 0.0f;						// 選択カラー

	int nCntWarning;							// ポーズカウンタ
	float fPosWarning = -170;					// ポーズ位置

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * WARNING_MAX_POLIGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	Load();

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntWarning = 0; nCntWarning < WARNING_MAX_POLIGON; nCntWarning++)
	{// ポーズをカウント
		if (nCntWarning == 0)
		{// BG
		 // 頂点設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			// 頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		}
		else if (nCntWarning == 1)
		{// 背景
		 // 頂点設定
			pVtx[0].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - 300, (SCREEN_HEIGHT / 2) - 250, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + 300, (SCREEN_HEIGHT / 2) - 250, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - 300, (SCREEN_HEIGHT / 2) + 250, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + 300, (SCREEN_HEIGHT / 2) + 250, 0.0f);
			// 頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (nCntWarning == 2)
		{// YES
		 // 頂点設定
			pVtx[0].pos = D3DXVECTOR3((500) - 100, 400, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((500) + 100, 400, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((500) - 100, 400 + 100, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((500) + 100, 400 + 100, 0.0f);

			// 頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			fPosWarning += 100;
		}
		else if (nCntWarning == 3)
		{// NO
		 // 頂点設定
			pVtx[0].pos = D3DXVECTOR3((800) - 100, 400, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((800) + 100, 400, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((800) - 100, 400 + 100, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((800) + 100, 400 + 100, 0.0f);

			// 頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			fPosWarning += 100;
		}

		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWarning::Uninit(void)
{
	Unload();

	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CWarning::Update(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	CFade::FADE		fade	= CFade::GetFade();					// フェードの取得
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// 入力情報の取得
	CInputMouse		*pMouse = CManager::GetInputMouse();		// 入力情報の取得(マウス)
	CSound			*pSound = CManager::GetSound();				// サウンドの取得

	// 頂点バッファをロック
	m_pVtxBuff->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);
	if (pKey->GetKeyboardTrigger(DIK_UP) == true || pKey->GetKeyboardTrigger(DIK_W) == true)
	{// 選択切り替え(上)
		// 頂点カラー設定
		pVtx[m_SelectNum * 4 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		m_SelectNum		= (m_SelectNum + 1) % 2;
		m_SelectColor	= 1.0f;

		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SWITCH);
	}
	if (pKey->GetKeyboardTrigger(DIK_DOWN) == true || pKey->GetKeyboardTrigger(DIK_S) == true)
	{// 選択切り替え(下)
		// 頂点カラー設定
		pVtx[m_SelectNum * 4 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[m_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		m_SelectNum		= (m_SelectNum + 1) % 2;
		m_SelectColor	= 1.0f;

		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SWITCH);
	}

	m_SelectColor -= 0.004f;

	if (m_SelectColor < 0.65f)
	{
		m_SelectColor = 1.0f;
	}

	// 頂点カラー設定
	pVtx[m_SelectNum * 4 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor);
	pVtx[m_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor);
	pVtx[m_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor);
	pVtx[m_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	if (pKey->GetKeyboardTrigger(DIK_RETURN) == true || pMouse->GetMouseTrigger(0) == true)
	{// 決定
		CTutorial *pGame	= CManager::GetTutorial();
		CTutorial *pWarning = ((CTutorial*)pGame);
		bool bWarning		= pWarning->GetWarning();

		if (m_SelectNum == 0)
		{
			if (fade != CFade::FADE_OUT)
			{
				CFade::SetFade(CManager::MODE_GAME);
				pWarning->SetWarning(false);

				// 頂点カラー設定
				pVtx[m_SelectNum * 4 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[m_SelectNum * 4 + 1 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[m_SelectNum * 4 + 2 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
				pVtx[m_SelectNum * 4 + 3 + 8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

				m_SelectNum = 0;

				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_DECISION);
			}
		}
		else if (m_SelectNum == 1)
		{
			pWarning->SetWarning(false);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPENCLOSE);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CWarning::Draw(void)
{
	int nCntWarning;			// ポーズカウンタ

	// デバイスの取得
	CRenderer			*pRenderer	= CManager::GetRenderer();
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWarning = 0; nCntWarning < WARNING_MAX_POLIGON; nCntWarning++)
	{// ポーズをカウント
	 // テクスチャ設定
		pDevice->SetTexture(0, m_pTexture[nCntWarning]);

		// ポーズの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWarning * 4, 2);
	}
}