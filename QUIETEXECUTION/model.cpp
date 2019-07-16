//=============================================================================
//
// プレイヤー処理 [model.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//===============================================================================
//　オーバーロードコンストラクタ
//===============================================================================
CModel::CModel()
{
	m_pMesh			= NULL;									// メッシュ情報へのポインタ
	m_pBuffMat		= NULL;									// マテリアルの情報へのポインタ
	m_nNumMat		= 0;									// マテリアルの情報数
	m_VtxMin		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// モデルの最小値
	m_VtxMax		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// モデルの最大値
	m_Pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_Rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_bColChange	= false;
}

//===============================================================================
//　デストラクタ
//===============================================================================
CModel::~CModel()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nNumVtx;				// 頂点数
	DWORD sizeFVF;				// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;				// 頂点バッファへのポインタ

	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_Rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き達する
	m_col		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色の設定
	m_VtxMax	= D3DXVECTOR3(-10000, -10000, -10000);		// 最大値
	m_VtxMin	= D3DXVECTOR3(10000, 10000, 10000);			// 最小値

	// 頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

		// 最大値
		if (vtx.x > m_VtxMax.x)
		{
			m_VtxMax.x = vtx.x;
		}
		if (vtx.y > m_VtxMax.y)
		{
			m_VtxMax.y = vtx.y;
		}
		if (vtx.z > m_VtxMax.z)
		{
			m_VtxMax.z = vtx.z;
		}
		// 最小値
		if (vtx.x < m_VtxMin.x)
		{
			m_VtxMin.x = vtx.x;
		}
		if (vtx.y < m_VtxMin.y)
		{
			m_VtxMin.y = vtx.y;
		}
		if (vtx.z < m_VtxMin.z)
		{
			m_VtxMin.z = vtx.z;
		}

		// サイズ文のポインタを進める
		pVtxBuff += sizeFVF;
	}

	// 頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	m_Rot.y = 0.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX		mtxRot, mtxTrans, mtxParent;	// 計算
	D3DMATERIAL9	matDef;							// 現在のマテリアルを保存
	D3DXMATERIAL	*pMat;							// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{//親の場合
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//親ではない場合
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//親のマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	float fCol = 0.0f;
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (m_bColChange == true)
		{
			fCol = pMat[nCntMat].MatD3D.Diffuse.r;
			pMat[nCntMat].MatD3D.Diffuse.r = m_col.r;
		}
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャをNULLにする
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		//オブジェクト(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);

		// カラーの変更
		if (m_bColChange == true)
		{
			pMat[nCntMat].MatD3D.Diffuse.r = fCol;
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	// 変更終了
	m_bColChange = false;
}