//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : �r�J�R�N
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "model3D.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME_0	"data\\MODEL\\object\\barrel.x"			// �M
#define MODEL_NAME_1	"data\\MODEL\\object\\stone000.x"		// ��
#define MODEL_NAME_2	"data\\MODEL\\object\\crystal.x"		// �N���X�^��
#define MAX_OBJECT_TEXTURE			(3)				// �I�u�W�F�N�g�̃e�N�X�`����
#define MAX_OBJECT					(3)				// �I�u�W�F�N�g�̃��f����
#define OBJECT_PRIOTITY				(2)				// �I�u�W�F�N�g�̕`�揇
#define CRYSTAL_MOVE				(0.01f)			// �N���X�^���̉�]��
#define CRYSTAL_MOVETIME			(200.0f)		// �N���X�^���̉�]��
#define CLEAR_CRYSTALNUM			(18)			// �N���X�^���̖ڕW�l����
#define MODEL_SPEED					(5.0f)			// �ړ���
#define PLAYER_DEPTH				(10)			// �v���C���[�̕������p
#define ENEMY_DEPTH					(10)			// �G�l�~�[�̕������p

//=====================
//  CModel3D�̔h���N���X
//=====================
class CObject : public CModel3D
{
public:// �N�ł�������
	CObject();
	~CObject();
	static HRESULT	Load	(int nMaxModel);
	static void		UnLoad	(void);
	static CObject	*Create	(D3DXVECTOR3 pos, float move, int nTexTypeint,int nObjectType, MOVETYPE nMovetype, int nCollision);

	HRESULT Init					(void);
	void	Uninit					(void);
	void	Update					(void);
	void	Draw					(void);
	bool	CollisionObject			(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, bool bTouchOld);
	bool	CollisionObjectEnemy	(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
	void	CollisionBullet			(void);

private:// �l�ł̂ݎg��
	static LPD3DXMESH			m_pMeshModel	[MAX_OBJECT];				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMatModel	[MAX_OBJECT];				// �}�e���A���̏��ւ̃|�C���^
	static DWORD				m_nNumMatModel	[MAX_OBJECT];				// �}�e���A���̏��
	static LPDIRECT3DTEXTURE9	m_pMeshTextures	[MAX_OBJECT_TEXTURE];
	D3DXVECTOR3					m_pos;										// �ʒu
	D3DXVECTOR3					m_rot;										// ����
	D3DXVECTOR3					m_move;										// �ړ���
	int							m_nCount;									// �J�E���^�[
	int							m_nCollision;								// �����蔻���ONOFF
	int							m_nType;									// ���
	D3DXVECTOR3					m_ModelMove;								// �ړ��̎d��
	bool						m_bCrystalMove;								// �N���X�^���̈ړ��؂�ւ�
	float						m_fCrystalMoveFleam;						// �N���X�^���̈ړ�����
	static int					m_nCrystalCounter;							// �N���X�^���̊l����
};
#endif