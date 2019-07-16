//=============================================================================
//
// �X�R�A���� [score.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "number.h"

#define MAX_NUMBER (2)			// ����

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CScore : public CScene  // ���h���N���X
{
public:
	CScore();
	~CScore();
	static CScore *Create(D3DXVECTOR3 pos);

	HRESULT		Init	(void);
	void		Uninit	(void);
	void		Update	(void);
	void		Draw	(void);
	static void SetScore(int nScore);
	int			GetScore(void);
	static void AddScore(int nValue);

private:
	static CNumber			*m_apNumber[MAX_NUMBER];	// �������̃|�C���^
	static int				m_nScore;					// �X�R�A
	static D3DXVECTOR3		m_pos;						// �|���S���̈ʒu
};
#endif