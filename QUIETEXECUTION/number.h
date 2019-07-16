//=============================================================================
//
// �X�R�A���� [number.h]
// Author :		Yoshiro Araya
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "scene2D.h"

#define NUMBER_TEXTURENAME00 "data\\TEXTURE\\UI\\number000.png"

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CNumber
{
public:
	CNumber();
	~CNumber();
	static CNumber *Create	(D3DXVECTOR3 pos, float width, float height);

	static HRESULT	Load	(void);
	static void		Unload	(void);

	HRESULT		Init		(D3DXVECTOR3 pos, float width, float height);
	void		Uninit		(void);
	void		Update		(void);
	void		Draw		(void);
	void		SetNumber	(int number);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3						m_pos;			// �|���S���̈ʒu
	float							m_fWidth;		// ��
	float							m_fHeight;		// ����
};
#endif