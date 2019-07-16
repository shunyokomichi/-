//=============================================================================
//
// メッシュフィールド処理 [MeshField.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "MeshField.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debuglog.h"
#include "player.h"
#include "loadtext.h"

//=============================================================================
//　コンストラクタ
//=============================================================================
CMeshField::CMeshField(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture	= NULL;
	m_pVtxBuff	= NULL;
	m_pldxBuff	= NULL;
	m_mtxWorld	= {};
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth	= 0.0f;
	m_fHeight	= 0.0f;
	m_meshX		= 0;
	m_meshZ		= 0;
	m_posMAX	= 0;
	m_meshMAX	= 0;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
//　生成処理
//=============================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, float width, float height, int meshX, int meshY, int nTexType)
{
	CMeshField *pMeshField = NULL;

	if (pMeshField == NULL)
	{
		pMeshField				= new CMeshField;
		pMeshField->m_pos		= pos;
		pMeshField->m_fWidth	= width;
		pMeshField->m_fHeight	= height;
		pMeshField->m_meshX		= meshX;
		pMeshField->m_meshZ		= meshY;
		pMeshField->m_nTexType	= nTexType;
		pMeshField->Init();
	}

	return pMeshField;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	m_posMAX	= ((m_meshX + 1) * (m_meshZ + 1));
	m_meshMAX	= ((m_meshX * m_meshZ * 2) + (4 * (m_meshZ - 1)));

	char *cTexName = {};
	cTexName = CLoadText::GetName(m_nTexType, 1);

	// テクスチャの読み込み(1体分)
	D3DXCreateTextureFromFile(pDevice, cTexName, &m_pTexture);

	// 頂点バッファの読み込み
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_posMAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_meshMAX + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pldxBuff,NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報・テクスチャ座標
	for (int nCntZ = 0; nCntZ < m_meshZ + 1; nCntZ++)
	{// 縦軸
		for (int nCntX = 0; nCntX < m_meshX + 1; nCntX++)
		{// 横軸
			pVtx[(nCntZ * (m_meshX + 1)) + nCntX].pos = D3DXVECTOR3((m_fWidth / m_meshX) * nCntX, 0.0f, -(m_fHeight / m_meshZ) * nCntZ);
			pVtx[(nCntZ * (m_meshX + 1)) + nCntX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[(nCntZ * (m_meshX + 1)) + nCntX].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntZ * (m_meshX + 1)) + nCntX].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);
		}
	}

	// 法線の設定
	D3DXVECTOR3 VecA, VecC; // 計算用
	D3DXVECTOR3 nor[2];

	for (int nCntMeshZ = 0; nCntMeshZ < m_meshZ; nCntMeshZ++)
	{	// 縦軸
		for (int nCntMeshX = 0; nCntMeshX < m_meshX; nCntMeshX++)
		{	// 横軸
			VecA = pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos
				- pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos
				- pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
			D3DXVec3Cross(&nor[0], &VecA, &VecC);
			D3DXVec3Normalize(&nor[0], &nor[0]);

			VecA = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos
				- pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1)) + 1].pos
				- pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos;
			D3DXVec3Cross(&nor[1], &VecA, &VecC);
			D3DXVec3Normalize(&nor[1], &nor[1]);

			pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].nor = nor[0];
			pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].nor = (nor[0] + nor[1]) / 2; // 中間値を割り与える
			pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].nor = (nor[0] + nor[1]) / 2;
			pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].nor = nor[1];

		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	WORD*pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pldxBuff->Lock(0, 0, (void**)&pIdx, 0);
	// インデックスの設定
	for (int nCount_yz = 0; nCount_yz < m_meshZ; nCount_yz++)
	{// yzをカウント
		for (int nCount_x = 0; nCount_x < m_meshX + 1; nCount_x++)
		{// xをカウント
		 // インデックス情報の設定
			pIdx[0] = (WORD)(((nCount_yz + 1) * (m_meshX + 1)) + nCount_x);
			pIdx[1] = (WORD)(((nCount_yz) * (m_meshX + 1)) + nCount_x);

			pIdx += 2;   // インデックスバッファを進める

			if (m_meshZ > 1 && nCount_yz != m_meshZ - 1 && nCount_x == m_meshX)
			{// 末端に到達したら
			 // インデックス情報の設定（縮退ポリゴン）
				pIdx[0] = (WORD)(((nCount_yz) * (m_meshX + 1)) + nCount_x);
				pIdx[1] = (WORD)(((nCount_yz + 2) * (m_meshX + 1)));

				pIdx += 2;  // インデックスバッファを進める
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pldxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// インデックスバッファの破棄
	if (m_pldxBuff != NULL)
	{
		m_pldxBuff->Release();
		m_pldxBuff = NULL;
	}
	// オブジェクト(自分自身)の破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX				mtxRot, mtxTrans;				// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pldxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_posMAX, 0, m_meshMAX);
}

//=============================================================================
// 起伏の判定
//=============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 posold)
{
	D3DXVECTOR3 VecA, VecC; // 計算用
	float fGaiseki[5];		// 外積

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshZ = 0; nCntMeshZ < m_meshZ; nCntMeshZ++)
	{	// 縦軸
		for (int nCntMeshX = 0; nCntMeshX < m_meshX; nCntMeshX++)
		{	// 横軸
			// 上
			VecA = pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pos - pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
			fGaiseki[0] = (VecA.z * VecC.x) - (VecA.x * VecC.z);
			// 下
			VecA = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pos - pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos;
			fGaiseki[2] = (VecA.z * VecC.x) - (VecA.x * VecC.z);
			// 右
			VecA = pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pos - pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos;
			fGaiseki[1] = (VecA.z * VecC.x) - (VecA.x * VecC.z);
			// 左
			VecA = pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos;
			VecC = pos - pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos;
			fGaiseki[3] = (VecA.z * VecC.x) - (VecA.x * VecC.z);

			if (fGaiseki[0] >= 0 && fGaiseki[1] >= 0 &&
				fGaiseki[2] >= 0 && fGaiseki[3] >= 0)
			{// ポリゴン内にいるとき
			 // 左上か右下かどうか
				VecA = pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos - pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
				VecC = pos - pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos;
				fGaiseki[4] = (VecA.z * VecC.x) - (VecA.x * VecC.z);
				if (fGaiseki[4] > 0)
				{// 左下
					D3DXVECTOR3 Vec0;
					D3DXVECTOR3 Nor0;
					D3DXVECTOR3 Pos0, Pos2, Pos3;	// 計算用
					float PosStockY;				// Y軸の仮置き
					PosStockY = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;
					Pos0 = pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos; Pos0.y -= pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// pVtx[0].pos.yを0にしたから
					Pos3 = pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos; Pos3.y -= pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// pVtx[0].pos.yを0にしたから
					Pos2 = pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos; Pos2.y -= pVtx[nCntMeshX + m_meshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// Nor0を求めるとき、pVtx[0].pos.yを0にする

					VecA = Pos3 - Pos2;						// Nor0を求めるときのVecA
					VecC = Pos0 - Pos2;						// Nor0を求めるときのVecC
					D3DXVec3Cross(&Nor0, &VecA, &VecC);		// D3DXVec3CrossでNor0のXYZを計算している
					D3DXVec3Normalize(&Nor0, &Nor0);		// 正規化

					Vec0 = pos - Pos2;

					Vec0.y = ((Nor0.x * Vec0.x) + (Nor0.z * Vec0.z)) / -Nor0.y;	//  VecY = ((norX * VecX) + (norZ * VecZ)) / -norY + プレイヤーのY軸

					//Vec0.y = (Vec0.x * Nor0.x) + (Vec0.y * Nor0.y) + (Vec0.z * Nor0.z);		// 法線が正しいか確認

					pos.y = Vec0.y + PosStockY;		// 減らしたY軸を戻る
					CManager::m_pDebuglog->Print(1, "PosStockY ～%.1f～\n", PosStockY);
				}
				else
				{// 右上
					D3DXVECTOR3 Vec0;
					D3DXVECTOR3 Nor0;
					D3DXVECTOR3 Pos0, Pos1, Pos3;	// 計算用
					float PosStockY;				// Y軸の仮置き
					PosStockY = pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;
					Pos0 = pVtx[nCntMeshX + (nCntMeshZ * (m_meshX + 1))].pos; Pos0.y -= pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// pVtx[3].pos.yを0にしたから
					Pos3 = pVtx[nCntMeshX + m_meshX + 2 + (nCntMeshZ * (m_meshX + 1))].pos; Pos3.y -= pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// pVtx[3].pos.yを0にしたから
					Pos1 = pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos; Pos1.y -= pVtx[nCntMeshX + 1 + (nCntMeshZ * (m_meshX + 1))].pos.y;	// Nor0を求めるとき、pVtx[3].pos.yを0にする

					VecA = Pos0 - Pos1;						// Nor0を求めるときのVecA
					VecC = Pos3 - Pos1;						// Nor0を求めるときのVecC
					D3DXVec3Cross(&Nor0, &VecA, &VecC);		// D3DXVec3CrossでNor0のXYZを計算している
					D3DXVec3Normalize(&Nor0, &Nor0);

					Vec0 = pos - Pos1;

					Vec0.y = ((Nor0.x * Vec0.x) + (Nor0.z * Vec0.z)) / -Nor0.y;	//  VecY = (norX * VecX) + (norZ * VecZ) - norY + プレイヤーのY軸

					//Vec0.y = (Vec0.x * Nor0.x) + (Vec0.y * Nor0.y) + (Vec0.z * Nor0.z);		// 法線が正しいか確認

					pos.y = Vec0.y + PosStockY;		// 減らしたY軸を戻る
					CManager::m_pDebuglog->Print(1, "PosStockY ～%.1f～\n", PosStockY);
				}
			}
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return pos.y;
}