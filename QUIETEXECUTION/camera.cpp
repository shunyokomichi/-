//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "debuglog.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "tutorial.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
D3DXVECTOR3			CCamera::m_posV		= {};
D3DXVECTOR3			CCamera::m_rot		= {};
CCamera::CAMTYPE	CCamera::m_nType	= CAMTYPE_FPS;

//=============================================================================
//　コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_posV			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxProjection = {};
	m_mtxView		= {};
	m_nType			= CAMTYPE_RESULT;
	m_bCamSwitch	= false;
	m_posMouse		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posMouseOld	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posMouseAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle		= 0.0f;
	m_fLength		= m_posV.z - m_posR.z;
}

//=============================================================================
//　デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//　生成処理
//=============================================================================
CCamera *CCamera::Create()
{
	CCamera *pCamera = NULL;

	if (pCamera == NULL)
	{
		pCamera = new CCamera;
		pCamera->Init();
	}

	return pCamera;
}

//=============================================================================
// カメラの初期化処理
//=============================================================================
void CCamera::Init(void)
{
	CManager::GAMERESULT gameresult = CManager::GetResult();			// リザルトの取得
	D3DXVECTOR3 posPlayer			= CPlayer::GetPos();				// プレイヤー座標の取得

	switch (m_nType)
	{// カメラのタイプ別処理
	case CAMTYPE_TITLE:
		m_posV = D3DXVECTOR3(1000.0f, 600.0f, -1000.0f);			// 視点
		m_posR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);					// 注視点
		break;
	case CAMTYPE_RESULT:
		switch (gameresult)
		{
		case CManager::GAMERESULT_WIN:
			m_posV = D3DXVECTOR3(1000.0f, 600.0f, -1000.0f);		// 視点
			m_posR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);				// 注視点
			break;
		case CManager::GAMERESULT_LOSE:
			m_posV = D3DXVECTOR3(0.0f, 200.0f, -20.0f);				// 視点
			m_posR = D3DXVECTOR3(0.0f, -700.0f, 700.0f);			// 注視点
			break;
		}
		break;
	case CAMTYPE_FPS:
		m_posV = posPlayer;											// 視点
		m_posR = D3DXVECTOR3(15.0f, 1000.0f, -350.0f);				// 注視点
		break;
	case CAMTYPE_TPS:
		m_posV = D3DXVECTOR3(100.0f, 400.0f, -550.1f);				// 視点
		m_posR = D3DXVECTOR3(100.0f, 0.0f, -150.0f);				// 注視点
		break;
	}

	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
	m_vecU		= D3DXVECTOR3(0.0f, 0.1f, 0.0f);					// 上方向ベクトル
	m_fLength	= m_posV.z - m_posR.z;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	bool pPause		= false;
	bool pWarning	= false;
	CGame			*pGame		= CManager::GetGame();				// ゲーム情報の取得
	CTutorial		*pTutorial	= CManager::GetTutorial();			// チュートリアル情報の取得
	CInputKeyBoard	*pKey		= CManager::GetInputKeyboard();		// 入力情報の取得(キーボード)
	CInputMouse		*pMouse		= CManager::GetInputMouse();		// 入力情報の取得(マウス)

	if (pGame != NULL)
	{// ポーズの取得
		pPause = pGame->GetPause();
	}
	else
	{
		pPause = false;
	}

	if (pTutorial != NULL)
	{// 警告の取得
		pWarning = pTutorial->GetWarning();
	}
	else
	{
		pWarning = false;
	}

	if (pPause == false && pWarning == false)
	{// ポーズや警告が出てないとき
		switch (m_nType)
		{// カメラのタイプ別処理
		case CAMTYPE_TITLE:
			TitleMove();
			break;
		case CAMTYPE_RESULT:
			ResultMove();
			break;
		case CAMTYPE_FPS:
			FPSMove();
			break;
		case CAMTYPE_TPS:
			TPSMove();
			break;
		}
#ifdef _DEBUG	// デバック処理
		if (pKey != NULL)
		{// 入力処理
			CManager::MODE m_mode = CManager::GetMode();	// ゲームモードの取得
			if (m_mode == CManager::MODE_GAME || m_mode == CManager::MODE_TUTORIAL)
			{// ゲームモードがゲーム、チュートリアルのとき
				if (pKey->GetKeyboardTrigger(DIK_1) == true)
				{	//視点切り替え
					m_bCamSwitch = m_bCamSwitch ? false : true;
					if (m_bCamSwitch == false)
					{
						m_nType = CAMTYPE_FPS;		// 一人称
					}
					else if (m_bCamSwitch == true)
					{
						m_nType = CAMTYPE_TPS;		// 三人称
					}
				}
			}
		}
#endif
	}
	else
	{
		m_posMouse = *pMouse->GetMousePos();	// マウスの位置の取得
		m_posMouseOld = m_posMouse;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::Set(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),								//画角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			//画面比率(アスペクト比)
		10.0f,
		20000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// カメラタイプの設定処理
//=============================================================================
CCamera::CAMTYPE CCamera::SetType(CAMTYPE type)
{
	CManager::GAMERESULT gameresult = CManager::GetResult();	// リザルトの取得
	D3DXVECTOR3 posPlayer			= CPlayer::GetPos();		// プレイヤー座標の取得

	switch (type)
	{// カメラのタイプ別処理
	case CAMTYPE_TITLE:
		m_posV = D3DXVECTOR3(1000.0f, 600.0f, -1000.0f);		// 視点
		m_posR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);				// 注視点
		break;
	case CAMTYPE_RESULT:
		switch (gameresult)
		{// リザルトのタイプ別処理
		case CManager::GAMERESULT_WIN:
			m_posV = D3DXVECTOR3(1000.0f, 600.0f, -1000.0f);	// 視点
			m_posR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);			// 注視点
			break;
		case CManager::GAMERESULT_LOSE:
			m_posV = D3DXVECTOR3(0.0f, 200.0f, -20.0f);			// 視点
			m_posR = D3DXVECTOR3(0.0f, -700.0f, 700.0f);		// 注視点
			break;
		}
		break;
	case CAMTYPE_FPS:
		m_posV = posPlayer;										// 視点
		m_posR = D3DXVECTOR3(15.0f, 1000.0f, -350.0f);			// 注視点
		break;
	case CAMTYPE_TPS:
		m_posV = D3DXVECTOR3(100.0f, 400.0f, -550.1f);			// 視点
		m_posR = D3DXVECTOR3(100.0f, 0.0f, -150.0f);			// 注視点
		break;
	}

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 向きの初期化

	return (m_nType = type);	// タイプの更新
}

//=============================================================================
// タイトルの処理
//=============================================================================
void CCamera::TitleMove(void)
{
	m_fAngle += CIRCLEMOVE;		// 角度の加算

	if (m_fAngle > D3DX_PI)
	{// 角度が3.14以上のとき
		m_fAngle = -D3DX_PI;
	}

	m_posV = D3DXVECTOR3(m_posR.x + sinf(m_fAngle) * m_fLength, m_posV.y, m_posR.z + cosf(m_fAngle) * m_fLength);

	//=============================
	//   デバックログ
	//=============================
	CManager::m_pDebuglog->Print(1, "カメラV ; %0.1f,%0.1f,%0.1f\n", m_posV.x, m_posV.y, m_posV.z);
	CManager::m_pDebuglog->Print(1, "カメラR ; %0.1f,%0.1f,%0.1f\n", m_posR.x, m_posR.y, m_posR.z);
	CManager::m_pDebuglog->Print(1, "ROT ; %0.1f,%0.1f,%0.1f\n", m_rot.x, m_rot.y, m_rot.z);
}

//=============================================================================
// リザルトの処理
//=============================================================================
void CCamera::ResultMove(void)
{
}

//=============================================================================
// 一人称の処理
//=============================================================================
void CCamera::FPSMove(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;						// 計算用マトリックス
	CInputMouse	*pMouse		= CManager::GetInputMouse();		// 入力情報の取得(マウス)
	D3DXVECTOR3 posPlayer	= CPlayer::GetPos();				// プレイヤーの位置の取得
	D3DXVECTOR3 &PlayerRot	= CManager::GetPlayer()->GetRot();	// プレイヤー向きの取得

	if (pMouse != NULL)
	{
		m_posMouse	= *pMouse->GetMousePos();	// マウスの位置の取得
		m_posV		= posPlayer;				// 視点の更新

		//=============================
		//   マウスでカメラ操作
		//=============================
		//注視点位置変更
		if (m_posMouseOld != m_posMouse)
		{
			m_posMouseAngle = m_posMouse - m_posMouseOld;		// 差分の計算

			if (m_posMouseAngle.y < 0)
			{//Y座標(上移動)
				if (m_rot.x > -1.5f)
				{
					if (m_posMouseAngle.y <= -15.0f)
					{
						m_posMouseAngle.y = -15.0f;
					}
					m_rot.x += (m_posMouseAngle.y * 0.002f);
				}
			}
			if (m_posMouseAngle.y > 0)
			{//Y座標(下移動)
				if (m_rot.x < 1.5f)
				{
					if (m_posMouseAngle.y >= 15.0f)
					{
						m_posMouseAngle.y = 15.0f;
					}
					m_rot.x += (m_posMouseAngle.y * 0.002f);
				}
			}
			if (m_posMouseAngle.x < 0)
			{//X座標(左移動)
				m_rot.y += (m_posMouseAngle.x * 0.002f);
			}
			if (m_posMouseAngle.x > 0)
			{//X座標(右移動)
				m_rot.y += (m_posMouseAngle.x * 0.002f);
			}
			m_posMouseOld = m_posMouse;		// 過去位置の更新
		}

		// カメラの追尾************************************************************
		D3DXMatrixIdentity(&mtxWorld);		// ワールドマトリックスの初期化
		// 回転
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		// 位置
		D3DXMatrixTranslation(&mtxTrans, m_posV.x, m_posV.y, m_posV.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
		// 位置
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 100.0f);
		D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxWorld); // mtxWorldの角度を利用して生成

		m_posR.x = mtxTrans._41;
		m_posR.y = mtxTrans._42;
		m_posR.z = mtxTrans._43;
		// ************************************************************************
		// マウスループ************************************************************
		if (m_posMouse.x <= 0.0f)
		{
			SetCursorPos(1279, (int)m_posMouse.y);
			m_posMouse = D3DXVECTOR3(1279.0f, m_posMouse.y, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		else if (m_posMouse.x >= 1279.0f)
		{
			SetCursorPos(0, (int)m_posMouse.y);
			m_posMouse = D3DXVECTOR3(0.0f, m_posMouse.y, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		if (m_posMouse.y <= 0.0f)
		{
			SetCursorPos((int)m_posMouse.x, 719);
			m_posMouse = D3DXVECTOR3(m_posMouse.x, 719.0f, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		else if (m_posMouse.y >= 719.0f)
		{
			SetCursorPos((int)m_posMouse.x, 0);
			m_posMouse = D3DXVECTOR3(m_posMouse.x, 0.0f, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		// ************************************************************************

		PlayerRot = m_rot;		// プレイヤー追尾
	}
	//=============================
	//   デバックログ
	//=============================
	CManager::m_pDebuglog->Print(1, "カメラV ; %0.1f,%0.1f,%0.1f\n", m_posV.x, m_posV.y, m_posV.z);
	CManager::m_pDebuglog->Print(1, "カメラR ; %0.1f,%0.1f,%0.1f\n", m_posR.x, m_posR.y, m_posR.z);
	CManager::m_pDebuglog->Print(1, "マウス差分 ; %0.1f,%0.1f,%0.1f\n", m_posMouseAngle.x, m_posMouseAngle.y, m_posMouseAngle.z);
	CManager::m_pDebuglog->Print(1, "ROT ; %0.1f,%0.1f,%0.1f\n", m_rot.x, m_rot.y, m_rot.z);
	CManager::m_pDebuglog->Print(1, "～PlayerRot.x %.1f  PlayerRot.y %.1f  PlayerRot.z %.1f～\n", PlayerRot.x, PlayerRot.y, PlayerRot.z);
}
//=============================================================================
// 三人称の処理
//=============================================================================
void CCamera::TPSMove(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;						// 計算用マトリックス
	CInputMouse	*pMouse		= CManager::GetInputMouse();		// 入力情報の取得(マウス)
	D3DXVECTOR3 posPlayer	= CPlayer::GetPos();				// プレイヤーの位置の取得
	D3DXVECTOR3 &PlayerRot	= CManager::GetPlayer()->GetRot();	// プレイヤー向きの取得

	if (pMouse != NULL)
	{
		m_posMouse	= *pMouse->GetMousePos();	// マウスの位置の取得
		m_posR		= posPlayer;				// 視点の更新

		//=============================
		//   マウスでカメラ操作
		//=============================
		//注視点位置変更
		if (m_posMouseOld != m_posMouse)
		{
			m_posMouseAngle = m_posMouse - m_posMouseOld;		// 差分の計算

			if (m_posMouseAngle.y < 0)
			{//Y座標(上移動)
				if (m_rot.x > -1.5f)
				{
					if (m_posMouseAngle.y <= -15.0f)
					{
						m_posMouseAngle.y = -15.0f;
					}
					m_rot.x += (m_posMouseAngle.y * 0.005f);
				}
			}
			if (m_posMouseAngle.y > 0)
			{//Y座標(下移動)
				if (m_rot.x < 1.5f)
				{
					if (m_posMouseAngle.y >= 15.0f)
					{
						m_posMouseAngle.y = 15.0f;
					}
					m_rot.x += (m_posMouseAngle.y * 0.005f);
				}
			}
			if (m_posMouseAngle.x < 0)
			{//X座標(左移動)
				m_rot.y += (m_posMouseAngle.x * 0.005f);
			}
			if (m_posMouseAngle.x > 0)
			{//X座標(右移動)
				m_rot.y += (m_posMouseAngle.x * 0.005f);
			}
			m_posMouseOld = m_posMouse;		// 過去位置の更新
		}

		// カメラの追尾************************************************************
		D3DXMatrixIdentity(&mtxWorld);		// ワールドマトリックスの初期化
		// 回転
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, (m_rot.x * -1.0f), m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		// 位置
		D3DXMatrixTranslation(&mtxTrans, m_posR.x, m_posR.y, m_posR.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
		// 位置
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 800.0f, 10000.0f);
		D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxWorld); // mtxWorldの角度を利用して生成

		m_posV.x = mtxTrans._41;
		m_posV.y = mtxTrans._42;
		m_posV.z = mtxTrans._43;
		// ************************************************************************
		// マウスループ************************************************************
		if (m_posMouse.x <= 0.0f)
		{
			SetCursorPos(1279, (int)m_posMouse.y);
			m_posMouse = D3DXVECTOR3(1279.0f, m_posMouse.y, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		else if (m_posMouse.x >= 1279.0f)
		{
			SetCursorPos(0, (int)m_posMouse.y);
			m_posMouse = D3DXVECTOR3(0.0f, m_posMouse.y, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		if (m_posMouse.y <= 0.0f)
		{
			SetCursorPos((int)m_posMouse.x, 719);
			m_posMouse = D3DXVECTOR3(m_posMouse.x, 719.0f, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		else if (m_posMouse.y >= 719.0f)
		{
			SetCursorPos((int)m_posMouse.x, 0);
			m_posMouse = D3DXVECTOR3(m_posMouse.x, 0.0f, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		// ************************************************************************

		PlayerRot	= m_rot;		// プレイヤー追尾
		PlayerRot.y += D3DX_PI;
	}
	//=============================
	//   デバックログ
	//=============================
	CManager::m_pDebuglog->Print(1, "カメラV ; %0.1f,%0.1f,%0.1f\n", m_posV.x, m_posV.y, m_posV.z);
	CManager::m_pDebuglog->Print(1, "カメラR ; %0.1f,%0.1f,%0.1f\n", m_posR.x, m_posR.y, m_posR.z);
	CManager::m_pDebuglog->Print(1, "マウス差分 ; %0.1f,%0.1f,%0.1f\n", m_posMouseAngle.x, m_posMouseAngle.y, m_posMouseAngle.z);
	CManager::m_pDebuglog->Print(1, "ROT ; %0.1f,%0.1f,%0.1f\n", m_rot.x, m_rot.y, m_rot.z);
	CManager::m_pDebuglog->Print(1, "～PlayerRot.x %.1f  PlayerRot.y %.1f  PlayerRot.z %.1f～\n", PlayerRot.x, PlayerRot.y, PlayerRot.z);
}