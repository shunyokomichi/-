//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "particle.h"		// エフェクト
#include "scene.h"			// Scene
#include "scene3D.h"		// Scene3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "debuglog.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CParticle::m_pTexture		[MAX_EFFECTTYPE]		= {};		// テクスチャ情報へのポインタ
D3DXVECTOR3			CParticle::m_pos									= {};		// 位置
int					CParticle::m_nEffectType							= {};		// エフェクト種類
char				CParticle::m_cTextName		[MAX_EFFECTTYPE][128]	= {};		// テキスト名
char				CParticle::m_cTextureName	[MAX_EFFECTTYPE][128]	= {};		// テクスチャ名
int					CParticle::m_nTextureType							= 0;		// テクスチャ種類

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle() : CScene(3, CScene::OBJTYPE_EFFECT)
{
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{	// エフェクトの数文デフォルトに
		m_apScene3D[nCnt] = NULL;
		m_NumMove[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nNumLife[nCnt] = 0;
	}

	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 方向
	m_col			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// カラー
	m_TexUV			= D3DXVECTOR2(0.0f, 0.0f);				// UV
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_posRange		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置範囲
	m_nSetPoriMAX	= 0;									// ポリゴン数
	m_nEffectType	= 0;									// エフェクト種類
	m_nLife			= 0;									// 寿命
	m_fGravity		= 0;									// 重力
	m_nBillType		= 0;									// 加算合成
	m_nBindText		= 0;									// テクスチャ割り当て
	m_nTextureType	= 0;									// テクスチャ種類
	m_nAnimCounter	= 0;									// アニメーションカウンタ
	m_nPatternCnter = 0;									// パターンカウンタ
}

//=============================================================================
// テクスチャ読み込み
//=============================================================================
HRESULT CParticle::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	テクスチャ生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Effect/smoke.png", &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CParticle::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_EFFECTTYPE; nCnt++)
	{	//	未使用メモリ開放
		if (m_pTexture[nCnt] != NULL)
		{	// テクスチャの破棄
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
//=============================================================================
//	生成処理
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, int textype, int texttype)
{
	// ポインタ生成
	CParticle *pEffect = NULL;

	if (pEffect == NULL)
	{	//	エフェクト生成
		pEffect = new CParticle;
		if (pEffect != NULL)
		{	// エフェクト初期化
			pEffect->m_pos			= pos;			//	位置設定
			pEffect->m_nEffectType	= textype;		//	テクスチャ種類
			pEffect->m_nBindText	= texttype;		//	エフェクト割り当て
			pEffect->Init();
		}
	}
	return pEffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{	// エフェクトの数分回す（初期化）
		m_apScene3D[nCnt] = NULL;
		m_NumMove[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nNumLife[nCnt] = 0;
	}

	m_nCntAnim = 0;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 方向
	m_TexUV = D3DXVECTOR2(1.0f, 1.0f);			// UV

	if (m_nEffectType == 0)
	{	// 爆発エフェクト
		m_col			= D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f);	// カラー
		m_size			= D3DXVECTOR3(20.0f, 20.0f, 0.0f);		// サイズ
		m_posRange		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置範囲
		m_nSetPoriMAX	= 100;									// ポリゴン数
		m_nLife			= 100;									// 寿命
		m_fGravity		= 0.0;									// 重力
		m_nAnimCounter	= 0;									// アニメーションカウンタ
		m_nPatternCnter = 0;									// パターンカウンタ
		m_move			= D3DXVECTOR3(1.5f, 4.0f, 1.5f);		// 移動量
	}
	else if (m_nEffectType == 1)
	{	// 煙
		m_col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// カラー
		m_size			= D3DXVECTOR3(50.0f, 50.0f, 50.0f);		// サイズ
		m_posRange		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置範囲
		m_nSetPoriMAX	= 8;									// ポリゴン数
		m_nLife			= 20;									// 寿命
		m_fGravity		= -1.0;									// 重力
		m_nAnimCounter	= 20;									// アニメーションカウンタ
		m_nPatternCnter = 0;									// パターンカウンタ
		m_move			= D3DXVECTOR3(2.0f, 0.00005f, 2.0f);	// 移動量
	}

	//	テキストデータ読み込み
	for (int nCnt = 0; nCnt < m_nSetPoriMAX; nCnt++)
	{
		m_apScene3D[nCnt] = NULL;

		if (m_apScene3D[nCnt] == NULL)
		{	// Scene3D取得
			m_apScene3D[nCnt] = new CScene3D(5, CScene::OBJTYPE_EFFECT);

			if (m_apScene3D[nCnt] != NULL)
			{	//	メモリ確保成功時

				//	移動量初期設定(スモーク以外)
				m_NumMove[nCnt] = D3DXVECTOR3(
					(sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_move.x)),		// X
					(cosf((rand() % 628 / 100.0f)) * ((rand() % (int)1) + m_move.y)),		// Y
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_move.z)));		// Z


				//	位置初期設定
				m_pos += D3DXVECTOR3((sinf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.x)),	// X
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.y)),						// Y
					(cosf((rand() % 628) / 100.0f) * ((rand() % (int)1) + m_posRange.z)));						// Z

				//	寿命設定
				m_nNumLife[nCnt] = m_nLife;

				m_apScene3D[nCnt]->Init();		// CScene3Dの初期化

				//	テクスチャ割り当て
				m_apScene3D[nCnt]->BindTexture(m_pTexture[m_nEffectType]);

				if (m_nBillType == 0)
				{	//	加算合成なし(それぞれの設定)
					m_apScene3D[nCnt]->SetPosSize(m_pos, D3DXVECTOR2(m_size.x, m_size.y));					// 座標
					m_apScene3D[nCnt]->SetRotation(m_rot);													// 向き
					m_apScene3D[nCnt]->SetColor(&m_col);													// 色
					m_apScene3D[nCnt]->SetTex(0, 0, D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));		// UV
					m_apScene3D[nCnt]->SetTypeNumber(CScene3D::TYPE_EFFECT);								// タイプ

				}

				else if (m_nBillType == 1)
				{	//	加算合成あり
					m_apScene3D[nCnt]->SetPosSize(m_pos, D3DXVECTOR2(m_size.x, m_size.y));					// 座標
					m_apScene3D[nCnt]->SetRotation(m_rot);													// 向き
					m_apScene3D[nCnt]->SetColor(&m_col);													// 色
					m_apScene3D[nCnt]->SetTex(0, 10, D3DXVECTOR2(0.1f, 1.0f), D3DXVECTOR2(0.1f, 0.0f));		// UV
					m_apScene3D[nCnt]->SetTypeNumber(CScene3D::TYPE_BILLBOORD);								// タイプ
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (m_apScene3D[nCnt] != NULL)
		{	// オブジェクト破棄
			m_apScene3D[nCnt]->Uninit();
			m_apScene3D[nCnt] = NULL;
		}
	}
	// Scene開放(死亡フラグ)
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	D3DXVECTOR3 pos	[MAX_EFFECT];	//	位置取得
	D3DXVECTOR3 rot	[MAX_EFFECT];	//	方向取得
	D3DXVECTOR2 size[MAX_EFFECT];	//	サイズ取得
	D3DXCOLOR	col	[MAX_EFFECT];	//	カラー取得

	for (int nCnt = 0; nCnt < m_nSetPoriMAX; nCnt++)
	{	//	ポリゴン数分回す
		if (m_apScene3D[nCnt] != NULL)
		{//	NULL以外
			pos[nCnt] = m_apScene3D[nCnt]->GetPosition();	//	位置取得
			rot[nCnt] = m_apScene3D[nCnt]->GetRotation();	//	方向取得
			size[nCnt] = m_apScene3D[nCnt]->GetSize();		//	サイズ取得
			col[nCnt] = m_apScene3D[nCnt]->GetColor();		//	カラー取得

			//	位置に動きを代入
			pos[nCnt] += m_NumMove[nCnt];

			//	重力設定
			m_NumMove[nCnt].y -= m_fGravity;

			m_apScene3D[nCnt]->SetPosSize(pos[nCnt], size[nCnt]);	//	位置とサイズ設定
			m_apScene3D[nCnt]->SetColor(&col[nCnt]);				//	カラー設定
			m_apScene3D[nCnt]->SetRotation(rot[nCnt]);				//	方向設定

			//	寿命値減少
			m_nNumLife[nCnt]--;

			// 0になったら破棄
			if (m_nNumLife[nCnt] < 0) { Uninit(); }
		}
	}

	m_nCntAnim++;
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticle::Draw(void)
{
	for (int nCnt = 0; nCnt < m_nSetPoriMAX; nCnt++)
	{
		if (m_apScene3D[nCnt] != NULL)
		{
			m_apScene3D[nCnt]->Draw();
		}
	}
}