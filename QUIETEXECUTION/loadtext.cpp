//=======================================================//
//
// �e�L�X�g�ǂݍ��݂̏��� [loadtext.cpp]
// Author :
//
//=======================================================//
#include "loadtext.h"
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[

//*******************************************************//
// �v���g�^�C�v�錾
//*******************************************************//

//*******************************************************//
// �O���[�o���ϐ�
//*******************************************************//

//*******************************************************//
// �ÓI�����o�ϐ�
//*******************************************************//
CLoadText::MAPTEXT CLoadText::m_text = {};	// �e�L�X�g�̍\����

//*******************************************************//
// �R���X�g���N�^
//*******************************************************//
CLoadText::CLoadText()
{

}

//*******************************************************//
// �f�X�g���N�^
//*******************************************************//
CLoadText::~CLoadText()
{

}

//*******************************************************//
// �e�L�X�g�̓ǂݍ���
//*******************************************************//
void CLoadText::TextRoad(char *SetFailName)
{
	int nTypeNum = 0;		// ��ސ��̕ϐ�
	int nCntWord = 0;		// �����̃J�E���g
	int nTextCnt = 0;		// Text�̃J�E���g
	char *cFailName = {};

	pFile = fopen(SetFailName, "r");		// �t�@�C����, r : �ǂݍ���

	if (pFile != NULL)
	{	// �w�肳�ꂽ�t�@�C�����J�����ꍇ
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_OBJ = ", 10) != 0);	// �w�肵��������0�ȊO�Ȃ����

															// ������i�߂�
		pStrCur += 10;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����

		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE = ", 11) != 0);	// �w�肵��������0�ȊO�Ȃ����

															// ������i�߂�
		pStrCur += 11;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����

		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "MODEL_FILENAME = ", 17) == 0)
			{
				pStrCur += 17;
				strcpy(&aStr[0], pStrCur);
				do
				{	// ���f����������Ă���
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// �J�E���g����
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// �����J�E���g��������
				nTypeNum++;		// ���f���̎�ނ𑝉�
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// �w�肵��������0�ȊO�Ȃ����
		nTextCnt++;


		// ���̔z�u���ǂݍ��� & ���
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_FLOOR = ", 12) != 0);	// �w�肵��������0�ȊO�Ȃ����

																// ������i�߂�
		pStrCur += 12;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����

												// ����Texture��ނ̓ǂݍ��� & ���
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE_TEX = ", 15) != 0);	// �w�肵��������0�ȊO�Ȃ����

																// ������i�߂�
		pStrCur += 15;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����
		nTypeNum = 0;

		// ����Textuure���̓ǂݍ��� & ���
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "TEXTURE_NAME = ", 15) == 0)
			{
				pStrCur += 15;
				strcpy(&aStr[0], pStrCur);
				do
				{	// ���f����������Ă���
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// �J�E���g����
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// �����J�E���g��������
				nTypeNum++;		// ���f���̎�ނ𑝉�
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// �w�肵��������0�ȊO�Ȃ����
																	// ������i�߂�
		nTextCnt = 0;
		do
		{
			do
			{	// �w�肵�������܂ŉ�
				pStrCur = ReadLine(pFile, &aLine[0], 0);
				strcpy(&aStr[0], pStrCur);

				if (memcmp(&aStr[0], "ARRANGE", 7) == 0)
				{	// �w�蕶���ɓ�������
					for (int nCnt = 0; nCnt < m_text.nNum[nTextCnt];)
					{	// �z�u����
						pStrCur = ReadLine(pFile, &aLine[0], 0);
						strcpy(&aStr[0], pStrCur);

						if (memcmp(&aStr[0], "OBJSET", 6) == 0)
						{	// �w�萔�ɓ�������
							do
							{	// �I�u�W�F�N�g�z�u���̓ǂݍ���
								pStrCur = ReadLine(pFile, &aLine[0], 0);
								strcpy(&aStr[0], pStrCur);

								if (memcmp(&aStr[0], "TYPE = ", 7) == 0)
								{	// �^�C�v���Ȃ�
									pStrCur += 7;
									strcpy(&aStr[0], pStrCur);
									m_text.nType[nTextCnt][nCnt] = (int)atof(pStrCur);
								}
								else if (memcmp(&aStr[0], "POS = ", 6) == 0)
								{	// �ʒu���Ȃ�
									pStrCur += 6;
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].x = (float)atof(pStrCur);		// pos.x�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].y = (float)atof(pStrCur);		// pos.y�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].z = (float)atof(pStrCur);		// pos.z�̏��
								}
								else if (memcmp(&aStr[0], "ROT = ", 6) == 0)
								{	// ��]���Ȃ�
									pStrCur += 6;
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].x = (float)atof(pStrCur);		// rot.x�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].y = (float)atof(pStrCur);		// rot.y�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].z = (float)atof(pStrCur);		// rot.z�̏��
								}
								else if (memcmp(&aStr[0], "SIZE = ", 7) == 0)
								{	// ��]���Ȃ�
									pStrCur += 7;
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].x = (float)atof(pStrCur);		// rot.x�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].y = (float)atof(pStrCur);		// rot.y�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].z = (float)atof(pStrCur);		// rot.z�̏��
								}


							} while (memcmp(&aStr[0], "OBJEND", 6) != 0);	// �w�肵��������0�ȊO�Ȃ����
							nCnt++;		// for���̃J�E���^�[�̑���
						}
					}
				}
			} while (memcmp(&aStr[0], "END_ARRANGE", 11) != 0);	// �w�肵��������0�ȊO�Ȃ����
			nTextCnt++;
		} while (nTextCnt != MAX_TEXT);
	}
	fclose(pFile);

}

//*************************************************************
//	Text���ǂݍ��ݏ���
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
//	�ǂݍ��݂����f�[�^�̓ǂݍ��ݏ���(pos, rot�Ȃ�)
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
	}		// TAB�̏ꍇ

	return pDst;
}

//*************************************************************
//	�e�L�X�g�ǂݍ��ݏ������Z�b�g
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
		{//	�e�L�X�g�̒��g�̏�����
			m_text.pos[nCntText][nCntMap] = { NULL, NULL, NULL };
			m_text.rot[nCntText][nCntMap] = { NULL, NULL, NULL };
			m_text.nType[nCntText][nCntMap] = NULL;
			m_text.MeshNum[nCntMap] = { NULL, NULL, NULL };
		}
	}

}
