//=============================================================================
//
// プレイヤー処理 [player.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "sceneX.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_FILE				"data\\TEXT\\Motion_player.txt"	// 読み込むtxtファイルの名前
#define MAX_PLAYERANIM			(8)								// アニメーション数
#define MAX_PLAYERKEY			(8)								// キーフレーム数

//新規
#define MAX_PLAYERPARTS			(36)		// パーツの最大数
#define MAX_PLAYERMOTION		(10)		// モーションの最大数

#define SQUAT_SPEED				(7.0f)		// しゃがみ速度
#define WALK_SPEED				(25.0f)		// 移動速度
#define RUN_SPEED				(50.0f)		// ダッシュ状態の速度
#define JUMP_POWER				(1000.0f)	// ジャンプ量
#define PLAYER_UP_HEIGHT		(100.0f)	// プレイヤーの高さ(上)
#define PLAYER_DOWN_HEIGHT		(340.0f)	// プレイヤーの高さ(下)
#define PLAYER_SQUAT_HEIGHT		(200.0f)	// プレイヤーの高さ(しゃがみ)
#define PLAYER_RADIUS			(100.0f)	// 当たり判定の範囲(半径)
#define PLAYER_LIFE				(30)		// 体力	の最大値
#define PLAYER_MP				(100)		// MP	の最大値
#define GRAVITY_REAL			(-9.8f)		// 重力(-9.8m/s^2)

//=============================================================================
// クラスの定義
//=============================================================================
class CPlayer : public CScene  // ←派生クラス
{
public:
	//キー要素
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;

	//キー情報
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_PLAYERPARTS];
	}KEY_INFO;

	//モーション情報
	typedef struct
	{
		bool		bLoop;
		int			nNumKey;
		KEY_INFO	aKayInfo[MAX_PLAYERMOTION];
	}MOTION_INFO;

	typedef enum
	{
		PLAYERANIM_NEUTRAL = 0,		// ニュートラルモーション
		PLAYERANIM_MOVE,			// 移動
		PLAYERANIM_ATTACK,			// 攻撃
		PLAYERANIM_CATCH,			// キャッチ
		PLAYERANIM_RESULT,			// リザルト
		PLAYERANIM_MAX				// モーションの最大数
	}PlayerAnim;

	typedef enum
	{
		PLAYERSTAGESTATUS_NONE = 0,	// 通常
		PLAYERSTAGESTATUS_DAMAGE,	// ダメージ
		PLAYERSTAGESTATUS_DEAD,		// 死亡
		PLAYERSTAGESTATUS_MAX		// 状態の最大数
	}PlayerStageStatus;

	CPlayer(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	static HRESULT	Load	(void);
	static void		Unload	(void);
	static CPlayer *Create	(D3DXVECTOR3 pos);

	HRESULT Init			(void);
	void	Uninit			(void);
	void	Update			(void);
	void	Draw			(void);
	void	Set				(D3DXVECTOR3 pos) { GetPos() = pos; }
	void	PlayerMove		(void);
	void	InvincibleMove	(void);

	void CollisitionWall(void);

	static D3DXVECTOR3			&GetPos		(void) { return m_pos; }
	static D3DXVECTOR3			&GetPosOld	(void) { return m_posOld; }
	static D3DXVECTOR3			&GetRot		(void) { return m_rot; }
	static bool					&GetJump	(void) { return m_bJump; }
	static int					&GetLife	(void) { return m_nLife; }
	static int					&GetMp		(void) { return m_nMp; }
	static PlayerStageStatus	&GetStatus	(void) { return m_Status; }

	static void SetStatus	(PlayerStageStatus Status)	{ m_Status = Status; }
	static void SetLife		(int Life)					{ m_nLife = Life; }

	//モーションの更新関数
	void UpdateMotion(void);

	// 当たり判定
	void Collision			(D3DXVECTOR3 posEnemy);
	void CollisionAttack	(D3DXVECTOR3 posEnemy, D3DXMATRIX mtxWorld, float Radius, int NumModel);
	void CollisitionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, bool bTouchOld);

	//ファイル読み込み関数
	void FileLoad	(void);						// ファイル読み込み
	char *ReadLine	(FILE *pFile, char *pDst);	// 1行読み込み
	char *GetLineTop(char *pStr);				// 行の先頭を取得
	int  PopString	(char *pStr, char *pDest);	// 行の最後を切り捨て

private:
	static	LPD3DXMESH			m_pMesh		[MAX_PLAYERPARTS];
	static LPD3DXBUFFER			m_pBuffMat	[MAX_PLAYERPARTS];
	static DWORD				m_nNumMat	[MAX_PLAYERPARTS];
	static LPDIRECT3DTEXTURE9	*m_pTexture	[MAX_PLAYERPARTS];		// テクスチャへのポインタ
	static D3DXVECTOR3			m_pos;								// 現在の位置
	static D3DXVECTOR3			m_posOld;							// 過去の位置
	static D3DXVECTOR3			m_rot;								// 向き
	CModel						*m_pModel	[MAX_PLAYERPARTS];		// モデルのポインタ
	D3DXMATRIX					m_mtxWorld;							// ワールドマトリックス
	static PlayerStageStatus	m_Status;							// プレイヤーの状態
	PlayerAnim					m_nAnimnow;							// 現在のアニメーション
	D3DXVECTOR3					m_rotDest;							//
	D3DXVECTOR3					m_move;								// 移動
	D3DXVECTOR3					m_vtxMin;							// モデルの最小値
	D3DXVECTOR3					m_vtxMax;							// モデルの最大値
	D3DXVECTOR3					m_fAngle;							//
	static bool					m_bJump;							// ジャンプ状態
	static bool					m_bLand;							// 着地状態
	bool						m_bInvincible;						// デバッグ用
	bool						m_bAttack;							// 攻撃状態
	bool						m_bCatch;							// アイテム回収状態
	bool						m_bRun;								// ダッシュ状態
	bool						m_bTouchOld;						// 触れたかどうか(過去)
	int							m_nAttackDelay;						// 攻撃の硬直時間
	static int					m_nLife;							// ライフ
	static int					m_nMp;								// MP
	float						m_fGravityFrame;					// ジャンプの経過時間
	bool						m_bUse;								// 血しぶきのフラグ

	// モーション関数	新規
	KEY_INFO			*m_pKeyInfo		[MAX_PLAYERMOTION];			//キー情報へのポインタ
	int					m_nKey;										//現在のキーナンバー
	int					m_nCountFlame;								//フレーム数
	int					m_nNumParts;								//パーツ数
	int					m_aIndexParent	[MAX_PLAYERPARTS];			//親のインデックス
	KEY					m_aKayOffset	[MAX_PLAYERPARTS];			//オフセット情報
	MOTION_INFO			m_aMotionInfo	[MAX_PLAYERMOTION];			//モーション情報
	int					m_nMotionType;								//モーションのタイプ(int型)
	bool				m_bMotionEnd;
	D3DXVECTOR3			m_OffSetPos		[MAX_PLAYERPARTS];
};
#endif