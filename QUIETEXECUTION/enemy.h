//=============================================================================
//
// �G�̏��� [enemy.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "sceneX.h"
#include "model.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_FILE				"data\\TEXT\\Motion_enemy.txt"	// �ǂݍ���txt�t�@�C���̖��O
#define MAX_ENEMYANIM			(8)								// �A�j���[�V������
#define MAX_ENEMYKEY			(8)								// �L�[�t���[����
#define MAX_ENEMYPARTS			(16)							// �p�[�c�̍ő吔
#define MAX_ENEMYMOTION			(10)							// ���[�V�����̍ő吔
#define ENEMY_HEIGHT			(500.0f)						// �G�̍���
#define ENEMY_EYE				(350.0f)						// �G�̖ڐ�
#define ENEMY_LIFE				(100)							// �̗͂̍ő�l
#define ENEMY_SPEED				(2.0f)							// �ړ����x
#define ENEMY_VIGILANCESPEED	(0.004f)						// �ړ����x
#define ENEMY_PURSUITSPEED		(0.02f)							// �ړ����x
#define ENEMY_RADIUS			(40.0f)							// �����蔻��͈̔�(���a)
#define ENEMY_CIRCLEMOVE		(0.5f)							// �J�����̈ړ���
#define ENEMY_SEARCH_NUM		(10)							// �{���e�̍ő吔
#define ENEMY_SEARCH_INTERVAL	(0.07f)							// �{���e�̊Ԋu
#define MAX_ENEMYCOLLBILL		(19)							// �r���{�[�h�̍ő吔
#define ENEMY_SEARCH_PURSUIT	(1000.0f)						// �ǐՂ͈̔�
#define ENEMY_SEARCH_VIGILANCE	(3000.0f)						// �x���͈̔�
#define ENEMY_ATTACK			(500.0f)						// �U���͈̔�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBillBoord;
class CBullet;

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CEnemy : public CScene  // ���h���N���X
{
public:
	// �L�[�v�f
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;

	// �L�[���
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_ENEMYPARTS];
	}KEY_INFO;

	// ���[�V�������
	typedef struct
	{
		bool		bLoop;
		int			nNumKey;
		KEY_INFO	aKayInfo[MAX_ENEMYMOTION];
	}MOTION_INFO;

	// ���[�V�����^�C�v���
	typedef enum
	{
		ENEMYANIM_NEUTRAL = 0,	// �j���[�g�������[�V����(�ʏ�)
		ENEMYANIM_OVERLOOKING,	// ���n��
		ENEMYANIM_WALK,			// ����(�ʏ�&�x��)
		ENEMYANIM_RUN,			// ����(�ǐ�)
		ENEMYANIM_ATTACK,		// �U��
		ENEMYANIM_DAMAGE,		// �_���[�W
		ENEMYANIM_DEDA,			// ���S
		ENEMYANIM_MAX			// ���[�V�����̍ő吔
	}EnemyAnim;

	// �X�e�[�^�X���
	typedef enum
	{
		ENEMYSTATSE_NONE = 0,	// �ʏ�
		ENEMYSTATSE_VIGILANCE,	// �x��
		ENEMYSTATSE_PURSUIT,	// �ǐ�
		ENEMYSTATSE_DAMAGE,		// �_���[�W
		ENEMYSTATSE_DEDA,		// ���S
		ENEMYSTATSE_MAX			// �X�e�[�^�X�̍ő吔
	}EnemyStatse;

	CEnemy	(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_ENEMY);
	~CEnemy	();
	static CEnemy	*Create	(D3DXVECTOR3 pos);
	static HRESULT	Load	(void);
	static void		Unload	(void);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);
	void	SetCol	(D3DXCOLOR col)
	{
		for (int nCntModel = 0; nCntModel < MAX_ENEMYPARTS; nCntModel++)
		{
			m_pModel[nCntModel]->SetCol(col);
		}
	}

	EnemyStatse GetStatse(void) { return m_nStatse; }

	// ���ꂼ��̏�ԓ���
	void MoveNone		(void);		// �ʏ�
	void MoveVigilance	(void);		// �x��
	void MovePursuit	(void);		// �ǐ�
	void MoveDeda		(void);		// ���S
	void AttackMove		(void);		// �U��

	// ���[�V�����̍X�V�֐�
	void UpdateMotion(void);


	// �����蔻��
	void Collision				(D3DXVECTOR3 posPlayer);
	void CollisionPlayer		(void);																// �v���C���[�Ƃ̓����蔻��
	void CollisionDamege		(D3DXVECTOR3 posHand, CPlayer::PlayerAnim playerState);				// �_���[�W����
	void CollisionSearch		(void);																// �{���̔���
	bool CollisionDistance		(CBullet *pBullet);													// �G�ƃv���C���[�̋���
	void CollisitionWall		(void);																// �ǂ̓����蔻��
	void CollisitionObject3D	(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);		// �I�u�W�F�N�g�̓����蔻��

	// �t�@�C���ǂݍ��݊֐�
	void FileLoad		(void);						// �t�@�C���ǂݍ���
	char *ReadLine		(FILE *pFile, char *pDst);	// 1�s�ǂݍ���
	char *GetLineTop	(char *pStr);				// �s�̐擪���擾
	int  PopString		(char *pStr, char *pDest);	// �s�̍Ō��؂�̂�

private:
	static	LPD3DXMESH			m_pMesh		[MAX_ENEMYPARTS];
	static LPD3DXBUFFER			m_pBuffMat	[MAX_ENEMYPARTS];
	static DWORD				m_nNumMat	[MAX_ENEMYPARTS];
	static LPDIRECT3DTEXTURE9	*m_pTexture	[MAX_ENEMYPARTS];		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3					m_pos;								// ���݂̈ʒu
	D3DXVECTOR3					m_posOld;							// �ߋ��̈ʒu
	D3DXVECTOR3					m_rot;								// ����
	CModel						*m_pModel	[MAX_ENEMYPARTS];		// ���f���̃|�C���^
	static CBillBoord			*m_pBillBoord;						// �r���{�[�h�̃|�C���^
	static CBullet				*m_pBullet;							// �e�̃|�C���^
	D3DXMATRIX					m_mtxWorld;							// ���[���h�}�g���b�N�X
	EnemyAnim					m_nAnimnow;							// ���݂̃A�j���[�V����
	EnemyStatse					m_nStatse;							// ���݂̏��
	D3DXVECTOR3					m_move;								// �ړ�
	D3DXVECTOR3					m_vtxMin;							// ���f���̍ŏ��l
	D3DXVECTOR3					m_vtxMax;							// ���f���̍ő�l
	bool						m_bAttack;							// �U�����
	int							m_nAttackDelay;						// �U���̍d������
	bool						m_bDamage;							// �_���[�W���
	int							m_nDamageDelay;						// �_���[�W�̍d������
	int							m_nLife;							// ���C�t
	int							m_nCntSearch;						// �{���J�E���^�[
	int							m_nCntStatse;						// ��ԃJ�E���^�[
	float						m_fAngle;							// �J�����̊p�x
	int							m_fWalkFrame;						// �N�����J�E���^�[
	bool						m_bMove;							// �ړ����
	int							m_fMoveFlame;						// �ړ�����
	D3DXVECTOR3					m_VigilancePos;						// �x�����̖ړI�n
	D3DXVECTOR3					m_PosDest;							// �ړI�̍��W
	D3DXVECTOR3					m_posAngle;							// ���W�̍���
	int							m_nCntAttack;						// �U���̃J�E���^�[

	// ���[�V�����֐�	�V�K
	KEY_INFO			*m_pKeyInfo[MAX_ENEMYMOTION];		// �L�[���ւ̃|�C���^
	int					m_nKey;								// ���݂̃L�[�i���o�[
	int					m_nCountFlame;						// �t���[����
	int					m_nNumParts;						// �p�[�c��
	int					m_aIndexParent[MAX_ENEMYPARTS];		// �e�̃C���f�b�N�X
	KEY					m_aKayOffset[MAX_ENEMYPARTS];		// �I�t�Z�b�g���
	MOTION_INFO			m_aMotionInfo[MAX_ENEMYMOTION];		// ���[�V�������
	int					m_nMotionType;						// ���[�V�����̃^�C�v(int�^)
	bool				m_bMotionEnd;						// ���[�V�����G���h
	D3DXVECTOR3			m_OffSetPos[MAX_ENEMYPARTS];		// �I�t�Z�b�g
};
#endif