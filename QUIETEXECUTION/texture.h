//=============================================================================
//
// �e�L�X�g�̏��� [texture.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"
#include "enemy.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �^�C�g��
#define TEXTURENAME_TITLE_0		"data\\TEXTURE\\title\\title000.png"			// �^�C�g��(��)
#define TEXTURENAME_TITLE_1		"data\\TEXTURE\\title\\title001.png"			// �^�C�g��(�E)
#define TEXTURENAME_TITLE_2		"data\\TEXTURE\\title\\title002.png"			// PRESS�w�i
#define TEXTURENAME_TITLE_3		"data\\TEXTURE\\title\\puressanybotton.png"		// PRESS

// UI
#define TEXTURENAME_UI_0		"data\\TEXTURE\\UI\\gauge000.png"				// �Q�[�W(�g)
#define TEXTURENAME_UI_1		"data\\TEXTURE\\UI\\lifegauge001.png"			// HP�}�[�N
#define TEXTURENAME_UI_2		"data\\TEXTURE\\UI\\lifegauge002.png"			// HP�Q�[�W
#define TEXTURENAME_UI_3		"data\\TEXTURE\\UI\\Degree000.png"				// �댯�x(�w�i)
#define TEXTURENAME_UI_4		"data\\TEXTURE\\UI\\Degree001.png"				// �댯�x(�}�[�N)
#define TEXTURENAME_UI_5		"data\\TEXTURE\\UI\\crystal000.png"				// �c��N���X�^����
#define TEXTURENAME_UI_6		"data\\TEXTURE\\UI\\cross000.png"				// �~
#define TEXTURENAME_UI_7		"data\\TEXTURE\\UI\\damage000.png"				// �����Ԃ�
#define TEXTURENAME_UI_8		"data\\TEXTURE\\UI\\pointer000.png"				// ���e�B�N��

// �`���[�g���A��
#define TEXTURENAME_TUTORIAL_0	"data\\TEXTURE\\tutorial\\tutorial_board.png"	// �\���w�i
#define TEXTURENAME_TUTORIAL_1	"data\\TEXTURE\\tutorial\\tutorial000.png"		// �J��������
#define TEXTURENAME_TUTORIAL_2	"data\\TEXTURE\\tutorial\\tutorial001.png"		// �ړ�����
#define TEXTURENAME_TUTORIAL_3	"data\\TEXTURE\\tutorial\\tutorial002.png"		// �W�����v����
#define TEXTURENAME_TUTORIAL_4	"data\\TEXTURE\\tutorial\\tutorial003.png"		// �U������
#define TEXTURENAME_TUTORIAL_5	"data\\TEXTURE\\tutorial\\tutorial004.png"		// ���C�t����
#define TEXTURENAME_TUTORIAL_6	"data\\TEXTURE\\tutorial\\tutorial005.png"		// �댯�x����
#define TEXTURENAME_TUTORIAL_7	"data\\TEXTURE\\tutorial\\tutorial006.png"		// �I������
#define TEXTURENAME_TUTORIAL_8	"data\\TEXTURE\\tutorial\\fsde_tutorial.png"	// �m�F���(�w�i)
#define TEXTURENAME_TUTORIAL_9	"data\\TEXTURE\\tutorial\\YES000.png"			// �m�F���(YES)
#define TEXTURENAME_TUTORIAL_10	"data\\TEXTURE\\tutorial\\NO000.png"			// �m�F���(NO)

// ���U���g
#define TEXTURENAME_RESULT_0	"data\\TEXTURE\\result\\GAMECLEAR.png"			// GAMECLEAR
#define TEXTURENAME_RESULT_1	"data\\TEXTURE\\result\\GAMEOVER.png"			// GAMEOVER
#define TEXTURENAME_RESULT_2	"data\\TEXTURE\\pause\\pause000.png"			// ���j���[�w�i
#define TEXTURENAME_RESULT_3	"data\\TEXTURE\\pause\\RETRY.png"				// RETRY
#define TEXTURENAME_RESULT_4	"data\\TEXTURE\\pause\\QUIT.png"				// QUIT


class CEnemy;
//=====================
//  CScene2D�̔h���N���X
//=====================
class CTexture : public CScene2D
{
public://�N�ł�������
	CTexture();
	~CTexture();
	static HRESULT	Load	(void);
	static void		UnLoad	(void);
	static CTexture *Create	(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	void	TitleMove	(void);		// �^�C�g���̓���
	void	TutorialMove(void);		// �`���[�g���A���̓���
	void	GameMove	(void);		// �Q�[���̓���
	void	ResultMove	(void);		// ���U���g�̓���

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_TEXTURE];	// ���L�e�N�X�`���̃|�C���^
	int							m_nType;					// ���
	D3DXVECTOR2					m_size;						// �T�C�Y(����)
	D3DXVECTOR2					m_sizeDest;					// �T�C�Y(�ړI)
	float						m_fLength;					// ����
	float						m_fAngle;					// �p�x
	D3DXVECTOR3					m_rot;						// ����
	D3DXVECTOR3					m_posDest;					// ���W(�ړI)
	int							m_nCounterAnim;				// �A�j���[�V�����̃J�E���^�[
	int							m_nPatternAnim;				// �A�j���[�V�����p�^�[���̃J�E���^�[
	int							m_nCntflame;				// �J�E���^�[
	int							m_nCntColor;				// �J�E���^�[
	static int					m_nCntTutorial;				// ��������J�E���^�[
	bool						m_nBlink;					// �_��
	CEnemy::EnemyStatse			m_EnemyStatse;				// �G�l�~�[�̃X�e�[�^�X
};
#endif