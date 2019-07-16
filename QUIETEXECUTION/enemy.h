//=============================================================================
//
// 敵の処理 [enemy.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "sceneX.h"
#include "model.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_FILE				"data\\TEXT\\Motion_enemy.txt"	// 読み込むtxtファイルの名前
#define MAX_ENEMYANIM			(8)								// アニメーション数
#define MAX_ENEMYKEY			(8)								// キーフレーム数
#define MAX_ENEMYPARTS			(16)							// パーツの最大数
#define MAX_ENEMYMOTION			(10)							// モーションの最大数
#define ENEMY_HEIGHT			(500.0f)						// 敵の高さ
#define ENEMY_EYE				(350.0f)						// 敵の目線
#define ENEMY_LIFE				(100)							// 体力の最大値
#define ENEMY_SPEED				(2.0f)							// 移動速度
#define ENEMY_VIGILANCESPEED	(0.004f)						// 移動速度
#define ENEMY_PURSUITSPEED		(0.02f)							// 移動速度
#define ENEMY_RADIUS			(40.0f)							// 当たり判定の範囲(半径)
#define ENEMY_CIRCLEMOVE		(0.5f)							// カメラの移動量
#define ENEMY_SEARCH_NUM		(10)							// 捜索弾の最大数
#define ENEMY_SEARCH_INTERVAL	(0.07f)							// 捜索弾の間隔
#define MAX_ENEMYCOLLBILL		(19)							// ビルボードの最大数
#define ENEMY_SEARCH_PURSUIT	(1000.0f)						// 追跡の範囲
#define ENEMY_SEARCH_VIGILANCE	(3000.0f)						// 警戒の範囲
#define ENEMY_ATTACK			(500.0f)						// 攻撃の範囲

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBillBoord;
class CBullet;

//=============================================================================
// クラスの定義
//=============================================================================
class CEnemy : public CScene  // ←派生クラス
{
public:
	// キー要素
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;

	// キー情報
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_ENEMYPARTS];
	}KEY_INFO;

	// モーション情報
	typedef struct
	{
		bool		bLoop;
		int			nNumKey;
		KEY_INFO	aKayInfo[MAX_ENEMYMOTION];
	}MOTION_INFO;

	// モーションタイプ情報
	typedef enum
	{
		ENEMYANIM_NEUTRAL = 0,	// ニュートラルモーション(通常)
		ENEMYANIM_OVERLOOKING,	// 見渡し
		ENEMYANIM_WALK,			// 歩き(通常&警戒)
		ENEMYANIM_RUN,			// 走り(追跡)
		ENEMYANIM_ATTACK,		// 攻撃
		ENEMYANIM_DAMAGE,		// ダメージ
		ENEMYANIM_DEDA,			// 死亡
		ENEMYANIM_MAX			// モーションの最大数
	}EnemyAnim;

	// ステータス情報
	typedef enum
	{
		ENEMYSTATSE_NONE = 0,	// 通常
		ENEMYSTATSE_VIGILANCE,	// 警戒
		ENEMYSTATSE_PURSUIT,	// 追跡
		ENEMYSTATSE_DAMAGE,		// ダメージ
		ENEMYSTATSE_DEDA,		// 死亡
		ENEMYSTATSE_MAX			// ステータスの最大数
	}EnemyStatse;

	CEnemy	(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_ENEMY);
	~CEnemy	();
	static CEnemy	*Create	(D3DXVECTOR3 pos);
	static HRESULT	Load	(void);
	static void		Unload	(void);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);
	void	SetCol	(D3DXCOLOR col)
	{
		for (int nCntModel = 0; nCntModel < MAX_ENEMYPARTS; nCntModel++)
		{
			m_pModel[nCntModel]->SetCol(col);
		}
	}

	EnemyStatse GetStatse(void) { return m_nStatse; }

	// それぞれの状態動作
	void MoveNone		(void);		// 通常
	void MoveVigilance	(void);		// 警戒
	void MovePursuit	(void);		// 追跡
	void MoveDeda		(void);		// 死亡
	void AttackMove		(void);		// 攻撃

	// モーションの更新関数
	void UpdateMotion(void);


	// 当たり判定
	void Collision				(D3DXVECTOR3 posPlayer);
	void CollisionPlayer		(void);																// プレイヤーとの当たり判定
	void CollisionDamege		(D3DXVECTOR3 posHand, CPlayer::PlayerAnim playerState);				// ダメージ判定
	void CollisionSearch		(void);																// 捜索の判定
	bool CollisionDistance		(CBullet *pBullet);													// 敵とプレイヤーの距離
	void CollisitionWall		(void);																// 壁の当たり判定
	void CollisitionObject3D	(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);		// オブジェクトの当たり判定

	// ファイル読み込み関数
	void FileLoad		(void);						// ファイル読み込み
	char *ReadLine		(FILE *pFile, char *pDst);	// 1行読み込み
	char *GetLineTop	(char *pStr);				// 行の先頭を取得
	int  PopString		(char *pStr, char *pDest);	// 行の最後を切り捨て

private:
	static	LPD3DXMESH			m_pMesh		[MAX_ENEMYPARTS];
	static LPD3DXBUFFER			m_pBuffMat	[MAX_ENEMYPARTS];
	static DWORD				m_nNumMat	[MAX_ENEMYPARTS];
	static LPDIRECT3DTEXTURE9	*m_pTexture	[MAX_ENEMYPARTS];		// テクスチャへのポインタ
	D3DXVECTOR3					m_pos;								// 現在の位置
	D3DXVECTOR3					m_posOld;							// 過去の位置
	D3DXVECTOR3					m_rot;								// 向き
	CModel						*m_pModel	[MAX_ENEMYPARTS];		// モデルのポインタ
	static CBillBoord			*m_pBillBoord;						// ビルボードのポインタ
	static CBullet				*m_pBullet;							// 弾のポインタ
	D3DXMATRIX					m_mtxWorld;							// ワールドマトリックス
	EnemyAnim					m_nAnimnow;							// 現在のアニメーション
	EnemyStatse					m_nStatse;							// 現在の状態
	D3DXVECTOR3					m_move;								// 移動
	D3DXVECTOR3					m_vtxMin;							// モデルの最小値
	D3DXVECTOR3					m_vtxMax;							// モデルの最大値
	bool						m_bAttack;							// 攻撃状態
	int							m_nAttackDelay;						// 攻撃の硬直時間
	bool						m_bDamage;							// ダメージ状態
	int							m_nDamageDelay;						// ダメージの硬直時間
	int							m_nLife;							// ライフ
	int							m_nCntSearch;						// 捜索カウンター
	int							m_nCntStatse;						// 状態カウンター
	float						m_fAngle;							// カメラの角度
	int							m_fWalkFrame;						// 起動時カウンター
	bool						m_bMove;							// 移動状態
	int							m_fMoveFlame;						// 移動時間
	D3DXVECTOR3					m_VigilancePos;						// 警戒時の目的地
	D3DXVECTOR3					m_PosDest;							// 目的の座標
	D3DXVECTOR3					m_posAngle;							// 座標の差分
	int							m_nCntAttack;						// 攻撃のカウンター

	// モーション関数	新規
	KEY_INFO			*m_pKeyInfo[MAX_ENEMYMOTION];		// キー情報へのポインタ
	int					m_nKey;								// 現在のキーナンバー
	int					m_nCountFlame;						// フレーム数
	int					m_nNumParts;						// パーツ数
	int					m_aIndexParent[MAX_ENEMYPARTS];		// 親のインデックス
	KEY					m_aKayOffset[MAX_ENEMYPARTS];		// オフセット情報
	MOTION_INFO			m_aMotionInfo[MAX_ENEMYMOTION];		// モーション情報
	int					m_nMotionType;						// モーションのタイプ(int型)
	bool				m_bMotionEnd;						// モーションエンド
	D3DXVECTOR3			m_OffSetPos[MAX_ENEMYPARTS];		// オフセット
};
#endif