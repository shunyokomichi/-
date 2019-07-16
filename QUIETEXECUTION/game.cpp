//=============================================================================
//
// ゲーム処理 [game.h]
// Author :		荒谷由朗
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "MeshField.h"
#include "wall.h"
#include "object.h"
#include "bullet.h"
#include "particle.h"
#include "score.h"
#include "pause.h"
#include "loadtext.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FILENAME	( "data/TEXT/Map.txt" )		// ファイル名
#define WORD_NUM		(50)						// 文字数
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CPlayer			*CGame::m_pPlayer	= NULL;
CWall			*CGame::m_pWall		= NULL;
CObject			*CGame::m_pObject	= NULL;
CEnemy			*CGame::m_pEnemy	= NULL;

//=============================================================================
//　コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_pPlayer = NULL;
	m_bPause = false;
	m_text	= NULL;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//　初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	m_nCntType = 0;
	CLoadText::MAPTEXT Map;
	//***********************************
	// ロード
	//***********************************
	// テキスト読み込み
	char *cFileName[WORD_NUM] = { MAX_FILENAME };
	if (m_text == NULL)
	{
		m_text = new CLoadText;
		m_text->TextRoad(cFileName[0]);
		Map = m_text->GetMapText();
	}
	// スコアのロード
	CNumber::Load();
	// テクスチャのロード
	CTexture::Load();
	// パーティクルのテクスチャの読み込み
	CParticle::Load();
	//壁のテクスチャの読み込み
	m_pWall->Load();
	//オブジェクトのテクスチャの読み込み
	m_pObject->Load(m_text->GetMapText().nTypeNum[0]);

	//***********************************
	// 生成
	//***********************************
	SetWall();		 // 壁
	SetObjet(Map);		 // オブジェクト
	SetTex();		 // UI
	SetFloor(Map);		 // 床

	CScore::AddScore(CLEAR_CRYSTALNUM);		// スコアの設定

	if (m_pPlayer == NULL)
	{// プレイヤーの生成
		m_pPlayer->Create(D3DXVECTOR3(0.0f, PLAYER_DOWN_HEIGHT, 0.0f));
	}

	// エネミーの生成
	SetEnemy();

	// サウンドの生成
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//=============================================================================
//　終了処理
//=============================================================================
void CGame::Uninit(void)
{
	CNumber::Unload();		// スコア
	CTexture::UnLoad();		// テクスチャ
	CParticle::Unload();	// パーティクル

	if (m_text != NULL)
	{
		m_text->Reset();
		m_text = NULL;
	}

	//フェード以外削除
	CScene::NotFadeReleseAll();

	// サウンドの削除
	CSound *pSound = CManager::GetSound();
	if (pSound != NULL)
	{
		pSound->AllStopSound();
	}
}

//=============================================================================
//　更新処理
//=============================================================================
void CGame::Update(void)
{
	CFade::FADE fade		= CFade::GetFade();					// フェードの取得
	CInputKeyBoard *pKey	= CManager::GetInputKeyboard();		// 入力情報の取得

	if (fade == CFade::FADE_IN)
	{// フェードイン状態のとき
		if (pKey->GetKeyboardTrigger(DIK_P) == true)
		{// ポーズ切り替え
				m_bPause = m_bPause ? false : true;
				CSound *pSound = CManager::GetSound();		// サウンド情報の取得
				if (m_bPause == true)
				{
					pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPENCLOSE);
				}
				else if (m_bPause == false)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPENCLOSE);
				}
		}
	}

	if (pKey->GetKeyboardTrigger(DIK_RETURN))
	{
		CFade::SetFade(CManager::MODE_RESULT);
	}
}

//=============================================================================
//　描画処理
//=============================================================================
void CGame::Draw(void)
{
}

//=============================================================================
//　テクスチャの生成
//=============================================================================
void CGame::SetTex(void)
{// CTexture::Create(座標、サイズ、種類);
	// HPゲージ
	CTexture::Create(D3DXVECTOR3(180.0f, 30.0f, 0.0f), D3DXVECTOR2(170.0f, 20.0f), 4);				// ゲージ(枠)
	CTexture::Create(D3DXVECTOR3(33.0f, 30.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f), 5);				// HPアイコン
	CTexture::Create(D3DXVECTOR3(60.0f, 30.0f, 0.0f), D3DXVECTOR2(GAUGE_SIZE, 5.0f), 6);			// HPゲージ

	// 危険度
	CTexture::Create(D3DXVECTOR3(SCREEN_WIDTH/2, 50.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), 7);		// 危険度(背景)
	CTexture::Create(D3DXVECTOR3(SCREEN_WIDTH/2, 50.0f, 0.0f), D3DXVECTOR2(35.0f, 35.0f), 8);		// 危険度(マーク)

	// 残りクリスタル
	CTexture::Create(D3DXVECTOR3(960.0f, 50.0f, 0.0f), D3DXVECTOR2(100.0f, 30.0f), 9);				// 残りクリスタル数
	CTexture::Create(D3DXVECTOR3(1100.0f, 50.0f, 0.0f), D3DXVECTOR2(20.0f, 20.0f), 10);				// ×
	CScore::Create(D3DXVECTOR3(1220.0f, 50.0f, 0.0f));												// 数字

	// 血しぶき
	CTexture::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH , SCREEN_HEIGHT), 11);

	// レティクル
	CTexture::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(30.0f, 30.0f), 12);

}

//=============================================================================
//　床の生成
//=============================================================================
void CGame::SetFloor(CLoadText::MAPTEXT Map)
{// CMeshField::Create(座標、サイズ、分割数)
	for (int nCntFloor = 0; nCntFloor < Map.nNum[m_nCntType]; nCntFloor++)
	{
		CMeshField::Create(Map.pos[m_nCntType][m_nCntType], Map.size[m_nCntType].x, Map.size[m_nCntType].z, 100, 100, Map.nType[m_nCntType][m_nCntType]);	// Create(座標、サイズ、分割数)
	}
	m_nCntType++;
}

//=============================================================================
//　壁の生成
//=============================================================================
void CGame::SetWall(void)
{// CWall::Create(座標、サイズ、向き、タイプ)
	CWall::Create(D3DXVECTOR3(1000.0f, 0.0f, 0.0f)		, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// ステージ
	CWall::Create(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f)		, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(750.0f, 0.0f, 750.0f)		, D3DXVECTOR2(250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-750.0f, 0.0f, 750.0f)	, D3DXVECTOR2(250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-0.0f, 0.0f, -750.0f)		, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-500.0f, 0.0f, 1750.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// 通路
	CWall::Create(D3DXVECTOR3(500.0f, 0.0f, 1750.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-3000.0f, 0.0f, 6500.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// ステージ
	CWall::Create(D3DXVECTOR3(3000.0f, 0.0f, 5750.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(0.0f, 0.0f, 8750.0f)		, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-1750.0f, 0.0f, 2750.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(1250.0f, 0.0f, 2750.0f)	, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-5000.0f, 0.0f, 4250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);	// ステージ
	CWall::Create(D3DXVECTOR3(-7500.0f, 0.0f, 2250.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-6000.0f, 0.0f, -1750.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-3000.0f, 0.0f, 500.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-9000.0f, 0.0f, 250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-7000.0f, 0.0f, 3250.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-9000.0f, 0.0f, 5750.0f)	, D3DXVECTOR2(3500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// 通路
	CWall::Create(D3DXVECTOR3(-8000.0f, 0.0f, 3500.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-8000.0f, 0.0f, 8500.0f)	, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-7000.0f, 0.0f, 6250.0f)	, D3DXVECTOR2(1500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-7500.0f, 0.0f, 7750.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-7500.0f, 0.0f, 4750.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-11000.0f, 0.0f, 13250.0f), D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// ステージ
	CWall::Create(D3DXVECTOR3(-3000.0f, 0.0f, 13250.0f)	, D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-10000.0f, 0.0f, 9250.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-5500.0f, 0.0f, 9250.0f)	, D3DXVECTOR2(2500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-9000.0f, 0.0f, 17250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-4500.0f, 0.0f, 17250.0f)	, D3DXVECTOR2(1500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(-7000.0f, 0.0f, 19250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// 通路
	CWall::Create(D3DXVECTOR3(-6000.0f, 0.0f, 18750.0f)	, D3DXVECTOR2(1500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-4250.0f, 0.0f, 21250.0f)	, D3DXVECTOR2(2750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-3750.0f, 0.0f, 20250.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(750.0f, 0.0f, 21250.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);	// ステージ
	CWall::Create(D3DXVECTOR3(750.0f, 0.0f, 11250.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(-1500.0f, 0.0f, 15750.0f)	, D3DXVECTOR2(4500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(3000.0f, 0.0f, 13750.0f)	, D3DXVECTOR2(2500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(3000.0f, 0.0f, 19250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(4250.0f, 0.0f, 16250.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);	// 通路
	CWall::Create(D3DXVECTOR3(4250.0f, 0.0f, 17250.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(4500.0f, 0.0f, 12750.0f)	, D3DXVECTOR2(2500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// ステージ
	CWall::Create(D3DXVECTOR3(3500.0f, 0.0f, 20250.0f)	, D3DXVECTOR2(2000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(5500.0f, 0.0f, 17750.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(5500.0f, 0.0f, 15750.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(7500.0f, 0.0f, 22250.0f)	, D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(4500.0f, 0.0f, 18250.0f)	, D3DXVECTOR2(1000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(5000.0f, 0.0f, 15250.0f)	, D3DXVECTOR2(500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(7500.0f, 0.0f, 10250.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(11500.0f, 0.0f, 16250.0f)	, D3DXVECTOR2(6000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(11500.0f, 0.0f, 6250.0f)	, D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// 通路
	CWall::Create(D3DXVECTOR3(10500.0f, 0.0f, 6250.0f)	, D3DXVECTOR2(4000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(11500.0f, 0.0f, -2750.0f)	, D3DXVECTOR2(5000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// ステージ
	CWall::Create(D3DXVECTOR3(4500.0f, 0.0f, 0.0f)		, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(4500.0f, 0.0f, -5500.0f)	, D3DXVECTOR2(2250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(8000.0f, 0.0f, -7750.0f)	, D3DXVECTOR2(3500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(7500.0f, 0.0f, 2250.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CWall::Create(D3DXVECTOR3(2000.0f, 0.0f, -250.0f)	, D3DXVECTOR2(3000.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);	// 通路
	CWall::Create(D3DXVECTOR3(3000.0f, 0.0f, 250.0f)	, D3DXVECTOR2(2500.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(3250.0f, 0.0f, -3250.0f)	, D3DXVECTOR2(1250.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
	CWall::Create(D3DXVECTOR3(3750.0f, 0.0f, -2250.0f)	, D3DXVECTOR2(750.0f, MAX_WALL_HEIGHT)	, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
}

//=============================================================================
//　オブジェクトの生成
//=============================================================================
void CGame::SetObjet(CLoadText::MAPTEXT Map)
{// CObject::Create( 座標 , 移動量 , 使用する画像の番号 , 生成するオブジェクトの番号 , オブジェクトのタイプ , 当たり判定の有無 );
	for (int nCntModel = 0; nCntModel < Map.nNum[m_nCntType]; nCntModel++)
	{
		//	// クリスタル
		CObject::Create(Map.pos[m_nCntType][nCntModel], 0.0f, Map.nType[m_nCntType][nCntModel], Map.nType[m_nCntType][nCntModel], CObject::MOVETYPE_NOT, 0);
	}

	m_nCntType++;
}
void CGame::SetEnemy(void)
{// CEnemy::Create(座標)
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-1000.0f, 0.0f, 8500.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(2000.0f, 0.0f, 8500.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-7000.0f, 0.0f, 500.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-4000.0f, 0.0f, 500.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-5500.0f, 0.0f, 4000.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-8000.0f, 0.0f, 13000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-5500.0f, 0.0f, 13000.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(2000.0f, 0.0f, 17000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 15000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 21000.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(7300.0f, 0.0f, 13000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(6300.0f, 0.0f, 15000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(8000.0f, 0.0f, 20000.0f));

	m_pEnemy = CEnemy::Create(D3DXVECTOR3(9000.0f, 0.0f, 2000.0f));
	m_pEnemy = CEnemy::Create(D3DXVECTOR3(7000.0f, 0.0f, -4000.0f));
}