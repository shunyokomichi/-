//=============================================================================
//
// テキストの処理 [texture.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "result.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CTexture::m_pTexture[MAX_TEXTURE]	= {};
int					CTexture::m_nCntTutorial			= NULL;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CTexture::CTexture() : CScene2D(4, CScene::OBJTYPE_2DPOLYGON)//優先優位イニシャライズ
{
	m_nType			= 0;
	m_size			= D3DXVECTOR2(0.0f, 0.0f);
	m_sizeDest		= D3DXVECTOR2(0.0f, 0.0f);
	m_fLength		= 0.0f;
	m_fAngle		= 0.0f;
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCounterAnim	= 0;
	m_nPatternAnim	= 0;
	m_nCntflame		= 0;
	m_nCntColor		= 0;
	m_nBlink		= false;
	m_EnemyStatse	= CEnemy::ENEMYSTATSE_NONE;
}

//===============================================================================
//　デストラクタ
//===============================================================================
CTexture::~CTexture()
{
}

//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CTexture::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//// テクスチャの生成
	// タイトル
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TITLE_0, &m_pTexture[0]);		// タイトル(左)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TITLE_1, &m_pTexture[1]);		// タイトル(右)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TITLE_2, &m_pTexture[2]);		// PRESS背景
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TITLE_3, &m_pTexture[3]);		// PRESS

	// UI
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_0, &m_pTexture[4]);			// ゲージ(枠)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_1, &m_pTexture[5]);			// HPマーク
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_2, &m_pTexture[6]);			// HPゲージ
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_3, &m_pTexture[7]);			// 危険度(背景)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_4, &m_pTexture[8]);			// 危険度(マーク)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_5, &m_pTexture[9]);			// 残りクリスタル数
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_6, &m_pTexture[10]);			// ×
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_7, &m_pTexture[11]);			// 血しぶき
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_UI_8, &m_pTexture[12]);			// レティクル

	// チュートリアル
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_0, &m_pTexture[13]);	// 表示背景
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_1, &m_pTexture[14]);	// カメラ操作
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_2, &m_pTexture[15]);	// 移動操作
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_3, &m_pTexture[16]);	// ジャンプ操作
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_4, &m_pTexture[17]);	// 攻撃操作
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_5, &m_pTexture[18]);	// ライフ説明
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_6, &m_pTexture[19]);	// 危険度説明
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_7, &m_pTexture[20]);	// 終了説明
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_8, &m_pTexture[21]);	// 確認画面(背景)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_9, &m_pTexture[22]);	// 確認画面(YES)
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_TUTORIAL_10, &m_pTexture[23]);	// 確認画面(NO)

	// リザルト
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_0, &m_pTexture[24]);		// GAMECLEAR
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_1, &m_pTexture[25]);		// GAMEOVER
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_2, &m_pTexture[26]);		// メニュー背景
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_3, &m_pTexture[27]);		// RETRY
	D3DXCreateTextureFromFile(pDevice, TEXTURENAME_RESULT_4, &m_pTexture[28]);		// QUIT

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CTexture::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
	{
		//テクスチャーの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//===============================================================================
//　生成処理
//===============================================================================
CTexture * CTexture::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType)
{
	CTexture *pTitleTEX = NULL;

	// NULLチェック
	if (pTitleTEX == NULL)
	{// メモリの動的確保

		pTitleTEX = new CTexture;

		if (pTitleTEX != NULL)
		{
			// 種類を割り当てる
			pTitleTEX->m_nType = nType;
			// オブジェクトクラスの生成
			pTitleTEX->Init();
			// テクスチャを割り当てる
			pTitleTEX->BindTexture(m_pTexture[nType]);
			// 位置を割り当てる
			pTitleTEX->SetPosSize(pos, size);
			// サイズの割り当て
			pTitleTEX->m_size = size;
			pTitleTEX->m_sizeDest = size;
			if (nType == 13 )
			{// 表示背景だった時
				D3DXCOLOR col[4];
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
				pTitleTEX->SetColor(&col[0]);
			}
			else if (nType >= 14 && nType <= 23)
			{
				D3DXCOLOR col[4];
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				pTitleTEX->SetColor(&col[0]);
			}
		}
	}

	return pTitleTEX;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTexture::Init(void)
{
	srand((unsigned int)time(0));		// ランダム
	CScene2D::Init();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTexture::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTexture::Update(void)
{
	CManager::MODE m_mode = CManager::GetMode();
	switch (m_mode)
	{// カメラのタイプ別処理
	case CManager::MODE_TITLE:
		TitleMove();
		break;
	case CManager::MODE_TUTORIAL:
		TutorialMove();
		break;
	case CManager::MODE_GAME:
		GameMove();
		break;
	case CManager::MODE_RESULT:
		ResultMove();
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTexture::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// タイトルの動き
//=============================================================================
void CTexture::TitleMove(void)
{
	CFade::FADE		fade		= CFade::GetFade();		// フェードの取得
	CTitle::TYPE	pTitleType	= CTitle::GetType();	// タイトルのタイプの取得
	D3DXVECTOR3		pos			= GetPosition();		// 位置の取得
	D3DXCOLOR	col[4];

	if (m_nType == 0 || m_nType == 1)
	{// タイトルロゴ
		if (pTitleType == CTitle::TYPE_PRESS)
		{
			// 移動 ------------------------------
			if (m_nType == 0)
			{
				pos.x += -7.5f;		// 移動量(X)
				pos.y += 4.4f;		// 移動量(Y)
			}
			else if (m_nType == 1)
			{
				pos.x += 7.5f;		// 移動量(X)
				pos.y += -4.4f;		// 移動量(Y)
			}
			// 透明 ------------------------------
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.05f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.05f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.05f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.05f));
			if (col[0].a <= 0.0f)
			{
				if (fade != CFade::FADE_OUT)
				{
					// モード設定
					CFade::SetFade(CManager::MODE_TUTORIAL);
				}
			}

			CScene2D::SetPosSize(pos, m_size);
			CScene2D::SetColor(&col[0]);
		}
	}
	else if (m_nType == 3)
	{//	PRESS
		if (pTitleType == CTitle::TYPE_NONE)
		{
			if (m_nBlink == false)
			{//	カラーを薄くする
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nBlink = true;
				}
			}
			else if (m_nBlink == true)
			{//	カラーを濃くする
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
				if (col[0].a >= 1.0f)
				{
					m_nCntColor = 0;
					m_nBlink = false;
				}
			}
		}
		else if (pTitleType == CTitle::TYPE_PRESS)
		{
			if (m_nBlink == false)
			{//	カラーを薄くする
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.1f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.1f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.1f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.1f));
				if (col[0].a <= 0.25f)
				{
					m_nCntColor = 0;
					m_nBlink = true;
				}
			}
			else if (m_nBlink == true)
			{//	カラーを濃くする
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.2f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.2f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.2f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.2f));
				if (col[0].a >= 1.0f)
				{
					m_nCntColor = 0;
					m_nBlink = false;
				}
			}
		}
		CScene2D::SetColor(&col[0]);
	}
	m_nCntflame++;		// カウンターの更新
}

//=============================================================================
// チュートリアルの動き
//=============================================================================
void CTexture::TutorialMove(void)
{
	CFade::FADE fade		= CFade::GetFade();						// フェードの取得
	D3DXVECTOR3 pos			= GetPosition();						// 位置の取得
	int			PlayerLife	= CGame::GetPlayer()->GetLife();		// プレイヤーライフの取得
	D3DXCOLOR	col[4];

	if (m_nType == 6)
	{// HPゲージ
		CScene2D::SetInitGauge(pos, D3DXVECTOR2((GAUGE_SIZE / PLAYER_LIFE)* PlayerLife, m_size.y));
	}
	else if (m_nType == 8)
	{//	危険度
		int nPatternAnim = 0;

		for (int nCntPriotity = 0; nCntPriotity < NUM_PRIORITY; nCntPriotity++)
		{
			CScene *pScene;
			// プライオリティーチェック
			pScene = CScene::GetTop(nCntPriotity);
			while (pScene != NULL)
			{// NULLチェック
			 // UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
				CScene *pSceneNext = pScene->GetNext();
				if (pScene->GetDeath() == false)
				{
					if (pScene->GetObjType() == OBJTYPE_ENEMY)
					{// エネミーだった時
						// オブジェクトの当たり判定
						CEnemy::EnemyStatse statse = ((CEnemy*)pScene)->GetStatse();
						switch (statse)
						{// カメラのタイプ別処理
						case CEnemy::ENEMYSTATSE_NONE:		// 通常
							if (nPatternAnim != 1 && nPatternAnim != 2)
							{// 警戒か追跡じゃないときだけ通る
								nPatternAnim = 0;
							}
							break;
						case CEnemy::ENEMYSTATSE_VIGILANCE:	// 警戒
							if (nPatternAnim != 2)
							{// 追跡じゃないときだけ通る
								nPatternAnim = 1;
							}
							break;
						case CEnemy::ENEMYSTATSE_PURSUIT:	// 追跡
							// 追跡だったら必ず通る
							nPatternAnim = 2;
							break;
						}
					}
				}
				// Nextに次のSceneを入れる
				pScene = pSceneNext;
			}
		}
		CScene2D::SetTexture(nPatternAnim, 3, 1);
	}

	if (m_nType == 14 && m_nCntTutorial == 0)
	{// カメラ操作
		if (m_nBlink == false)
		{//	カラーを濃くする
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// カラーの更新
		}
		else if (m_nBlink == true)
		{//	カラーを薄くする
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// カラーの更新
			}
		}
	}
	else if (m_nType == 15 && m_nCntTutorial == 1)
	{// 移動操作
		if (m_nBlink == false)
		{//	カラーを濃くする
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// カラーの更新
		}
		else if (m_nBlink == true)
		{//	カラーを薄くする
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// カラーの更新
			}
		}
	}
	else if (m_nType == 16 && m_nCntTutorial == 2)
	{// ジャンプ操作
		if (m_nBlink == false)
		{//	カラーを濃くする
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// カラーの更新
		}
		else if (m_nBlink == true)
		{//	カラーを薄くする
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// カラーの更新
			}
		}
	}
	else if (m_nType == 17 && m_nCntTutorial == 3)
	{// 攻撃操作
		if (m_nBlink == false)
		{//	カラーを濃くする
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// カラーの更新
		}
		else if (m_nBlink == true)
		{//	カラーを薄くする
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// カラーの更新
			}
		}
	}
	else if (m_nType == 18 && m_nCntTutorial == 4)
	{// ライフ説明
		if (m_nBlink == false)
		{//	カラーを濃くする
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// カラーの更新
		}
		else if (m_nBlink == true)
		{//	カラーを薄くする
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// カラーの更新
			}
		}
	}
	else if (m_nType == 19 && m_nCntTutorial == 5)
	{// 危険度説明
		if (m_nBlink == false)
		{//	カラーを濃くする
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// カラーの更新
		}
		else if (m_nBlink == true)
		{//	カラーを薄くする
			m_nCntflame++;
			if (m_nCntflame >= 600)
			{
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.0f)
				{
					m_nCntColor = 0;
					m_nCntTutorial++;
				}
				CScene2D::SetColor(&col[0]);	// カラーの更新
			}
		}
	}
	else if (m_nType == 20 && m_nCntTutorial == 6)
	{// 終了説明
		if (m_nBlink == false)
		{//	カラーを濃くする
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + (m_nCntColor * 0.01f));
			if (col[0].a >= 1.0f)
			{
				m_nCntColor = 0;
				m_nCntTutorial++;
				m_nBlink = true;
			}
			CScene2D::SetColor(&col[0]);	// カラーの更新
		}
	}
}

//=============================================================================
// ゲームの動き
//=============================================================================
void CTexture::GameMove(void)
{
	D3DXVECTOR3 pos = GetPosition();					// プレイヤー座標の取得
	int PlayerLife	= CGame::GetPlayer()->GetLife();	// プレイヤーライフの取得

	// 敵の状態を取得
	for (int nCntPriotity = 0; nCntPriotity < NUM_PRIORITY; nCntPriotity++)
	{
		CScene *pScene;
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriotity);

		// NULLチェック
		while (pScene != NULL)
		{
			// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetDeath() == false)
			{
				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{
					if (m_EnemyStatse != ((CEnemy*)pScene)->GetStatse())
					{// ステータスが違うとき
						m_EnemyStatse = ((CEnemy*)pScene)->GetStatse();
						if (((CEnemy*)pScene)->GetStatse() != CEnemy::ENEMYSTATSE_NONE)
						{// ステータスが通常じゃないとき
							m_EnemyStatse = ((CEnemy*)pScene)->GetStatse();
							break;
						}
					}
				}
			}
			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}

	if (m_nType == 6)
	{// HPゲージ
		CScene2D::SetInitGauge(pos, D3DXVECTOR2((GAUGE_SIZE / PLAYER_LIFE)* PlayerLife, m_size.y));
	}
	else if (m_nType == 8)
	{//	危険度
		int nPatternAnim = 0;

		for (int nCntPriotity = 0; nCntPriotity < NUM_PRIORITY; nCntPriotity++)
		{
			CScene *pScene;
			// プライオリティーチェック
			pScene = CScene::GetTop(nCntPriotity);
			while (pScene != NULL)
			{// NULLチェック
				// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
				CScene *pSceneNext = pScene->GetNext();
				if (pScene->GetDeath() == false)
				{
					if (pScene->GetObjType() == OBJTYPE_ENEMY)
					{
						// オブジェクトのあたる判定
						CEnemy::EnemyStatse statse = ((CEnemy*)pScene)->GetStatse();
						switch (statse)
						{// カメラのタイプ別処理
						case CEnemy::ENEMYSTATSE_NONE:		// 通常
							if (nPatternAnim != 1 && nPatternAnim != 2)
							{// 警戒か追跡じゃないときだけ通る
								nPatternAnim = 0;
							}
							break;
						case CEnemy::ENEMYSTATSE_VIGILANCE:	// 警戒
							if (nPatternAnim != 2)
							{// 追跡じゃないときだけ通る
								nPatternAnim = 1;
							}
							break;
						case CEnemy::ENEMYSTATSE_PURSUIT:	// 追跡
							// 追跡だったら必ず通る
							nPatternAnim = 2;
							break;
						}
					}
				}
				// Nextに次のSceneを入れる
				pScene = pSceneNext;
			}
		}
		CScene2D::SetTexture(nPatternAnim, 3, 1);
	}

	if (m_nType == 11)
	{// 血しぶき
		CPlayer::PlayerStageStatus PlayerState = CGame::GetPlayer()->GetStatus();		// プレイヤーの状態を取得
		D3DXCOLOR col[4];

		switch (PlayerState)
		{// カメラのタイプ別処理
		case CPlayer::PLAYERSTAGESTATUS_NONE:
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			break;
		case CPlayer::PLAYERSTAGESTATUS_DAMAGE:
			m_nCntColor++;
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.02f));
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.02f));
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.02f));
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.02f));
			if (col[0].a <= 0.0f)
			{
				m_nCntColor = 0;
				CGame::GetPlayer()->SetStatus(CPlayer::PLAYERSTAGESTATUS_NONE);
			}
			break;
		}
		CScene2D::SetColor(&col[0]);
	}
	m_nCounterAnim++;
	m_nCntflame++;		// カウンターの更新
}

//=============================================================================
// リザルトの動き
//=============================================================================
void CTexture::ResultMove(void)
{
	int nSelect = CResult::GetSelect();		// 選択番号の取得
	D3DXCOLOR col[4];

	if (m_nType == 27)
	{// RETRY
		if (nSelect == 0)
		{
			if (m_nBlink == false)
			{//	カラーを薄くする
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.5f)
				{
					m_nCntColor = 0;
					m_nBlink = true;
				}
			}
			else if (m_nBlink == true)
			{//	カラーを濃くする
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				if (col[0].a >= 1.0f)
				{
					m_nCntColor = 0;
					m_nBlink = false;
				}
			}
		}
		else
		{
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		CScene2D::SetColor(&col[0]);
	}
	else if (m_nType == 28)
	{// QUIT
		if (nSelect == 1)
		{
			if (m_nBlink == false)
			{//	カラーを薄くする
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (m_nCntColor * 0.01f));
				if (col[0].a <= 0.5f)
				{
					m_nCntColor = 0;
					m_nBlink = true;
				}
			}
			else if (m_nBlink == true)
			{//	カラーを濃くする
				m_nCntColor++;
				col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f + (m_nCntColor * 0.01f));
				if (col[0].a >= 1.0f)
				{
					m_nCntColor = 0;
					m_nBlink = false;
				}
			}
		}
		else
		{
			col[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			col[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}
		CScene2D::SetColor(&col[0]);
	}
}