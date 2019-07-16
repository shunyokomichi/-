//=============================================================================
//
// �r���{�[�h�̏��� [billboord.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _BILLBOORD_H_
#define _BILLBOORD_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BILL_TEXTURE_NAME_0		"data\\TEXTURE\\Marker000.png"				//�ǂݍ��ރe�N�X�`���t�@�C��


//=====================
//  CScene�̔h���N���X
//=====================
class CBillBoord : public CScene3D
{
public://�N�ł�������
	CBillBoord();
	~CBillBoord();
	static CBillBoord	*Create	(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTexType);
	static HRESULT		Load	(void);
	static void			UnLoad	(void);
	HRESULT				Init	(void);
	void				Uninit	(void);
	void				Update	(void);
	void				Draw	(void);

private://�l�ł̂ݎg��
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_BILLBOORD_TEX];	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;							// �ʒu
	D3DXVECTOR2					m_size;							// �T�C�Y
};
#endif