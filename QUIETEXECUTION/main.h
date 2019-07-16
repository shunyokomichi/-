//=============================================================================
//
// ���C������ [main.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=============================================================================
// �w�b�_�t�@�C���̃C���N���[�h
//=============================================================================
#define DIRECTINPUT_VERSION (0x0800)	//�@�r���h���Ɍx���Ώ��p�}�N��
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "d3dx9.h"						//�@�`�揈���ɕK�v
#include "dinput.h"						//�@���͏����ɕK�v
#include "xaudio2.h"
#include "string.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <Xinput.h>
#include <stdarg.h>
#include <stdio.h>
#include <conio.h>
#include <crtdbg.h>						// ���������[�N�ɕK�v
#define _CRTDBG_MAP_ALLOC

//=============================================================================
// ���C�u�����t�@�C���̃����N
//=============================================================================
#pragma comment(lib,"d3d9.lib")			// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")		// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")		// �V�X�e�������擾
#pragma comment(lib,"dinput8.lib")		// ���͏����ɕK�v
#pragma comment(lib, "xinput.lib")		// �R���g���[���[�̏���

//=============================================================================
// �}�N����`
//=============================================================================
#define SCREEN_WIDTH	(1280)			// �E�B���h�E�̕�
#define SCREEN_HEIGHT	(720)			// �E�B���h�E�̍���

// 2�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / ���_�J���[ / �e�N�X�`�����W )
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// 3�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CFade;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CInput;
class CInputKeyBoard;
class CInputJoypad;
class CInputMouse;
class CScene;
class CSound;
class CCamera;
class CLight;
class CPlayer;
class CPause;
class CWarning;
class CDebuglog;

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float		rhw;	// 1.0f�ŌŒ�
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`���̍��W
}VERTEX_2D;
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`��
} VERTEX_3D;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
int	GetFPS(void);

#endif