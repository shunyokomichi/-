//=============================================================================
//
// オブジェクトの処理 [object.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "camera.h"
#include "renderer.h"
#include "game.h"
#include "bullet.h"
#include "enemy.h"
#include "debuglog.h"
#include "particle.h"
#include "score.h"
#include "tutorial.h"
#include "warning.h"
#include "loadtext.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPD3DXMESH			CObject::m_pMeshModel	[MAX_OBJECT]			= {};		// メッシュ情報へのポインタ
LPD3DXBUFFER		CObject::m_pBuffMatModel[MAX_OBJECT]			= {};		// マテリアルの情報へのポインタ
DWORD				CObject::m_nNumMatModel	[MAX_OBJECT]			= {};		// マテリアルの情報数
LPDIRECT3DTEXTURE9	CObject::m_pMeshTextures[MAX_OBJECT_TEXTURE]	= {};
int					CObject::m_nCrystalCounter						= NULL;		// クリスタルの獲得数

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CObject::CObject() : CModel3D(OBJECT_PRIOTITY,CScene::OBJTYPE_OBJECT)
{
	m_pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ModelMove			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bCrystalMove		= false;
	m_fCrystalMoveFleam	= 0.0f;
	m_nCrystalCounter	= 0;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CObject::~CObject()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 3DモデルのInit
	CModel3D::Init();

	// 位置の初期化
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 位置の代入
	CModel3D::SetPosition(pos);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject::Uninit(void)
{
	// 3DモデルのUninit
	CModel3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject::Update(void)
{
	switch (m_nType)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		// クリスタルの上下移動
		m_fCrystalMoveFleam++;
		if (m_bCrystalMove == false)
		{// 上昇時
			m_pos.y += 0.2f;
		}
		else
		{// 下降時
			m_pos.y -= 0.2f;
		}
		CModel3D::SetPosition(m_pos);
		if (m_fCrystalMoveFleam >= CRYSTAL_MOVETIME)
		{// 上下の入れ替え
			m_bCrystalMove = m_bCrystalMove ? false : true;
			m_fCrystalMoveFleam = 0.0f;
		}

		// クリスタルの回転
		m_rot = D3DXVECTOR3(m_rot.x, m_rot.y + CRYSTAL_MOVE, m_rot.z);
		CModel3D::SetRot(m_rot);
		break;
	}

	// 当たり判定
	CollisionBullet();
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	// プレイヤーの位置情報
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	// 近くのモデル以外を消す
	if (m_pos.x + 6000.0f >= PlayerPos.x && m_pos.x - 6000.0f <= PlayerPos.x)
	{
		// 3DモデルのDraw
		CModel3D::Draw();
	}
}

//===============================================================================
//　クリエイト
//===============================================================================
CObject * CObject::Create(D3DXVECTOR3 pos, float move, int nTexType,int nObjectType, MOVETYPE nMovetype, int nCollision)
{
	CObject *pObject = NULL;

	// NULLチェック
	if (pObject == NULL)
	{// メモリの動的確保

		pObject = new CObject;

		if (pObject != NULL)
		{
			// 種類の設定
			pObject->BindModel(m_pMeshModel[nObjectType], m_pBuffMatModel[nObjectType], m_nNumMatModel[nObjectType], m_pMeshTextures[nTexType]);
			// オブジェクトクラスの生成
			pObject->Init();
			// オブジェクトごとの設定用タイプ
			pObject->m_nType = nObjectType;
			// 位置を代入
			pObject->SetPosition(pos);
			pObject->m_pos = pos;
			// 動きの種類と移動量を設定
			pObject->SetMoveType(nMovetype);
			pObject->m_move = D3DXVECTOR3(move, move, move);
			// コリジョンをONOFF
			pObject->m_nCollision = nCollision;
		}
	}

	return pObject;
}

//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CObject::Load(int nMaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;			// マテリアルデータへのポインタ

	for (int nCntModel = 0; nCntModel < nMaxModel; nCntModel++)
	{
		char *cMoedlName = {};
		cMoedlName = CLoadText::GetName(nCntModel, 0);
		// Xファイルの読み込み
		D3DXLoadMeshFromX(cMoedlName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[nCntModel], NULL, &m_nNumMatModel[nCntModel], &m_pMeshModel[nCntModel]);		// 樽
	}

	for (int nCount = 0; nCount < nMaxModel; nCount++)
	{
		// マテリアル情報からテクスチャの取得
		pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCount]->GetBufferPointer();
	}

	// 使っているテクスチャ
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\UV\\object\\barrel.jpg",&m_pMeshTextures[0]);			// 樽
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\UV\\object\\stone_UV.png", &m_pMeshTextures[1]);		// 岩
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\UV\\object\\crystal_UV.jpg", &m_pMeshTextures[2]);	// クリスタル

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CObject::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		// メッシュの開放
		if (m_pMeshModel[nCount] != NULL)
		{
			m_pMeshModel[nCount]->Release();
			m_pMeshModel[nCount] = NULL;
		}
		// マテリアルの開放
		if (m_pBuffMatModel[nCount] != NULL)
		{
			m_pBuffMatModel[nCount]->Release();
			m_pBuffMatModel[nCount] = NULL;
		}
	}
	//テクスチャ
	for (int nCntTex = 0; nCntTex < MAX_OBJECT_TEXTURE; nCntTex++)
	{
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
}

//===============================================================================
// 当たり判定
//===============================================================================
bool CObject::CollisionObject(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove, bool bTouchOld)
{
	// あたっているかあたってないか
	bool bTouch		= false;
	int nCollision	= m_nCollision;

	// 各種情報の取得(どのモデルでも使用する)
	D3DXVECTOR3 ModelPos	= CModel3D::GetPosition();		// 位置
	D3DXVECTOR3 ModelMove	= CModel3D::GetMove();			// 移動量
	D3DXVECTOR3 VtxMax		= CModel3D::VtxMax();			// モデルの最大値
	D3DXVECTOR3 VtxMin		= CModel3D::VtxMin();			// モデルの最小値

	D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::VtxMax();	// 位置込みの最大値
	D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::VtxMin();	// 位置込みの最小値

	switch (m_nCollision)
	{
	case 1: // 岩＆樽

		// 移動量の保持
		if (ModelMove.x == 0.0f)
		{
			ModelMove.x = m_ModelMove.x;
		}
		if (ModelMove.y == 0.0f)
		{
			ModelMove.y = m_ModelMove.y;
		}
		if (ModelMove.z == 0.0f)
		{
			ModelMove.z = m_ModelMove.z;
		}

		if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
		{// Zの範囲内にいる
			if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
			{// Xの範囲内にいる
				if (pPosOld->y - PLAYER_DOWN_HEIGHT >= ModelMax.y && pPos->y - PLAYER_DOWN_HEIGHT <= ModelMax.y)
				{// オブジェクトの上から当たる場合
					pPos->y = ModelMax.y + abs(ModelMove.y) + PLAYER_DOWN_HEIGHT;
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X移動同期
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z移動同期
						pPos->z += ModelMove.z;
					}

					bTouch = true;
				}
				else if (pPosOld->y + PLAYER_UP_HEIGHT <= ModelMin.y && pPos->y + PLAYER_UP_HEIGHT >= ModelMin.y)
				{// オブジェクトの下から当たる場合
					pPos->y = ModelMin.y - PLAYER_UP_HEIGHT;
					pMove->y = 0.0f;
				}

				if (!(pPos->y - PLAYER_DOWN_HEIGHT >= ModelMax.y) && !(pPos->y + PLAYER_UP_HEIGHT <= ModelMin.y))
				{// オブジェクト横との当たり判定
					if (pPosOld->x - PLAYER_DEPTH >= ModelMax.x && pPos->x - PLAYER_DEPTH < ModelMax.x)
					{// 右から抜けた場合
						pPos->x = ModelMax.x + PLAYER_DEPTH;
					}
					else if (pPosOld->x + PLAYER_DEPTH <= ModelMin.x && pPos->x + PLAYER_DEPTH > ModelMin.x)
					{// 左から抜けた場合
						pPos->x = ModelMin.x - PLAYER_DEPTH;
					}
					if (pPosOld->z - PLAYER_DEPTH >= ModelMax.z && pPos->z - PLAYER_DEPTH < ModelMax.z)
					{// 奥から抜けた場合
						pPos->z = ModelMax.z + PLAYER_DEPTH;
					}
					else if (pPosOld->z + PLAYER_DEPTH <= ModelMin.z && pPos->z + PLAYER_DEPTH > ModelMin.z)
					{// 手前から抜けた場合
						pPos->z = ModelMin.z - PLAYER_DEPTH;
					}
				}
			}
		}

		// 位置の代入
		CModel3D::SetPosition(ModelPos);
		// 移動量の代入
		m_ModelMove = ModelMove;

		break;

	case 2:// クリスタル
		// 移動量の保持
		if (ModelMove.x == 0.0f)
		{
			ModelMove.x = m_ModelMove.x;
		}

		if (ModelMove.y == 0.0f)
		{
			ModelMove.y = m_ModelMove.y;
		}

		if (ModelMove.z == 0.0f)
		{
			ModelMove.z = m_ModelMove.z;
		}

		if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
		{// Zの範囲内にいる
			if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
			{// Xの範囲内にいる
				if (pPosOld->y - PLAYER_DOWN_HEIGHT >= ModelMax.y && pPos->y - PLAYER_DOWN_HEIGHT <= ModelMax.y)
				{// オブジェクトの上から当たる場合
					pPos->y = ModelMax.y + abs(ModelMove.y) + PLAYER_DOWN_HEIGHT;
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X移動同期
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z移動同期
						pPos->z += ModelMove.z;
					}
					bTouch = true;
				}
				else if (pPosOld->y + PLAYER_UP_HEIGHT <= ModelMin.y && pPos->y + PLAYER_UP_HEIGHT >= ModelMin.y)
				{// オブジェクトの下から当たる場合
					pPos->y = ModelMin.y - PLAYER_UP_HEIGHT;
					pMove->y = 0.0f;
					bTouch = true;
				}

				if (!(pPos->y - PLAYER_DOWN_HEIGHT >= ModelMax.y) && !(pPos->y + PLAYER_UP_HEIGHT <= ModelMin.y))
				{// オブジェクト横との当たり判定
					if (pPosOld->x - PLAYER_DEPTH >= ModelMax.x && pPos->x - PLAYER_DEPTH < ModelMax.x)
					{// 右から抜けた場合
						pPos->x = ModelMax.x + PLAYER_DEPTH;
						bTouch = true;
					}
					else if (pPosOld->x + PLAYER_DEPTH <= ModelMin.x && pPos->x + PLAYER_DEPTH > ModelMin.x)
					{// 左から抜けた場合
						pPos->x = ModelMin.x - PLAYER_DEPTH;
						bTouch = true;
					}
					if (pPosOld->z - PLAYER_DEPTH >= ModelMax.z && pPos->z - PLAYER_DEPTH < ModelMax.z)
					{// 奥から抜けた場合
						pPos->z = ModelMax.z + PLAYER_DEPTH;
						bTouch = true;
					}
					else if (pPosOld->z + PLAYER_DEPTH <= ModelMin.z && pPos->z + PLAYER_DEPTH > ModelMin.z)
					{// 手前から抜けた場合
						pPos->z = ModelMin.z - PLAYER_DEPTH;
						bTouch = true;
					}
				}
			}
		}
		// 位置の代入
		CModel3D::SetPosition(ModelPos);
		// 移動量の代入
		m_ModelMove = ModelMove;

		if (bTouch == true)
		{// 触れていたら消す
			CFade::FADE fade = CFade::GetFade();
			CManager::MODE m_mode = CManager::GetMode();
			m_nCrystalCounter++;
			if (m_mode == CManager::MODE_GAME)
			{
				CParticle::Create(m_pos, 0, 0);
				Uninit();
				if (m_nCrystalCounter >= CLEAR_CRYSTALNUM)
				{
					CManager::SetResult(CManager::GAMERESULT_WIN);

					if (fade != CFade::FADE_OUT)
					{
						//モード設定
						CFade::SetFade(CManager::MODE_RESULT);
					}
				}
				CScore::AddScore(-1);
			}
			else if(m_mode == CManager::MODE_TUTORIAL && bTouchOld == false)
			{
				if (fade != CFade::FADE_OUT)
				{
					CTutorial *pGame = CManager::GetTutorial();
					CTutorial *pWarning = ((CTutorial*)pGame);
					bool bWarning = pWarning->GetWarning();
					pWarning->SetWarning(true);
				}
			}
		}
		break;
	}
	return bTouch;
}

//===============================================================================
// エネミーとの当たり判定
//===============================================================================
bool CObject::CollisionObjectEnemy(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove)
{
	//あたっているかあたってないか
	bool bLand		= false;
	int nCollision	= m_nCollision;

	switch (m_nCollision)
	{
	case 1:

		// 各種情報の取得
		D3DXVECTOR3 ModelPos	= CModel3D::GetPosition();		// 位置
		D3DXVECTOR3 ModelMove	= CModel3D::GetMove();			// 移動量
		D3DXVECTOR3 VtxMax		= CModel3D::VtxMax();			// モデルの最大値
		D3DXVECTOR3 VtxMin		= CModel3D::VtxMin();			// モデルの最小値

		D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::VtxMax();	// 位置込みの最大値
		D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::VtxMin();	// 位置込みの最小値

		// 移動量の保持
		if (ModelMove.x == 0.0f)
		{
			ModelMove.x = m_ModelMove.x;
		}

		if (ModelMove.y == 0.0f)
		{
			ModelMove.y = m_ModelMove.y;
		}

		if (ModelMove.z == 0.0f)
		{
			ModelMove.z = m_ModelMove.z;
		}

		if (pPos->x + ENEMY_DEPTH >= ModelMin.x && pPos->x - ENEMY_DEPTH <= ModelMax.x)
		{// Zの範囲内にいる
			if (pPos->z + ENEMY_DEPTH >= ModelMin.z && pPos->z - ENEMY_DEPTH <= ModelMax.z)
			{// Xの範囲内にいる
				if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
				{// オブジェクトの上から当たる場合
					pPos->y = ModelMax.y + abs(ModelMove.y);
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X移動同期
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z移動同期
						pPos->z += ModelMove.z;
					}

					//bLand = true;
				}
				else if (pPosOld->y + ENEMY_HEIGHT <= ModelMin.y && pPos->y + ENEMY_HEIGHT >= ModelMin.y)
				{// オブジェクトの下から当たる場合
					pPos->y = ModelMin.y - ENEMY_HEIGHT;
					pMove->y = 0.0f;
				}

				if (!(pPos->y >= ModelMax.y) && !(pPos->y + ENEMY_HEIGHT <= ModelMin.y))
				{// オブジェクト横との当たり判定
					if (pPosOld->x - ENEMY_DEPTH >= ModelMax.x && pPos->x - ENEMY_DEPTH < ModelMax.x)
					{// 右から抜けた場合
						pPos->x = ModelMax.x + ENEMY_DEPTH;
						bLand = true;

						CManager::m_pDebuglog->Print(1, "右\n");
					}
					else if (pPosOld->x + ENEMY_DEPTH <= ModelMin.x && pPos->x + ENEMY_DEPTH > ModelMin.x)
					{// 左から抜けた場合
						pPos->x = ModelMin.x - ENEMY_DEPTH;
						bLand = true;
						CManager::m_pDebuglog->Print(1, "左\n");
					}
					if (pPosOld->z - ENEMY_DEPTH >= ModelMax.z && pPos->z - ENEMY_DEPTH < ModelMax.z)
					{// 奥から抜けた場合
						pPos->z = ModelMax.z + ENEMY_DEPTH;
						bLand = true;
						CManager::m_pDebuglog->Print(1, "奥\n");
					}
					else if (pPosOld->z + ENEMY_DEPTH <= ModelMin.z && pPos->z + ENEMY_DEPTH > ModelMin.z)
					{// 手前から抜けた場合
						pPos->z = ModelMin.z - ENEMY_DEPTH;
						bLand = true;
						CManager::m_pDebuglog->Print(1, "手前\n");
					}
				}
			}
		}

		// 位置の代入
		CModel3D::SetPosition(ModelPos);
		// 移動量の代入
		m_ModelMove = ModelMove;

		break;
	}

	return bLand;
}

//=============================================================================
// 弾の判定
//=============================================================================
void CObject::CollisionBullet()
{
	CScene *pScene;
	bool bCollision = false;

	// プライオリティーチェック
	pScene = CScene::GetTop(BULLET_PRIOTITY);

	// NULLチェック
	while (pScene != NULL)
	{
		// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();
		if (pScene->GetDeath() == false)
		{
			if (pScene->GetObjType() == OBJTYPE_BULLET)
			{// タイプが弾だったら
				bCollision = ((CBullet*)pScene)->CollisionBulletObje(this);
			}
		}
		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}