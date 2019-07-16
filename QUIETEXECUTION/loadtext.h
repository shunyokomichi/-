//=======================================================//
//
// テキストの読み込み処理 [loadtext.h]
// Author :
//
//=======================================================//
#ifndef _LOADTEXT_H_
#define _LOADTEXT_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"		// メイン
#include "manager.h"	// マネージャー

//*******************************************************//
// マクロ定義
//*******************************************************//
#define OBJ_MAX			(1800)
#define TEXT_OBJ_MAX	(3)		//	テキストの中のオブジェクトの最大数
#define MAX_TEXT		(2)		// Textの最大数

//*******************************************************//
// 前方宣言
//*******************************************************//

//*******************************************************//
// クラス定義
//*******************************************************//
class CLoadText
{
public:
	typedef struct
	{	// TEXTの情報(※追加後は記入すること記入すること※)
		int nNum[MAX_TEXT];					// 配置数
		int nTypeNum[MAX_TEXT];				// モデルの種類
		D3DXVECTOR3 pos[MAX_TEXT][OBJ_MAX];	// 位置情報
		D3DXVECTOR3 rot[MAX_TEXT][OBJ_MAX];	// 回転情報
		D3DXVECTOR3 size[OBJ_MAX];
		D3DXVECTOR3 MeshNum[OBJ_MAX];
		int nType[MAX_TEXT][OBJ_MAX];			// タイプ
		char ModeName[MAX_TEXT][20][256];
	}MAPTEXT;

	CLoadText();
	~CLoadText();

	// テキストファイル読み込み
	void TextRoad(char *SetFailName);						// 全体の読み込み
	char *ReadLine(FILE *pFile, char*pDst, int nPatten);	// 一行の読み込み
	char *SearchLine(char*pDst);							// 改行などの読み込み

	static MAPTEXT GetMapText(void) { return m_text; }		// テキスト構造体取得
	static char *GetName(int nNum, int nTextNum) { return &m_text.ModeName[nTextNum][nNum][0]; }		// モデル名の取得
	static void Reset(void);

private:
	FILE *pFile;
	char *pStrCur;
	char aLine[256];
	char aStr[256];
	int m_objNum = 0;
	static MAPTEXT m_text;

};

#endif // !_LOADTEXT_H_


