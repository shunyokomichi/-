//=============================================================================
//
// �f�o�b�N���O���� [debuglog.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _DEBUGLOG_H_
#define _DEBUGLOG_H_

#include "main.h"

#define MAX_STR (1024)
//=============================================================================
// �N���X�̒�`
//=============================================================================
class CDebuglog
{
public:
	CDebuglog();
	~CDebuglog();

	void Init	(void);
	void Uninit	(void);
	void Draw	(void);
	void Print	(int nNum,char *string, ...);

private:
	static LPD3DXFONT	m_pFont;
	static char			m_aStr[MAX_STR];
};

#endif