//=============================================================================
//
// 壁処理 [wall.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CWall::m_pTexture[MAX_WALL_TEX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CWall::CWall() : CScene3D(WALL_PRIOTITY, CScene::OBJTYPE_WALL)
{
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size	= D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CWall::~CWall()
{
}

//=============================================================================
// 読み込み処理
//=============================================================================
HRESULT CWall::Load(void)
{
	// デバイス取得
	CRenderer			*pRenderer	= CManager::GetRenderer();		// レンダラーの取得
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();		// デバイスを取得

	// テクスチャ生成
	D3DXCreateTextureFromFile(pDevice, WALL_TEX_0, &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CWall::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	{
		// テクスチャ破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CWall * CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, int nTexType)
{
	CWall *pWall = NULL;

	// NULLチェック
	if (pWall == NULL)
	{// メモリの動的確保

		pWall = new CWall;

		if (pWall != NULL)
		{
			// 種類の設定
			pWall->m_nType = nTexType;
			// 初期化処理
			pWall->Init();
			// 位置の設定
			pWall->m_pos = pos;
			// 当たり判定用
			pWall->m_size = size;
			// テクスチャの設定
			pWall->BindTexture(m_pTexture[nTexType]);
			// 位置と回転とサイズ
			pWall->SetWall(pos, rot, size);
		}
	}

	return pWall;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWall::Init()
{
	// 初期化処理
	CScene3D::Init();

	// 種類の設定
	SetTypeNumber(TYPE_WALL);

	if (m_nType == 9)
	{
		CScene3D::SetNor(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWall::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}

	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CWall::Update(void)
{
	CollisionBullet();		// 弾との当たり判定
	CScene3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CWall::Draw(void)
{
	D3DXVECTOR3		PlayerPos	= CGame::GetPlayer()->GetPos();			// プレイヤー座標の取得
	CManager::MODE	pMode		= CManager::GetMode();					// ゲームモードの取得

	// 近くの壁以外を消す
	if (m_pos.x + 6000.0f >= PlayerPos.x && m_pos.x - 6000.0f <= PlayerPos.x)
	{
		// 3DモデルのDraw
		CScene3D::Draw();
	}
	else if (pMode == CManager::MODE_TITLE || pMode == CManager::MODE_RESULT)
	{
		CScene3D::Draw();
	}
}
//=============================================================================
// プレイヤーとの当たり判定
//=============================================================================
void CWall::CollisionWallPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth)
{
	CScene *pScene;

	// プライオリティーチェック
	pScene = CScene::GetTop(WALL_PRIOTITY);

	// NULLチェック
	while (pScene != NULL)
	{
		// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();
		if (pScene->GetDeath() == false)
		{// タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{// posとrotを取得
				D3DXVECTOR3 WallPos		= ((CScene3D*)pScene)->GetPosition();
				D3DXVECTOR3 WallRot		= ((CScene3D*)pScene)->GetRotation();
				D3DXVECTOR2 WallSize	= ((CScene3D*)pScene)->GetSize();

				if (WallRot.y == (0.0f * D3DX_PI))
				{// Z向きの壁
					if (pPos->x <= WallPos.x + WallSize.x + 18.5f
						&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
					{// 壁と同じz軸に存在している
						if (pPos->z <= WallPos.z + WallSize.x + 18.5f
							&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
						{// 壁と同じx軸に存在している
							if (pPos->y - PLAYER_DOWN_HEIGHT < WallPos.y + WallSize.y || pPos->y - PLAYER_DOWN_HEIGHT >= WallPos.y)
							{// 壁よりも低い
								if (pPos->z >= WallPos.z - fDepth
									&& pPosOld->z <= WallPos.z - fDepth)
								{// 手前から壁を貫通した
									pPos->z = WallPos.z - fDepth;
								}
								else if (pPos->z <= WallPos.z + fDepth
									&& pPosOld->z >= WallPos.z + fDepth)
								{// 奥から壁を貫通した
									pPos->z = WallPos.z + fDepth;
								}
							}
						}
					}
				}
				else if (WallRot.y == (0.5f * D3DX_PI))
				{// X向きの壁
					if (pPos->x <= WallPos.x + WallSize.x + 18.5f
						&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
					{// 壁と同じz軸に存在している
						if (pPos->z <= WallPos.z + WallSize.x + 18.5f
							&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
						{// 壁と同じx軸に存在している
							if (pPos->y - PLAYER_DOWN_HEIGHT < WallPos.y + WallSize.y || pPos->y - PLAYER_DOWN_HEIGHT >= WallPos.y)
							{// 壁よりも低い
								if (pPos->x >= WallPos.x - fDepth
									&& pPosOld->x <= WallPos.x - fDepth)
								{// 手前から壁を貫通した
									pPos->x = WallPos.x - fDepth;
								}
								else if (pPos->x <= WallPos.x + fDepth
									&& pPosOld->x >= WallPos.x + fDepth)
								{// 奥から壁を貫通した
									pPos->x = WallPos.x + fDepth;
								}
							}
						}
					}
				}
			}
		}
		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 敵との当たり判定
//=============================================================================
void CWall::CollisionWallEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth)
{
	CScene *pScene;

	// プライオリティーチェック
	pScene = CScene::GetTop(WALL_PRIOTITY);

	// NULLチェック
	while (pScene != NULL)
	{
		// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();
		if (pScene->GetDeath() == false)
		{// タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{// posとrotを取得
				D3DXVECTOR3 WallPos		= ((CScene3D*)pScene)->GetPosition();
				D3DXVECTOR3 WallRot		= ((CScene3D*)pScene)->GetRotation();
				D3DXVECTOR2 WallSize	= ((CScene3D*)pScene)->GetSize();

				if (WallRot.y == (0.0f * D3DX_PI))
				{// Z向きの壁
					if (pPos->x <= WallPos.x + WallSize.x + 18.5f
						&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
					{// 壁と同じz軸に存在している
						if (pPos->z <= WallPos.z + WallSize.x + 18.5f
							&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
						{// 壁と同じx軸に存在している
							if (pPos->y < WallPos.y + WallSize.y && pPos->y + ENEMY_HEIGHT >= WallPos.y)
							{// 壁よりも低い
								if (pPos->z >= WallPos.z - fDepth
									&& pPosOld->z <= WallPos.z - fDepth)
								{// 手前から壁を貫通した
									pPos->z = WallPos.z - fDepth;
								}
								else if (pPos->z <= WallPos.z + fDepth
									&& pPosOld->z >= WallPos.z + fDepth)
								{// 奥から壁を貫通した
									pPos->z = WallPos.z + fDepth;
								}
							}
						}
					}
				}
				else if (WallRot.y == (0.5f * D3DX_PI))
				{// X向きの壁
					if (pPos->x <= WallPos.x + WallSize.x + 18.5f
						&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
					{// 壁と同じz軸に存在している
						if (pPos->z <= WallPos.z + WallSize.x + 18.5f
							&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
						{// 壁と同じx軸に存在している
							if (pPos->y < WallPos.y + WallSize.y && pPos->y + ENEMY_HEIGHT >= WallPos.y)
							{// 壁よりも低い
								if (pPos->x >= WallPos.x - fDepth
									&& pPosOld->x <= WallPos.x - fDepth)
								{// 手前から壁を貫通した
									pPos->x = WallPos.x - fDepth;
								}
								else if (pPos->x <= WallPos.x + fDepth
									&& pPosOld->x >= WallPos.x + fDepth)
								{// 奥から壁を貫通した
									pPos->x = WallPos.x + fDepth;
								}
							}
						}
					}
				}
			}
		}
		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 弾の判定
//=============================================================================
void CWall::CollisionBullet()
{
	CScene *pScene;
	// プライオリティーチェック
	pScene = CScene::GetTop(BULLET_PRIOTITY);

	bool bCollision = false;

	// NULLチェック
	while (pScene != NULL)
	{
		// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();
		if (pScene->GetDeath() == false)
		{
			if (pScene->GetObjType() == OBJTYPE_BULLET)
			{// タイプが弾だったら
				bCollision = ((CBullet*)pScene)->CollisionBulletWall(this);
			}
		}
		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}