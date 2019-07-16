//=============================================================================
//
// メッシュフィールド処理 [MeshField.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene.h"

#define POYGON_TEXTURENAME00 "data/TEXTURE/field.jpg"

//=============================================================================
// クラスの定義
//=============================================================================
class CMeshField : public CScene  // ←派生クラス
{
public:
	CMeshField(int nPriority = 2, CScene::OBJTYPE objType = CScene::OBJTYPE_MESHFILED);
	~CMeshField();
	static CMeshField *Create(D3DXVECTOR3 pos, float width, float height, int meshX , int meshY, int nTexType);

	HRESULT Init		(void);
	void	Uninit		(void);
	void	Update		(void);
	void	Draw		(void);
	float	GetHeight	(D3DXVECTOR3 pos, D3DXVECTOR3 posold);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pldxBuff;				// インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3				m_pos;					// 位置
	D3DXVECTOR3				m_rot;					// 向き
	float					m_fWidth, m_fHeight;	// 幅、高さ
	int						m_meshX, m_meshZ;		// 横、縦の分割数
	int						m_posMAX, m_meshMAX;	// 頂点、メッシュの最大数
	int						m_nTexType;				// テクスチャの番号
};
#endif