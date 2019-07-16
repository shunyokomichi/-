//=============================================================================
//
// モデル処理 [sceneX.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "sceneX.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPD3DXMESH		CSceneX::m_pMash	= NULL;
LPD3DXBUFFER	CSceneX::m_pBuffMat = NULL;
DWORD			CSceneX::m_nNumMat	= NULL;

//=============================================================================
//　コンストラクタ
//=============================================================================
CSceneX::CSceneX(int nPriority) :CScene(nPriority)
{
	m_mtxWorld		= {};
	m_bColChange	= false;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{
}

//=============================================================================
//	生成処理
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos)
{
	CSceneX *pSceneX = NULL;

	pSceneX = new CSceneX;
	pSceneX->Init();

	return pSceneX;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneX::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	// 位置・向き・色の初期設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	int		nNumVtx;		// 頂点数
	DWORD	sizeFVF;		// 頂点フォーマットのサイズ
	BYTE	*pVtxBuff;		// 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = m_pMash->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMash->GetFVF());

	// 頂点バッファをロック
	m_pMash->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			// 頂点座標の代入

		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}
		if (vtx.x > m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		pVtxBuff += sizeFVF;			// サイズ分のポインタを進める
	}

	// 頂点バッファをアンロック
	m_pMash->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	// オブジェクト(自分自身)の破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneX::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX		mtxRot, mtxTrans;		// 計算用マトリックス
	D3DMATERIAL9	matDef;					// 現在のマテリアル
	D3DXMATERIAL	*pMat;					// マテリアルデータへのポインタ
	D3DXMATRIX		mtxParent;				// ペアレントマトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_nIdxModelParent == -1)
	{
		mtxParent = m_mtxWorld;
	}
	else
	{
		mtxParent = m_mtxWorld;
	}

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	float fCol_R = 0.0f;		// 色(赤)

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (m_bColChange == true)
		{
			m_col.r = pMat[nCntMat].MatD3D.Diffuse.r;
			pMat[nCntMat].MatD3D.Diffuse.r = fCol_R;
		}

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		pDevice->SetTexture(0, NULL);
		// モデル(パーツ)の描画
		m_pMash->DrawSubset(nCntMat);

		if (m_bColChange == true)
		{	// カラー変更
			pMat[nCntMat].MatD3D.Diffuse.r = fCol_R;
		}
	}
	// マテリアルを返す
	pDevice->SetMaterial(&matDef);

	m_bColChange = false;		// 変更終了
}

//=============================================================================
// マトリックス情報の設定
//=============================================================================
void CSceneX::BindModel(LPD3DXMESH Mash, LPD3DXBUFFER BuffMat, DWORD NumMat)
{
	m_pMash = Mash;
	m_pBuffMat = BuffMat;
	m_nNumMat = NumMat;
}

//=============================================================================
// 座標の設定
//=============================================================================
void CSceneX::Set(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
