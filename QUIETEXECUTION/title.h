//=============================================================================
//
// �^�C�g������ [title.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "manager.h"
#include "loadtext.h"

//=============================================================================
// �O���錾
//=============================================================================
class CWall;
class CObject;
class CLoadText;

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CTitle
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// �������Ă��Ȃ�
		TYPE_PRESS,		// PRESS���ꂽ�Ƃ�
		TYPE_MAX
	}TYPE;

	CTitle();
	~CTitle();

	HRESULT Init	(void);		// ����������
	void	Update	(void);		// �X�V����
	void	Uninit	(void);		// �I������
	void	Draw	(void);		// �`�揈��

	void	SetTex	(void);		// �e�N�X�`���̐���
	void	SetFloor(CLoadText::MAPTEXT Map);		// ���̐���
	void	SetWall	(void);		// �ǂ̐���
	void	SetObjet(CLoadText::MAPTEXT Map);		// �I�u�W�F�N�g�̐���

	static TYPE GetType(void) { return m_pType; };	// �^�C�v�̎擾

private:
	static	CWall		*m_pWall;		// �ǂ̃|�C���^
	static	CObject		*m_pObject;		// �I�u�W�F�N�g�̃|�C���^
	static	TYPE		m_pType;		// ���
	CLoadText			*m_text;		// �e�L�X�g�ւ̃|�C���^
	int					m_nCntType;		// �e�L�X�g�̉�
};
#endif