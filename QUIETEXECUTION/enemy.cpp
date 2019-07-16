//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"
#include "fade.h"
#include "billboord.h"
#include "bullet.h"
#include "wall.h"
#include "object.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPD3DXMESH				CEnemy::m_pMesh		[MAX_ENEMYPARTS]	= {};
LPD3DXBUFFER			CEnemy::m_pBuffMat	[MAX_ENEMYPARTS]	= {};
DWORD					CEnemy::m_nNumMat	[MAX_ENEMYPARTS]	= {};
LPDIRECT3DTEXTURE9		*CEnemy::m_pTexture	[MAX_ENEMYPARTS]	= {};
CBillBoord				*CEnemy::m_pBillBoord					= NULL;
CBullet					*CEnemy::m_pBullet						= NULL;

//*****************************************************************************
//グローバル変数
//*****************************************************************************
int		g_nNumEnemyModel;
char	g_aFileNameEnemyModel[MAX_ENEMYPARTS][256];

//=============================================================================
//　コンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	//値の初期化
	m_mtxWorld		= {};
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCountModel = 0; nCountModel < MAX_ENEMYPARTS; nCountModel++)
	{
		m_pModel[nCountModel] = NULL;
	}
	m_nAnimnow		= ENEMYANIM_NEUTRAL;
	m_nStatse		= ENEMYSTATSE_NONE;
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bAttack		= true;
	m_nAttackDelay	= 0;
	m_bDamage		= true;
	m_nDamageDelay	= 0;
	m_nLife			= 0;
	m_nCntSearch	= 0;
	m_nCntStatse	= 0;
	m_fAngle		= 0.0f;
	m_fWalkFrame	= 0;
	m_bMove			= true;
	m_fMoveFlame	= (rand() % 400 + 150);
	m_VigilancePos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posAngle		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntAttack	= 0;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//　生成処理
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;
		pEnemy->Init();
		pEnemy->m_pos = pos;
	}
	return pEnemy;
}

//=============================================================================
//　ロード処理
//=============================================================================
HRESULT CEnemy::Load(void)
{
	return S_OK;
}

//=============================================================================
//　アンロード処理
//=============================================================================
void CEnemy::Unload(void)
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
HRESULT CEnemy::Init(void)
{
	srand((unsigned int)time(0));	// ランダム
	//ビルボードテクスチャの読み込み
	m_pBillBoord->Load();
	m_pBullet->Load();

	// 位置・向きの初期設定
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bMotionEnd	= true;
	m_nAnimnow		= ENEMYANIM_NEUTRAL;
	m_nStatse		= ENEMYSTATSE_NONE;
	m_nAttackDelay	= 0;
	m_nDamageDelay	= 0;
	m_fMoveFlame	= (rand() % 400 + 150);
	m_nLife			= 4;

	FileLoad();// ファイル情報の読み込み

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	//ビルボードテクスチャの破棄
	m_pBillBoord->UnLoad();
	m_pBullet->UnLoad();

	// モデルの破棄
	for (int nCntModel = 0; nCntModel < MAX_ENEMYPARTS; nCntModel++)
	{
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Uninit();
			//delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}
	}
	// オブジェクト(自分自身)の破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	CManager::MODE	m_mode		= CManager::GetMode();				// ゲームモードの取得
	CInputKeyBoard	*pKey		= CManager::GetInputKeyboard();		// 入力情報の取得
	CSound			*pSound		= CManager::GetSound();				// サウンド情報の取得
	D3DXVECTOR3		PlayerPos	= CGame::GetPlayer()->GetPos();		//プレイヤーの位置情報

	switch (m_mode)
	{// カメラのタイプ別処理
	case CManager::MODE_TITLE:
		break;
	case CManager::MODE_TUTORIAL:
		break;
	case CManager::MODE_GAME:
		if (m_pos.x + 6000.0f >= PlayerPos.x && m_pos.x - 6000.0f <= PlayerPos.x)
		{// プレイヤーに近いとき
			m_posOld = m_pos;		// 位置更新(過去の位置情報を保存)

			if (m_nCntSearch % 20 == 0 && m_nStatse != ENEMYSTATSE_DEDA)
			{// 捜索弾の生成
				D3DXVECTOR3 rot = (m_rot - D3DXVECTOR3(0.0f, (ENEMY_SEARCH_INTERVAL * (ENEMY_SEARCH_NUM / 2)), 0.0f));	// 捜索範囲の中心を設定
				for (int nCntSearch = 0; nCntSearch < ENEMY_SEARCH_NUM; nCntSearch++)
				{
					D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					move.x = sinf((D3DX_PI)+(rot.y + nCntSearch * ENEMY_SEARCH_INTERVAL)) * 35.0f;		// 方向に合わせて移動量を設定(X)
					move.z = cosf((D3DX_PI)+(rot.y + nCntSearch * ENEMY_SEARCH_INTERVAL)) * 35.0f;		// 方向に合わせて移動量を設定(Z)
					m_pBullet->Create(D3DXVECTOR3(m_pos.x, (m_pos.y + ENEMY_EYE), m_pos.z), move, D3DXVECTOR2(30.0f, 30.0f), this);
				}
			}

			// 状態処理
			switch (m_nStatse)
			{
			case ENEMYSTATSE_NONE:
				if (m_bDamage == true)
				{// ダメージを受けていないとき
					MoveNone();
				}
				CManager::m_pDebuglog->Print(1, "敵の状態 ～ 通常 ～\n");
				break;
			case ENEMYSTATSE_VIGILANCE:
				if (m_bDamage == true)
				{// ダメージを受けていないとき
					MoveVigilance();
				}
				CManager::m_pDebuglog->Print(1, "敵の状態 ～ 警戒 ～\n");
				break;
			case ENEMYSTATSE_PURSUIT:
				if (m_bDamage == true)
				{// ダメージを受けていないとき
					if (m_bAttack == true)
					{// 攻撃していないとき
						MovePursuit();
					}
					AttackMove();
				}
				CManager::m_pDebuglog->Print(1, "敵の状態 ～ 追跡 ～\n");
				break;
			case ENEMYSTATSE_DAMAGE:
				CManager::m_pDebuglog->Print(1, "敵の状態 ～ ダメージ ～\n");
				break;
			case ENEMYSTATSE_DEDA:
				MoveDeda();
				CManager::m_pDebuglog->Print(1, "敵の状態 ～ 死亡 ～\n");
				break;
			}

			// 当たり判定
			if (m_nStatse != ENEMYSTATSE_DEDA)
			{
				CollisionPlayer();									// プレイヤーとの当たり判定
				CollisionSearch();									// 捜索の判定
				CollisitionWall();									// 壁の当たり判定
				CollisitionObject3D(&m_pos, &m_posOld, &m_move);	// オブジェクトの当たり判定
			}

			if (m_nLife <= 0)
			{// ライフが0になったとき
				m_nStatse = ENEMYSTATSE_DEDA;		// ステータスの変更
				m_nAnimnow = ENEMYANIM_DEDA;		// モーションの変更
				pSound->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DEAD);
			}

			// モーションの更新
			m_nMotionType = m_nAnimnow;
			UpdateMotion();

			m_nCntSearch++;		// カウンターの加算
		}
		break;
	case CManager::MODE_RESULT:
		m_nAnimnow = ENEMYANIM_DEDA;		// モーションの変更
		// モーションの更新
		m_nMotionType = m_nAnimnow;
		UpdateMotion();
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
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

	//プレイヤーの位置情報
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//近くの敵以外を消す
	if (m_pos.x + 5000.0f >= PlayerPos.x && m_pos.x - 5000.0f <= PlayerPos.x)
	{
		for (int nCntParts = 0; nCntParts < MAX_ENEMYPARTS; nCntParts++)
		{
			if (m_pModel[nCntParts] != NULL)
			{
				m_pModel[nCntParts]->Draw();
			}
		}
	}
}

//*************************************************************************************************************
// それぞれの状態動作
//=============================================================================
// 通常の処理
//=============================================================================
void CEnemy::MoveNone(void)
{
	m_fWalkFrame++;		// カウンターの加算
	if (m_bMove == true)
	{// 移動しているとき
		if (m_fWalkFrame % m_fMoveFlame == 0)
		{// 向きの切り替え
			m_fMoveFlame	= (rand() % 400 + 150);
			m_bMove			= false;
		}

		// 移動量の更新
		m_move.x += sinf((D3DX_PI)+m_rot.y) * (ENEMY_SPEED);
		m_move.z += cosf((D3DX_PI)+m_rot.y) * (ENEMY_SPEED);

		// 減速
		m_move.x += (0.0f - m_move.x) * 0.3f;
		m_move.y += (0.0f - m_move.y) * 0.3f;
		m_move.z += (0.0f - m_move.z) * 0.3f;

		// 位置更新（移動）
		m_pos.x += m_move.x;
		m_pos.y += m_move.y;
		m_pos.z += m_move.z;

		m_nAnimnow = ENEMYANIM_WALK;		// モーションの変更
	}
	else if (m_bMove == false)
	{// 移動していないとき
		if (m_fWalkFrame % m_fMoveFlame == 0)
		{// 向きの切り替え
			m_fMoveFlame	= (rand() % 400 + 150);
			m_rot			= D3DXVECTOR3(0.0f, (float)(rand() % 10), 0.0f);
			m_bMove			= true;
		}
		m_nAnimnow = ENEMYANIM_OVERLOOKING;		// モーションの変更
	}
}

//=============================================================================
// 警戒の処理
//=============================================================================
void CEnemy::MoveVigilance(void)
{
	// 差分の生成
	float fAngle = atan2f(m_VigilancePos.x - m_pos.x, m_VigilancePos.z - m_pos.z) + D3DX_PI;

	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2;
	}

	m_rot.y = fAngle;	// 向きに代入

	// テキストデータから座標情報を取得する
	m_PosDest = m_VigilancePos;

	// 差分を計算する
	m_posAngle.x = m_PosDest.x - m_pos.x;		// 差分 = 目的 - 現在
	m_posAngle.z = m_PosDest.z - m_pos.z;		// 差分 = 目的 - 現在

	// 座標の更新
	m_pos.x += m_posAngle.x * ENEMY_VIGILANCESPEED;
	m_pos.z += m_posAngle.z * ENEMY_VIGILANCESPEED;

	m_nAnimnow = ENEMYANIM_WALK;		// モーションの変更

	CManager::m_pDebuglog->Print(1, "差分　～ %.0f , %.0f ～\n", m_posAngle.x, m_posAngle.z);
}
//=============================================================================
// 追跡の処理
//=============================================================================
void CEnemy::MovePursuit(void)
{
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();		// プレイヤー座標の取得

	// 差分の生成
	float fAngle = atan2f(PlayerPos.x - m_pos.x, PlayerPos.z - m_pos.z) + D3DX_PI;

	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2;
	}

	m_rot.y = fAngle;	// 向きに代入

	// テキストデータから座標情報を取得する
	m_PosDest = PlayerPos;

	// 差分を計算する
	m_posAngle.x = m_PosDest.x - m_pos.x;		// 差分 = 目的 - 現在
	m_posAngle.z = m_PosDest.z - m_pos.z;		// 差分 = 目的 - 現在

	// 座標の更新
	m_pos.x += m_posAngle.x * ENEMY_PURSUITSPEED;
	m_pos.z += m_posAngle.z * ENEMY_PURSUITSPEED;

	m_nAnimnow = ENEMYANIM_RUN;		// モーションの変更
}
//=============================================================================
// 死亡の処理
//=============================================================================
void CEnemy::MoveDeda(void)
{
	m_nStatse = ENEMYSTATSE_NONE;	// ステータスの変更
}
//*************************************************************************************************************

//=============================================================================
// 攻撃の処理
//=============================================================================
void CEnemy::AttackMove(void)
{
	CPlayer						*pPlayer	= CGame::GetPlayer();		// プレイヤーの情報を取得
	D3DXVECTOR3					PlayerPos	= pPlayer->GetPos();		// プレイヤーの位置を取得
	CPlayer::PlayerStageStatus	PlayerState	= pPlayer->GetStatus();		// プレイヤーの状態を取得
	CSound						*pSound		= CManager::GetSound();		// サウンド情報の取得

	if (m_pModel[15] != NULL)
	{// 武器のポインタのとき
		// 武器の状態を取得
		D3DXMATRIX apWorldmtx = m_pModel[15]->GetMtxWorld();	// 武器

		// 敵と自分の距離を測る
		float fLength = (((PlayerPos.x - m_pos.x) * (PlayerPos.x - m_pos.x)) + ((PlayerPos.z - m_pos.z) * (PlayerPos.z - m_pos.z)));
		// 目標との差分を出す
		float fWeapon = (apWorldmtx._41 - PlayerPos.x) * (apWorldmtx._41 - PlayerPos.x) + (apWorldmtx._43 - PlayerPos.z) * (apWorldmtx._43 - PlayerPos.z);	// プレイヤー

		if (!(PlayerPos.y - PLAYER_DOWN_HEIGHT >= m_vtxMax.y) && !(PlayerPos.y + PLAYER_UP_HEIGHT <= m_vtxMin.y))
		{// Y軸の判定
			if (ENEMY_ATTACK * ENEMY_ATTACK > fLength && m_bAttack == true && m_bDamage == true && m_nCntAttack > 180)
			{// 攻撃の範囲内だったら
				m_bAttack		= false;				// 攻撃状態に
				m_nAnimnow		= ENEMYANIM_ATTACK;		// モーションの変更
				m_nCntAttack	= 0;					// カウンターのリセット
			}

			if (fWeapon <= 500.0f * 500.0f && PlayerState == CPlayer::PLAYERSTAGESTATUS_NONE && m_nAnimnow == ENEMYANIM_ATTACK)
			{// 当たり判定
				int &pPlayerLife = CPlayer::GetLife();		// プレイヤーの体力の取得
				if (pPlayerLife > 0)
				{// プレーヤーの体力がある時
					pPlayerLife -= 1;										// 体力の減算
					pPlayer->SetLife(pPlayerLife);							// 体力の設定
					pPlayer->SetStatus(CPlayer::PLAYERSTAGESTATUS_DAMAGE);	// プレイヤーをダメージ状態に
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_DAMEG);
				}
			}
		}
		m_nCntAttack++;		// カウンターの加算
	}
}

//=============================================================================
// モーションの更新
//=============================================================================
void CEnemy::UpdateMotion(void)
{
	//モーション
	KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;

	//キーが最大数を上回らないように
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	//モーション更新
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			//現在のキーを取得
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			//次のキーを取得
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			//現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対値を算出
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			//ROT
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			//相対値を差分を使って各要素の値を算出
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			//相対値を差分を使って各要素の値を算出
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);

			//ROT
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			//相対値を差分を使って各要素の値を算出
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			//相対値を差分を使って各要素の値を算出
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);

			//ROT
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			//相対値を差分を使って各要素の値を算出
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			//相対値を差分を使って各要素の値を算出
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);

			//パーツを動かす
			m_pModel[nCntParts]->Setrot(rotmotion);

			//POS
			m_pModel[nCntParts]->SetPos(D3DXVECTOR3(m_OffSetPos[nCntParts].x + posmotion.x,
				m_OffSetPos[nCntParts].y + posmotion.y,
				m_OffSetPos[nCntParts].z + posmotion.z));

		}
	}

	//ループの判定
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:

		//ループする
		//フレームを進める
		m_nCountFlame++;
		//キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{// m_nCountFlame が フレーム数に達したら
			if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
			{// 最後のキーだったら0に戻す
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
		//ループしない
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{//フレームを進める
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			if (m_nAnimnow == ENEMYANIM_ATTACK)
			{//攻撃モーション
				m_nAttackDelay++;
				if (m_nAttackDelay > 40)
				{
					m_bAttack = true;
					m_nAttackDelay = 0;
				}
			}
			if (m_nAnimnow == ENEMYANIM_DAMAGE)
			{//ダメージモーション
				m_nDamageDelay++;
				if (m_nDamageDelay > 20)
				{
					m_bDamage = true;
					m_nDamageDelay = 0;
					m_nAnimnow = ENEMYANIM_OVERLOOKING;
				}
			}
			m_bMotionEnd = true;
		}
		//キーの更新
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
}

//=============================================================================
// 敵の当たり判定
//=============================================================================
void CEnemy::Collision(D3DXVECTOR3 posPlayer)
{
	// 敵と自分の距離を測る
	float fLength = (((m_pos.x - posPlayer.x) * (m_pos.x - posPlayer.x)) + ((m_pos.z - posPlayer.z) * (m_pos.z - posPlayer.z)));

	// 自分と敵の角度を測る
	float fAngle = atan2f(m_pos.x - posPlayer.x, m_pos.z - posPlayer.z);

	if (ENEMY_RADIUS * ENEMY_RADIUS > fLength)
	{// 範囲内だったら
		m_pos.x = posPlayer.x + sinf(fAngle) * ENEMY_RADIUS;
		m_pos.z = posPlayer.z + cosf(fAngle) * ENEMY_RADIUS;
	}
}

//=============================================================================
// プレイヤーとの当たり判定
//=============================================================================
void CEnemy::CollisionPlayer(void)
{
	for (int nCntPriotity = 0; nCntPriotity < NUM_PRIORITY; nCntPriotity++)
	{
		CScene *pScene;
		//プライオリティーチェック
		pScene = CScene::GetTop(nCntPriotity);

		//NULLチェック
		while (pScene != NULL)
		{
			//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetDeath() == false)
			{
				if (pScene->GetObjType() == OBJTYPE_PLAYER)
				{
					//オブジェクトのあたる判定
					((CPlayer*)pScene)->Collision(m_pos);
				}
			}
			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 敵のダメージ判定
//=============================================================================
void CEnemy::CollisionDamege(D3DXVECTOR3 posHand, CPlayer::PlayerAnim playerState)
{
	CSound *pSound = CManager::GetSound();
	if (playerState == CPlayer::PLAYERANIM_ATTACK)
	{
		int nNumBill = 0;
		for (int nCntModel = 0; nCntModel < MAX_ENEMYPARTS; nCntModel++)
		{
			if (m_pModel[nCntModel] != NULL)
			{
				D3DXMATRIX mtxRot, mtxTrans, mtxWorld;				// 計算用マトリックス
				int nCollisionNum;
				float fRadius;
				D3DXVECTOR3 CollisionMove;
				D3DXMATRIX ModelWorld = m_pModel[nCntModel]->GetMtxWorld();

				switch (nCntModel)
				{
				case 0:// 腰
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 60.0f;		// 半径
					break;
				case 1:// 体
					nCollisionNum = 2;		// 団子の数
					CollisionMove = D3DXVECTOR3(30.0f, 30.0f, 0.0f);	// 団子のずれ
					fRadius = 80.0f;		// 半径
					break;
				case 2:// 頭
					nCollisionNum = 3;		// 団子の数
					CollisionMove = D3DXVECTOR3(20.0f, 10.0f, -10.0f);	// 団子のずれ
					fRadius = 40.0f;		// 半径
					break;
				case 3:// 左上腕
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 4:// 左前腕
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 5:// 左手
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 6:// 右上腕
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 7:// 右前腕
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 8:// 右手
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 9:// 左腿
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 10:// 左脛
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 11:// 左足
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 12:// 右腿
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 13:// 右脛
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 14:// 右足
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				case 15:// 剣
					nCollisionNum = 1;		// 団子の数
					CollisionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 団子のずれ
					fRadius = 10.0f;		// 半径
					break;
				}
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorld);
				// 回転
				D3DXMatrixRotationYawPitchRoll(&mtxRot, ModelWorld._31, ModelWorld._32, ModelWorld._33);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
				// 位置
				D3DXMatrixTranslation(&mtxTrans, ModelWorld._41, ModelWorld._42, ModelWorld._43);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				for (int nCntCollision = 0; nCntCollision < nCollisionNum; nCntCollision++)
				{
					// 位置
					D3DXMatrixTranslation(&mtxTrans, (CollisionMove.x * nCntCollision), (CollisionMove.y * nCntCollision), (CollisionMove.z * nCntCollision));
					D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxWorld); // mtxWorldの角度を利用して生成

					// 敵と自分の差分を測る
					float fRadiusEnemy = powf(fRadius * 3.0f, 2);

					float fLengthX = posHand.x - mtxTrans._41;
					float fLengthY = posHand.y - mtxTrans._42;
					float fLengthZ = posHand.z - mtxTrans._43;

					float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);

					if (fRadiusEnemy > fLengthTotal && m_bDamage == true)
					{
						m_nLife--;
						m_bDamage	= false;				// ダメージ状態に
						m_bAttack	= true;					// 攻撃していない状態に
						m_nStatse	= ENEMYSTATSE_DAMAGE;	// ステータスの変更
						m_nAnimnow	= ENEMYANIM_DAMAGE;		// モーションの変更
						CManager::m_pDebuglog->Print(1, "[%d]HIT!!\n", nCntModel);
						pSound->PlaySound(CSound::SOUND_LABEL_SE_ENEMY_DAMEG);
					}
					nNumBill++;
				}
			}
		}
	}
}

//=============================================================================
// 捜索の判定
//=============================================================================
void CEnemy::CollisionSearch()
{
	CScene *pScene;
	//プライオリティーチェック
	pScene = CScene::GetTop(BULLET_PRIOTITY);

	bool bCollision = false;
	bool bDistance	= false;

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//タイプが敵だったら
			if (pScene->GetObjType() == OBJTYPE_BULLET)
			{//	バレットだったとき
				bCollision = ((CBullet*)pScene)->CollisionBulletSearch();
				if (bCollision == true)
				{
					bDistance = CollisionDistance((CBullet*)pScene);
					break;
				}
				else
				{
					CBullet *pBullet	= (CBullet*)pScene;
					CEnemy	*pEnemy		= pBullet->GetEnemy();

					if (pEnemy->m_nStatse != ENEMYSTATSE_NONE)
					{// 通常状態のとき
						if (pEnemy->m_nCntStatse >= 30000)
						{
							pEnemy->m_nStatse		= ENEMYSTATSE_NONE;		// ステータスの変更
							pEnemy->m_nCntStatse	= 0;
						}
						pEnemy->m_nCntStatse++;		// カウンターの加算
					}
				}
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 敵とプレイヤーの距離
//=============================================================================
bool CEnemy::CollisionDistance(CBullet *pBullet)
{
	D3DXVECTOR3 PlayerPos	= CGame::GetPlayer()->GetPos();		// プレイヤー座標の取得
	CEnemy		*pEnemy		= pBullet->GetEnemy();				// エネミーの取得
	bool bCollision			= false;

	// 敵と自分の距離を測る
	float fLength = (((PlayerPos.x - pEnemy->m_pos.x) * (PlayerPos.x - pEnemy->m_pos.x)) + ((PlayerPos.z - pEnemy->m_pos.z) * (PlayerPos.z - pEnemy->m_pos.z)));

	if (ENEMY_SEARCH_VIGILANCE * ENEMY_SEARCH_VIGILANCE > fLength && pEnemy->m_bAttack == true && pEnemy->m_bDamage == true)
	{// 警戒の範囲内だったら
		if (pEnemy->m_nStatse == ENEMYSTATSE_NONE)
		{// 通常状態だったとき
			pEnemy->m_nStatse		= ENEMYSTATSE_VIGILANCE;		// ステータスの変更
			pEnemy->m_VigilancePos	= PlayerPos;					// 目的地の更新
			pEnemy->m_nCntStatse	= 0;
		}
		else if (ENEMY_SEARCH_PURSUIT * ENEMY_SEARCH_PURSUIT > fLength && pEnemy->m_bAttack == true && pEnemy->m_bDamage == true)
		{// 追跡の範囲内だったら
			pEnemy->m_nStatse		= ENEMYSTATSE_PURSUIT;			// ステータスの変更
			pEnemy->m_nCntStatse	= 0;
		}
		bCollision = true;
	}
	return bCollision;
}

//=============================================================================
// 壁の当たり判定
//=============================================================================
void CEnemy::CollisitionWall(void)
{
	// プライオリティーチェック
	CScene *pScene = CScene::GetTop(WALL_PRIOTITY);

	// NULLチェック
	while (pScene != NULL)
	{
		// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			// タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{// 壁だったとき
				// 壁の当たり判定
				((CWall*)pScene)->CollisionWallEnemy(&m_pos, &m_posOld, &m_move, 20.0f);
			}
		}

		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
//	オブジェクトの当たり判定
//=============================================================================
void CEnemy::CollisitionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	// プライオリティーチェック
	CScene *pScene = CScene::GetTop(OBJECT_PRIOTITY);

	// NULLチェック
	while (pScene != NULL)
	{
		// UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			// タイプがメッシュだったら
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// オブジェクトだったとき
				// オブジェクトの当たり判定
				((CObject*)pScene)->CollisionObjectEnemy(pPos, pPosOld, pMove);
			}
		}

		// Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// ファイル読み込み
//=============================================================================
void CEnemy::FileLoad(void)
{
	// デバイスを取得
	CRenderer			*pRenderer	= CManager::GetRenderer();
	LPDIRECT3DDEVICE9	pDevice		= pRenderer->GetDevice();

	// ファイル読み込み用変数
	FILE *pFile;		// ファイルポインタ
	char *pStrcur;		// 現在の先頭の文字列
	char aLine[256];	// 文字列
	char aStr[256];		// 一時保存文字列
	int nIndex = 0;		// 現在のインデックス
	int nWord = 0;		// ポップで返された値を保持

	D3DXVECTOR3 ParentPos;	// 親の位置情報を取得
#if 1
	// ファイルを開く 読み込み
	pFile = fopen(ENEMY_FILE, "r");
	// NULLチェック
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_ENEMYMOTION; )
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
				g_nNumEnemyModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nNumEnemyModel; nCntModel++)
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
						strcpy(&g_aFileNameEnemyModel[nCntModel][0], aStr);

						if (m_pMesh[nCntModel] == NULL)
						{
							// Xファイルの読み込み
							D3DXLoadMeshFromX(&g_aFileNameEnemyModel[nCntModel][0],
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&m_pBuffMat[nCntModel],
								NULL,
								&m_nNumMat[nCntModel],
								&m_pMesh[nCntModel]);

							D3DXMATERIAL *pmat;			// マテリアルデータへのポインタ
							D3DMATERIAL9 *matDef;		// 現在のマテリアル保存用
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
char *CEnemy::ReadLine(FILE *pFile, char *pDst)
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
char * CEnemy::GetLineTop(char * pStr)
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
int CEnemy::PopString(char * pStr, char * pDest)
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
			// 頭出し
			nWord += strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	// 文字列の数を返す
	return nWord;
}