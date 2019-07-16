//=============================================================================
//
// カメラ処理 [camera.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CIRCLEMOVE (0.001f) //カメラの移動量

//=============================================================================
// クラスの定義
//=============================================================================
class CCamera
{
public:
	typedef enum
	{
		CAMTYPE_TITLE = 0,		// タイトル
		CAMTYPE_RESULT,			// リザルト
		CAMTYPE_FPS,			// 一人称
		CAMTYPE_TPS,			// 三人称
		CAMTYPE_MAX				// タイプの最大数
	}CAMTYPE;

	CCamera();
	~CCamera();
	static CCamera *Create();

	void	Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Set		(void);

	CAMTYPE				SetType	(CAMTYPE	type);
	static D3DXVECTOR3	GetRot	(void) { return m_rot; };
	static CAMTYPE		GetType	(void) { return m_nType; };
	static D3DXVECTOR3	GetPosV	(void) { return m_posV; }

	void	TitleMove	(void);
	void	ResultMove	(void);
	void	FPSMove		(void);
	void	TPSMove		(void);

private:
	static D3DXVECTOR3		m_posV;				// 現在の視点
	D3DXVECTOR3				m_posR;				// 現在の注視点
	static D3DXVECTOR3		m_rot;				// 現在の向き
	D3DXVECTOR3				m_vecU;				// 上方向ベクトル
	D3DXMATRIX				m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX				m_mtxView;			// ビューマトリックス
	static CAMTYPE			m_nType;			// 現在のタイプ
	bool					m_bCamSwitch;		// カメラ切り替え用
	D3DXVECTOR3				m_posMouse;			// マウスの位置
	D3DXVECTOR3				m_posMouseOld;		// マウスの位置(過去)
	D3DXVECTOR3				m_posMouseAngle;	// マウスの差分
	float					m_fAngle;			// カメラの角度
	float					m_fLength;			// カメラの距離
};
#endif
