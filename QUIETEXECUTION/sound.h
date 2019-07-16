//=============================================================================
//
// サウンド処理 [sound.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// クラス
//*************************************
class CSound
{
public:
	//*************************************
	// サウンドファイル
	//*************************************
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,			// タイトルBGM
		SOUND_LABEL_BGM_TUTORIAL,			// チュートリアルBGM
		SOUND_LABEL_BGM_GAME,				// ゲームBGM
		SOUND_LABEL_BGM_RESULT,				// リザルトBGM
		SOUND_LABEL_BGM_WARNING,			// 危険度BGM
		SOUND_LABEL_SE_PAUSE_OPENCLOSE,		// 開閉(ポーズ)
		SOUND_LABEL_SE_PAUSE_DECISION,		// 決定(ポーズ)
		SOUND_LABEL_SE_PAUSE_SWITCH,		// 切り替え(ポーズ)
		SOUND_LABEL_SE_PLAYER_WALK,			// 歩き(プレイヤ)
		SOUND_LABEL_SE_PLAYER_RUN,			// 走り(プレイヤ)
		SOUND_LABEL_SE_PLAYER_LANDING,		// 着地(プレイヤ)
		SOUND_LABEL_SE_PLAYER_ATTAK,		// 攻撃(プレイヤ)
		SOUND_LABEL_SE_PLAYER_DAMEG,		// ダメージ(プレイヤ)
		SOUND_LABEL_SE_ENEMY_DAMEG,			// ダメージ(エネミ)
		SOUND_LABEL_SE_ENEMY_DEAD,			// 死亡(エネミ)
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
		char					*m_pFilename;		// ファイル名
		int						m_nCntLoop;			// ループカウント(-1でループ再生)
	}SOUNDPARAM;

	IXAudio2					*m_pXAudio2;							// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice		*m_pMasteringVoice;						// マスターボイスへのポインタ
	IXAudio2SourceVoice			*m_apSourceVoice[SOUND_LABEL_MAX];		// ソースボイスへのポインタ
	BYTE						*m_apDataAudio	[SOUND_LABEL_MAX];		// オーディオデータへのポインタ
	DWORD						m_aSizeAudio	[SOUND_LABEL_MAX];		// オーディオデータサイズ
	SOUNDPARAM					m_aSoundParam	[SOUND_LABEL_MAX] =		// 各音素材のパラメータ
	{
		{ "data\\BGM\\TITLE_BGM.wav"				, -1 },		// タイトルBGM
		{ "data\\BGM\\TUTORIAL_BGM.wav"				, -1 },		// チュートリアルBGM
		{ "data\\BGM\\GAME_BGM.wav"					, -1 },		// ゲームBGM
		{ "data\\BGM\\RESULT_BGM.wav"				, -1 },		// リザルトBGM
		{ "data\\BGM\\WARNING_BGM.wav"				, -1 },		// 危険度BGM
		{ "data\\SE\\PAUSE\\PAUSE_OPEN&CLOSE_SE.wav",  0 },		// 開閉(ポーズ)
		{ "data\\SE\\PAUSE\\PAUSE_DECISION_SE.wav"	,  0 },		// 決定(ポーズ)
		{ "data\\SE\\PAUSE\\PAUSE_SWITCH_SE.wav"	,  0 },		// 切り替え(ポーズ)
		{ "data\\SE\\PLAYER\\PLAYER_WALK_SE.wav"	,  0 },		// 歩き(プレイヤ)
		{ "data\\SE\\PLAYER\\PLAYER_RUN_BGM.wav"	, -1 },		// 走り(プレイヤ)
		{ "data\\SE\\PLAYER\\PLAYER_LANDING_SE.wav"	,  0 },		// 着地(プレイヤ)
		{ "data\\SE\\PLAYER\\PLAYER_ATTAK_SE.wav"	,  0 },		// 攻撃(プレイヤ)
		{ "data\\SE\\PLAYER\\PLAYER_DAMEG_SE.wav"	,  0 },		// ダメージ(プレイヤ)
		{ "data\\SE\\ENEMY\\ENEMY_DAMEG_SE.wav"		,  0 },		// ダメージ(エネミ)
		{ "data\\SE\\ENEMY\\ENEMY_DEAD_SE.wav"		,  0 },		// 死亡(エネミ)
	};
};



#endif
