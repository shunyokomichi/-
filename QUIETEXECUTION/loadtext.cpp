//=======================================================//
//
// テキスト読み込みの処理 [loadtext.cpp]
// Author :
//
//=======================================================//
#include "loadtext.h"
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー

//*******************************************************//
// プロトタイプ宣言
//*******************************************************//

//*******************************************************//
// グローバル変数
//*******************************************************//

//*******************************************************//
// 静的メンバ変数
//*******************************************************//
CLoadText::MAPTEXT CLoadText::m_text = {};	// テキストの構造体

//*******************************************************//
// コンストラクタ
//*******************************************************//
CLoadText::CLoadText()
{

}

//*******************************************************//
// デストラクタ
//*******************************************************//
CLoadText::~CLoadText()
{

}

//*******************************************************//
// テキストの読み込み
//*******************************************************//
void CLoadText::TextRoad(char *SetFailName)
{
	int nTypeNum = 0;		// 種類数の変数
	int nCntWord = 0;		// 文字のカウント
	int nTextCnt = 0;		// Textのカウント
	char *cFailName = {};

	pFile = fopen(SetFailName, "r");		// ファイル名, r : 読み込み

	if (pFile != NULL)
	{	// 指定されたファイルが開いた場合
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_OBJ = ", 10) != 0);	// 指定した文字が0以外なら解除

															// 文字を進める
		pStrCur += 10;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入

		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE = ", 11) != 0);	// 指定した文字が0以外なら解除

															// 文字を進める
		pStrCur += 11;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入

		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "MODEL_FILENAME = ", 17) == 0)
			{
				pStrCur += 17;
				strcpy(&aStr[0], pStrCur);
				do
				{	// モデル名をいれていく
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// カウント増加
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// 文字カウントを初期化
				nTypeNum++;		// モデルの種類を増加
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// 指定した文字が0以外なら解除
		nTextCnt++;


		// 床の配置数読み込み & 代入
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_FLOOR = ", 12) != 0);	// 指定した文字が0以外なら解除

																// 文字を進める
		pStrCur += 12;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入

												// 床のTexture種類の読み込み & 代入
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE_TEX = ", 15) != 0);	// 指定した文字が0以外なら解除

																// 文字を進める
		pStrCur += 15;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入
		nTypeNum = 0;

		// 床のTextuure名の読み込み & 代入
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "TEXTURE_NAME = ", 15) == 0)
			{
				pStrCur += 15;
				strcpy(&aStr[0], pStrCur);
				do
				{	// モデル名をいれていく
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// カウント増加
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// 文字カウントを初期化
				nTypeNum++;		// モデルの種類を増加
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// 指定した文字が0以外なら解除
																	// 文字を進める
		nTextCnt = 0;
		do
		{
			do
			{	// 指定した文字まで回す
				pStrCur = ReadLine(pFile, &aLine[0], 0);
				strcpy(&aStr[0], pStrCur);

				if (memcmp(&aStr[0], "ARRANGE", 7) == 0)
				{	// 指定文字に入ったら
					for (int nCnt = 0; nCnt < m_text.nNum[nTextCnt];)
					{	// 配置数回す
						pStrCur = ReadLine(pFile, &aLine[0], 0);
						strcpy(&aStr[0], pStrCur);

						if (memcmp(&aStr[0], "OBJSET", 6) == 0)
						{	// 指定数に入ったら
							do
							{	// オブジェクト配置情報の読み込み
								pStrCur = ReadLine(pFile, &aLine[0], 0);
								strcpy(&aStr[0], pStrCur);

								if (memcmp(&aStr[0], "TYPE = ", 7) == 0)
								{	// タイプ情報なら
									pStrCur += 7;
									strcpy(&aStr[0], pStrCur);
									m_text.nType[nTextCnt][nCnt] = (int)atof(pStrCur);
								}
								else if (memcmp(&aStr[0], "POS = ", 6) == 0)
								{	// 位置情報なら
									pStrCur += 6;
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].x = (float)atof(pStrCur);		// pos.xの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].y = (float)atof(pStrCur);		// pos.yの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].z = (float)atof(pStrCur);		// pos.zの情報
								}
								else if (memcmp(&aStr[0], "ROT = ", 6) == 0)
								{	// 回転情報なら
									pStrCur += 6;
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].x = (float)atof(pStrCur);		// rot.xの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].y = (float)atof(pStrCur);		// rot.yの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].z = (float)atof(pStrCur);		// rot.zの情報
								}
								else if (memcmp(&aStr[0], "SIZE = ", 7) == 0)
								{	// 回転情報なら
									pStrCur += 7;
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].x = (float)atof(pStrCur);		// rot.xの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].y = (float)atof(pStrCur);		// rot.yの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].z = (float)atof(pStrCur);		// rot.zの情報
								}


							} while (memcmp(&aStr[0], "OBJEND", 6) != 0);	// 指定した文字が0以外なら解除
							nCnt++;		// for文のカウンターの増加
						}
					}
				}
			} while (memcmp(&aStr[0], "END_ARRANGE", 11) != 0);	// 指定した文字が0以外なら解除
			nTextCnt++;
		} while (nTextCnt != MAX_TEXT);
	}
	fclose(pFile);

}

//*************************************************************
//	Text内読み込み処理
//*************************************************************
char *CLoadText::ReadLine(FILE *pFile, char*pDst, int nPatten)
{
	bool bLine = false;
	while (bLine == false)
	{
		fgets(&aLine[0], 256, pFile);
		while (1)
		{
			if (memcmp(pDst, "\t", 1) == 0)
			{	//
				pDst += 1;
				while (1)
				{
					if (memcmp(pDst, "\t", 1) == 0)
					{
						pDst += 1;
					}
					else
					{
						break;
					}
				}
			}
			if (memcmp(pDst, "\0", 1) == 0)
			{
				pDst += 1;
				while (1)
				{
					if (memcmp(pDst, "\0", 1) == 0)
					{
						pDst += 1;
					}
					else
					{
						break;
					}
				}
			}
			else if (memcmp(pDst, "\n", 1) == 0)
			{
				bLine = true;
				break;
			}
			else if (memcmp(pDst, "#", 1) == 0)
			{
				bLine = true;
				break;
			}
			else
			{
				bLine = true;
				break;
			}
		}
	}
	return pDst;
}

//*************************************************************
//	読み込みたいデータの読み込み処理(pos, rotなど)
//*************************************************************
char *CLoadText::SearchLine(char*pDst)
{
	while (1)
	{
		if (memcmp(pDst, ",", 1) == 0)
		{	// NULL
			pDst += 1;
			break;
		}
		else if (memcmp(pDst, "\0", 1) == 0)
		{
			pDst += 1;
			break;
		}
		else
		{
			pDst += 1;
		}
	}		// TABの場合

	return pDst;
}

//*************************************************************
//	テキスト読み込み情報をリセット
//*************************************************************
void CLoadText::Reset(void)
{
	for (int nCntText = 0; nCntText < MAX_TEXT; nCntText++)
	{
		m_text.nNum[nCntText] = NULL;
		m_text.nTypeNum[nCntText] = NULL;
		for (int nCntFolder = 0; nCntFolder < 20; nCntFolder++)
		{
			for (int nCntName = 0; nCntName < 256; nCntName++)
			{
				m_text.ModeName[nCntText][nCntFolder][nCntName] = { NULL };
			}
		}

		for (int nCntMap = 0; nCntMap < OBJ_MAX; nCntMap++)
		{//	テキストの中身の初期化
			m_text.pos[nCntText][nCntMap] = { NULL, NULL, NULL };
			m_text.rot[nCntText][nCntMap] = { NULL, NULL, NULL };
			m_text.nType[nCntText][nCntMap] = NULL;
			m_text.MeshNum[nCntMap] = { NULL, NULL, NULL };
		}
	}

}
