//=============================================================================
//
// メイン処理 [main.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=============================================================================
// ヘッダファイルのインクルード
//=============================================================================
#define DIRECTINPUT_VERSION (0x0800)	//　ビルド時に警告対処用マクロ
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "d3dx9.h"						//　描画処理に必要
#include "dinput.h"						//　入力処理に必要
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
#include <crtdbg.h>						// メモリリークに必要
#define _CRTDBG_MAP_ALLOC

//=============================================================================
// ライブラリファイルのリンク
//=============================================================================
#pragma comment(lib,"d3d9.lib")			// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")		// DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")		// システム時刻取得
#pragma comment(lib,"dinput8.lib")		// 入力処理に必要
#pragma comment(lib, "xinput.lib")		// コントローラーの処理

//=============================================================================
// マクロ定義
//=============================================================================
#define SCREEN_WIDTH	(1280)			// ウィンドウの幅
#define SCREEN_HEIGHT	(720)			// ウィンドウの高さ

// 2Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 頂点カラー / テクスチャ座標 )
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// 3Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//*****************************************************************************
// 前方宣言
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
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float		rhw;	// 1.0fで固定
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャの座標
}VERTEX_2D;
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ
} VERTEX_3D;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
int	GetFPS(void);

#endif