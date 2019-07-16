//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CFade
{
public:
	//*************************************
	// �t�F�[�h�̏��
	//*************************************
	typedef enum
	{
		FADE_NONE = 0,		// �������Ă��Ȃ����
		FADE_IN,			// �t�F�[�h�C�����
		FADE_OUT,			// �t�F�[�h�A�E�g���
		FADE_MAX
	} FADE;

	CFade();
	~CFade();
	static CFade	*Create(void);

	static HRESULT	Init	(void);
	static void		Uninit	(void);
	static void		Update	(void);
	static void		Draw	(void);

	static void		SetFade(CManager::MODE modeNext);
	static FADE		GetFade(void) { return m_fadeType; }

private:
	static LPDIRECT3DTEXTURE9			m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3							m_pos;			// �|���S���̈ʒu
	static CManager::MODE				m_modeNext;		// ���̃Q�[���̏��
	static D3DXCOLOR					m_fadeColor;	// �t�F�[�h�̐F
	static FADE							m_fadeType;		// �t�F�[�h�̏��
};
#endif