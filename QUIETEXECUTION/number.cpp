//=============================================================================
//
// スコア処理 [number.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "number.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			CNumber::m_pTexture = NULL;

//=============================================================================
//　コンストラクタ
//=============================================================================
CNumber::CNumber()
{
	m_fWidth	= NULL;
	m_fHeight	= NULL;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
//　生成処理
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, float width, float height)
{
	CNumber *pNumber = NULL;

	pNumber = new CNumber;
	pNumber->Init(pos, width, height);

	return pNumber;
}

//=============================================================================
//　ロード
//=============================================================================
HRESULT CNumber::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, NUMBER_TEXTURENAME00, &m_pTexture);

	return S_OK;
}

//=============================================================================
//　アンロード
//=============================================================================
void CNumber::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float width, float height)
{
	m_pos = pos;
	m_fWidth = width;
	m_fHeight = height;

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）

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
void CNumber::Uninit(void)
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
void CNumber::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber::Draw(void)
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
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 設定処理
//=============================================================================
void CNumber::SetNumber(int score)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2((score % 10) * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((score % 10) * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((score % 10) * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((score % 10) * 0.1f + 0.1f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}