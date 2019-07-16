//=============================================================================
//
// �T�E���h���� [sound.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// �N���X
//*************************************
class CSound
{
public:
	//*************************************
	// �T�E���h�t�@�C��
	//*************************************
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,			// �^�C�g��BGM
		SOUND_LABEL_BGM_TUTORIAL,			// �`���[�g���A��BGM
		SOUND_LABEL_BGM_GAME,				// �Q�[��BGM
		SOUND_LABEL_BGM_RESULT,				// ���U���gBGM
		SOUND_LABEL_BGM_WARNING,			// �댯�xBGM
		SOUND_LABEL_SE_PAUSE_OPENCLOSE,		// �J��(�|�[�Y)
		SOUND_LABEL_SE_PAUSE_DECISION,		// ����(�|�[�Y)
		SOUND_LABEL_SE_PAUSE_SWITCH,		// �؂�ւ�(�|�[�Y)
		SOUND_LABEL_SE_PLAYER_WALK,			// ����(�v���C��)
		SOUND_LABEL_SE_PLAYER_RUN,			// ����(�v���C��)
		SOUND_LABEL_SE_PLAYER_LANDING,		// ���n(�v���C��)
		SOUND_LABEL_SE_PLAYER_ATTAK,		// �U��(�v���C��)
		SOUND_LABEL_SE_PLAYER_DAMEG,		// �_���[�W(�v���C��)
		SOUND_LABEL_SE_ENEMY_DAMEG,			// �_���[�W(�G�l�~)
		SOUND_LABEL_SE_ENEMY_DEAD,			// ���S(�G�l�~)
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Init		(HWND hWnd);
	void	Uninit		(void);
	HRESULT PlaySound	(SOUND_LABEL label);
	void	StopSound	(SOUND_LABEL label);
	void	AllStopSound(void);

private:
	typedef struct
	{
		char					*m_pFilename;		// �t�@�C����
		int						m_nCntLoop;			// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	}SOUNDPARAM;

	IXAudio2					*m_pXAudio2;							// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice		*m_pMasteringVoice;						// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice			*m_apSourceVoice[SOUND_LABEL_MAX];		// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE						*m_apDataAudio	[SOUND_LABEL_MAX];		// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD						m_aSizeAudio	[SOUND_LABEL_MAX];		// �I�[�f�B�I�f�[�^�T�C�Y
	SOUNDPARAM					m_aSoundParam	[SOUND_LABEL_MAX] =		// �e���f�ނ̃p�����[�^
	{
		{ "data\\BGM\\TITLE_BGM.wav"				, -1 },		// �^�C�g��BGM
		{ "data\\BGM\\TUTORIAL_BGM.wav"				, -1 },		// �`���[�g���A��BGM
		{ "data\\BGM\\GAME_BGM.wav"					, -1 },		// �Q�[��BGM
		{ "data\\BGM\\RESULT_BGM.wav"				, -1 },		// ���U���gBGM
		{ "data\\BGM\\WARNING_BGM.wav"				, -1 },		// �댯�xBGM
		{ "data\\SE\\PAUSE\\PAUSE_OPEN&CLOSE_SE.wav",  0 },		// �J��(�|�[�Y)
		{ "data\\SE\\PAUSE\\PAUSE_DECISION_SE.wav"	,  0 },		// ����(�|�[�Y)
		{ "data\\SE\\PAUSE\\PAUSE_SWITCH_SE.wav"	,  0 },		// �؂�ւ�(�|�[�Y)
		{ "data\\SE\\PLAYER\\PLAYER_WALK_SE.wav"	,  0 },		// ����(�v���C��)
		{ "data\\SE\\PLAYER\\PLAYER_RUN_BGM.wav"	, -1 },		// ����(�v���C��)
		{ "data\\SE\\PLAYER\\PLAYER_LANDING_SE.wav"	,  0 },		// ���n(�v���C��)
		{ "data\\SE\\PLAYER\\PLAYER_ATTAK_SE.wav"	,  0 },		// �U��(�v���C��)
		{ "data\\SE\\PLAYER\\PLAYER_DAMEG_SE.wav"	,  0 },		// �_���[�W(�v���C��)
		{ "data\\SE\\ENEMY\\ENEMY_DAMEG_SE.wav"		,  0 },		// �_���[�W(�G�l�~)
		{ "data\\SE\\ENEMY\\ENEMY_DEAD_SE.wav"		,  0 },		// ���S(�G�l�~)
	};
};



#endif
