//=============================================================================
//
// �`���[�g���A�����̌x������ [warning.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _WARNING_H_
#define _WARNING_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

#define WARNING_TEXTURENAME00	"data\\TEXTURE\\tutorial\\tutorial_board.png"		// BG
#define WARNING_TEXTURENAME01	"data\\TEXTURE\\tutorial\\fsde_tutorial.png"		// �w�i
#define WARNING_TEXTURENAME02	"data\\TEXTURE\\tutorial\\YES000.png"				// YES
#define WARNING_TEXTURENAME03	"data\\TEXTURE\\tutorial\\NO000.png"				// NO

#define WARNING_MAX_TEXTURE		(4)
#define WARNING_MAX_POLIGON		(4)
#define WARNING_WIDTH			(SCREEN_WIDTH)					// �|�[�Y�̕�
#define WARNING_HEIGHT			(SCREEN_HEIGHT)					// �|�[�Y�̍���

//*****************************************************************************
//	�N���X��`
//*****************************************************************************
class CWarning
{
public:
	typedef enum
	{
		TYPE_YES,		// YES
		TYPE_NO,		// NO
		TYPE_MAX
	}TYPE;

	CWarning();
	~CWarning();

	HRESULT			Init	(void);
	void			Uninit	(void);
	static void		Update	(void);
	static void		Draw	(void);

	// �ÓI�����o�ϐ�
	static HRESULT	Load	(void);		// �ǂݍ��ފ֐�(�e�N�X�`��)
	static void		Unload	(void);		// �J������֐�(�e�N�X�`��)

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[WARNING_MAX_TEXTURE];	// �e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3						m_pos;								// �|�[�Y�̈ʒu
	static int						m_SelectNum;						// �I��ԍ�
	static float					m_SelectColor;						// �I��F

};
#endif