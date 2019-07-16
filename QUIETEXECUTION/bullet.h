//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "scene3D.h"
#include "enemy.h"

#define BULLET_PRIOTITY (3)
#define BULLET_TEXTURENAME00 "data\\TEXTURE\\bullet000.png"

class CWall;
class CObject;
class CEnemy;
//=============================================================================
// �N���X�̒�`
//=============================================================================
class CBullet : public CScene3D  // ���h���N���X
{
public:
	CBullet();
	~CBullet();
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, CEnemy *pEnemy);

	static HRESULT	Load	(void);
	static void		UnLoad	(void);
	HRESULT			Init	(void);
	void			Uninit	(void);
	void			Update	(void);
	void			Draw	(void);

	bool CollisionBulletWall	(CWall *pWall);
	bool CollisionBulletObje	(CObject *pObj);
	bool CollisionBulletSearch	(void);

	CEnemy *&GetEnemy(void) { return m_pEnemy; };

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3					m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3					m_move;			// �ړ���
	D3DXVECTOR3					m_rot;			// ����
	D3DXVECTOR2					m_size;			// �T�C�Y
	int							m_nLife;		// ���C�t
	int							m_nCntLife;		// ���C�t�J�E���^�[
	CEnemy						*m_pEnemy;		// �G�l�~�[�̃|�C���^
};
#endif