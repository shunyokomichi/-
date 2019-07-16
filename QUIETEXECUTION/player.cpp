//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"
#include "camera.h"
#include "enemy.h"
#include "wall.h"
#include "object.h"
#include "fade.h"
#include "texture.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPD3DXMESH					CPlayer::m_pMesh	[MAX_PLAYERPARTS]	= {};
LPD3DXBUFFER				CPlayer::m_pBuffMat	[MAX_PLAYERPARTS]	= {};
DWORD						CPlayer::m_nNumMat	[MAX_PLAYERPARTS]	= {};
LPDIRECT3DTEXTURE9			*CPlayer::m_pTexture[MAX_PLAYERPARTS]	= {};
CPlayer::PlayerStageStatus	CPlayer::m_Status						= PLAYERSTAGESTATUS_NONE;
D3DXVECTOR3					CPlayer::m_pos							= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3					CPlayer::m_posOld						= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3					CPlayer::m_rot							= D3DXVECTOR3(0.0f, 0.5f, 0.0f);
bool						CPlayer::m_bJump						= NULL;
bool						CPlayer::m_bLand						= NULL;
int							CPlayer::m_nLife						= NULL;
int							CPlayer::m_nMp							= NULL;

//*****************************************************************************
//グローバル変数
//*****************************************************************************
int		g_nNumPlayerModel;
char	g_aFileNamePlayerModel[MAX_PLAYERPARTS][256];

//=============================================================================
//　コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	//値の初期化
	for (int nCount = 0; nCount < MAX_PLAYERPARTS; nCount++)
	{
		m_pModel[nCount] = NULL;
	}
	m_mtxWorld		= {};
	m_Status		= PLAYERSTAGESTATUS_NONE;
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bJump			= true;
	m_bLand			= true;
	m_bAttack		= true;
	m_bCatch		= true;
	m_bRun			= true;
	m_bTouchOld		= true;
	m_nLife			= 0;
	m_nMp			= 0;
	m_fGravityFrame = 0.0f;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
//　生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;
		pPlayer->Init();
		GetPos() = pos;
	}

	return pPlayer;
}

//=============================================================================
//　テクスチャの読み込み
//=============================================================================
HRESULT CPlayer::Load(void)
{
	return S_OK;
}

//=============================================================================
//　アンロード
//=============================================================================
void CPlayer::Unload(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < MAX_ENEMYPARTS; nCntParts++)
	{
		if (m_pBuffMat[nCntParts] != NULL)
		{
			m_pBuffMat[nCntParts]->Release();
			m_pBuffMat[nCntParts] = NULL;
		}
		if (m_pMesh[nCntParts] != NULL)
		{
			m_pMesh[nCntParts]->Release();
			m_pMesh[nCntParts] = NULL;
		}

		for (DWORD nCntTex = 0; nCntTex < m_nNumMat[nCntParts]; nCntTex++)
		{
			if (m_pTexture[nCntParts][nCntTex] != NULL)
			{
				m_pTexture[nCntParts][nCntTex]->Release();
				m_pTexture[nCntParts][nCntTex] = NULL;
			}
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();
	// 位置・向きの初期設定
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bInvincible	= false;
	m_bMotionEnd	= true;
	m_nAnimnow		= PLAYERANIM_NEUTRAL;
	m_nAttackDelay	= 0;
	m_nLife			= PLAYER_LIFE;
	m_nMp			= PLAYER_MP;
	m_bUse			= false;

	//プレイヤー情報の読み込み
	FileLoad();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// モデルの破棄
	for (int nCntModel = 0; nCntModel < MAX_PLAYERPARTS; nCntModel++)
	{
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Uninit();
			delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}
	}

	// オブジェクト(自分自身)の破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	CManager::MODE	m_mode	= CManager::GetMode();				// ゲームモードの取得
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// 入力情報の取得
	CSound			*pSound = CManager::GetSound();				// サウンドの取得

	if (m_mode == CManager::MODE_GAME || m_mode == CManager::MODE_TUTORIAL)
	{// ゲームとチュートリアルのとき
		if (m_bInvincible == false)
		{
			PlayerMove();		// プレイヤーの移動処理
		}
		else
		{
			InvincibleMove();

		}
		if (m_bJump == false && m_pos.y - PLAYER_DOWN_HEIGHT < 0.0f)
		{
			m_bJump = true;
		}

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
					{// タイプがメッシュだったら
						// オブジェクトのあたる判定
						((CEnemy*)pScene)->Collision(m_pos);
					}
				}
				// Nextに次のSceneを入れる
				pScene = pSceneNext;
			}
		}

		// モーションの更新
		m_nMotionType = m_nAnimnow;
		UpdateMotion();

#ifdef _DEBUG	// デバック用
		if (pKey != NULL)
		{// 入力処理
		 // それぞれの条件で動く向きを変える（加速）
			if (pKey->GetKeyboardPress(DIK_UP) == true)
			{// ライフの加算
				if (m_nLife < 100)
				{
					m_nLife++;
				}
			}
			else if (pKey->GetKeyboardPress(DIK_DOWN) == true)
			{// ライフの減算
				if (m_nLife > 0)
				{
					m_nLife--;
				}
			}
		}
		if (pKey->GetKeyboardPress(DIK_Z) == true && pKey->GetKeyboardTrigger(DIK_X) == true)
		{// デバックモード切り替え
			m_bInvincible = m_bInvincible ? false : true;
		}
		if (m_bInvincible == false)
		{
			CManager::m_pDebuglog->Print(1, "当たり判定 ～有～\n");
		}
		else
		{
			CManager::m_pDebuglog->Print(1, "当たり判定 ～無～\n");
		}
#endif

		if (m_bInvincible == false)
		{
			// 壁の当たり判定
			CollisitionWall();
			// オブジェクトの当たり判定
			CollisitionObject3D(&m_pos, &m_posOld, &m_move, m_bTouchOld);
		}

		// 体力チェック
		if (m_nLife <= 0)
		{
			CManager::SetResult(CManager::GAMERESULT_LOSE);

			CFade::FADE fade = CFade::GetFade();

			if (fade != CFade::FADE_OUT)
			{
				//モード設定
				CFade::SetFade(CManager::MODE_RESULT);
			}
		}


		// 表示処理
		CManager::m_pDebuglog->Print(1, "プレイヤー ～pos.x %.1f  pos.y %.1f  pos.z %.1f～\n", m_pos.x, m_pos.y, m_pos.z);
		CManager::m_pDebuglog->Print(1, "プレイヤー ～HP:%d , MP:%d～\n", m_nLife, m_nMp);
	}
	else if(m_mode == CManager::MODE_RESULT)
	{
		m_nAnimnow = PLAYERANIM_RESULT;
		// モーションの更新
		m_nMotionType = m_nAnimnow;
		UpdateMotion();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DXMATRIX mtxParent;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	mtxParent = m_mtxWorld;

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			m_pModel[nCntParts]->Draw();
		}
	}
}

//=============================================================================
// プレイヤーの移動処理
//=============================================================================
void CPlayer::PlayerMove(void)
{
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// 入力情報の取得
	CInputMouse		*pMouse = CManager::GetInputMouse();		// 入力情報の取得(マウス)
	CSound			*pSound = CManager::GetSound();				// サウンドの取得

	D3DXVECTOR3		&m_pos	= CManager::GetPlayer()->GetPos();
	m_posOld				= CManager::GetPlayer()->GetPos();

	if (pKey != NULL)
	{// 入力処理
	 // それぞれの条件で動く向きを変える（加速）
		if (pKey->GetKeyboardPress(DIK_LSHIFT) == true && m_bAttack == true && m_bCatch == true)
		{// ダッシュ時
			if (pMouse->GetMouseTrigger(0) == true && m_bAttack == true && m_bCatch == true)
			{// 攻撃
			 // 攻撃モーション
				m_nKey		= 0;
				m_bRun		= true;						// 走ってない状態に
				m_bAttack	= false;					// 攻撃状態に
				m_nAnimnow	= PLAYERANIM_ATTACK;		// モーションの変更
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_ATTAK);
			}
			else if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
			{// 攻撃後、ニュートラルに戻す
				m_nAnimnow = PLAYERANIM_NEUTRAL;	// モーションの変更
			}

			if (pKey->GetKeyboardPress(DIK_D) == true)
			{
				if (pKey->GetKeyboardPress(DIK_S) == true)
				{	//左下移動
					m_move.x += sinf((D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf((D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
				}
				else if (pKey->GetKeyboardPress(DIK_W) == true)
				{	//左上移動
					m_move.x += sinf((D3DX_PI * 0.25f) + m_rot.y) * RUN_SPEED;
					m_move.z += cosf((D3DX_PI * 0.25f) + m_rot.y) * RUN_SPEED;
				}
				else
				{	//左移動
					m_move.x += sinf((D3DX_PI * 0.5f) + m_rot.y) * (RUN_SPEED + 2.0f);
					m_move.z += cosf((D3DX_PI * 0.5f) + m_rot.y) * (RUN_SPEED + 2.0f);
				}
				if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
				{// 攻撃中に走っているかどうか
					// 移動モーション
					m_bRun		= false;				// 走ってる状態に
					m_nAnimnow	= PLAYERANIM_MOVE;		// モーションの変更
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
				}
			}
			else if (pKey->GetKeyboardPress(DIK_A) == true)
			{
				if (pKey->GetKeyboardPress(DIK_S) == true)
				{	// 右下移動
					m_move.x += sinf(-(D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf(-(D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
				}
				else if (pKey->GetKeyboardPress(DIK_W) == true)
				{	// 右上移動
					m_move.x += sinf(-(D3DX_PI * 0.25f) + m_rot.y)* RUN_SPEED;
					m_move.z += cosf(-(D3DX_PI * 0.25f) + m_rot.y) * RUN_SPEED;
				}
				else
				{	// 右移動
					m_move.x += sinf(-(D3DX_PI * 0.5f) + m_rot.y)* (RUN_SPEED + 2.0f);
					m_move.z += cosf(-(D3DX_PI * 0.5f) + m_rot.y)* (RUN_SPEED + 2.0f);
				}
				if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
				{// 攻撃中に走っているかどうか
					// 移動モーション
					m_bRun		= false;				// 走ってる状態に
					m_nAnimnow	= PLAYERANIM_MOVE;		// モーションの変更
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
				}
			}
			else if (pKey->GetKeyboardPress(DIK_W) == true)
			{	// 前に移動
				m_move.x += sinf((0) + m_rot.y) * (RUN_SPEED + 2.0f);
				m_move.z += cosf((0) + m_rot.y) * (RUN_SPEED + 2.0f);
				if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
				{// 攻撃中に走っているかどうか
					// 移動モーション
					m_bRun		= false;				// 走ってる状態に
					m_nAnimnow	= PLAYERANIM_MOVE;		// モーションの変更
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
				}
			}
			else if (pKey->GetKeyboardPress(DIK_S) == true)
			{	// 後ろに移動
				m_move.x += sinf((D3DX_PI)+m_rot.y) * (WALK_SPEED + 2.0f);
				m_move.z += cosf((D3DX_PI)+m_rot.y) * (WALK_SPEED + 2.0f);
				if ((m_bJump || m_bLand) && m_bRun == true && m_bAttack == true && m_bCatch == true)
				{// 攻撃中に走っているかどうか
					// 移動モーション
					m_bRun		= false;					// 走ってる状態に
					m_nAnimnow	= PLAYERANIM_NEUTRAL;		// モーションの変更
				}
			}
			else if ((m_bJump || m_bLand) && m_bRun == false && m_bAttack == true && m_bCatch == true)
			{// シフトを押しながら移動していない時
				// 待機モーション
				m_bRun		= true;						// 走ってない状態に
				m_nAnimnow	= PLAYERANIM_NEUTRAL;		// モーションの変更
				pSound->StopSound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
			}
		}
		else if (m_bRun == false)
		{// 走っている状況でシフトを放した時
			m_bRun = true;		// 走ってない状態に
			pSound->StopSound(CSound::SOUND_LABEL_SE_PLAYER_RUN);
		}
		else
		{// 歩き
			if (pMouse->GetMouseTrigger(0) == true && m_bAttack == true && m_bCatch == true)
			{// 攻撃
			 // 攻撃モーション
				m_nKey		= 0;
				m_bAttack	= false;					// 攻撃状態に
				m_nAnimnow	= PLAYERANIM_ATTACK;		// モーションの変更
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_ATTAK);
			}
			if (pKey->GetKeyboardPress(DIK_D) == true)
			{
				if (pKey->GetKeyboardPress(DIK_S) == true)
				{	// 左下移動
					m_move.x += sinf((D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf((D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
				}
				else if (pKey->GetKeyboardPress(DIK_W) == true)
				{	// 左上移動
					m_move.x += sinf((D3DX_PI * 0.25f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf((D3DX_PI * 0.25f) + m_rot.y) * WALK_SPEED;
				}
				else
				{	// 左移動
					m_move.x += sinf((D3DX_PI * 0.5f) + m_rot.y) * (WALK_SPEED + 2.0f);
					m_move.z += cosf((D3DX_PI * 0.5f) + m_rot.y) * (WALK_SPEED + 2.0f);
				}
				if ((m_bJump || m_bLand) && m_bAttack == true && m_bCatch == true)
				{// 攻撃中かどうか
					// 移動モーション
					m_nAnimnow = PLAYERANIM_NEUTRAL;	// モーションの変更
				}
			}
			else if (pKey->GetKeyboardPress(DIK_A) == true)
			{
				if (pKey->GetKeyboardPress(DIK_S) == true)
				{	// 右下移動
					m_move.x += sinf(-(D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
					m_move.z += cosf(-(D3DX_PI * 0.75f) + m_rot.y) * WALK_SPEED;
				}
				else if (pKey->GetKeyboardPress(DIK_W) == true)
				{	// 右上移動
					m_move.x += sinf(-(D3DX_PI * 0.25f) + m_rot.y)* WALK_SPEED;
					m_move.z += cosf(-(D3DX_PI * 0.25f) + m_rot.y) * WALK_SPEED;
				}
				else
				{	// 右移動
					m_move.x += sinf(-(D3DX_PI * 0.5f) + m_rot.y)* (WALK_SPEED + 2.0f);
					m_move.z += cosf(-(D3DX_PI * 0.5f) + m_rot.y)* (WALK_SPEED + 2.0f);
				}
				if ((m_bJump || m_bLand) && m_bAttack == true && m_bCatch == true)
				{// 攻撃中かどうか
					// 移動モーション
					m_nAnimnow = PLAYERANIM_NEUTRAL;	// モーションの変更
				}
			}
			else if (pKey->GetKeyboardPress(DIK_W) == true)
			{	// 下に移動
				m_move.x += sinf((0) + m_rot.y) * (WALK_SPEED + 2.0f);
				m_move.z += cosf((0) + m_rot.y) * (WALK_SPEED + 2.0f);
				if ((m_bJump || m_bLand) && m_bAttack == true && m_bCatch == true)
				{// 攻撃中かどうか
					// 移動モーション
					m_nAnimnow = PLAYERANIM_NEUTRAL;	// モーションの変更
				}
			}
			else if (pKey->GetKeyboardPress(DIK_S) == true)
			{	// 上に移動
				m_move.x += sinf((D3DX_PI)+m_rot.y) * (WALK_SPEED + 2.0f);
				m_move.z += cosf((D3DX_PI)+m_rot.y) * (WALK_SPEED + 2.0f);
				if ((m_bJump || m_bLand) && m_bAttack == true && m_bCatch == true)
				{// 攻撃中かどうか
					// 移動モーション
					m_nAnimnow = PLAYERANIM_NEUTRAL;	// モーションの変更
				}
			}
			else if ((m_bJump || m_bLand) && m_bAttack == true && m_bRun == true && m_bCatch == true)
			{
				// 待機モーション
				m_nAnimnow = PLAYERANIM_NEUTRAL;		// モーションの変更
			}
		}

		if (m_pos.y - PLAYER_DOWN_HEIGHT > 0.0f)
		{
			// 重力
			m_move.y = GRAVITY_REAL * 10.0f;
		}
		else if (m_bLand == true)
		{
			m_bJump = true;
		}

		if (pKey->GetKeyboardTrigger(DIK_SPACE) == true && (m_bJump == true || m_bLand == true))
		{
			m_bJump		= false;
			m_move.y	= cosf(D3DX_PI) * -JUMP_POWER;
		}

		//減速
		m_move.x += (0.0f - m_move.x) * 0.6f;
		m_move.y += (0.0f - m_move.y) * 0.6f;
		m_move.z += (0.0f - m_move.z) * 0.6f;

		//位置更新（移動）
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
		m_pos.z += m_move.z;
	}
}

//=============================================================================
// デバックモードの移動処理
//=============================================================================
void CPlayer::InvincibleMove(void)
{
	CInputKeyBoard	*pKey	= CManager::GetInputKeyboard();		// 入力情報の取得
	D3DXVECTOR3		&m_pos	= CManager::GetPlayer()->GetPos();
	m_posOld				= CManager::GetPlayer()->GetPos();

	if (pKey->GetKeyboardPress(DIK_D) == true)
	{
		if (pKey->GetKeyboardPress(DIK_S) == true)
		{	// 左下移動
			m_move.x += sinf((D3DX_PI * 0.75f) + m_rot.y) * 100.0f;
			m_move.z += cosf((D3DX_PI * 0.75f) + m_rot.y) * 100.0f;
		}
		else if (pKey->GetKeyboardPress(DIK_W) == true)
		{	// 左上移動
			m_move.x += sinf((D3DX_PI * 0.25f) + m_rot.y) * 100.0f;
			m_move.z += cosf((D3DX_PI * 0.25f) + m_rot.y) * 100.0f;
		}
		else
		{	// 左移動
			m_move.x += sinf((D3DX_PI * 0.5f) + m_rot.y) * (100.0f + 2.0f);
			m_move.z += cosf((D3DX_PI * 0.5f) + m_rot.y) * (100.0f + 2.0f);
		}
	}
	else if (pKey->GetKeyboardPress(DIK_A) == true)
	{
		if (pKey->GetKeyboardPress(DIK_S) == true)
		{	// 右下移動
			m_move.x += sinf(-(D3DX_PI * 0.75f) + m_rot.y) * 100.0f;
			m_move.z += cosf(-(D3DX_PI * 0.75f) + m_rot.y) * 100.0f;
		}
		else if (pKey->GetKeyboardPress(DIK_W) == true)
		{	// 右上移動
			m_move.x += sinf(-(D3DX_PI * 0.25f) + m_rot.y)* 100.0f;
			m_move.z += cosf(-(D3DX_PI * 0.25f) + m_rot.y) * 100.0f;
		}
		else
		{	// 右移動
			m_move.x += sinf(-(D3DX_PI * 0.5f) + m_rot.y)* (100.0f + 2.0f);
			m_move.z += cosf(-(D3DX_PI * 0.5f) + m_rot.y)* (100.0f + 2.0f);
		}
	}
	else if (pKey->GetKeyboardPress(DIK_W) == true)
	{	// 下に移動
		m_move.x += sinf((0) + m_rot.y) * (100.0f + 2.0f);
		m_move.z += cosf((0) + m_rot.y) * (100.0f + 2.0f);
	}
	else if (pKey->GetKeyboardPress(DIK_S) == true)
	{	// 上に移動
		m_move.x += sinf((D3DX_PI)+m_rot.y) * (100.0f + 2.0f);
		m_move.z += cosf((D3DX_PI)+m_rot.y) * (100.0f + 2.0f);
	}

	// 減速
	m_move.x += (0.0f - m_move.x) * 0.6f;
	m_move.y += (0.0f - m_move.y) * 0.6f;
	m_move.z += (0.0f - m_move.z) * 0.6f;

	// 位置更新（移動）
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

}

//=============================================================================
//	壁の当たり判定
//=============================================================================
void CPlayer::CollisitionWall(void)
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
		{
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{// タイプが壁だったら
				((CWall*)pScene)->CollisionWallPlayer(&m_pos, &m_posOld, &m_move, 20.0f);
			}
		}

		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// プレイヤーのモーション
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	// モーション
	KEY			*pKey, *pNextKey;
	float		fRateMotion;
	float		fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;

	// キーが最大数を上回らないように
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	// モーション更新
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			// 現在のキーを取得
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			// 次のキーを取得
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			// 現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対値を算出
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			// ROT
			// 現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			// 相対値を差分を使って各要素の値を算出
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);

			// POS
			// 現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			// 相対値を差分を使って各要素の値を算出
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);


			// 現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			// 相対値を差分を使って各要素の値を算出
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			// POS
			// 現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			// 相対値を差分を使って各要素の値を算出
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);


			// 現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			// 相対値を差分を使って各要素の値を算出
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			// POS
			// 現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			// 相対値を差分を使って各要素の値を算出
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);


			// パーツを動かす
			m_pModel[nCntParts]->Setrot(rotmotion);

			// POS
			m_pModel[nCntParts]->SetPos(D3DXVECTOR3(m_OffSetPos[nCntParts].x + posmotion.x,
				m_OffSetPos[nCntParts].y + posmotion.y,
				m_OffSetPos[nCntParts].z + posmotion.z));

		}
	}

	// ループの判定
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:
		// ループする
		// フレームを進める
		m_nCountFlame++;
		// キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
			{
				m_nKey = 0;
			}
			else
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}
		break;
	case false:
		// ループしない
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{// フレームを進める
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			if (m_nAnimnow == PLAYERANIM_ATTACK)
			{// 攻撃モーション
				m_nAttackDelay++;
				if (m_nAttackDelay > 10)
				{
					m_bAttack = true;
					m_nAttackDelay = 0;
				}
			}
			if (m_nAnimnow == PLAYERANIM_CATCH)
			{// 攻撃モーション
				m_nAttackDelay++;
				if (m_nAttackDelay > 10)
				{
					m_bCatch = true;
					m_nAttackDelay = 0;
				}
			}
			m_bMotionEnd = true;
		}
		// キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey > m_nKey)
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}
		break;
	}

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
					D3DXVECTOR3 HandPos = m_pos + m_pModel[19]->GetPos();
					// オブジェクトのあたる判定
					((CEnemy*)pScene)->CollisionDamege(HandPos, m_nAnimnow);
				}
			}
			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 敵の当たり判定
//=============================================================================
void CPlayer::Collision(D3DXVECTOR3 posEnemy)
{
	// 敵と自分の距離を測る
	float fLength = (((m_pos.x - posEnemy.x) * (m_pos.x - posEnemy.x)) + ((m_pos.z - posEnemy.z) * (m_pos.z - posEnemy.z)));

	// 自分と敵の角度を測る
	float fAngle = atan2f(m_pos.x - posEnemy.x, m_pos.z - posEnemy.z);

	if (PLAYER_RADIUS * PLAYER_RADIUS > fLength)
	{// 範囲内だったら
		m_pos.x = posEnemy.x + sinf(fAngle) * PLAYER_RADIUS;
		m_pos.z = posEnemy.z + cosf(fAngle) * PLAYER_RADIUS;
	}
}
//=============================================================================
// 攻撃の当たり判定
//=============================================================================
void CPlayer::CollisionAttack(D3DXVECTOR3 posEnemy, D3DXMATRIX mtxWorld, float Radius, int NumModel)
{
	if (m_pModel[19] != NULL)
	{// 拳だったとき
		D3DXMATRIX mtxModelWorld = m_pModel[19]->GetMtxWorld();		// 拳マトリックス

		// 敵と自分の差分を測る
		float fRadiusEnemy = powf(Radius * 2.0f, 2);

		float fLengthX = posEnemy.x - mtxModelWorld._41;
		float fLengthY = posEnemy.y - mtxModelWorld._42;
		float fLengthZ = posEnemy.z - mtxModelWorld._43;

		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);

		if (fRadiusEnemy > fLengthTotal)
		{// 当たり判定に入っていたとき
			CManager::m_pDebuglog->Print(1, "[%d]HIT!!\n", NumModel);
		}
	}
}

//=============================================================================
//	オブジェクトの当たり判定
//=============================================================================
void CPlayer::CollisitionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, bool bTouchOld)
{
	CScene *pScene;

	// プライオリティーチェック
	pScene = CScene::GetTop(OBJECT_PRIOTITY);

	// NULLチェック
	while (pScene != NULL)
	{
		// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			// タイプがメッシュだったら
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{
				// オブジェクトのあたる判定
				m_bLand		= ((CObject*)pScene)->CollisionObject(pPos, pPosOld, pMove, bTouchOld);
				m_bTouchOld = m_bLand;
				if (m_bLand == true)
				{
					m_bJump = true;
				}
			}
		}

		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// ファイル読み込み
//=============================================================================
void CPlayer::FileLoad(void)
{
	// デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ファイル読み込み用変数
	FILE	*pFile;				// ファイルポインタ
	char	*pStrcur;			// 現在の先頭の文字列
	char	aLine[256];			// 文字列
	char	aStr[256];			// 一時保存文字列
	int		nIndex = 0;			// 現在のインデックス
	int		nWord = 0;			// ポップで返された値を保持

	D3DXVECTOR3 ParentPos;		// 親の位置情報を取得
#if 1
	// ファイルを開く 読み込み
	pFile = fopen(PLAYER_FILE, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_PLAYERMOTION; )
		{
			// 文字列の先頭を設定
			pStrcur = ReadLine(pFile, &aLine[0]);
			// 文字列を取り出す
			strcpy(aStr, pStrcur);

			// 文字列のデータ 比較する文字列 比較する文字数
			if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				// 頭出し
				pStrcur += strlen("NUM_MODEL = ");
				// 文字列の先頭を設定
				strcpy(aStr, pStrcur);
				// 文字列抜き出し
				g_nNumPlayerModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nNumPlayerModel; nCntModel++)
				{
					// 文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					// 文字列を取り戻す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
						// 頭出し
						pStrcur += strlen("MODEL_FILENAME = ");

						// 文字列を設定
						strcpy(aStr, pStrcur);

						// 必要な文字列の最後の文字までの文字数を数える
						int nNullNum = PopString(pStrcur, &aStr[0]);

						// 文字列を取り戻す
						strcpy(aStr, pStrcur);

						// 最後の文字にNULL文字を入れる
						aStr[nNullNum - 1] = '\0';

						// 対象の文字列から抜き出し
						strcpy(&g_aFileNamePlayerModel[nCntModel][0], aStr);

						if (m_pMesh[nCntModel] == NULL)
						{
							// Xファイルの読み込み
							D3DXLoadMeshFromX(&g_aFileNamePlayerModel[nCntModel][0],
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&m_pBuffMat[nCntModel],
								NULL,
								&m_nNumMat[nCntModel],
								&m_pMesh[nCntModel]);

							D3DXMATERIAL *pmat;        // マテリアルデータへのポインタ
							D3DMATERIAL9 *matDef;      // 現在のマテリアル保存用
							pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
							matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
							m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

							for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
							{// カウント
								matDef[tex] = pmat[tex].MatD3D;
								matDef[tex].Ambient = matDef[tex].Diffuse;
								m_pTexture[nCntModel][tex] = NULL;
								if (pmat[tex].pTextureFilename != NULL &&
									lstrlen(pmat[tex].pTextureFilename) > 0)
								{// テクスチャを使用している
									if (FAILED(D3DXCreateTextureFromFile(pDevice,
										pmat[tex].pTextureFilename,
										&m_pTexture[nCntModel][tex])))
									{
										MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
									}
								}
							}

							delete[] matDef;
							matDef = NULL;
						}
					}
				}
				// 文字列の先頭を設定
				pStrcur = ReadLine(pFile, &aLine[0]);
				// 文字列を取り出す
				strcpy(aStr, pStrcur);
			}


			// 文字列のデータ 比較する文字列 比較する文字数
			if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
			{
				while (1)
				{
					// 文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					// 文字列を取り出す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{
						// 頭出し
						pStrcur += strlen("NUM_PARTS = ");
						// 文字列の先頭を設定
						strcpy(aStr, pStrcur);
						// 文字列抜き出し
						m_nNumParts = atoi(pStrcur);
					}
					if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							// 文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);

							// INDEXを読み込み
							if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								// 頭出し
								pStrcur += strlen("INDEX = ");
								// 文字列の先頭を設定
								strcpy(aStr, pStrcur);
								// 文字列抜き出し
								nIndex = atoi(pStrcur);
							}
							// PARENTを読み込み
							if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								// 頭出し
								pStrcur += strlen("PARENT = ");
								// 文字列の先頭を設定
								strcpy(aStr, pStrcur);
								// 文字列抜き出し
								m_aIndexParent[nIndex] = atoi(pStrcur);
							}
							// POSを読み込み
							if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
							{
								// 頭出し
								pStrcur += strlen("POS = ");
								// 文字列の先頭を設定
								strcpy(aStr, pStrcur);

								// 文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								// 文字列変換
								m_aKayOffset[nIndex].fposX = (float)atof(pStrcur);
								// 文字数分進める
								pStrcur += nWord;

								// 文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								// 文字列変換
								m_aKayOffset[nIndex].fposY = (float)atof(pStrcur);
								// 文字数分進める
								pStrcur += nWord;

								// 文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								// 文字列変換
								m_aKayOffset[nIndex].fposZ = (float)atof(pStrcur);

							}
							// ROTを読み込み
							if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
							{
								// 頭出し
								pStrcur += strlen("ROT = ");
								// 文字列の先頭を設定
								strcpy(aStr, pStrcur);

								// 文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								// 文字列変換
								m_aKayOffset[nIndex].frotX = (float)atof(pStrcur);

								// 文字数分進める
								pStrcur += nWord;
								// 文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								// 文字列変換
								m_aKayOffset[nIndex].frotY = (float)atof(pStrcur);

								// 文字数分進める
								pStrcur += nWord;
								// 文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								// 文字列変換
								m_aKayOffset[nIndex].frotZ = (float)atof(pStrcur);
							}
							// パーツセット終了
							else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{
								// NULLチェック
								if (m_pModel[nIndex] == NULL)
								{// 動的確保
									m_pModel[nIndex] = new CModel;
									// NULLチェック
									if (m_pModel[nIndex] != NULL)
									{
										// モデルの生成
										m_pModel[nIndex]->BindModel(m_pMesh[nIndex], m_pBuffMat[nIndex], m_nNumMat[nIndex], m_pTexture[nIndex]);
										m_pModel[nIndex]->Init();
									}
								}

								// モデルを生成	オフセット設定
								m_pModel[nIndex]->SetPos(D3DXVECTOR3(m_pos.x + m_aKayOffset[nIndex].fposX,
									m_pos.y + m_aKayOffset[nIndex].fposY,
									m_pos.z + m_aKayOffset[nIndex].fposZ));

								// posを代入
								ParentPos = m_pModel[nIndex]->GetPos();
								m_OffSetPos[nIndex] = m_pModel[nIndex]->GetPos();

								// モデルを割り当て
								if (m_aIndexParent[nIndex] == -1)
								{
									// モデルの親を指定
									m_pModel[nIndex]->SetParent(NULL);
									ParentPos = m_pModel[nIndex]->GetPos();
								}
								else
								{
									// モデルの親を指定
									m_pModel[nIndex]->SetParent(m_pModel[m_aIndexParent[nIndex]]);
								}

								break;
							}
						}
					}
					// キャラクターセット終了
					else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{
						break;
					}
				}
				// 文字列の先頭を設定
				pStrcur = ReadLine(pFile, &aLine[0]);
				// 文字列を取り出す
				strcpy(aStr, pStrcur);
			}

			// モーション読み込み
			if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				// 頭出し
				pStrcur += strlen("MOTIONSET");

				while (1)
				{
					// 文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					// 文字列を取り出す
					strcpy(aStr, pStrcur);

					if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
					{
						// 頭出し
						pStrcur += strlen("LOOP = ");
						// 文字列の先頭を設定
						strcpy(aStr, pStrcur);

						switch (atoi(pStrcur))
						{
						case 0:
							// 文字列抜き出し
							m_aMotionInfo[nCntMotion].bLoop = false;
							break;
						case 1:
							// 文字列抜き出し
							m_aMotionInfo[nCntMotion].bLoop = true;
							break;
						}
						// 文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
					}
					if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{
						// 頭出し
						pStrcur += strlen("NUM_KEY = ");
						// 文字列の先頭を設定
						strcpy(aStr, pStrcur);
						// 文字列抜き出し
						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrcur);

						// 文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
						// 文字列を取り出す
						strcpy(aStr, pStrcur);
					}

					// キーの設定
					for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey;)
					{
						if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
						{
							// 頭出し
							pStrcur += strlen("KEYSET");
							// 文字列の先頭を設定
							strcpy(aStr, pStrcur);
							// 文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);

							if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								// 頭出し
								pStrcur += strlen("FRAME = ");

								m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

								// 文字列の先頭を設定
								strcpy(aStr, pStrcur);
								// 文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);
							}

							// パーツ分回す
							for (int nCntParts = 0; nCntParts < m_nNumParts;)
							{
								if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
								{
									// 文字列の先頭を設定
									pStrcur = ReadLine(pFile, &aLine[0]);

									if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
									{
										// 頭出し
										pStrcur += strlen("POS = ");
										// 文字列の先頭を設定
										strcpy(aStr, pStrcur);

										// 文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										// POS.X代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
										// 文字数分進める
										pStrcur += nWord;

										// 文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										// POS.Y代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
										// 文字数分進める
										pStrcur += nWord;

										// 文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										// POS.Z代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
										// 文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									// ROTを読み込み
									if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
									{
										// 頭出し
										pStrcur += strlen("ROT = ");
										// 文字列の先頭を設定
										strcpy(aStr, pStrcur);

										// 文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										// RotX
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
										// 文字数分進める
										pStrcur += nWord;

										// 文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										// RotY
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
										// 文字数分進める
										pStrcur += nWord;

										// 文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										// RotZ
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

										// 文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
									{
										// 頭出し
										pStrcur += strlen("END_KEY");
										// 文字列の先頭を設定
										strcpy(aStr, pStrcur);
										// 文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
										// パーツのカウントを進める
										nCntParts++;
									}
								}
								else
								{
									// 文字列の先頭を設定
									pStrcur = ReadLine(pFile, &aLine[0]);
								}
							}
							if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								// 文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);
								// カウントを進める
								nCntKey++;
							}
						}
						else
						{
							// 文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);
						}

					}
					if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						// モーションの情報をセット
						m_pKeyInfo[nCntMotion] = &m_aMotionInfo[nCntMotion].aKayInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			// スクリプトの終わり
			if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
	}
	else
	{	// ファイルが開けなかった
		printf("ファイルが開けませんでした\n");
	}
	// ファイルを閉じる
	fclose(pFile);
#endif
}

//=============================================================================
//　ファイル読み込み無効文を排除
//=============================================================================
char *CPlayer::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		// 1行分読み込み
		fgets(&pDst[0], 256, pFile);

		// 文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		// 文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		// 文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		// 文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//　ファイル読み込み先頭を排除
//=============================================================================
char * CPlayer::GetLineTop(char * pStr)
{
	while (1)
	{
		// 文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		// 文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//　文字数を返す
//=============================================================================
int CPlayer::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	// 頭出し
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	// 頭出し
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	// 頭出し
			pStr = "\0";
			nWord += strlen("\t");
			break;
		}
		// 文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//頭出し
			nWord += strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	// 文字列の数を返す
	return nWord;
}