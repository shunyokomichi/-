//=============================================================================
//
// リザルト処理 [result.h]
// Author :		荒谷由朗
//
//=============================================================================
#include "result.h"
#include "player.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "MeshField.h"
#include "wall.h"
#include "texture.h"
#include "object.h"
#include "loadtext.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FILENAME	( "data/TEXT/Map.txt" )		// ファイル名
#define WORD_NUM		(50)						// 文字数

//=============================================================================
// 静的メンバ変数
//=============================================================================
CPlayer			*CResult::m_pPlayer = NULL;
CWall			*CResult::m_pWall	= NULL;
CObject			*CResult::m_pObject = NULL;
CEnemy			*CResult::m_pEnemy	= NULL;
int				CResult::m_nSelect	= NULL;
//=============================================================================
//　コンストラクタ
//=============================================================================
CResult::CResult()
{
	m_pPlayer = NULL;
	m_nSelect = 0;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//　初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	m_nCntType = 0;
	CLoadText::MAPTEXT Map;

	//===================================
	// ロード
	//===================================
	// テキスト読み込み
	char *cFileName[WORD_NUM] = { MAX_FILENAME };
	if (m_text == NULL)
	{
		m_text = new CLoadText;
		m_text->TextRoad(cFileName[0]);
		Map = m_text->GetMapText();
	}

	CTexture::Load();

	//壁のテクスチャの読み込み
	m_pWall->Load();

	//オブジェクトのテクスチャの読み込み
	m_pObject->Load(m_text->GetMapText().nTypeNum[0]);

	//===================================
	// 生成
	//===================================
	SetWall();		 // 壁
	SetObjet(Map);	 // オブジェクト
	SetTex();		 // UI
	SetFloor(Map);	 // 床

	// サウンドの生成
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//=============================================================================
//　終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// テクスチャの破棄
	CTexture::UnLoad();

	if (m_text != NULL)
	{
		m_text->Reset();
		m_text = NULL;
	}

	// フェード以外削除
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
void CResult::Update(void)
{
	CFade::FADE		fade	= CFade::GetFade();					// フェード情報を取得
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// 入力情報を取得
	CInputMouse		*pMouse = CManager::GetInputMouse();		// 入力情報の取得(マウス)
	CSound			*pSound = CManager::GetSound();				// サウンドの取得

	// 選択処理
	if (pKey->GetKeyboardTrigger(DIK_UP) == true || pKey->GetKeyboardTrigger(DIK_W) == true)
	{// 選択切り替え(上)
		m_nSelect = (m_nSelect + 1) % 2;
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SWITCH);
	}
	if (pKey->GetKeyboardTrigger(DIK_DOWN) == true || pKey->GetKeyboardTrigger(DIK_S) == true)
	{// 選択切り替え(下)
		m_nSelect = (m_nSelect + 1) % 2;
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SWITCH);
	}

	if (pKey->GetKeyboardTrigger(DIK_RETURN) == true || pMouse->GetMouseTrigger(0) == true)
	{// 決定時
		if (m_nSelect == 0)
		{// RETRYの時
			if (fade != CFade::FADE_OUT)
			{
				CFade::SetFade(CManager::MODE_GAME);
			}
		}
		else if (m_nSelect == 1)
		{// QUITの時
			if (fade != CFade::FADE_OUT)
			{
				CFade::SetFade(CManager::MODE_TITLE);
			}
		}
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_DECISION);
	}
	// デバックログ
	CManager::m_pDebuglog->Print(1, "選択番号 ～ %d ～\n", m_nSelect);
}

//=============================================================================
//　描画処理
//=============================================================================
void CResult::Draw(void)
{
}

//=============================================================================
//　テクスチャの生成
//=============================================================================
void CResult::SetTex(void)
{
	CManager::GAMERESULT gameresult = CManager::GetResult();		// リザルトの取得
	switch (gameresult)
	{
	case CManager::GAMERESULT_WIN:
		CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 150.0f, 0.0f), D3DXVECTOR2(350.0f, 130.0f), 24);	// GAME CREAR
		break;
	case CManager::GAMERESULT_LOSE:
		CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 180.0f, 0.0f), D3DXVECTOR2(350.0f, 150.0f), 25);	// GAME OVER
		break;
	}
	CTexture::Create(D3DXVECTOR3(1000.0f, 500.0f, 0.0f), D3DXVECTOR2(250.0f, 200.0f), 26);	// メニュー背景
	CTexture::Create(D3DXVECTOR3(1000.0f, 425.0f, 0.0f), D3DXVECTOR2(150.0f, 50.0f), 27);	// RETRY
	CTexture::Create(D3DXVECTOR3(1000.0f, 575.0f, 0.0f), D3DXVECTOR2(150.0f, 50.0f), 28);	// QUIT
}

//=============================================================================
//　床の生成
//=============================================================================
void CResult::SetFloor(CLoadText::MAPTEXT Map)
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
void CResult::SetWall(void)
{
}

//=============================================================================
//　オブジェクトの生成
//=============================================================================
void CResult::SetObjet(CLoadText::MAPTEXT Map)
{// CObject::Create( 座標 , 移動量 , 使用する画像の番号 , 生成するオブジェクトの番号 , オブジェクトのタイプ , 当たり判定の有無 );
	CManager::GAMERESULT gameresult = CManager::GetResult();
	switch (gameresult)
	{
	case CManager::GAMERESULT_WIN:
		if (m_pEnemy == NULL)
		{
			m_pEnemy = CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		break;
	case CManager::GAMERESULT_LOSE:
		if (m_pPlayer == NULL)
		{
			m_pPlayer->Create(D3DXVECTOR3(-50.0f, 50.0f, 0.0f));
		}
		break;
	}

	for (int nCntModel = 0; nCntModel < Map.nNum[m_nCntType]; nCntModel++)
	{
		//	// クリスタル
		CObject::Create(Map.pos[m_nCntType][nCntModel], 0.0f, Map.nType[m_nCntType][nCntModel], Map.nType[m_nCntType][nCntModel], CObject::MOVETYPE_NOT, 0);
	}

	m_nCntType++;
}