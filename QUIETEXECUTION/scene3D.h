//=============================================================================
//
// ���C������ [scene3D.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BILLBOORD_TEX		(10)

//=====================
//  CScene�̔h���N���X
//=====================
class CScene3D : public CScene
{
public://�N�ł�������
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_BULLET,
		TYPE_BILLBOORD,
		TYPE_WALL,
		TYPE_GROUND,
		TYPE_EFFECT,
		TYPE_SHADOW,
		TYPE_MAX
	}TYPE;

	CScene3D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_3DPOLYGON);
	~CScene3D();
	static CScene3D *Create();

	HRESULT Init		(void);
	void	Uninit		(void);
	void	Update		(void);
	void	Draw		(void);
	void	BindTexture	(LPDIRECT3DTEXTURE9 Texture);

	D3DXVECTOR3 GetPosition		(void)				{ return m_pos; }
	D3DXVECTOR3 GetRotation		(void)				{ return m_rot; }
	D3DXVECTOR2 GetSize			(void)				{ return m_size; }
	D3DXVECTOR3 GetMove			(void)				{ return m_Move; }
	D3DXCOLOR   GetColor		(void)				{ return m_color; }
	int			GetTexType		(void)				{ return m_TexType; }
	void		SetTypeNumber	(TYPE Type)			{ m_Type = Type; }
	void		SetTexType		(int Type)			{ m_TexType = Type; }
	void		SetMove			(D3DXVECTOR3 move)	{ m_Move = move; }
	void		SetPosition		(D3DXVECTOR3 pos)	{ m_pos = pos; }
	void		SetWall			(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 nSize);
	void		SetPosSize		(D3DXVECTOR3 pos,D3DXVECTOR2 size);
	void		SetGroundPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void		SetColor		(D3DXCOLOR *Color);
	void		SetNor			(D3DXVECTOR3 Nor);
	void		SetVtxEffect	(float fRadius);
	void		SetRotation		(D3DXVECTOR3 rot)	{ m_rot = rot; }
	void		SetTex			(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV);	// �A�j���[�V�������ݒ�


private://�l�ł̂ݎg��
	LPDIRECT3DTEXTURE9		m_pTexture;						// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;							// �ʒu
	D3DXVECTOR3				m_rot;							// ����
	D3DXVECTOR2				m_size;							// �T�C�Y
	D3DXVECTOR3				m_Move;							// ����
	D3DXCOLOR				m_color;						// �F
	D3DXMATRIX				m_mtxWorld;						// ���[���h�}�g���b�N�X
	TYPE					m_Type;							// �r���{�[�h�F�P���G�t�F�N�g�F�Q��
	int						m_TexType;						// �e�N�X�`���̃^�C�v
	D3DXVECTOR2				m_TexUV;						// UV
};
#endif