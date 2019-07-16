//=============================================================================
//
// フェード処理 [fade.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"

CFade::FADE					CFade::m_fadeType	= {};		// フェードの種類
CManager::MODE				CFade::m_modeNext	= {};		// 次のゲームの状態
D3DXCOLOR					CFade::m_fadeColor	= {};		// フェードの色
LPDIRECT3DTEXTURE9			CFade::m_pTexture	= NULL;
LPDIRECT3DVERTEXBUFFER9		CFade::m_pVtxBuff	= NULL;

//=============================================================================
//　コンストラクタ
//=============================================================================
CFade::CFade()
{
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_modeNext	= CManager::MODE_GAME;
	m_fadeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_fadeType	= FADE_IN;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
//　生成処理
//=============================================================================
CFade *CFade::Create(void)
{
	CFade *pFade = NULL;

	pFade = new CFade;
	pFade->Init();

	return pFade;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(void)
{
	// レンダラーの取得
	CRenderer			*pRenderer	= CManager::GetRenderer();
	// デバイスの取得
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 2, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// Uが幅、Vが高さ　(0.0f～1.0f）
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		// Uが幅、Vが高さ　(0.0f～1.0f）
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// Uが幅、Vが高さ　(0.0f～1.0f）
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		// Uが幅、Vが高さ　(0.0f～1.0f）

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
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
void CFade::Update(void)
{
	if (m_fadeType != FADE_NONE)
	{// 何もしていない状態じゃないとき
		if (m_fadeType == FADE_IN)
		{// フェードイン状態のとき
			m_fadeColor.a -= 0.01f;				// 画面を透明にしていく
			if (m_fadeColor.a == 0.0f)
			{
				m_fadeColor.a = 0.0f;
				m_fadeType = FADE_NONE;			// 何もしていない状態に

			}
		}
		else if (m_fadeType == FADE_OUT)
		{// フェードアウト状態のとき
			m_fadeColor.a += 0.01f;				// 画面を不透明にしていく
			if (m_fadeColor.a >= 1.0f)
			{
				m_fadeColor.a = 1.0f;
				m_fadeType = FADE_IN;			// フェードイン状態に
				// モードの設定
				CManager::SetMode(m_modeNext);
			}
		}

		VERTEX_2D*pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = m_fadeColor;  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
		pVtx[1].col = m_fadeColor;  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
		pVtx[2].col = m_fadeColor;  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
		pVtx[3].col = m_fadeColor;  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = {};
	if (CManager::GetRenderer() != NULL)
	{// GetRenderer()がNULLだったら落ちるため
		pDevice = CManager::GetRenderer()->GetDevice();
	}

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, 0);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// フェードの設定
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	m_fadeType	= FADE_OUT;								// フェードアウト状態に
	m_modeNext	= modeNext;								// 次のモードの更新
	m_fadeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒い画面(透明)にしておく
}