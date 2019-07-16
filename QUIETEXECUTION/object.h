//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 荒谷由朗
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "model3D.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME_0	"data\\MODEL\\object\\barrel.x"			// 樽
#define MODEL_NAME_1	"data\\MODEL\\object\\stone000.x"		// 岩
#define MODEL_NAME_2	"data\\MODEL\\object\\crystal.x"		// クリスタル
#define MAX_OBJECT_TEXTURE			(3)				// オブジェクトのテクスチャ数
#define MAX_OBJECT					(3)				// オブジェクトのモデル数
#define OBJECT_PRIOTITY				(2)				// オブジェクトの描画順
#define CRYSTAL_MOVE				(0.01f)			// クリスタルの回転量
#define CRYSTAL_MOVETIME			(200.0f)		// クリスタルの回転量
#define CLEAR_CRYSTALNUM			(18)			// クリスタルの目標獲得数
#define MODEL_SPEED					(5.0f)			// 移動量
#define PLAYER_DEPTH				(10)			// プレイヤーの幅調整用
#define ENEMY_DEPTH					(10)			// エネミーの幅調整用

//=====================
//  CModel3Dの派生クラス
//=====================
class CObject : public CModel3D
{
public:// 誰でも扱える
	CObject();
	~CObject();
	static HRESULT	Load	(int nMaxModel);
	static void		UnLoad	(void);
	static CObject	*Create	(D3DXVECTOR3 pos, float move, int nTexTypeint,int nObjectType, MOVETYPE nMovetype, int nCollision);

	HRESULT Init					(void);
	void	Uninit					(void);
	void	Update					(void);
	void	Draw					(void);
	bool	CollisionObject			(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, bool bTouchOld);
	bool	CollisionObjectEnemy	(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
	void	CollisionBullet			(void);

private:// 個人でのみ使う
	static LPD3DXMESH			m_pMeshModel	[MAX_OBJECT];				// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMatModel	[MAX_OBJECT];				// マテリアルの情報へのポインタ
	static DWORD				m_nNumMatModel	[MAX_OBJECT];				// マテリアルの情報数
	static LPDIRECT3DTEXTURE9	m_pMeshTextures	[MAX_OBJECT_TEXTURE];
	D3DXVECTOR3					m_pos;										// 位置
	D3DXVECTOR3					m_rot;										// 向き
	D3DXVECTOR3					m_move;										// 移動量
	int							m_nCount;									// カウンター
	int							m_nCollision;								// 当たり判定のONOFF
	int							m_nType;									// 種類
	D3DXVECTOR3					m_ModelMove;								// 移動の仕方
	bool						m_bCrystalMove;								// クリスタルの移動切り替え
	float						m_fCrystalMoveFleam;						// クリスタルの移動時間
	static int					m_nCrystalCounter;							// クリスタルの獲得数
};
#endif