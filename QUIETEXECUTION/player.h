//=============================================================================
//
// �v���C���[���� [player.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "sceneX.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_FILE				"data\\TEXT\\Motion_player.txt"	// �ǂݍ���txt�t�@�C���̖��O
#define MAX_PLAYERANIM			(8)								// �A�j���[�V������
#define MAX_PLAYERKEY			(8)								// �L�[�t���[����

//�V�K
#define MAX_PLAYERPARTS			(36)		// �p�[�c�̍ő吔
#define MAX_PLAYERMOTION		(10)		// ���[�V�����̍ő吔

#define SQUAT_SPEED				(7.0f)		// ���Ⴊ�ݑ��x
#define WALK_SPEED				(25.0f)		// �ړ����x
#define RUN_SPEED				(50.0f)		// �_�b�V����Ԃ̑��x
#define JUMP_POWER				(1000.0f)	// �W�����v��
#define PLAYER_UP_HEIGHT		(100.0f)	// �v���C���[�̍���(��)
#define PLAYER_DOWN_HEIGHT		(340.0f)	// �v���C���[�̍���(��)
#define PLAYER_SQUAT_HEIGHT		(200.0f)	// �v���C���[�̍���(���Ⴊ��)
#define PLAYER_RADIUS			(100.0f)	// �����蔻��͈̔�(���a)
#define PLAYER_LIFE				(30)		// �̗�	�̍ő�l
#define PLAYER_MP				(100)		// MP	�̍ő�l
#define GRAVITY_REAL			(-9.8f)		// �d��(-9.8m/s^2)

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CPlayer : public CScene  // ���h���N���X
{
public:
	//�L�[�v�f
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;

	//�L�[���
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_PLAYERPARTS];
	}KEY_INFO;

	//���[�V�������
	typedef struct
	{
		bool		bLoop;
		int			nNumKey;
		KEY_INFO	aKayInfo[MAX_PLAYERMOTION];
	}MOTION_INFO;

	typedef enum
	{
		PLAYERANIM_NEUTRAL = 0,		// �j���[�g�������[�V����
		PLAYERANIM_MOVE,			// �ړ�
		PLAYERANIM_ATTACK,			// �U��
		PLAYERANIM_CATCH,			// �L���b�`
		PLAYERANIM_RESULT,			// ���U���g
		PLAYERANIM_MAX				// ���[�V�����̍ő吔
	}PlayerAnim;

	typedef enum
	{
		PLAYERSTAGESTATUS_NONE = 0,	// �ʏ�
		PLAYERSTAGESTATUS_DAMAGE,	// �_���[�W
		PLAYERSTAGESTATUS_DEAD,		// ���S
		PLAYERSTAGESTATUS_MAX		// ��Ԃ̍ő吔
	}PlayerStageStatus;

	CPlayer(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_PLAYER);
	~CPlayer();
	static HRESULT	Load	(void);
	static void		Unload	(void);
	static CPlayer *Create	(D3DXVECTOR3 pos);

	HRESULT Init			(void);
	void	Uninit			(void);
	void	Update			(void);
	void	Draw			(void);
	void	Set				(D3DXVECTOR3 pos) { GetPos() = pos; }
	void	PlayerMove		(void);
	void	InvincibleMove	(void);

	void CollisitionWall(void);

	static D3DXVECTOR3			&GetPos		(void) { return m_pos; }
	static D3DXVECTOR3			&GetPosOld	(void) { return m_posOld; }
	static D3DXVECTOR3			&GetRot		(void) { return m_rot; }
	static bool					&GetJump	(void) { return m_bJump; }
	static int					&GetLife	(void) { return m_nLife; }
	static int					&GetMp		(void) { return m_nMp; }
	static PlayerStageStatus	&GetStatus	(void) { return m_Status; }

	static void SetStatus	(PlayerStageStatus Status)	{ m_Status = Status; }
	static void SetLife		(int Life)					{ m_nLife = Life; }

	//���[�V�����̍X�V�֐�
	void UpdateMotion(void);

	// �����蔻��
	void Collision			(D3DXVECTOR3 posEnemy);
	void CollisionAttack	(D3DXVECTOR3 posEnemy, D3DXMATRIX mtxWorld, float Radius, int NumModel);
	void CollisitionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, bool bTouchOld);

	//�t�@�C���ǂݍ��݊֐�
	void FileLoad	(void);						// �t�@�C���ǂݍ���
	char *ReadLine	(FILE *pFile, char *pDst);	// 1�s�ǂݍ���
	char *GetLineTop(char *pStr);				// �s�̐擪���擾
	int  PopString	(char *pStr, char *pDest);	// �s�̍Ō��؂�̂�

private:
	static	LPD3DXMESH			m_pMesh		[MAX_PLAYERPARTS];
	static LPD3DXBUFFER			m_pBuffMat	[MAX_PLAYERPARTS];
	static DWORD				m_nNumMat	[MAX_PLAYERPARTS];
	static LPDIRECT3DTEXTURE9	*m_pTexture	[MAX_PLAYERPARTS];		// �e�N�X�`���ւ̃|�C���^
	static D3DXVECTOR3			m_pos;								// ���݂̈ʒu
	static D3DXVECTOR3			m_posOld;							// �ߋ��̈ʒu
	static D3DXVECTOR3			m_rot;								// ����
	CModel						*m_pModel	[MAX_PLAYERPARTS];		// ���f���̃|�C���^
	D3DXMATRIX					m_mtxWorld;							// ���[���h�}�g���b�N�X
	static PlayerStageStatus	m_Status;							// �v���C���[�̏��
	PlayerAnim					m_nAnimnow;							// ���݂̃A�j���[�V����
	D3DXVECTOR3					m_rotDest;							//
	D3DXVECTOR3					m_move;								// �ړ�
	D3DXVECTOR3					m_vtxMin;							// ���f���̍ŏ��l
	D3DXVECTOR3					m_vtxMax;							// ���f���̍ő�l
	D3DXVECTOR3					m_fAngle;							//
	static bool					m_bJump;							// �W�����v���
	static bool					m_bLand;							// ���n���
	bool						m_bInvincible;						// �f�o�b�O�p
	bool						m_bAttack;							// �U�����
	bool						m_bCatch;							// �A�C�e��������
	bool						m_bRun;								// �_�b�V�����
	bool						m_bTouchOld;						// �G�ꂽ���ǂ���(�ߋ�)
	int							m_nAttackDelay;						// �U���̍d������
	static int					m_nLife;							// ���C�t
	static int					m_nMp;								// MP
	float						m_fGravityFrame;					// �W�����v�̌o�ߎ���
	bool						m_bUse;								// �����Ԃ��̃t���O

	// ���[�V�����֐�	�V�K
	KEY_INFO			*m_pKeyInfo		[MAX_PLAYERMOTION];			//�L�[���ւ̃|�C���^
	int					m_nKey;										//���݂̃L�[�i���o�[
	int					m_nCountFlame;								//�t���[����
	int					m_nNumParts;								//�p�[�c��
	int					m_aIndexParent	[MAX_PLAYERPARTS];			//�e�̃C���f�b�N�X
	KEY					m_aKayOffset	[MAX_PLAYERPARTS];			//�I�t�Z�b�g���
	MOTION_INFO			m_aMotionInfo	[MAX_PLAYERMOTION];			//���[�V�������
	int					m_nMotionType;								//���[�V�����̃^�C�v(int�^)
	bool				m_bMotionEnd;
	D3DXVECTOR3			m_OffSetPos		[MAX_PLAYERPARTS];
};
#endif