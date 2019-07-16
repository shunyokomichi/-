//=============================================================================
//
// �J�������� [camera.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "debuglog.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "tutorial.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
D3DXVECTOR3			CCamera::m_posV		= {};
D3DXVECTOR3			CCamera::m_rot		= {};
CCamera::CAMTYPE	CCamera::m_nType	= CAMTYPE_FPS;

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	m_posV			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxProjection = {};
	m_mtxView		= {};
	m_nType			= CAMTYPE_RESULT;
	m_bCamSwitch	= false;
	m_posMouse		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posMouseOld	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posMouseAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle		= 0.0f;
	m_fLength		= m_posV.z - m_posR.z;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//�@��������
//=============================================================================
CCamera *CCamera::Create()
{
	CCamera *pCamera = NULL;

	if (pCamera == NULL)
	{
		pCamera = new CCamera;
		pCamera->Init();
	}

	return pCamera;
}

//=============================================================================
// �J�����̏���������
//=============================================================================
void CCamera::Init(void)
{
	CManager::GAMERESULT gameresult = CManager::GetResult();			// ���U���g�̎擾
	D3DXVECTOR3 posPlayer			= CPlayer::GetPos();				// �v���C���[���W�̎擾

	switch (m_nType)
	{// �J�����̃^�C�v�ʏ���
	case CAMTYPE_TITLE:
		m_posV = D3DXVECTOR3(1000.0f, 600.0f, -1000.0f);			// ���_
		m_posR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);					// �����_
		break;
	case CAMTYPE_RESULT:
		switch (gameresult)
		{
		case CManager::GAMERESULT_WIN:
			m_posV = D3DXVECTOR3(1000.0f, 600.0f, -1000.0f);		// ���_
			m_posR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);				// �����_
			break;
		case CManager::GAMERESULT_LOSE:
			m_posV = D3DXVECTOR3(0.0f, 200.0f, -20.0f);				// ���_
			m_posR = D3DXVECTOR3(0.0f, -700.0f, 700.0f);			// �����_
			break;
		}
		break;
	case CAMTYPE_FPS:
		m_posV = posPlayer;											// ���_
		m_posR = D3DXVECTOR3(15.0f, 1000.0f, -350.0f);				// �����_
		break;
	case CAMTYPE_TPS:
		m_posV = D3DXVECTOR3(100.0f, 400.0f, -550.1f);				// ���_
		m_posR = D3DXVECTOR3(100.0f, 0.0f, -150.0f);				// �����_
		break;
	}

	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ����
	m_vecU		= D3DXVECTOR3(0.0f, 0.1f, 0.0f);					// ������x�N�g��
	m_fLength	= m_posV.z - m_posR.z;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void)
{
	bool pPause		= false;
	bool pWarning	= false;
	CGame			*pGame		= CManager::GetGame();				// �Q�[�����̎擾
	CTutorial		*pTutorial	= CManager::GetTutorial();			// �`���[�g���A�����̎擾
	CInputKeyBoard	*pKey		= CManager::GetInputKeyboard();		// ���͏��̎擾(�L�[�{�[�h)
	CInputMouse		*pMouse		= CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)

	if (pGame != NULL)
	{// �|�[�Y�̎擾
		pPause = pGame->GetPause();
	}
	else
	{
		pPause = false;
	}

	if (pTutorial != NULL)
	{// �x���̎擾
		pWarning = pTutorial->GetWarning();
	}
	else
	{
		pWarning = false;
	}

	if (pPause == false && pWarning == false)
	{// �|�[�Y��x�����o�ĂȂ��Ƃ�
		switch (m_nType)
		{// �J�����̃^�C�v�ʏ���
		case CAMTYPE_TITLE:
			TitleMove();
			break;
		case CAMTYPE_RESULT:
			ResultMove();
			break;
		case CAMTYPE_FPS:
			FPSMove();
			break;
		case CAMTYPE_TPS:
			TPSMove();
			break;
		}
#ifdef _DEBUG	// �f�o�b�N����
		if (pKey != NULL)
		{// ���͏���
			CManager::MODE m_mode = CManager::GetMode();	// �Q�[�����[�h�̎擾
			if (m_mode == CManager::MODE_GAME || m_mode == CManager::MODE_TUTORIAL)
			{// �Q�[�����[�h���Q�[���A�`���[�g���A���̂Ƃ�
				if (pKey->GetKeyboardTrigger(DIK_1) == true)
				{	//���_�؂�ւ�
					m_bCamSwitch = m_bCamSwitch ? false : true;
					if (m_bCamSwitch == false)
					{
						m_nType = CAMTYPE_FPS;		// ��l��
					}
					else if (m_bCamSwitch == true)
					{
						m_nType = CAMTYPE_TPS;		// �O�l��
					}
				}
			}
		}
#endif
	}
	else
	{
		m_posMouse = *pMouse->GetMousePos();	// �}�E�X�̈ʒu�̎擾
		m_posMouseOld = m_posMouse;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::Set(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),								//��p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			//��ʔ䗦(�A�X�y�N�g��)
		10.0f,
		20000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// �J�����^�C�v�̐ݒ菈��
//=============================================================================
CCamera::CAMTYPE CCamera::SetType(CAMTYPE type)
{
	CManager::GAMERESULT gameresult = CManager::GetResult();	// ���U���g�̎擾
	D3DXVECTOR3 posPlayer			= CPlayer::GetPos();		// �v���C���[���W�̎擾

	switch (type)
	{// �J�����̃^�C�v�ʏ���
	case CAMTYPE_TITLE:
		m_posV = D3DXVECTOR3(1000.0f, 600.0f, -1000.0f);		// ���_
		m_posR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);				// �����_
		break;
	case CAMTYPE_RESULT:
		switch (gameresult)
		{// ���U���g�̃^�C�v�ʏ���
		case CManager::GAMERESULT_WIN:
			m_posV = D3DXVECTOR3(1000.0f, 600.0f, -1000.0f);	// ���_
			m_posR = D3DXVECTOR3(0.0f, 300.0f, 0.0f);			// �����_
			break;
		case CManager::GAMERESULT_LOSE:
			m_posV = D3DXVECTOR3(0.0f, 200.0f, -20.0f);			// ���_
			m_posR = D3DXVECTOR3(0.0f, -700.0f, 700.0f);		// �����_
			break;
		}
		break;
	case CAMTYPE_FPS:
		m_posV = posPlayer;										// ���_
		m_posR = D3DXVECTOR3(15.0f, 1000.0f, -350.0f);			// �����_
		break;
	case CAMTYPE_TPS:
		m_posV = D3DXVECTOR3(100.0f, 400.0f, -550.1f);			// ���_
		m_posR = D3DXVECTOR3(100.0f, 0.0f, -150.0f);			// �����_
		break;
	}

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �����̏�����

	return (m_nType = type);	// �^�C�v�̍X�V
}

//=============================================================================
// �^�C�g���̏���
//=============================================================================
void CCamera::TitleMove(void)
{
	m_fAngle += CIRCLEMOVE;		// �p�x�̉��Z

	if (m_fAngle > D3DX_PI)
	{// �p�x��3.14�ȏ�̂Ƃ�
		m_fAngle = -D3DX_PI;
	}

	m_posV = D3DXVECTOR3(m_posR.x + sinf(m_fAngle) * m_fLength, m_posV.y, m_posR.z + cosf(m_fAngle) * m_fLength);

	//=============================
	//   �f�o�b�N���O
	//=============================
	CManager::m_pDebuglog->Print(1, "�J����V ; %0.1f,%0.1f,%0.1f\n", m_posV.x, m_posV.y, m_posV.z);
	CManager::m_pDebuglog->Print(1, "�J����R ; %0.1f,%0.1f,%0.1f\n", m_posR.x, m_posR.y, m_posR.z);
	CManager::m_pDebuglog->Print(1, "ROT ; %0.1f,%0.1f,%0.1f\n", m_rot.x, m_rot.y, m_rot.z);
}

//=============================================================================
// ���U���g�̏���
//=============================================================================
void CCamera::ResultMove(void)
{
}

//=============================================================================
// ��l�̂̏���
//=============================================================================
void CCamera::FPSMove(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;						// �v�Z�p�}�g���b�N�X
	CInputMouse	*pMouse		= CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)
	D3DXVECTOR3 posPlayer	= CPlayer::GetPos();				// �v���C���[�̈ʒu�̎擾
	D3DXVECTOR3 &PlayerRot	= CManager::GetPlayer()->GetRot();	// �v���C���[�����̎擾

	if (pMouse != NULL)
	{
		m_posMouse	= *pMouse->GetMousePos();	// �}�E�X�̈ʒu�̎擾
		m_posV		= posPlayer;				// ���_�̍X�V

		//=============================
		//   �}�E�X�ŃJ��������
		//=============================
		//�����_�ʒu�ύX
		if (m_posMouseOld != m_posMouse)
		{
			m_posMouseAngle = m_posMouse - m_posMouseOld;		// �����̌v�Z

			if (m_posMouseAngle.y < 0)
			{//Y���W(��ړ�)
				if (m_rot.x > -1.5f)
				{
					if (m_posMouseAngle.y <= -15.0f)
					{
						m_posMouseAngle.y = -15.0f;
					}
					m_rot.x += (m_posMouseAngle.y * 0.002f);
				}
			}
			if (m_posMouseAngle.y > 0)
			{//Y���W(���ړ�)
				if (m_rot.x < 1.5f)
				{
					if (m_posMouseAngle.y >= 15.0f)
					{
						m_posMouseAngle.y = 15.0f;
					}
					m_rot.x += (m_posMouseAngle.y * 0.002f);
				}
			}
			if (m_posMouseAngle.x < 0)
			{//X���W(���ړ�)
				m_rot.y += (m_posMouseAngle.x * 0.002f);
			}
			if (m_posMouseAngle.x > 0)
			{//X���W(�E�ړ�)
				m_rot.y += (m_posMouseAngle.x * 0.002f);
			}
			m_posMouseOld = m_posMouse;		// �ߋ��ʒu�̍X�V
		}

		// �J�����̒ǔ�************************************************************
		D3DXMatrixIdentity(&mtxWorld);		// ���[���h�}�g���b�N�X�̏�����
		// ��]
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		// �ʒu
		D3DXMatrixTranslation(&mtxTrans, m_posV.x, m_posV.y, m_posV.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
		// �ʒu
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 100.0f);
		D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxWorld); // mtxWorld�̊p�x�𗘗p���Đ���

		m_posR.x = mtxTrans._41;
		m_posR.y = mtxTrans._42;
		m_posR.z = mtxTrans._43;
		// ************************************************************************
		// �}�E�X���[�v************************************************************
		if (m_posMouse.x <= 0.0f)
		{
			SetCursorPos(1279, (int)m_posMouse.y);
			m_posMouse = D3DXVECTOR3(1279.0f, m_posMouse.y, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		else if (m_posMouse.x >= 1279.0f)
		{
			SetCursorPos(0, (int)m_posMouse.y);
			m_posMouse = D3DXVECTOR3(0.0f, m_posMouse.y, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		if (m_posMouse.y <= 0.0f)
		{
			SetCursorPos((int)m_posMouse.x, 719);
			m_posMouse = D3DXVECTOR3(m_posMouse.x, 719.0f, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		else if (m_posMouse.y >= 719.0f)
		{
			SetCursorPos((int)m_posMouse.x, 0);
			m_posMouse = D3DXVECTOR3(m_posMouse.x, 0.0f, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		// ************************************************************************

		PlayerRot = m_rot;		// �v���C���[�ǔ�
	}
	//=============================
	//   �f�o�b�N���O
	//=============================
	CManager::m_pDebuglog->Print(1, "�J����V ; %0.1f,%0.1f,%0.1f\n", m_posV.x, m_posV.y, m_posV.z);
	CManager::m_pDebuglog->Print(1, "�J����R ; %0.1f,%0.1f,%0.1f\n", m_posR.x, m_posR.y, m_posR.z);
	CManager::m_pDebuglog->Print(1, "�}�E�X���� ; %0.1f,%0.1f,%0.1f\n", m_posMouseAngle.x, m_posMouseAngle.y, m_posMouseAngle.z);
	CManager::m_pDebuglog->Print(1, "ROT ; %0.1f,%0.1f,%0.1f\n", m_rot.x, m_rot.y, m_rot.z);
	CManager::m_pDebuglog->Print(1, "�`PlayerRot.x %.1f  PlayerRot.y %.1f  PlayerRot.z %.1f�`\n", PlayerRot.x, PlayerRot.y, PlayerRot.z);
}
//=============================================================================
// �O�l�̂̏���
//=============================================================================
void CCamera::TPSMove(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;						// �v�Z�p�}�g���b�N�X
	CInputMouse	*pMouse		= CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)
	D3DXVECTOR3 posPlayer	= CPlayer::GetPos();				// �v���C���[�̈ʒu�̎擾
	D3DXVECTOR3 &PlayerRot	= CManager::GetPlayer()->GetRot();	// �v���C���[�����̎擾

	if (pMouse != NULL)
	{
		m_posMouse	= *pMouse->GetMousePos();	// �}�E�X�̈ʒu�̎擾
		m_posR		= posPlayer;				// ���_�̍X�V

		//=============================
		//   �}�E�X�ŃJ��������
		//=============================
		//�����_�ʒu�ύX
		if (m_posMouseOld != m_posMouse)
		{
			m_posMouseAngle = m_posMouse - m_posMouseOld;		// �����̌v�Z

			if (m_posMouseAngle.y < 0)
			{//Y���W(��ړ�)
				if (m_rot.x > -1.5f)
				{
					if (m_posMouseAngle.y <= -15.0f)
					{
						m_posMouseAngle.y = -15.0f;
					}
					m_rot.x += (m_posMouseAngle.y * 0.005f);
				}
			}
			if (m_posMouseAngle.y > 0)
			{//Y���W(���ړ�)
				if (m_rot.x < 1.5f)
				{
					if (m_posMouseAngle.y >= 15.0f)
					{
						m_posMouseAngle.y = 15.0f;
					}
					m_rot.x += (m_posMouseAngle.y * 0.005f);
				}
			}
			if (m_posMouseAngle.x < 0)
			{//X���W(���ړ�)
				m_rot.y += (m_posMouseAngle.x * 0.005f);
			}
			if (m_posMouseAngle.x > 0)
			{//X���W(�E�ړ�)
				m_rot.y += (m_posMouseAngle.x * 0.005f);
			}
			m_posMouseOld = m_posMouse;		// �ߋ��ʒu�̍X�V
		}

		// �J�����̒ǔ�************************************************************
		D3DXMatrixIdentity(&mtxWorld);		// ���[���h�}�g���b�N�X�̏�����
		// ��]
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, (m_rot.x * -1.0f), m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		// �ʒu
		D3DXMatrixTranslation(&mtxTrans, m_posR.x, m_posR.y, m_posR.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
		// �ʒu
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 800.0f, 10000.0f);
		D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxWorld); // mtxWorld�̊p�x�𗘗p���Đ���

		m_posV.x = mtxTrans._41;
		m_posV.y = mtxTrans._42;
		m_posV.z = mtxTrans._43;
		// ************************************************************************
		// �}�E�X���[�v************************************************************
		if (m_posMouse.x <= 0.0f)
		{
			SetCursorPos(1279, (int)m_posMouse.y);
			m_posMouse = D3DXVECTOR3(1279.0f, m_posMouse.y, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		else if (m_posMouse.x >= 1279.0f)
		{
			SetCursorPos(0, (int)m_posMouse.y);
			m_posMouse = D3DXVECTOR3(0.0f, m_posMouse.y, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		if (m_posMouse.y <= 0.0f)
		{
			SetCursorPos((int)m_posMouse.x, 719);
			m_posMouse = D3DXVECTOR3(m_posMouse.x, 719.0f, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		else if (m_posMouse.y >= 719.0f)
		{
			SetCursorPos((int)m_posMouse.x, 0);
			m_posMouse = D3DXVECTOR3(m_posMouse.x, 0.0f, 0.0f);
			m_posMouseOld = m_posMouse;
		}
		// ************************************************************************

		PlayerRot	= m_rot;		// �v���C���[�ǔ�
		PlayerRot.y += D3DX_PI;
	}
	//=============================
	//   �f�o�b�N���O
	//=============================
	CManager::m_pDebuglog->Print(1, "�J����V ; %0.1f,%0.1f,%0.1f\n", m_posV.x, m_posV.y, m_posV.z);
	CManager::m_pDebuglog->Print(1, "�J����R ; %0.1f,%0.1f,%0.1f\n", m_posR.x, m_posR.y, m_posR.z);
	CManager::m_pDebuglog->Print(1, "�}�E�X���� ; %0.1f,%0.1f,%0.1f\n", m_posMouseAngle.x, m_posMouseAngle.y, m_posMouseAngle.z);
	CManager::m_pDebuglog->Print(1, "ROT ; %0.1f,%0.1f,%0.1f\n", m_rot.x, m_rot.y, m_rot.z);
	CManager::m_pDebuglog->Print(1, "�`PlayerRot.x %.1f  PlayerRot.y %.1f  PlayerRot.z %.1f�`\n", PlayerRot.x, PlayerRot.y, PlayerRot.z);
}