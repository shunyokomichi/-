//=============================================================================
//
// �Ǐ��� [wall.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �ǂݍ��ރe�N�X�`����
#define WALL_TEX_0	"data\\TEXTURE\\wall000.png"			// ��

#define WALL_PRIOTITY		(3)
#define	MAX_WALL_TEX		(10)
#define	MAX_WALL_HEIGHT		(2000.0f)

//=============================================================================
// �N���X��`
//=============================================================================
class CWall : public CScene3D
{
public:
	CWall();
	~CWall();

	static HRESULT	Load	(void);
	static void		UnLoad	(void);
	static CWall	*Create	(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, int nType);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	void	CollisionWallPlayer	(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth);
	void	CollisionWallEnemy	(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth);
	void	CollisionBullet		(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_WALL_TEX];	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3					m_pos;						// �ʒu
	D3DXVECTOR2					m_size;						// �傫��
	int							m_nType;					// �ǂ̕���
};
#endif
