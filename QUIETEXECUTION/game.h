//=============================================================================
//
// �Q�[������ [game.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "manager.h"
#include "loadtext.h"

#define GAUGE_SIZE (140.0f)		// �Q�[�W�̃T�C�Y

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
class CGame
{
public:
	CGame();
	~CGame();

	HRESULT Init	(void);		// ����������
	void	Update	(void);		// �X�V����
	void	Uninit	(void);		// �I������
	void	Draw	(void);		// �`�揈��

	void	SetTex(void);		// �e�N�X�`���̐���
	void	SetFloor(CLoadText::MAPTEXT Map);		// ���̐���
	void	SetWall(void);		// �ǂ̐���
	void	SetObjet(CLoadText::MAPTEXT Map);		// �I�u�W�F�N�g�̐���
	void	SetEnemy(void);		// �G�̐���

	static CPlayer *GetPlayer(void)		{ return m_pPlayer; };
	static CObject *Get3DObject(void)	{ return m_pObject; }

	bool GetPause(void)			{ return m_bPause; }
	void SetPause(bool bPause)	{ m_bPause = bPause; }

private:
	static CPlayer		*m_pPlayer;			// �v���C���[�̃|�C���^
	static CWall		*m_pWall;			// �ǂ̃|�C���^
	static CObject		*m_pObject;			// �I�u�W�F�N�g�̃|�C���^
	static CEnemy		*m_pEnemy;			// �G�̃|�C���^
	bool				m_bPause;			// �|�[�Y
	CLoadText			*m_text;			// �e�L�X�g�ւ̃|�C���^
	int					m_nCntType;		// �e�L�X�g�̉�
};
#endif