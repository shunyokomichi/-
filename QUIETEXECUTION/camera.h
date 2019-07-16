//=============================================================================
//
// �J�������� [camera.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CIRCLEMOVE (0.001f) //�J�����̈ړ���

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CCamera
{
public:
	typedef enum
	{
		CAMTYPE_TITLE = 0,		// �^�C�g��
		CAMTYPE_RESULT,			// ���U���g
		CAMTYPE_FPS,			// ��l��
		CAMTYPE_TPS,			// �O�l��
		CAMTYPE_MAX				// �^�C�v�̍ő吔
	}CAMTYPE;

	CCamera();
	~CCamera();
	static CCamera *Create();

	void	Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Set		(void);

	CAMTYPE				SetType	(CAMTYPE	type);
	static D3DXVECTOR3	GetRot	(void) { return m_rot; };
	static CAMTYPE		GetType	(void) { return m_nType; };
	static D3DXVECTOR3	GetPosV	(void) { return m_posV; }

	void	TitleMove	(void);
	void	ResultMove	(void);
	void	FPSMove		(void);
	void	TPSMove		(void);

private:
	static D3DXVECTOR3		m_posV;				// ���݂̎��_
	D3DXVECTOR3				m_posR;				// ���݂̒����_
	static D3DXVECTOR3		m_rot;				// ���݂̌���
	D3DXVECTOR3				m_vecU;				// ������x�N�g��
	D3DXMATRIX				m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX				m_mtxView;			// �r���[�}�g���b�N�X
	static CAMTYPE			m_nType;			// ���݂̃^�C�v
	bool					m_bCamSwitch;		// �J�����؂�ւ��p
	D3DXVECTOR3				m_posMouse;			// �}�E�X�̈ʒu
	D3DXVECTOR3				m_posMouseOld;		// �}�E�X�̈ʒu(�ߋ�)
	D3DXVECTOR3				m_posMouseAngle;	// �}�E�X�̍���
	float					m_fAngle;			// �J�����̊p�x
	float					m_fLength;			// �J�����̋���
};
#endif
