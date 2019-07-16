//=============================================================================
//
// ���U���g���� [result.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "manager.h"
#include "loadtext.h"

//=============================================================================
// �O���錾
//=============================================================================
class CWall;
class CObject;
class CEnemy;
class CLoadText;

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CResult
{
public:
	CResult();
	~CResult();

	HRESULT Init	(void);				// ����������
	void	Update	(void);				// �X�V����
	void	Uninit	(void);				// �I������
	void	Draw	(void);				// �`�揈��

	void	SetTex(void);		// �e�N�X�`���̐���
	void	SetFloor(CLoadText::MAPTEXT Map);		// ���̐���
	void	SetWall(void);		// �ǂ̐���
	void	SetObjet(CLoadText::MAPTEXT Map);		// �I�u�W�F�N�g�̐���

	static int GetSelect(void) { return m_nSelect; }

private:
	static	CPlayer		*m_pPlayer;		// �v���C���[�̃|�C���^
	static	CWall		*m_pWall;		// �ǂ̃|�C���^
	static	CObject		*m_pObject;		// �I�u�W�F�N�g�̃|�C���^
	static CEnemy		*m_pEnemy;		// �G�̃|�C���^
	static int			m_nSelect;		// �I��ԍ�
	CLoadText			*m_text;			// �e�L�X�g�ւ̃|�C���^
	int					m_nCntType;		// �e�L�X�g�̉�
};
#endif