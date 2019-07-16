//=======================================================//
//
// �e�L�X�g�̓ǂݍ��ݏ��� [loadtext.h]
// Author :
//
//=======================================================//
#ifndef _LOADTEXT_H_
#define _LOADTEXT_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

//*******************************************************//
// �}�N����`
//*******************************************************//
#define OBJ_MAX			(1800)
#define TEXT_OBJ_MAX	(3)		//	�e�L�X�g�̒��̃I�u�W�F�N�g�̍ő吔
#define MAX_TEXT		(2)		// Text�̍ő吔

//*******************************************************//
// �O���錾
//*******************************************************//

//*******************************************************//
// �N���X��`
//*******************************************************//
class CLoadText
{
public:
	typedef struct
	{	// TEXT�̏��(���ǉ���͋L�����邱�ƋL�����邱�Ɓ�)
		int nNum[MAX_TEXT];					// �z�u��
		int nTypeNum[MAX_TEXT];				// ���f���̎��
		D3DXVECTOR3 pos[MAX_TEXT][OBJ_MAX];	// �ʒu���
		D3DXVECTOR3 rot[MAX_TEXT][OBJ_MAX];	// ��]���
		D3DXVECTOR3 size[OBJ_MAX];
		D3DXVECTOR3 MeshNum[OBJ_MAX];
		int nType[MAX_TEXT][OBJ_MAX];			// �^�C�v
		char ModeName[MAX_TEXT][20][256];
	}MAPTEXT;

	CLoadText();
	~CLoadText();

	// �e�L�X�g�t�@�C���ǂݍ���
	void TextRoad(char *SetFailName);						// �S�̂̓ǂݍ���
	char *ReadLine(FILE *pFile, char*pDst, int nPatten);	// ��s�̓ǂݍ���
	char *SearchLine(char*pDst);							// ���s�Ȃǂ̓ǂݍ���

	static MAPTEXT GetMapText(void) { return m_text; }		// �e�L�X�g�\���̎擾
	static char *GetName(int nNum, int nTextNum) { return &m_text.ModeName[nTextNum][nNum][0]; }		// ���f�����̎擾
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


