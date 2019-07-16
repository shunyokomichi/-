//=============================================================================
//
// ���C�g���� [light.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

#define MAX_LIGHT		(3)			// ���C�g�̍ő吔

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CLight
{
public:
	CLight();
	~CLight();
	CLight *Create();

	void Init	(void);
	void Uninit	(void);
	void Update	(void);
private:
	D3DLIGHT9 m_light[MAX_LIGHT];	// ���C�g�̏��
};
#endif