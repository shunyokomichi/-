//=============================================================================
//
// パーティクル処理 [particle.h]
// Author :			荒谷由朗
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "scene3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EFFECTTYPE	(2)		// エフェクト最大数
#define MAX_EFFECT		(180)	// エフェクト最大描画数

//=============================================================================
// クラス定義
//=============================================================================
class CParticle : public CScene
{
public:
	CParticle();		// コンストラクタ
	~CParticle() {};	// デストラクタ(中身無し)

	static HRESULT		Load	(void);													// テクスチャ読み込み
	static void			Unload	(void);													// テクスチャ開放
	static CParticle	*Create	(D3DXVECTOR3 pos, int nTextureType, int ntexttype);		// 生成処理

	HRESULT Init	(void);		// 初期化処理
	void	Uninit	(void);		// 終了処理
	void	Update	(void);		// 更新処理
	void	Draw	(void);		// 描画処理(中身無し)

private:
	static LPDIRECT3DTEXTURE9	m_pTexture		[MAX_EFFECTTYPE];			// テクスチャ情報へのポインタ
	D3DXMATRIX					m_mtxWorld;									// ワールドマトリックス
	CScene3D					*m_apScene3D	[MAX_EFFECT];				// シーン3Dのポインタ型変数(4)
	static int					m_nEffectType;								// エフェクト種類
	static D3DXVECTOR3			m_pos;										// 位置
	D3DXVECTOR3					m_rot, m_size, m_move, m_posRange;			// 方向,サイズ,移動量,位置範囲
	D3DXVECTOR2					m_TexUV;									// UV
	D3DXCOLOR					m_col;										// 色
	int							m_nSetPoriMAX;								// ポリゴン数
	int							m_nLife;									// 寿命
	float						m_fGravity;									// 重力
	int							m_nBillType;								// 加算合成有無の種類
	int							m_nAnimCounter, m_nPatternCnter;			// アニメーションカウンタ,パターン
	D3DXVECTOR3					m_NumMove		[MAX_EFFECT];				// 動きの割り当て
	int							m_nNumLife		[MAX_EFFECT];				// 寿命割り当て設定
	static char					m_cTextName		[MAX_EFFECTTYPE][128];		// テキスト名
	static char					m_cTextureName	[MAX_EFFECTTYPE][128];		// テクスチャ名
	int							m_nBindText;								// テクスチャ割り当て用
	static int					m_nTextureType;								// テクスチャ種類
	int							m_nCntAnim;									// アニメーションカウンター
};
#endif