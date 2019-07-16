//=============================================================================
//
// タイトル処理 [title.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "fade.h"		// ←"texture.h"で使っているので消さない
#include "title.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "texture.h"
#include "MeshField.h"
#include "object.h"
#include "wall.h"
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
CWall			*CTitle::m_pWall	= NULL;
CObject			*CTitle::m_pObject	= NULL;
CTitle::TYPE	CTitle::m_pType		= TYPE_NONE;

//=============================================================================
//　コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_pType = TYPE_NONE;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//　初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
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

	// UIの読み込み
	CTexture::Load();

	// 壁のテクスチャの読み込み
	m_pWall->Load();

	//オブジェクトのテクスチャの読み込み
	m_pObject->Load(Map.nTypeNum[m_nCntType]);

	//===================================
	// 生成
	//===================================
	SetWall();		 // 壁
	SetObjet(Map);		 // オブジェクト
	SetTex();		 // UI
	SetFloor(Map);		 // 床

	// サウンドの生成
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//=============================================================================
//　終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	CTexture::UnLoad();		// テクスチャ

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
void CTitle::Update(void)
{
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// 入力情報の取得
	CInputMouse		*pMouse = CManager::GetInputMouse();		// 入力情報の取得(マウス)

	if (pKey != NULL)
	{
		if (pKey->GetKeyboardPress(DIK_RETURN) == true || pMouse->GetMouseTrigger(0) == true)
		{// マウスクリックしたとき
			m_pType = TYPE_PRESS;	// PRESS状態にする
			CSound *pSound = CManager::GetSound();
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_DECISION);
		}
	}
}

//=============================================================================
//　描画処理
//=============================================================================
void CTitle::Draw(void)
{
}

//=============================================================================
//　テクスチャの生成
//=============================================================================
void CTitle::SetTex(void)
{
	CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 180.0f, 0.0f), D3DXVECTOR2(350.0f, 150.0f), 0);	// タイトルロゴ(左)
	CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 180.0f, 0.0f), D3DXVECTOR2(350.0f, 150.0f), 1);	// タイトルロゴ(右)
	CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 650.0f, 0.0f), D3DXVECTOR2(170.0f, 40.0f), 2);		// PRESS背景
	CTexture::Create(D3DXVECTOR3((SCREEN_WIDTH / 2), 650.0f, 0.0f), D3DXVECTOR2(150.0f, 20.0f), 3);		// PRESS
}

//=============================================================================
//　床の生成
//=============================================================================
void CTitle::SetFloor(CLoadText::MAPTEXT Map)
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
void CTitle::SetWall(void)
{
}

//=============================================================================
//　オブジェクトの生成
//=============================================================================
void CTitle::SetObjet(CLoadText::MAPTEXT Map)
{// CObject::Create( 座標 , 移動量 , 使用する画像の番号 , 生成するオブジェクトの番号 , オブジェクトのタイプ , 当たり判定の有無 );
	for (int nCntModel = 0; nCntModel < Map.nNum[m_nCntType]; nCntModel++)
	{
	//	// クリスタル
		CObject::Create(Map.pos[m_nCntType][nCntModel], 0.0f, Map.nType[m_nCntType][nCntModel], Map.nType[m_nCntType][nCntModel], CObject::MOVETYPE_NOT, 0);
	}

	m_nCntType++;
}