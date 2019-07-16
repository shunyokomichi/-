//=============================================================================
//
// バレット処理 [bullet.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "object.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "wall.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
//　コンストラクタ
//=============================================================================
CBullet::CBullet() : CScene3D(BULLET_PRIOTITY, CScene::OBJTYPE_BULLET)
{
	m_move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size		= D3DXVECTOR2(0.0f, 0.0f);
	m_nLife		= 50;
	m_nCntLife	= 0;
	m_pEnemy	= NULL;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
//　生成処理
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, CEnemy *pEnemy)
{
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		pBullet = new CBullet;
		if (pBullet != NULL)
		{
			// オブジェクトクラスの生成
			pBullet->Init();
			// テクスチャを割り当てる
			pBullet->BindTexture(m_pTexture);
			// 移動量の割り当て
			pBullet->m_move = move;
			// サイズの割り当て
			pBullet->m_size = size;
			// 誰が打ったのか
			pBullet->m_pEnemy = pEnemy;
			// 弾の位置とサイズを指定
			pBullet->SetPosSize(pos, size);
		}
	}

	return pBullet;
}
//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CBullet::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME00, &m_pTexture);

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CBullet::UnLoad(void)
{
	// テクスチャーの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(void)
{
	CScene3D::Init();
	// 種類の設定
	SetTypeNumber(TYPE_BULLET);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	// m_posを代入
	m_pos = CScene3D::GetPosition();

	// 移動量の代入
	m_pos += m_move;

	// 移動値のセット
	CScene3D::SetMove(m_move);
	// 位置のセット
	CScene3D::SetPosition(m_pos);

	if (m_nCntLife % 60)
	{// ライフの減算
		m_nLife--;
		if (m_nLife < 0)
		{// 0になった時
			Uninit();
		}
	}
	m_nCntLife++;	// カウンターの加算
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
#ifdef  _DEBUG
	//プレイヤーの位置情報
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//近くの弾以外を消す
	if (m_pos.x + 3000.0f >= PlayerPos.x && m_pos.x - 3000.0f <= PlayerPos.x)
	{
		//CScene3D::Draw();
	}
#endif
}

//=============================================================================
// 壁の当たり判定
//=============================================================================
bool CBullet::CollisionBulletWall(CWall *pWall)
{
	bool bCollision = false;

	D3DXVECTOR3 BulletPos	= CScene3D::GetPosition();
	D3DXVECTOR2 BulletSize	= CScene3D::GetSize();

	D3DXVECTOR3 WallPos		= pWall->GetPosition();
	D3DXVECTOR3 WallRot		= pWall->GetRotation();
	D3DXVECTOR2 WallSize	= pWall->GetSize();

	if (WallRot.y == (0.0f * D3DX_PI))
	{// Z向きの壁
		if (BulletPos.x + BulletSize.x <= WallPos.x + WallSize.x
			&& BulletPos.x - BulletSize.x >= WallPos.x - WallSize.x)
		{// 壁と同じz軸に存在している
			if (BulletPos.z + BulletSize.x >= WallPos.z
				&& BulletPos.z - BulletSize.x <= WallPos.z)
			{// 壁と同じx軸に存在している
				if (BulletPos.y - BulletSize.y < WallPos.y + WallSize.y && BulletPos.y + BulletSize.y >= WallPos.y)
				{// 壁よりも低い
					Uninit();
					bCollision = true;
				}
			}
		}
	}
	else if (WallRot.y == (0.5f * D3DX_PI))
	{// X向きの壁
		if (BulletPos.x + BulletSize.x >= WallPos.x
			&& BulletPos.x - BulletSize.x <= WallPos.x)
		{// 壁と同じz軸に存在している
			if (BulletPos.z + BulletSize.x <= WallPos.z + WallSize.x
				&& BulletPos.z - BulletSize.x >= WallPos.z - WallSize.x)
			{// 壁と同じx軸に存在している
				if (BulletPos.y - BulletSize.y < WallPos.y + WallSize.y && BulletPos.y + BulletSize.y >= WallPos.y)
				{// 壁よりも低い
					Uninit();
					bCollision = true;
				}
			}
		}
	}

	return bCollision;
}
//=============================================================================
// オブジェクトの当たり判定
//=============================================================================
bool CBullet::CollisionBulletObje(CObject * pObje)
{
	bool bCollision = false;

	D3DXVECTOR3 BulletPos = CScene3D::GetPosition();
	D3DXVECTOR2 BulletSize = CScene3D::GetSize();

	D3DXVECTOR3 ObjePos = pObje->GetPosition();
	D3DXVECTOR3 ObjeMin = pObje->GetPosition() + pObje->VtxMin();
	D3DXVECTOR3 ObjeMax = pObje->GetPosition() + pObje->VtxMax();

	if (BulletPos.x >= ObjeMin.x && BulletPos.x <= ObjeMax.x)
	{// Zの範囲内にいる
		if (BulletPos.z >= ObjeMin.z && BulletPos.z <= ObjeMax.z)
		{// Xの範囲内にいる
			if (!(BulletPos.y - BulletSize.y >= ObjeMax.y) && !(BulletPos.y + BulletSize.y <= ObjeMin.y))
			{// オブジェクト横との当たり判定
				if (BulletPos.x - BulletSize.x >= ObjeMax.x && BulletPos.x - BulletSize.x >= ObjeMin.x)
				{// 右から抜けた場合
					Uninit();
					bCollision = true;
				}
				else if (BulletPos.x + BulletSize.x <= ObjeMin.x && BulletPos.x + BulletSize.x > ObjeMin.x)
				{// 左から抜けた場合
					Uninit();
					bCollision = true;
				}
				if (BulletPos.z - BulletSize.x <= ObjeMax.z && BulletPos.z - BulletSize.x >= ObjeMin.z)
				{// 奥から抜けた場合
					Uninit();
					bCollision = true;
				}
				else if (BulletPos.z + BulletSize.x <= ObjeMin.z && BulletPos.z + BulletSize.x > ObjeMin.z)
				{// 手前から抜けた場合
					Uninit();
					bCollision = true;
				}
			}
		}
	}

	return bCollision;
}

//=============================================================================
// 捜索の当たり判定
//=============================================================================
bool CBullet::CollisionBulletSearch(void)
{
	bool bCollision = false;

	D3DXVECTOR3 BulletPos = CScene3D::GetPosition();
	D3DXVECTOR2 BulletSize = CScene3D::GetSize();

	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	// 敵と自分の距離を測る
	float fLength = (((PlayerPos.x - BulletPos.x) * (PlayerPos.x - BulletPos.x)) + ((PlayerPos.z - BulletPos.z) * (PlayerPos.z - BulletPos.z)));

	if (PLAYER_RADIUS * PLAYER_RADIUS > fLength)
	{// 範囲内だったら
		if (BulletPos.y - BulletSize.y >= PlayerPos.y - PLAYER_DOWN_HEIGHT)
		{
			Uninit();
			bCollision = true;
		}
	}

	return bCollision;
}
